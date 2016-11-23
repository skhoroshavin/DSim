
#include "hash.h"
#include <memory.h>

struct dsim_hash_find_result
{
    uint32_t key_i;
    uint32_t prev_i;
    uint32_t hash_i;
};

#define dsim_create_hash_find_result() { \
    .key_i  = DSIM_INVALID_INDEX, \
    .prev_i = DSIM_INVALID_INDEX, \
    .hash_i = DSIM_INVALID_INDEX } \

static struct dsim_hash_find_result _dsim_hash_find( const struct dsim_hash *h, uint64_t key )
{
    struct dsim_hash_find_result r = dsim_create_hash_find_result();
    if( h->_hash.count == 0 )
        return r;

    r.hash_i = dsim_hash64( key ) % h->_hash.count;
    r.key_i = h->_hash.data[r.hash_i];
    while( r.key_i != DSIM_INVALID_INDEX )
    {
        if( h->keys.data[r.key_i] == key )
            return r;

        r.prev_i = r.key_i;
        r.key_i = h->_next.data[r.key_i];
    }

    return r;
}

static void _dsim_hash_key_insert( struct dsim_hash *h, uint32_t pos )
{
    uint32_t hash_i = dsim_hash64( h->keys.data[pos] ) % h->_hash.count;
    h->_next.data[pos] = h->_hash.data[hash_i];
    h->_hash.data[hash_i] = pos;
}

static void _dsim_hash_key_remove( struct dsim_hash *h, uint32_t pos, uint32_t src )
{
    struct dsim_hash_find_result fr = _dsim_hash_find( h, h->keys.data[pos] );

    if( fr.prev_i == DSIM_INVALID_INDEX )
        h->_hash.data[fr.hash_i] = h->_next.data[pos];
    else
        h->_next.data[fr.prev_i] = h->_next.data[pos];

    if( src < h->keys.count )
    {
        fr = _dsim_hash_find( h, h->keys.data[src] );
        if( fr.prev_i == DSIM_INVALID_INDEX )
            h->_hash.data[fr.hash_i] = pos;
        else
            h->_next.data[fr.prev_i] = pos;

        h->keys.data[pos] = h->keys.data[src];
        h->_next.data[pos] = h->_next.data[src];
    }
}

static void _dsim_hash_rehash( struct dsim_hash *h, uint32_t count )
{
    dsim_uint32_array_resize( &h->_hash, count );
    dsim_uint32_array_fill( &h->_hash, DSIM_INVALID_INDEX );

    for( uint32_t i = 0; i < h->keys.count; ++i )
        _dsim_hash_key_insert( h, i );
}

uint32_t dsim_hash_find( const struct dsim_hash *h, uint64_t key )
{
    return _dsim_hash_find( h, key ).key_i;
}

void dsim_hash_reserve( struct dsim_hash *h, uint32_t count )
{
    if( count <= h->keys.count )
        return;

    dsim_uint64_array_reserve( &h->keys, count );
    dsim_uint32_array_reserve( &h->_next, count );
    if( h->_hash.count * 11 / 16 < count )
        _dsim_hash_rehash( h, 2*count );
}

void dsim_hash_insert( struct dsim_hash *h, uint64_t start_key, uint32_t count )
{
    if( dsim_hash_find( h, start_key ) != DSIM_INVALID_INDEX )
        return;

    if( h->keys.count + count > h->keys.capacity )
        dsim_hash_reserve( h, dsim_next_pow_2(h->keys.count + count) );

    for( uint32_t i = 0; i < count; ++i )
    {
        dsim_uint64_array_push_back( &h->keys, start_key + i );
        dsim_uint32_array_push_back( &h->_next, DSIM_INVALID_INDEX );
        _dsim_hash_key_insert( h, h->keys.count-1 );
    }
}

void dsim_hash_remove_range( struct dsim_hash *h, uint32_t start, uint32_t count )
{
    if( h->keys.count - start - count < count )
    {
        for( uint32_t i = 0; i < count; ++i )
            _dsim_hash_key_remove( h, i + start, i + start + count );
    }
    else
    {
        for( uint32_t i = 0; i < count; ++i )
            _dsim_hash_key_remove( h, i + start, i + h->keys.count - count );
    }

    dsim_uint64_array_pop_back_n( &h->keys, count );
    dsim_uint32_array_pop_back_n( &h->_next, count );
}

void dsim_hash_clear( struct dsim_hash *h )
{
    dsim_uint64_array_clear( &h->keys );
    dsim_uint32_array_clear( &h->_next );
    dsim_uint32_array_fill( &h->_hash, DSIM_INVALID_INDEX );
}

void dsim_hash_reset( struct dsim_hash *h )
{
    dsim_uint64_array_reset( &h->keys );
    dsim_uint32_array_reset( &h->_hash );
    dsim_uint32_array_reset( &h->_next );
}
