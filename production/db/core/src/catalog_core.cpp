////////////////////////////////////////////////////
// Copyright (c) Gaia Platform LLC
//
// Use of this source code is governed by the MIT
// license that can be found in the LICENSE.txt file
// or at https://opensource.org/licenses/MIT.
////////////////////////////////////////////////////

#include "gaia_internal/db/catalog_core.hpp"

#include <optional>

#include "gaia/common.hpp"

#include "gaia_internal/common/generator_iterator.hpp"
#include "gaia_internal/common/system_table_types.hpp"

#include "db_object_helpers.hpp"
#include "gaia_field_generated.h"
#include "gaia_index_generated.h"
#include "gaia_relationship_generated.h"
#include "gaia_table_generated.h"

using namespace gaia::common;
using namespace gaia::common::iterators;

namespace gaia
{
namespace db
{
namespace catalog_core
{

[[nodiscard]] const char* field_view_t::name() const
{
    return catalog::Getgaia_field(m_obj_ptr->data())->name()->c_str();
}

[[nodiscard]] data_type_t field_view_t::data_type() const
{
    return static_cast<data_type_t>(catalog::Getgaia_field(m_obj_ptr->data())->type());
}

[[nodiscard]] field_position_t field_view_t::position() const
{
    return catalog::Getgaia_field(m_obj_ptr->data())->position();
}

[[nodiscard]] bool field_view_t::optional() const
{
    return catalog::Getgaia_field(m_obj_ptr->data())->optional();
}

[[nodiscard]] const char* table_view_t::name() const
{
    return catalog::Getgaia_table(m_obj_ptr->data())->name()->c_str();
}

[[nodiscard]] gaia_type_t table_view_t::table_type() const
{
    return catalog::Getgaia_table(m_obj_ptr->data())->type();
}

[[nodiscard]] buffer* table_view_t::binary_schema() const
{
    return catalog::Getgaia_table(m_obj_ptr->data())->binary_schema();
}

[[nodiscard]] buffer* table_view_t::serialization_template() const
{
    return catalog::Getgaia_table(m_obj_ptr->data())->serialization_template();
}

[[nodiscard]] const char* relationship_view_t::name() const
{
    return catalog::Getgaia_relationship(m_obj_ptr->data())->name()->c_str();
}

[[nodiscard]] const char* relationship_view_t::to_child_name() const
{
    return catalog::Getgaia_relationship(m_obj_ptr->data())->to_child_name()->c_str();
}

[[nodiscard]] const char* relationship_view_t::to_parent_name() const
{
    return catalog::Getgaia_relationship(m_obj_ptr->data())->to_parent_name()->c_str();
}

[[nodiscard]] reference_offset_t relationship_view_t::first_child_offset() const
{
    return catalog::Getgaia_relationship(m_obj_ptr->data())->first_child_offset();
}

[[nodiscard]] reference_offset_t relationship_view_t::next_child_offset() const
{
    return catalog::Getgaia_relationship(m_obj_ptr->data())->next_child_offset();
}

[[nodiscard]] reference_offset_t relationship_view_t::prev_child_offset() const
{
    return catalog::Getgaia_relationship(m_obj_ptr->data())->prev_child_offset();
}

[[nodiscard]] reference_offset_t relationship_view_t::parent_offset() const
{
    return catalog::Getgaia_relationship(m_obj_ptr->data())->parent_offset();
}

[[nodiscard]] gaia_id_t relationship_view_t::parent_table_id() const
{
    gaia_id_t anchor_id = m_obj_ptr->references()[c_parent_gaia_table_ref_offset];
    auto anchor_ptr = id_to_ptr(anchor_id);
    return anchor_ptr->references()[c_ref_anchor_parent_offset];
}

[[nodiscard]] gaia_id_t relationship_view_t::child_table_id() const
{
    gaia_id_t anchor_id = m_obj_ptr->references()[c_child_gaia_table_ref_offset];
    auto anchor_ptr = id_to_ptr(anchor_id);
    return anchor_ptr->references()[c_ref_anchor_parent_offset];
}

[[nodiscard]] const flatbuffers::Vector<uint16_t>* relationship_view_t::parent_field_positions() const
{
    return catalog::Getgaia_relationship(m_obj_ptr->data())->parent_field_positions();
}

[[nodiscard]] const flatbuffers::Vector<uint16_t>* relationship_view_t::child_field_positions() const
{
    return catalog::Getgaia_relationship(m_obj_ptr->data())->child_field_positions();
}

[[nodiscard]] cardinality_t relationship_view_t::cardinality() const
{
    uint8_t relationship_cardinality_value = catalog::Getgaia_relationship(m_obj_ptr->data())->cardinality();
    if (relationship_cardinality_value == static_cast<uint8_t>(gaia::catalog::relationship_cardinality_t::one))
    {
        return cardinality_t::one;
    }
    else if (relationship_cardinality_value == static_cast<uint8_t>(gaia::catalog::relationship_cardinality_t::many))
    {
        return cardinality_t::many;
    }
    else
    {
        return cardinality_t::not_set;
    }
}

[[nodiscard]] bool relationship_view_t::is_value_linked() const
{
    return parent_field_positions() != nullptr && parent_field_positions()->size() > 0;
}

[[nodiscard]] const char* index_view_t::name() const
{
    return catalog::Getgaia_index(m_obj_ptr->data())->name()->c_str();
}

[[nodiscard]] bool index_view_t::unique() const
{
    return catalog::Getgaia_index(m_obj_ptr->data())->unique();
}

[[nodiscard]] gaia::catalog::index_type_t index_view_t::type() const
{
    return static_cast<gaia::catalog::index_type_t>(
        catalog::Getgaia_index(m_obj_ptr->data())->type());
}

[[nodiscard]] const flatbuffers::Vector<common::gaia_id_t>* index_view_t::fields() const
{
    // This cast works because a gaia_id_t is a thin wrapper over uint64_t,
    // but its success is not guaranteed by the language and is undefined behavior (UB).
    // TODO: Replace reinterpret_cast with bit_cast when it becomes available.
    return reinterpret_cast<const flatbuffers::Vector<common::gaia_id_t>*>(
        catalog::Getgaia_index(m_obj_ptr->data())->fields());
}

[[nodiscard]] gaia_id_t index_view_t::table_id() const
{
    gaia_id_t anchor_id = m_obj_ptr->references()[c_parent_table_ref_offset];
    auto anchor_ptr = id_to_ptr(anchor_id);
    return anchor_ptr->references()[c_ref_anchor_parent_offset];
}

table_view_t get_table(gaia_id_t table_id)
{
    return table_view_t{id_to_ptr(table_id)};
}

table_generator_t::table_generator_t(generator_iterator_t<gaia_ptr_t>&& iterator)
    : m_gaia_ptr_iterator(std::move(iterator))
{
}

std::optional<table_view_t> table_generator_t::operator()()
{
    if (m_gaia_ptr_iterator)
    {
        gaia_ptr_t gaia_ptr = *m_gaia_ptr_iterator;
        if (gaia_ptr)
        {
            ++m_gaia_ptr_iterator;
            return table_view_t(gaia_ptr.to_ptr());
        }
    }
    return std::nullopt;
}

table_list_t list_tables()
{
    auto gaia_ptr_iterator = table_generator_t(gaia_ptr_t::find_all_iterator(
        static_cast<gaia_type_t::value_type>(catalog_core_table_type_t::gaia_table)));
    return range_from_generator(gaia_ptr_iterator);
}

template <typename T_catalog_obj_view>
generator_range_t<T_catalog_obj_view>
list_catalog_obj_reference_chain(gaia_id_t table_id, uint16_t first_offset, uint16_t next_offset)
{
    auto obj_ptr = id_to_ptr(table_id);
    gaia_id_t anchor_id = obj_ptr->references()[first_offset];
    if (!anchor_id.is_valid())
    {
        return generator_range_t<T_catalog_obj_view>();
    }
    auto anchor_ptr = id_to_ptr(anchor_id);
    gaia_id_t first_obj_id = anchor_ptr->references()[c_ref_anchor_first_child_offset];

    auto generator = [id = first_obj_id, next_offset]() mutable -> std::optional<T_catalog_obj_view> {
        if (!id.is_valid())
        {
            return std::nullopt;
        }
        auto obj_ptr = id_to_ptr(id);
        T_catalog_obj_view obj_view{obj_ptr};
        id = obj_ptr->references()[next_offset];
        return obj_view;
    };
    return range_from_generator(generator);
}

field_list_t list_fields(gaia_id_t table_id)
{
    return list_catalog_obj_reference_chain<field_view_t>(
        table_id, c_gaia_table_first_gaia_field_offset, c_gaia_field_next_gaia_field_offset);
}

relationship_list_t list_relationship_from(gaia_id_t table_id)
{
    return list_catalog_obj_reference_chain<relationship_view_t>(
        table_id,
        c_gaia_table_first_parent_gaia_relationship_offset,
        c_gaia_relationship_next_parent_gaia_relationship_offset);
}

relationship_list_t list_relationship_to(gaia_id_t table_id)
{
    return list_catalog_obj_reference_chain<relationship_view_t>(
        table_id,
        c_gaia_table_first_child_gaia_relationship_offset,
        c_gaia_relationship_next_child_gaia_relationship_offset);
}

index_list_t list_indexes(gaia_id_t table_id)
{
    return list_catalog_obj_reference_chain<index_view_t>(
        table_id,
        c_gaia_table_first_gaia_index_offset,
        c_gaia_index_next_gaia_index_offset);
}

gaia_id_t find_index(gaia_id_t table_id, field_position_t field_position)
{
    for (const auto& index : list_indexes(table_id))
    {
        const field_view_t first_field_of_index(id_to_ptr(index.fields()->Get(0)));
        if (first_field_of_index.position() == field_position)
        {
            return index.id();
        }
    }
    return c_invalid_gaia_id;
}

} // namespace catalog_core
} // namespace db
} // namespace gaia
