
#pragma once

#include "array.h"

DSIM_BEGIN_HEADER

typedef struct _dsim_hash_
{
    dsim_uint64_array keys;
    _dsim_array       data;
    dsim_uint32_array _hash;
    dsim_uint32_array _next;
} _dsim_hash;

#define dsim_hash_init(alloc) { \
    /* .keys  = */ dsim_array_init(alloc), \
    /* .data  = */ dsim_array_init(alloc), \
    /* ._hash = */ dsim_array_init(alloc), \
    /* ._next = */ dsim_array_init(alloc) }

void _dsim_hash_reserve( _dsim_hash *h, uint32_t count, uint32_t elem_size );
void _dsim_hash_reset( _dsim_hash *h , uint32_t elem_size );

#define DSIM_DEFINE_HASH(type) \
    typedef struct dsim_##type##_hash_ \
    { \
        dsim_uint64_array   keys; \
        dsim_##type##_array data; \
        dsim_uint32_array   _hash; \
        dsim_uint32_array   _next; \
    } dsim_##type##_hash; \
    inline static void dsim_##type##_hash_reserve( dsim_##type##_hash *h, uint32_t count ) \
    { _dsim_hash_reserve( (_dsim_hash*)h, count, sizeof(type) ); } \
    inline static void dsim_##type##_hash_reset( dsim_##type##_hash *h ) \
    { _dsim_hash_reset( (_dsim_hash*)h, sizeof(type) ); } \

DSIM_DEFINE_HASH(uint32)
DSIM_DEFINE_HASH(uint64)

DSIM_END_HEADER
