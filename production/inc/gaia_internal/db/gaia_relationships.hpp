////////////////////////////////////////////////////
// Copyright (c) Gaia Platform LLC
//
// Use of this source code is governed by the MIT
// license that can be found in the LICENSE.txt file
// or at https://opensource.org/licenses/MIT.
////////////////////////////////////////////////////

#pragma once

#include "gaia/common.hpp"

namespace gaia
{
namespace db
{

/**
 * Denotes the cardinality of the children a parent can have.
 */
enum class cardinality_t
{
    not_set,
    one,
    many
};

/**
 * A relationship describes the logical connection between two Gaia types whose
 * direct access classes have methods to to allow objects of one type to
 * directly visit the objects of the other type in the relationship.
 *
 * To facilitate efficient traversal of objects in relationships, the database
 * will allocate reference slots (in the object header before the data payload)
 * for each relationship the object is in.
 *
 * The objects in a relationship form a structure from the references that
 * connect them. The connected data structures are called reference containers
 * which look like the following graph.
 *
 *                +---------------------------------------+
 *                |                                       |
 *                |  +-----------------------+            |
 *                |  |                       |            |
 *                v  v                       |            |
 *  +----+  -->  +----+  <--  +----+       +----+       +----+
 *  | P  |  <--  | A  |  -->  | C1 |  -->  | C2 |  -->  | C3 |
 *  +----+       +----+       +----+  <--  +----+  <--  +----+
 *
 * In the above graph, P is a parent node; A is an anchor node; C1, C2, and C3
 * are child nodes. A parent node has one reference at the 'first_child_offset'
 * slot pointing to the anchor node. An anchor node has two references: one at
 * the 'first_child_offset' slot pointing to the first child node in the list;
 * one at the 'parent_offset' slot pointing to the parent node. Each child node
 * has three references: one at the 'next_child_offset' slot pointing to the
 * next child in the list; one at the 'prev_child_offset' slot pointing to the
 * previous node in the list; one at the 'parent_offset' slot pointing to the
 * anchor node.
 *
 */
struct relationship_t
{
    gaia::common::gaia_type_t parent_type;

    gaia::common::gaia_type_t child_type;

    gaia::common::reference_offset_t first_child_offset;

    gaia::common::reference_offset_t next_child_offset;

    gaia::common::reference_offset_t prev_child_offset;

    gaia::common::reference_offset_t parent_offset;

    cardinality_t cardinality;

    bool parent_required;

    bool value_linked;
};

} // namespace db
} // namespace gaia
