/////////////////////////////////////////////
// Copyright (c) Gaia Platform LLC
// All rights reserved.
/////////////////////////////////////////////

#pragma once

#include <limits>

#include "gaia_common.hpp"

namespace gaia {
namespace common {

constexpr uint64_t c_system_table_reserved_range = 4096;
constexpr uint64_t c_system_table_reserved_range_end = std::numeric_limits<gaia_type_t>::max();
constexpr uint64_t c_system_table_reserved_range_start = c_system_table_reserved_range_end - c_system_table_reserved_range + 1;

enum class catalog_table_type_t : gaia_type_t {
    gaia_field = c_system_table_reserved_range_end,
    gaia_table = gaia_field - 1,
    gaia_rule = gaia_table - 1,
    gaia_ruleset = gaia_rule - 1,
    gaia_database = gaia_ruleset - 1,
};

enum class system_table_type_t : gaia_type_t {
    catalog_gaia_table = static_cast<gaia_type_t>(catalog_table_type_t::gaia_table),
    catalog_gaia_field = static_cast<gaia_type_t>(catalog_table_type_t::gaia_field),
    catalog_gaia_ruleset = static_cast<gaia_type_t>(catalog_table_type_t::gaia_ruleset),
    catalog_gaia_rule = static_cast<gaia_type_t>(catalog_table_type_t::gaia_rule),
    // Assign constant IDs to other system tables starting from lower end of the reserved range.
    event_log = c_system_table_reserved_range_start,
};

} // namespace common
} // namespace gaia