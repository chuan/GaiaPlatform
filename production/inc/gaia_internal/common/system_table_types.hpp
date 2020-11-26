/////////////////////////////////////////////
// Copyright (c) Gaia Platform LLC
// All rights reserved.
/////////////////////////////////////////////

#pragma once

#include <limits>

#include "gaia/common.hpp"

namespace gaia
{
namespace common
{

constexpr gaia_type_t c_system_table_reserved_range = 4096;
constexpr gaia_type_t c_system_table_reserved_range_end = std::numeric_limits<gaia_type_t>::max();
constexpr gaia_type_t c_system_table_reserved_range_start = c_system_table_reserved_range_end - c_system_table_reserved_range + 1;

// The order of these fields is relevant to the generated order of the catalog
// table structs in the extended data classes (EDC) code. The child table
// referencing the parent table (child->parent) should come before the parent
// table. In other words, the child table should have a larger id than the
// parent table. This allows incomplete forward declaration of structs that
// refer to each other in the EDC code.
enum class catalog_table_type_t : gaia_type_t
{
    gaia_field = c_system_table_reserved_range_end,
    gaia_table = gaia_field - 1,
    gaia_rule = gaia_table - 1,
    gaia_ruleset = gaia_rule - 1,
    gaia_database = gaia_ruleset - 1,
    gaia_relationship = gaia_database - 1
};

enum class system_table_type_t : gaia_type_t
{
    catalog_gaia_table = static_cast<gaia_type_t>(catalog_table_type_t::gaia_table),
    catalog_gaia_field = static_cast<gaia_type_t>(catalog_table_type_t::gaia_field),
    catalog_gaia_ruleset = static_cast<gaia_type_t>(catalog_table_type_t::gaia_ruleset),
    catalog_gaia_rule = static_cast<gaia_type_t>(catalog_table_type_t::gaia_rule),
    catalog_gaia_database = static_cast<gaia_type_t>(catalog_table_type_t::gaia_database),
    catalog_gaia_relationship = static_cast<gaia_type_t>(catalog_table_type_t::gaia_relationship),
    // Assign constant IDs to other system tables starting from lower end of the reserved range.
    event_log = c_system_table_reserved_range_start,
};

} // namespace common
} // namespace gaia