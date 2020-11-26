/////////////////////////////////////////////
// Copyright (c) Gaia Platform LLC
// All rights reserved.
/////////////////////////////////////////////
#pragma once

#include <memory>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include "gaia/common.hpp"
#include "gaia/direct_access/auto_transaction.hpp"
#include "gaia/events.hpp"
#include "gaia/exception.hpp"

namespace gaia
{
/**
 * \addtogroup Gaia
 * @{
 */

namespace rules
{

/**
 * \addtogroup Rules
 * @{
 *
 * Provides facilities for subscribing and unsubscribing rules
 * to events.
 */

struct rule_context_t;

/**
 * All rules must adhere to this prototype.  Since the preamble of the rule
 * is generated by a GaiaProcessor, the context parameter should
 * be cast to the appropriate derived context before use.
 */
typedef void (*gaia_rule_fn)(const rule_context_t* context);

/**
 * The application must provide an implementation of initialize_rules().  This
 * function is invoked when the event manager singleton is created.
 */
extern "C" void initialize_rules();

/**
 * The application may provide an implementation of subscribe_ruleset().  This
 * is a convenience method to subscribe all the rules in a ruleset at once.
 */
extern "C" void subscribe_ruleset(const char* ruleset_name);

/**
 * The application may provide an implementation of unsubscribe_ruleset().  This
 * is a convenience method to unsubscribe all the rules in a ruleset at once.
 */
extern "C" void unsubscribe_ruleset(const char* ruleset_name);

/**
 * The caller supplies a rule_binding to subscribe/unsubscribe rules to/from events.
 * The caller must supply the ruleset_name, rule_name, and the function pointer for the rule.
 * The ruleset_name and the rule_name must uniquely identify the rule.
  */
struct rule_binding_t
{
    rule_binding_t()
        : ruleset_name(nullptr)
        , rule_name(nullptr)
        , rule(nullptr)
    {
    }

    rule_binding_t(
        const char* a_ruleset_name,
        const char* a_rule_name,
        gaia_rule_fn a_rule)
        : ruleset_name(a_ruleset_name)
        , rule_name(a_rule_name)
        , rule(a_rule)
    {
    }

    const char* ruleset_name;
    const char* rule_name;
    gaia_rule_fn rule;
};

/**
 * This type is returned in a caller-supplied vector when
 * the list_rules api is called
 */
struct subscription_t
{
    subscription_t()
        : ruleset_name(nullptr)
        , rule_name(nullptr)
        , gaia_type(common::c_invalid_gaia_type)
        , event_type(db::triggers::event_type_t::not_set)
        , field(0)
    {
    }

    subscription_t(
        const char* a_ruleset_name,
        const char* a_rule_name,
        common::gaia_type_t a_gaia_type,
        db::triggers::event_type_t an_event_type,
        common::field_position_t a_field)
        : ruleset_name(a_ruleset_name)
        , rule_name(a_rule_name)
        , gaia_type(a_gaia_type)
        , event_type(an_event_type)
        , field(a_field)
    {
    }

    const char* ruleset_name;
    const char* rule_name;
    common::gaia_type_t gaia_type;
    db::triggers::event_type_t event_type;
    const common::field_position_t field;
};

/**
 * Caller must provide an instance of this type when using the list_subscribed_rules
 * method below
 */
typedef std::vector<std::unique_ptr<subscription_t>> subscription_list_t;

/**
 * Use this constant to specify that no fields are needed for the
 * subscribe_rule call.
 */
const common::field_position_list_t empty_fields;

/**
 * Enumeration for the last database operation performed for a given gaia type.
 * This is a different enum type than the event type because there may be
 * events that are not caused by a table operation.  In addition, a 'none'
 * enum value is provided to indicate that no operation was performed on a
 * table. See the rule_context_t::last_operation() method for further
 * explanation.
 */
enum class last_operation_t : uint8_t
{
    none,
    row_update,
    row_insert,
    row_delete
};

/**
 * The rule context wraps the event (or data) context as well as information
 * about the event and rule metadata.  In the future the rule context may also
 * maintain the error state of the rule invocation.  Therefore, the rule
 * contexts map 1:1 to each rule that is bound to an event.
 * Data contexts may apply to more than one rule.
 *
 * Note:  A single event may be bound to multiple rules or a rule may be bound
 * to multiple events. Rules may also be invoked both synchronously or
 * asynchronously.  For this reason, events and rules are decoupled in
 * the system.
 *
 * The rule binding is included for debugging and may be removed in a later
 * iteration.
 */
struct rule_context_t
{
public:
    rule_context_t(
        direct_access::auto_transaction_t& a_txn,
        common::gaia_type_t a_gaia_type,
        db::triggers::event_type_t a_event_type,
        common::gaia_id_t a_record,
        const common::field_position_list_t& a_field_list)
        : txn(a_txn)
        , gaia_type(a_gaia_type)
        , event_type(a_event_type)
        , record(a_record)
        , fields(a_field_list)
    {
    }

    rule_context_t() = delete;

    /**
     * Helper to enable a declarative rule to check what the last database
     * operation was for the passed-in type. For example, consider a rule that
     * may be invoked either from a table operation on type X or a change to
     * a field reference in table Y. This method enables the rule author to
     * determine the reason for the rule being invoked.
     *
     * if (X.last_operation == last_operation_t::row_update) { do A stuff }
     * else { do B stuff }
     *
     * This method will return last_operation_t::none if this rule was not
     * invoked due to an operation on X.
     */
    last_operation_t last_operation(common::gaia_type_t other_gaia_type) const;

    direct_access::auto_transaction_t& txn;
    common::gaia_type_t gaia_type;
    db::triggers::event_type_t event_type;
    common::gaia_id_t record;
    const common::field_position_list_t& fields;
};

/**
 * Thrown when the caller provides an incomplete rule_binding_t structure.
 *
 * The system needs the function pointer, rule_name, and ruleset_name to
 * be provided by the caller.
 */
class invalid_rule_binding : public gaia::common::gaia_exception
{
public:
    invalid_rule_binding();
};

/**
 * Thrown under two circumstances.
 *
 * First, the ruleset_name and rule_name must be unique across the system.
 * If a caller submits a rule_binding that generates the same key but has
 * a different rule definition then this exception is thrown.
 *
 * Second, if a user attempts to subscribe the same rule to the same
 * gaia type and event  type then this event is thrown.
 */
class duplicate_rule : public gaia::common::gaia_exception
{
public:
    duplicate_rule(const rule_binding_t& binding, bool duplicate_key_found);
};

/**
 * Thrown when the caller either does not initialize the event manager
 * or attempts to initialize an already initialized event manager.
 */
class initialization_error : public gaia::common::gaia_exception
{
public:
    initialization_error();
};

/**
 * Thrown when the caller provides an invalid subscription.  See the
 * constructor methods for the reasons that his could occur.
 */
class invalid_subscription : public gaia::common::gaia_exception
{
public:
    // Invalid event type specified.
    invalid_subscription(db::triggers::event_type_t event_type, const char* reason);
    // Table type not found.
    invalid_subscription(common::gaia_type_t gaia_type);
    // Field not found.
    invalid_subscription(common::gaia_type_t gaia_type, const char* table, uint16_t position);
    // Field not active or has been deprecated
    invalid_subscription(common::gaia_type_t gaia_type, const char* table, uint16_t position, const char* field_name, bool is_deprecated);
};

/**
 * ruleset_not_found_subscription : public gaia::common::gaia_exception
 */
class ruleset_not_found : public gaia::common::gaia_exception
{
public:
    ruleset_not_found(const char* ruleset_name);
};

/**
 * Initializes the rules engine.  Should only be called once
 * per process.
 *
 * @throw initialization_error
 */
void initialize_rules_engine();

/**
 * Shutdown the rules engine.
 *
 * @throw initialization_error
 */
void shutdown_rules_engine();

/**
 * Subscribes this rule to one of:
 * transaction operation (begin, commit, rollback)
 * table operation (insert, update, delete)
 * field references (update a specific field)
 *
 * @param gaia_type table type to bind the rule to
 * @param event_type read or write field event
 * @param fields the set of fields that will cause this rule to be fired if changed.
 * @param rule_binding caller-supplied rule information; this call will populate rule_name
 * @throw invalid_rule_binding
 * @throw duplicate_rule
 * @throw initialization_error
 */
void subscribe_rule(
    gaia::common::gaia_type_t gaia_type,
    gaia::db::triggers::event_type_t event_type,
    const common::field_position_list_t& fields,
    const rule_binding_t& rule_binding);

/**
 * Unsubscribes this rule from the specified table event scoped by the gaia_type.
 *
 * @param gaia_type table type to bind the rule to
 * @param type the event type to bind this rule to
 * @param fields the set of columns to unsubscribe the rule from
 * @param rule_binding caller-supplied rule information
 * @return true if the rule was unsubscribed; false otherwise.
 * @throw invalid_rule_binding
 * @throw initialization_error
 */
bool unsubscribe_rule(
    gaia::common::gaia_type_t gaia_type,
    gaia::db::triggers::event_type_t type,
    const common::field_position_list_t& fields,
    const rule_binding_t& rule_binding);

/**
 * Unsubscribes all rules that were subscribed from the system.  May be called
 * even if no rules have been subscribed.
 * @throw initialization_error
 */
void unsubscribe_rules();

/**
 * List all rules already subscribed to events.
 *
 * Enable filtering on ruleset name, gaia_type, event_type, and field.
 *
 * @param ruleset_name Scope returned rules to specified rulset if provided.  May be null.
 * @param gaia_type Filter results by the object they refer to.  May be null.
 * @param event_type Filter by event you want.
 * @param field Filter by the field ordinal.
 * @param subscriptions Caller provided vector to hold the results.  This method will clear any existing
 *      entries before adding new ones.
 * @throw initialization_error
 */
void list_subscribed_rules(
    const char* ruleset_name,
    const gaia::common::gaia_type_t* gaia_type,
    const gaia::db::triggers::event_type_t* event_type,
    const common::field_position_t* field,
    subscription_list_t& subscriptions);

/*@}*/
} // namespace rules
/*@}*/
} // namespace gaia