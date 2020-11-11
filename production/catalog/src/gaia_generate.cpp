/////////////////////////////////////////////
//// Copyright (c) Gaia Platform LLC
//// All rights reserved.
///////////////////////////////////////////////

#include <memory>
#include <set>
#include <vector>

#include "flatbuffers/code_generators.h"

#include "catalog.hpp"
#include "gaia_catalog.h"
#include "type_id_record_id_cache.hpp"

using namespace std;

namespace gaia
{
namespace catalog
{

const string c_indent_string("    ");

struct field_strings_t
{
    string name;
    data_type_t type;
};

typedef std::vector<field_strings_t> field_vector_t;
typedef std::vector<gaia_relationship_t> relationship_vector_t;

static string field_cpp_type_string(data_type_t data_type)
{
    switch (data_type)
    {
    case data_type_t::e_bool:
        return "bool";
    case data_type_t::e_int8:
        return "int8_t";
    case data_type_t::e_uint8:
        return "uint8_t";
    case data_type_t::e_int16:
        return "int16_t";
    case data_type_t::e_uint16:
        return "uint16_t";
    case data_type_t::e_int32:
        return "int32_t";
    case data_type_t::e_uint32:
        return "uint32_t";
    case data_type_t::e_int64:
        return "int64_t";
    case data_type_t::e_uint64:
        return "uint64_t";
    case data_type_t::e_float:
        return "float";
    case data_type_t::e_double:
        return "double";
    case data_type_t::e_string:
        return "const char*";
    default:
        throw gaia::common::gaia_exception("Unknown type");
    }
}

// List the relationships where table appear as parent, sorted by offset.
static relationship_vector_t list_parent_relationships(gaia_table_t table)
{
    relationship_vector_t relationships;

    for (const auto& relationship : table.parent_gaia_relationship_list())
    {
        relationships.push_back(relationship);
    }

    std::sort(relationships.begin(), relationships.end(), [](auto r1, auto r2) -> bool {
        return r1.first_child_offset() < r2.first_child_offset();
    });

    return relationships;
}

// List the relationships where table appear as child, sorted by offset.
static relationship_vector_t list_child_relationships(gaia_table_t table)
{
    relationship_vector_t relationships;

    for (const auto& relationship : table.child_gaia_relationship_list())
    {
        relationships.push_back(relationship);
    }

    std::sort(relationships.begin(), relationships.end(), [](auto r1, auto r2) -> bool {
        return r1.parent_offset() < r2.parent_offset();
    });

    return relationships;
}

// List all the table relationships sorted by offset.
static relationship_vector_t list_relationships(gaia_table_t table)
{
    std::map<uint8_t, gaia_relationship_t> relationships_map{};

    for (const gaia_relationship_t& relationship : table.parent_gaia_relationship_list())
    {
        relationships_map.insert({relationship.first_child_offset(), relationship});
    }

    for (const gaia_relationship_t& relationship : table.child_gaia_relationship_list())
    {
        relationships_map.insert({relationship.parent_offset(), relationship});
    }

    relationship_vector_t relationships;
    transform(relationships_map.begin(), relationships_map.end(), back_inserter(relationships), [](const auto& val) { return val.second; });

    // Removes duplicates from the vector. Not using a set because we need custom sorting (which differs in parent/child relationships).
    relationships.erase(std::unique(relationships.begin(), relationships.end()), relationships.end());

    return relationships;
}

static string generate_boilerplate_top(string dbname)
{
    flatbuffers::CodeWriter code(c_indent_string);
    code.SetValue("DBNAME", dbname);
    code += "/////////////////////////////////////////////";
    code += "// Copyright (c) Gaia Platform LLC";
    code += "// All rights reserved.";
    code += "/////////////////////////////////////////////";
    code += "";
    code += "// Automatically generated by the Gaia Data Classes code generator.";
    code += "// Do not modify.";
    code += "";
    code += "#include <iterator>";
    code += "";
    code += "#ifndef GAIA_GENERATED_{{DBNAME}}_H_";
    code += "#define GAIA_GENERATED_{{DBNAME}}_H_";
    code += "";
    code += "#include \"gaia_object.hpp\"";
    code += "#include \"{{DBNAME}}_generated.h\"";
    code += "#include \"gaia_iterators.hpp\"";
    code += "";
    code += "using namespace std;";
    code += "using namespace gaia::direct_access;";
    code += "";
    code += "namespace " + c_gaia_namespace + " {";
    if (!dbname.empty())
    {
        code += "namespace {{DBNAME}} {";
    }
    string str = code.ToString();
    return str;
}

static string generate_boilerplate_bottom(string dbname)
{
    flatbuffers::CodeWriter code(c_indent_string);
    code.SetValue("DBNAME", dbname);
    if (!dbname.empty())
    {
        code += "}  // namespace {{DBNAME}}";
    }
    code += "}  // namespace " + c_gaia_namespace;
    code += "";
    code += "#endif  // GAIA_GENERATED_{{DBNAME}}_H_";
    string str = code.ToString();
    return str;
}

// Generate the list of constants referred to by the class definitions and templates.
static string generate_constant_list(const gaia_id_t db_id)
{
    flatbuffers::CodeWriter code(c_indent_string);
    // A fixed constant is used for the flatbuffer builder constructor.
    code += "";
    code += "// The initial size of the flatbuffer builder buffer.";
    code += "constexpr int c_flatbuffer_builder_size = 128;";
    code += "";
    for (auto& table_record : gaia_database_t::get(db_id).gaia_table_list())
    {
        relationship_vector_t relationships = list_relationships(table_record);

        auto const_count = 0;
        code.SetValue("TABLE_NAME", table_record.name());
        code.SetValue("TABLE_TYPE", to_string(table_record.type()));
        code += "// Constants contained in the {{TABLE_NAME}} object.";
        code += "constexpr uint32_t c_gaia_type_{{TABLE_NAME}} = {{TABLE_TYPE}}u;";

        for (auto& relationship : relationships)
        {
            if (strlen(relationship.name()))
            {
                code.SetValue("REF_NAME", relationship.name());
            }
            else
            {
                // Anonymous relationship
                code.SetValue("REF_NAME", relationship.child_gaia_table().name());
            }

            if (table_record == relationship.parent_gaia_table())
            {
                // relationship where table_record appears as parent
                code.SetValue("FIRST_CHILD_OFFSET", to_string(relationship.first_child_offset()));
                code.SetValue("CHILD_TABLE", relationship.child_gaia_table().name());
                code += "constexpr int c_first_{{REF_NAME}}_{{CHILD_TABLE}} = {{FIRST_CHILD_OFFSET}};";
                const_count++;
            }

            // Can't use 'else if' because self-relationships appear both as parent and child.
            if (table_record == relationship.child_gaia_table())
            {
                // relationship where table_record appears as child
                code.SetValue("NEXT_CHILD_OFFSET", to_string(relationship.next_child_offset()));
                code.SetValue("PARENT_OFFSET", to_string(relationship.parent_offset()));
                code.SetValue("CHILD_TABLE", relationship.child_gaia_table().name());
                code.SetValue("PARENT_TABLE", relationship.parent_gaia_table().name());
                code += "constexpr int c_parent_{{REF_NAME}}_{{PARENT_TABLE}} = {{PARENT_OFFSET}};";
                code += "constexpr int c_next_{{REF_NAME}}_{{CHILD_TABLE}} = {{NEXT_CHILD_OFFSET}};";
                const_count += 2;
            }
        }
        code.SetValue("CONST_VALUE", to_string(const_count));
        code += "constexpr int c_num_{{TABLE_NAME}}_ptrs = {{CONST_VALUE}};";
        code += "";
    }
    string str = code.ToString();
    return str;
}

static string generate_declarations(const gaia_id_t db_id)
{
    flatbuffers::CodeWriter code(c_indent_string);

    for (auto const& table : gaia_database_t::get(db_id).gaia_table_list())
    {
        code.SetValue("TABLE_NAME", table.name());
        code += "struct {{TABLE_NAME}}_t;";
    }
    code += "";
    string str = code.ToString();
    return str;
}

static string generate_edc_struct(
    gaia_type_t table_type_id,
    gaia_table_t table_record,
    field_vector_t& field_strings,
    relationship_vector_t parent_relationships,
    relationship_vector_t child_relationships)
{
    flatbuffers::CodeWriter code(c_indent_string);

    // Struct statement.
    code.SetValue("TABLE_NAME", table_record.name());
    code.SetValue("POSITION", to_string(table_type_id));

    // Iterate over the relationships where the current table appear as parent
    for (auto& relationship : parent_relationships)
    {
        code.SetValue("REF_TABLE", relationship.child_gaia_table().name());

        if (strlen(relationship.name()))
        {
            code.SetValue("REF_NAME", relationship.name());

            code += "typedef reference_chain_container_t<{{TABLE_NAME}}_t, {{REF_TABLE}}_t, "
                    "c_parent_{{REF_NAME}}_{{TABLE_NAME}}, "
                    "c_first_{{REF_NAME}}_{{REF_TABLE}}, c_next_{{REF_NAME}}_{{REF_TABLE}}> "
                    "{{REF_NAME}}_{{REF_TABLE}}_list_t;";
        }
        else
        {
            // This relationship is anonymous.
            code.SetValue("REF_NAME", relationship.child_gaia_table().name());

            code += "typedef reference_chain_container_t<{{TABLE_NAME}}_t, {{REF_TABLE}}_t, "
                    "c_parent_{{REF_TABLE}}_{{TABLE_NAME}}, "
                    "c_first_{{REF_NAME}}_{{REF_TABLE}}, c_next_{{REF_NAME}}_{{REF_TABLE}}> {{REF_NAME}}_list_t;";
        }
    }

    code += "typedef gaia_writer_t<c_gaia_type_{{TABLE_NAME}}, {{TABLE_NAME}}_t, {{TABLE_NAME}}, {{TABLE_NAME}}T, "
            "c_num_{{TABLE_NAME}}_ptrs> {{TABLE_NAME}}_writer;";
    code += "struct {{TABLE_NAME}}_t : public gaia_object_t<c_gaia_type_{{TABLE_NAME}}, {{TABLE_NAME}}_t, "
            "{{TABLE_NAME}}, {{TABLE_NAME}}T, c_num_{{TABLE_NAME}}_ptrs> {";

    code.IncrementIdentLevel();

    // Default public constructor.
    code += "{{TABLE_NAME}}_t() : gaia_object_t(\"{{TABLE_NAME}}_t\") {}";

    // Below, a flatbuffer method is invoked as Create{{TABLE_NAME}}() or
    // as Create{{TABLE_NAME}}Direct. The choice is determined by whether any of the
    // fields are strings. If at least one is a string, than the Direct variation
    // is used.
    // NOTE: There may be a third variation of this if any of the fields are vectors
    // or possibly arrays.
    bool has_string = false;
    // Accessors.
    for (auto const& f : field_strings)
    {
        code.SetValue("TYPE", field_cpp_type_string(f.type));
        code.SetValue("FIELD_NAME", f.name);
        if (f.type == data_type_t::e_string)
        {
            has_string = true;
            code.SetValue("FCN_NAME", "GET_STR");
        }
        else
        {
            code.SetValue("FCN_NAME", "GET");
        }
        code += "{{TYPE}} {{FIELD_NAME}}() const {return {{FCN_NAME}}({{FIELD_NAME}});}";
    }

    code += "using gaia_object_t::insert_row;";

    // The typed insert_row().
    string param_list("static gaia_id_t insert_row(");
    bool first = true;
    for (auto const& f : field_strings)
    {
        if (!first)
        {
            param_list += ", ";
        }
        else
        {
            first = false;
        }
        param_list += field_cpp_type_string(f.type) + " ";
        param_list += f.name;
    }
    code += param_list + ") {";
    code.IncrementIdentLevel();
    code += "flatbuffers::FlatBufferBuilder b(c_flatbuffer_builder_size);";
    code.SetValue("DIRECT", has_string ? "Direct" : "");
    param_list = "b.Finish(Create{{TABLE_NAME}}{{DIRECT}}(b";
    for (auto const& f : field_strings)
    {
        param_list += ", ";
        param_list += f.name;
    }
    param_list += "));";
    code += param_list;
    code += "return gaia_object_t::insert_row(b);";
    code.DecrementIdentLevel();
    code += "}";

    // Iterate over the relationships where the current table appear as child
    for (auto& relationship : child_relationships)
    {
        if (strlen(relationship.name()))
        {
            code.SetValue("REF_NAME", relationship.name());
            code.SetValue("REF_TABLE", relationship.parent_gaia_table().name());
            code += "{{REF_TABLE}}_t {{REF_NAME}}_{{REF_TABLE}}() {";
            code.IncrementIdentLevel();
            code += "return {{REF_TABLE}}_t::get(this->references()[c_parent_{{REF_NAME}}_{{REF_TABLE}}]);";
        }
        else
        {
            // This relationship is anonymous.
            code.SetValue("REF_NAME", relationship.parent_gaia_table().name());
            code.SetValue("REF_TABLE", relationship.parent_gaia_table().name());
            code += "{{REF_TABLE}}_t {{REF_NAME}}() {";
            code.IncrementIdentLevel();
            code += "return {{REF_TABLE}}_t::get(this->references()[c_parent_{{TABLE_NAME}}_{{REF_TABLE}}]);";
        }
        code.DecrementIdentLevel();
        code += "}";
    }

    // The table range.
    code += "static gaia_container_t<c_gaia_type_{{TABLE_NAME}}, {{TABLE_NAME}}_t>& list() {";
    code.IncrementIdentLevel();
    code += "static gaia_container_t<c_gaia_type_{{TABLE_NAME}}, {{TABLE_NAME}}_t> list;";
    code += "return list;";
    code.DecrementIdentLevel();
    code += "}";

    // Iterate over the relationships where the current table appear as parent
    for (auto& relationship : parent_relationships)
    {
        code.SetValue("REF_TABLE", relationship.child_gaia_table().name());

        if (strlen(relationship.name()))
        {
            code.SetValue("REF_NAME", relationship.name());

            code += "{{REF_NAME}}_{{REF_TABLE}}_list_t "
                    "{{REF_NAME}}_{{REF_TABLE}}_list() {";

            code.IncrementIdentLevel();
            code += "return {{REF_NAME}}_{{REF_TABLE}}_list_t(gaia_id());";
        }
        else
        {
            // This relationship is anonymous.
            code.SetValue("REF_NAME", relationship.child_gaia_table().name());

            code += "{{REF_NAME}}_list_t {{REF_NAME}}_list() {";

            code.IncrementIdentLevel();
            code += "return {{REF_NAME}}_list_t(gaia_id());";
        }
        code.DecrementIdentLevel();
        code += "}";
    }

    // The private area.
    code.DecrementIdentLevel();
    code += "private:";
    code.IncrementIdentLevel();
    code += "friend struct gaia_object_t<c_gaia_type_{{TABLE_NAME}}, {{TABLE_NAME}}_t, {{TABLE_NAME}}, "
            "{{TABLE_NAME}}T, c_num_{{TABLE_NAME}}_ptrs>;";

    // The constructor.
    code += "explicit {{TABLE_NAME}}_t(gaia_id_t id) : gaia_object_t(id, \"{{TABLE_NAME}}_t\") {}";

    // Finishing brace.
    code.DecrementIdentLevel();
    code += "};";
    code += "";

    string str = code.ToString();
    return str;
}

string gaia_generate(const string& dbname)
{
    gaia_id_t db_id = find_db_id(dbname);
    if (db_id == c_invalid_gaia_id)
    {
        throw db_not_exists(dbname);
    }

    string code_lines;
    begin_transaction();

    std::map<uint8_t, gaia_relationship_t> relationships;

    code_lines = generate_boilerplate_top(dbname);

    code_lines += generate_constant_list(db_id);

    code_lines += generate_declarations(db_id);

    // This is to workaround the issue of incomplete forward declaration of structs that refer to each other.
    // By collecting the IDs in the sorted set, the structs are generated in the ascending order of their IDs.
    set<gaia_id_t> table_ids;
    for (auto const& table : gaia_database_t::get(db_id).gaia_table_list())
    {
        table_ids.insert(table.gaia_id());
    }
    for (auto table_id : table_ids)
    {
        field_vector_t field_strings;
        auto table_record = gaia_table_t::get(table_id);
        for (auto field_id : list_fields(table_id))
        {
            gaia_field_t field_record(gaia_field_t::get(field_id));
            field_strings.push_back(
                field_strings_t{field_record.name(), static_cast<data_type_t>(field_record.type())});
        }

        relationship_vector_t parent_relationships = list_parent_relationships(table_record);
        relationship_vector_t child_relationships = list_child_relationships(table_record);

        code_lines += generate_edc_struct(
            table_id,
            table_record,
            field_strings,
            parent_relationships,
            child_relationships);
    }
    commit_transaction();

    code_lines += generate_boilerplate_bottom(dbname);

    return code_lines;
}
} // namespace catalog
} // namespace gaia