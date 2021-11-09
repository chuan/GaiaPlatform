/////////////////////////////////////////////
// Copyright (c) Gaia Platform LLC
// All rights reserved.
/////////////////////////////////////////////

#include <chrono>

#include <thread>

#include <gtest/gtest.h>

#include "gaia/common.hpp"

#include "gaia_internal/db/db_catalog_test_base.hpp"

#include "gaia_addr_book.h"

using namespace gaia::db;
using namespace gaia::common;
using namespace gaia::direct_access;
using namespace gaia::addr_book;

class test_invalid_iterator : public db_catalog_test_base_t
{
protected:
    test_invalid_iterator()
        : db_catalog_test_base_t("addr_book.ddl"){};
};

void set_value(uint64_t timeout_in_seconds)
{
    constexpr uint16_t c_max_wait_seconds = 10;

    uint64_t duration = 0;
    std::default_random_engine generator;
    std::uniform_int_distribution<uint16_t> distribution(1, c_max_wait_seconds);

    gaia::db::begin_session();

    while (duration < timeout_in_seconds)
    {
        begin_transaction();
        auto my_table_writer = my_table_t::list().begin()->writer();
        my_table_writer.is_done = false;
        my_table_writer.update_row();
        commit_transaction();

        uint16_t wait_duration = distribution(generator);
        std::this_thread::sleep_for(std::chrono::seconds(wait_duration));
        duration += wait_duration;
    }
    begin_transaction();
    auto my_table_writer = my_table_t::list().begin()->writer();
    my_table_writer.is_done = true;
    my_table_writer.update_row();
    commit_transaction();
    gaia::db::end_session();
}

TEST_F(test_invalid_iterator, invalid_iterator)
{
    constexpr uint64_t c_timeout_in_seconds = 3600;
    constexpr uint64_t c_wait_duration_in_micro_seocnds = 10;

    begin_transaction();
    my_table_t::insert_row(false);
    commit_transaction();

    std::thread wait_for(set_value, c_timeout_in_seconds);

    while (true)
    {
        begin_transaction();
        my_table_t my_table = *(my_table_t::list().begin());
        bool done = my_table.is_done();
        commit_transaction();

        if (done)
        {
            break;
        }

        std::this_thread::sleep_for(std::chrono::microseconds(c_wait_duration_in_micro_seocnds));
    }

    wait_for.join();
}
