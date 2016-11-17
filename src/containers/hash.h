
#pragma once

#include "array.h"

DSIM_BEGIN_HEADER

struct dsim_hash
{
    struct dsim_uint64_array keys;
    struct dsim_uint32_array _hash;
    struct dsim_uint32_array _next;
};

#define dsim_hash_static_init(alloc) { \
    /* .keys  = */ dsim_array_static_init(alloc), \
    /* ._hash = */ dsim_array_static_init(alloc), \
    /* ._next = */ dsim_array_static_init(alloc) }

void dsim_hash_reserve( struct dsim_hash *h, uint32_t count );
void dsim_hash_reset( struct dsim_hash *h  );

DSIM_END_HEADER
