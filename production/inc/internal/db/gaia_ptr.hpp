/////////////////////////////////////////////
// Copyright (c) Gaia Platform LLC
// All rights reserved.
/////////////////////////////////////////////

#pragma once

#include <cstring>

#include "retail_assert.hpp"
#include "gaia_db.hpp"

using namespace gaia::common;

namespace gaia {
namespace db {

class gaia_ptr {
    // We need access to private implementation state of the client library.
    friend class client;

   private:
    int64_t row_id;

    struct object {
        gaia_id_t id;
        gaia_type_t type;
        size_t num_references;
        size_t payload_size;
        char payload[0];
    };

   public:
    gaia_ptr(const std::nullptr_t = nullptr)
        : row_id(0) {}

    gaia_ptr(const gaia_ptr& other)
        : row_id(other.row_id) {}

    bool operator==(const gaia_ptr& other) const {
        return row_id == other.row_id;
    }

    bool operator==(const std::nullptr_t) const {
        return to_ptr() == nullptr;
    }

    bool operator!=(const std::nullptr_t) const {
        return to_ptr() != nullptr;
    }

    operator bool() const {
        return to_ptr() != nullptr;
    }

    static gaia_id_t generate_id();

    static gaia_ptr create(
        gaia_id_t id,
        gaia_type_t type,
        size_t data_size,
        const void* data,
        bool log_updates = true) {
        return create(id, type, 0, data_size, data, log_updates);
    }

    static gaia_ptr create(
        gaia_id_t id,
        gaia_type_t type,
        size_t num_refs,
        size_t data_size,
        const void* data,
        bool log_updates = true) {
        size_t refs_len = num_refs * sizeof(gaia_id_t);
        size_t total_len = data_size + refs_len;
        gaia_ptr obj(id, total_len + sizeof(gaia_ptr::object), log_updates);
        gaia_ptr::object* obj_ptr = obj.to_ptr();
        obj_ptr->id = id;
        obj_ptr->type = type;
        obj_ptr->num_references = num_refs;
        if (num_refs) {
            memset(obj_ptr->payload, 0, refs_len);
        }
        obj_ptr->payload_size = total_len;
        memcpy(obj_ptr->payload + refs_len, data, data_size);
        return obj;
    }

    static gaia_ptr open(
        gaia_id_t id) {
        return gaia_ptr(id);
    }

    static void remove(gaia_ptr& node) {
        if (!node) {
            return;
        }

        const gaia_id_t* references = node.references();
        for (size_t i = 0; i < node.num_references(); i++) {
            if (references[i] != INVALID_GAIA_ID) {
                throw node_not_disconnected(node.id(), node.type());
            }
        }
        node.reset();
    }

    gaia_ptr& clone();

    gaia_ptr& update_payload(size_t data_size, const void* data);

    static gaia_ptr find_first(gaia_type_t type) {
        gaia_ptr ptr;
        ptr.row_id = 1;

        if (!ptr.is(type)) {
            ptr.find_next(type);
        }

        return ptr;
    }

    gaia_ptr find_next() {
        if (row_id) {
            find_next(to_ptr()->type);
        }

        return *this;
    }

    gaia_ptr operator++() {
        if (row_id) {
            find_next(to_ptr()->type);
        }
        return *this;
    }

    bool is_null() const {
        return row_id == 0;
    }

    int64_t id() const {
        return to_ptr()->id;
    }

    gaia_type_t type() const {
        return to_ptr()->type;
    }

    char* data() const {
        return data_size() ? (char*)(to_ptr()->payload + (to_ptr()->num_references * sizeof(gaia_id_t))) : nullptr;
    }

    size_t data_size() const {
        size_t total_len = to_ptr()->payload_size;
        size_t refs_len = to_ptr()->num_references * sizeof(gaia_id_t);
        size_t data_size = total_len - refs_len;
        return data_size;
    }

    gaia_id_t* references() const {
        return reinterpret_cast<gaia_id_t*>(to_ptr()->payload);
    }

    size_t num_references() const {
        return to_ptr()->num_references;
    }

   protected:
    gaia_ptr(const gaia_id_t id);

    gaia_ptr(const gaia_id_t id, const size_t size, bool log_updates = true);

    void allocate(const size_t size);

    gaia_ptr::object* to_ptr() const;

    int64_t to_offset() const;

    bool is(gaia_type_t type) const {
        return to_ptr() && to_ptr()->type == type;
    }

    void find_next(gaia_type_t type);

    void reset();
};

}  // namespace db
}  // namespace gaia