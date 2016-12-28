
#pragma once

#include "array.h"

DSIM_BEGIN_HEADER

struct dsim_hash
{
    dsim_array(uint64_t) keys;
    dsim_array(uint32_t) _hash;
    dsim_array(uint32_t) _next;
};

#define dsim_hash_static_init(alloc) { \
    /* .keys  = */ dsim_array_static_init(alloc), \
    /* ._hash = */ dsim_array_static_init(alloc), \
    /* ._next = */ dsim_array_static_init(alloc) }

void dsim_hash_init( struct dsim_hash *h, struct dsim_allocator *alloc );
uint32_t dsim_hash_find( const struct dsim_hash *h, uint64_t key );
uint32_t dsim_hash_find_next( const struct dsim_hash *h, uint32_t pos );
void dsim_hash_reserve( struct dsim_hash *h, uint32_t count );
void dsim_hash_push_back( struct dsim_hash *h, uint64_t key );
void dsim_hash_push_back_n( struct dsim_hash *h, const uint64_t *keys, uint32_t count );
void dsim_hash_remove_fast( struct dsim_hash *h, uint32_t pos, uint32_t count );
void dsim_hash_clear( struct dsim_hash *h );
void dsim_hash_reset( struct dsim_hash *h  );

DSIM_END_HEADER
