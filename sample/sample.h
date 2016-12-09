
#pragma once

#include "storage/hash_storage.h"

#include "dsim_lua.h"

#include <stdio.h>
#include <memory.h>

#define BEGIN_STORAGE(name) \
    static struct dsim_storage_array name##_arrays[] = {

#define END_STORAGE(name) \
    }; \
    static struct dsim_hash_storage name##_hash = dsim_hash_storage_static_init( \
        count_of(name##_arrays), name##_arrays, &dsim_default_allocator ); \
    static struct dsim_storage * strg_##name = &name##_hash.storage;
