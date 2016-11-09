
#pragma once

#include "array.h"

DSIM_BEGIN_HEADER

typedef struct dsim_hash_
{
    dsim_index_array _hash;
    dsim_id_array    _keys;
    dsim_index_array _next;
} dsim_hash;

#define dsim_hash_init(alloc) { \
    /* ._hash = */ dsim_array_init(alloc), \
    /* ._keys = */ dsim_array_init(alloc), \
    /* ._next = */ dsim_array_init(alloc) }

int dsim_hash_contains( const dsim_hash *h, dsim_id_t key );

void dsim_hash_reserve( dsim_hash *h, dsim_index_t count );
dsim_index_t dsim_hash_insert( dsim_hash *h, dsim_id_t key );
dsim_index_t dsim_hash_insert_multi( dsim_hash *h, dsim_id_t key );
void dsim_hash_reset( dsim_hash *h );

DSIM_END_HEADER
