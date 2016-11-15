
#pragma once

#include "array.h"

DSIM_BEGIN_HEADER

struct _dsim_hash
{
    struct dsim_uint64_array keys;
    struct _dsim_array       data;
    struct dsim_uint32_array _hash;
    struct dsim_uint32_array _next;
};

#define dsim_hash_static_init(alloc) { \
    /* .keys  = */ dsim_array_static_init(alloc), \
    /* .data  = */ dsim_array_static_init(alloc), \
    /* ._hash = */ dsim_array_static_init(alloc), \
    /* ._next = */ dsim_array_static_init(alloc) }

void _dsim_hash_reserve( struct _dsim_hash *h, uint32_t count, uint32_t elem_size );
void _dsim_hash_reset( struct _dsim_hash *h , uint32_t elem_size );

#define DSIM_DEFINE_HASH(type,short_type) \
    struct dsim_##short_type##_hash \
    { \
        struct dsim_uint64_array         keys; \
        struct dsim_##short_type##_array data; \
        struct dsim_uint32_array        _hash; \
        struct dsim_uint32_array        _next; \
    }; \
    inline static void dsim_##short_type##_hash_reserve( struct dsim_##short_type##_hash *h, uint32_t count ) \
    { _dsim_hash_reserve( (struct _dsim_hash*)h, count, sizeof(type) ); } \
    inline static void dsim_##short_type##_hash_reset( struct dsim_##short_type##_hash *h ) \
    { _dsim_hash_reset( (struct _dsim_hash*)h, sizeof(type) ); } \

DSIM_DEFINE_HASH(uint32_t, uint32)
DSIM_DEFINE_HASH(uint64_t, uint64)

DSIM_END_HEADER
