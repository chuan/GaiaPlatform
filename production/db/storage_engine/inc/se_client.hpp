/////////////////////////////////////////////
// Copyright (c) Gaia Platform LLC
// All rights reserved.
/////////////////////////////////////////////

#pragma once

#include "gaia_db.hpp"
#include "retail_assert.hpp"
#include "se_shared_data.hpp"
#include "system_table_types.hpp"
#include "triggers.hpp"

namespace gaia
{

namespace db
{

class client
{
    friend class gaia_ptr;
    friend gaia::db::locators* gaia::db::get_shared_locators();
    friend gaia::db::data* gaia::db::get_shared_data();

public:
    static inline bool is_transaction_active()
    {
        return (s_locators != nullptr);
    }

    static inline bool set_commit_trigger(triggers::commit_trigger_fn trigger_fn)
    {
        return __sync_val_compare_and_swap(&s_txn_commit_trigger, 0, trigger_fn);
    }

    // This test-only function is exported from gaia_db_internal.hpp.
    static void clear_shared_memory();

    // These public functions are exported from and documented in gaia_db.hpp.
    static void begin_session();
    static void end_session();
    static void begin_transaction();
    static void rollback_transaction();
    static void commit_transaction();

    // This returns a generator object for gaia_ids of a given type.
    static std::function<std::optional<gaia::common::gaia_id_t>()> get_id_generator_for_type(gaia_type_t type);

private:
    // s_fd_log & s_locators have transaction lifetime.
    thread_local static inline log* s_log = nullptr;
    thread_local static inline int s_fd_log = -1;
    thread_local static inline locators* s_locators = nullptr;

    // s_fd_locators, s_data, s_session_socket have session lifetime.
    thread_local static inline int s_fd_locators = -1;
    thread_local static inline data* s_data = nullptr;
    thread_local static inline int s_session_socket = -1;
    thread_local static inline gaia_txn_id_t s_txn_id = c_invalid_gaia_txn_id;

    // s_events has transaction lifetime and is cleared after each transaction.
    // Set by the rules engine.
    thread_local static inline std::vector<gaia::db::triggers::trigger_event_t> s_events{};
    static inline triggers::commit_trigger_fn s_txn_commit_trigger = nullptr;

    // Maintain a static filter in the client to disable generating events
    // for system types.
    static constexpr gaia_type_t trigger_excluded_types[] = {
        static_cast<gaia_type_t>(system_table_type_t::catalog_gaia_table),
        static_cast<gaia_type_t>(system_table_type_t::catalog_gaia_field),
        static_cast<gaia_type_t>(system_table_type_t::catalog_gaia_relationship),
        static_cast<gaia_type_t>(system_table_type_t::catalog_gaia_ruleset),
        static_cast<gaia_type_t>(system_table_type_t::catalog_gaia_rule),
        static_cast<gaia_type_t>(system_table_type_t::event_log)};

    static void txn_cleanup();

    static void destroy_log_mapping();

    static int get_session_socket();

    static int get_id_cursor_socket_for_type(gaia_type_t type);

    // This is a helper for higher-level methods that use
    // this generator to build a range or iterator object.
    template <typename element_type>
    static std::function<std::optional<element_type>()>
    get_stream_generator_for_socket(int stream_socket);

    /**
     *  Check if an event should be generated for a given type.
     */
    static inline bool is_valid_event(gaia_type_t type)
    {
        constexpr const gaia_type_t* end = trigger_excluded_types + std::size(trigger_excluded_types);
        return (s_txn_commit_trigger && (std::find(trigger_excluded_types, end, type) == end));
    }

    static inline void verify_txn_active()
    {
        if (!is_transaction_active())
        {
            throw transaction_not_open();
        }
    }

    static inline void verify_no_txn()
    {
        if (is_transaction_active())
        {
            throw transaction_in_progress();
        }
    }

    static inline void verify_session_active()
    {
        if (s_session_socket == -1)
        {
            throw no_session_active();
        }
    }

    static inline void verify_no_session()
    {
        if (s_session_socket != -1)
        {
            throw session_exists();
        }
    }

    static inline void txn_log(
        gaia_locator_t locator,
        gaia_offset_t old_offset,
        gaia_offset_t new_offset,
        gaia_operation_t operation,
        // 'deleted_id' is required to keep track of deleted keys which will be propagated to the persistent layer.
        // Memory for other operations will be unused. An alternative would be to keep a separate log for deleted keys only.
        gaia::common::gaia_id_t deleted_id = 0)
    {
        gaia::common::retail_assert(s_log->count < c_max_log_records, "Log count exceeds maximum log record count!");
        log::log_record* lr = s_log->log_records + s_log->count++;
        lr->locator = locator;
        lr->old_offset = old_offset;
        lr->new_offset = new_offset;
        lr->deleted_id = deleted_id;
        lr->operation = operation;
    }
};

} // namespace db
} // namespace gaia