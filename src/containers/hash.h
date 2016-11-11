
#pragma once

#include "array.h"

DSIM_BEGIN_HEADER

typedef struct dsim_hash_
{
    dsim_uint64_array keys;

    dsim_uint32_array _hash;
    dsim_uint32_array _next;
} dsim_hash;

#define dsim_hash_init(alloc) { \
    /* .keys  = */ dsim_array_init(alloc), \
    /* ._hash = */ dsim_array_init(alloc), \
    /* ._next = */ dsim_array_init(alloc) }

int dsim_hash_contains( const dsim_hash *h, uint64_t key );

void dsim_hash_reserve( dsim_hash *h, uint32_t count );
uint32_t dsim_hash_insert( dsim_hash *h, uint64_t key );
uint32_t dsim_hash_insert_multi( dsim_hash *h, uint64_t key );
void dsim_hash_reset( dsim_hash *h );

DSIM_END_HEADER
