
#pragma once

#include "array.h"

DSIM_BEGIN_HEADER

struct dsim_hash
{
    struct dsim_array_uint64 keys;
    struct dsim_array_uint32 _hash;
    struct dsim_array_uint32 _next;
};

#define dsim_hash_static_init(alloc) { \
    /* .keys  = */ dsim_array_static_init(alloc), \
    /* ._hash = */ dsim_array_static_init(alloc), \
    /* ._next = */ dsim_array_static_init(alloc) }

uint32_t dsim_hash_find( const struct dsim_hash *h, uint64_t key );
uint32_t dsim_hash_find_next( const struct dsim_hash *h, uint32_t pos );
uint32_t dsim_hash_count_of( const struct dsim_hash *h, uint64_t key );
void dsim_hash_reserve( struct dsim_hash *h, uint32_t count );
void dsim_hash_push_back( struct dsim_hash *h, uint64_t key );
void dsim_hash_push_back_n( struct dsim_hash *h, const uint64_t * keys, uint32_t count );
void dsim_hash_remove_fast( struct dsim_hash *h, uint32_t pos, uint32_t count );
void dsim_hash_clear( struct dsim_hash *h );
void dsim_hash_reset( struct dsim_hash *h  );

DSIM_END_HEADER
