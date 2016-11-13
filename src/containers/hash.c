
#include "hash.h"
#include <memory.h>

#define DSIM_HASH_INITIAL_HASH_SIZE     16
#define DSIM_HASH_INITIAL_DATA_SIZE      8

struct _dsim_hash_find_result
{
    uint32_t hash_index;
    uint32_t data_index;
    uint32_t data_prev_index;
};

#define _dsim_create_hash_set_find_result() { \
    .hash_index = DSIM_INVALID_INDEX, \
    .data_index = DSIM_INVALID_INDEX, \
    .data_prev_index = DSIM_INVALID_INDEX } \

static int _dsim_hash_is_full( const struct _dsim_hash *h )
{
    return h->keys.count >= h->_hash.count * 11 / 16;
}

static struct _dsim_hash_find_result _dsim_hash_find( const struct _dsim_hash *h, uint64_t key )
{
    struct _dsim_hash_find_result res = _dsim_create_hash_set_find_result();
    if( !h->_hash.count )
        return res;

    res.hash_index = key % h->_hash.count;
    res.data_index = h->_hash.data[res.hash_index];
    while( res.data_index != DSIM_INVALID_INDEX )
    {
        if( h->keys.data[res.data_index] == key )
            return res;
        res.data_prev_index = res.data_index;
        res.data_index = h->_next.data[res.data_index];
    }

    return res;
}

static uint32_t _dsim_hash_append( struct _dsim_hash *h, uint64_t key )
{
    uint32_t result = h->keys.count;
    dsim_uint64_array_push_back( &h->keys, key );
    dsim_uint32_array_push_back( &h->_next, DSIM_INVALID_INDEX );
    return result;
}

static uint32_t _dsim_hash_make( struct _dsim_hash *h, uint64_t key )
{
    const struct _dsim_hash_find_result fr = _dsim_hash_find( h, key );
    const uint32_t i = _dsim_hash_append( h, key );

    if( fr.data_prev_index == DSIM_INVALID_INDEX )
        h->_hash.data[fr.hash_index] = i;
    else
        h->_next.data[fr.data_prev_index] = i;

    h->_next.data[i] = fr.data_index;
    return i;
}

uint32_t _dsim_hash_insert_multi( struct _dsim_hash *h, uint64_t key, uint32_t elem_size );

static void _dsim_hash_rehash( struct _dsim_hash *h, uint32_t count, uint32_t elem_size )
{
    if( count <= h->_hash.count )
        return;

    struct _dsim_hash new_set = dsim_hash_static_init(h->_hash.alloc);
    dsim_uint32_array_resize( &new_set._hash, count );
    dsim_uint32_array_fill( &new_set._hash, 0xff );

    uint32_t key_capacity = max( DSIM_HASH_INITIAL_DATA_SIZE, h->keys.capacity );
    dsim_uint64_array_reserve( &new_set.keys, key_capacity );
    dsim_uint32_array_reserve( &new_set._next, key_capacity );

    for( uint32_t i = 0; i != h->keys.count; ++i )
        _dsim_hash_insert_multi( &new_set, h->keys.data[i], elem_size );

    _dsim_hash_reset( h, elem_size );
    memcpy( h, &new_set, sizeof(new_set) );
}

int _dsim_hash_contains( const struct _dsim_hash *h, uint64_t key )
{
    return _dsim_hash_find( h, key ).data_index != DSIM_INVALID_INDEX;
}

void _dsim_hash_reserve( struct _dsim_hash *h, uint32_t count, uint32_t elem_size )
{
    if( count <= h->keys.count )
        return;

    dsim_uint64_array_reserve( &h->keys, count );
    _dsim_array_reserve( &h->data, count, elem_size );

    dsim_uint32_array_reserve( &h->_next, count );
    //_dsim_hash_rehash( h, dsim_next_pow_2(count*16/11), elem_size );
}

uint32_t _dsim_hash_insert( struct _dsim_hash *h, uint64_t key, uint32_t elem_size )
{
    if( !h->_hash.count )
        _dsim_hash_rehash( h, DSIM_HASH_INITIAL_HASH_SIZE, elem_size );

    /*
    const uint32_t i = hash_internal::find_or_make(h, key);
    */

    if( _dsim_hash_is_full(h) )
        _dsim_hash_rehash( h, 2*h->_hash.count, elem_size );
}

uint32_t _dsim_hash_insert_multi( struct _dsim_hash *h, uint64_t key, uint32_t elem_size )
{
    if( !h->_hash.count )
        _dsim_hash_rehash( h, DSIM_HASH_INITIAL_HASH_SIZE, elem_size );

    const uint32_t i = _dsim_hash_make( h, key );
    if( _dsim_hash_is_full(h) )
        _dsim_hash_rehash( h, 2*h->_hash.count, elem_size );

    return i;
}

void _dsim_hash_reset( struct _dsim_hash *h, uint32_t elem_size )
{
    dsim_uint64_array_reset( &h->keys );
    _dsim_array_reset( &h->data, elem_size );
    dsim_uint32_array_reset( &h->_hash );
    dsim_uint32_array_reset( &h->_next );
}
