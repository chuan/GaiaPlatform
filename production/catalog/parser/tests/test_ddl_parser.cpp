/////////////////////////////////////////////
// Copyright (c) Gaia Platform LLC
// All rights reserved.
/////////////////////////////////////////////

#include "gaia_catalog.hpp"
#include "gaia_parser.hpp"
#include "yy_lexer.hpp"
#include "yy_parser.hpp"
#include "gtest/gtest.h"

using namespace gaia::catalog::ddl;

TEST(catalog_ddl_parser_test, create_table) {
    parser_t parser;
    yy_scan_string("CREATE TABLE t (c INT32);");
    yy::parser parse(parser);
    EXPECT_EQ(EXIT_SUCCESS, parse());
    EXPECT_EQ(1, parser.statements.size());
    EXPECT_EQ(parser.statements[0]->type(), statment_type_t::CREATE);
    create_statement_t *createStmt =
        reinterpret_cast<create_statement_t *>(parser.statements[0]);
    EXPECT_EQ(createStmt->type, create_type_t::CREATE_TABLE);
    EXPECT_EQ(createStmt->tableName, "t");
    yylex_destroy();
}

TEST(catalog_ddl_parser_test, create_table_multiple_fields) {
    parser_t parser;
    yy_scan_string("CREATE TABLE t (c1 INT32[], c2 FLOAT64[2], c3 tt, c4 tt[3]);");
    yy::parser parse(parser);
    EXPECT_EQ(EXIT_SUCCESS, parse());
    EXPECT_EQ(1, parser.statements.size());
    EXPECT_EQ(parser.statements[0]->type(), statment_type_t::CREATE);
    create_statement_t *createStmt =
        reinterpret_cast<create_statement_t *>(parser.statements[0]);
    EXPECT_EQ(createStmt->type, create_type_t::CREATE_TABLE);
    EXPECT_EQ(createStmt->tableName, "t");
    EXPECT_EQ(createStmt->fields->size(), 4);

    EXPECT_EQ(createStmt->fields->at(0)->name, "c1");
    EXPECT_EQ(createStmt->fields->at(0)->type, data_type_t::INT32);
    EXPECT_EQ(createStmt->fields->at(0)->length, 0);

    EXPECT_EQ(createStmt->fields->at(1)->name, "c2");
    EXPECT_EQ(createStmt->fields->at(1)->type, data_type_t::FLOAT64);
    EXPECT_EQ(createStmt->fields->at(1)->length, 2);

    EXPECT_EQ(createStmt->fields->at(2)->name, "c3");
    EXPECT_EQ(createStmt->fields->at(2)->type, data_type_t::TABLE);
    EXPECT_EQ(createStmt->fields->at(2)->length, 1);

    EXPECT_EQ(createStmt->fields->at(3)->name, "c4");
    EXPECT_EQ(createStmt->fields->at(3)->type, data_type_t::TABLE);
    EXPECT_EQ(createStmt->fields->at(3)->length, 3);
    yylex_destroy();
}