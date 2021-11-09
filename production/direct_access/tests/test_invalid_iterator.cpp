/////////////////////////////////////////////
// Copyright (c) Gaia Platform LLC
// All rights reserved.
/////////////////////////////////////////////

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

TEST_F(test_invalid_iterator, invalid_iterator)
{
    begin_transaction();
    my_table_t::insert_row(false);
    commit_transaction();

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
    }
}
