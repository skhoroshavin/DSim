
#include "hash.h"
#include <memory.h>

#define DSIM_HASH_INITIAL_HASH_SIZE     16
#define DSIM_HASH_INITIAL_DATA_SIZE      8

typedef struct _dsim_hash_set_find_result_
{
    dsim_index_t hash_index;
    dsim_index_t data_index;
    dsim_index_t data_prev_index;
} _dsim_hash_find_result;

#define _dsim_create_hash_set_find_result() { \
    .hash_index = DSIM_INVALID_INDEX, \
    .data_index = DSIM_INVALID_INDEX, \
    .data_prev_index = DSIM_INVALID_INDEX } \

static int _dsim_hash_is_full( const dsim_hash *h )
{
    return h->_keys.count >= h->_hash.count * 11 / 16;
}

static _dsim_hash_find_result _dsim_hash_find( const dsim_hash *h, dsim_id_t key )
{
    _dsim_hash_find_result res = _dsim_create_hash_set_find_result();
    if( !h->_hash.count )
        return res;

    res.hash_index = key % h->_hash.count;
    res.data_index = h->_hash.data[res.hash_index];
    while( res.data_index != DSIM_INVALID_INDEX )
    {
        if( h->_keys.data[res.data_index] == key )
            return res;
        res.data_prev_index = res.data_index;
        res.data_index = h->_next.data[res.data_index];
    }

    return res;
}

static dsim_index_t _dsim_hash_append( dsim_hash *h, dsim_id_t key )
{
    dsim_index_t result = h->_keys.count;
    dsim_id_array_push_back( &h->_keys, key );
    dsim_index_array_push_back( &h->_next, DSIM_INVALID_INDEX );
    return result;
}

static dsim_index_t _dsim_hash_make( dsim_hash *h, dsim_id_t key )
{
    const _dsim_hash_find_result fr = _dsim_hash_find( h, key );
    const dsim_index_t i = _dsim_hash_append( h, key );

    if( fr.data_prev_index == DSIM_INVALID_INDEX )
        h->_hash.data[fr.hash_index] = i;
    else
        h->_next.data[fr.data_prev_index] = i;

    h->_next.data[i] = fr.data_index;
    return i;
}

static void _dsim_hash_rehash( dsim_hash *h, dsim_index_t count )
{
    if( count <= h->_hash.count )
        return;

    dsim_hash new_set = dsim_hash_init(h->_hash.alloc);
    dsim_index_array_resize( &new_set._hash, count );
    dsim_index_array_fill( &new_set._hash, 0xff );

    dsim_index_t key_capacity = max( DSIM_HASH_INITIAL_DATA_SIZE, h->_keys.capacity );
    dsim_id_array_reserve( &new_set._keys, key_capacity );
    dsim_index_array_reserve( &new_set._next, key_capacity );

    for( dsim_index_t i = 0; i != h->_keys.count; ++i )
        dsim_hash_insert_multi( &new_set, h->_keys.data[i] );

    dsim_hash_reset( h );
    memcpy( h, &new_set, sizeof(new_set) );
}

int dsim_hash_contains( const dsim_hash *h, dsim_id_t key )
{
    return _dsim_hash_find( h, key ).data_index != DSIM_INVALID_INDEX;
}

void dsim_hash_reserve( dsim_hash *h, dsim_index_t count )
{
    if( count <= h->_keys.count )
        return;

    _dsim_hash_rehash( h, dsim_next_pow_2(count*16/11) );
    dsim_id_array_reserve( &h->_keys, count );
    dsim_index_array_reserve( &h->_next, count );
}

dsim_index_t dsim_hash_insert( dsim_hash *h, dsim_id_t key )
{
    if( !h->_hash.count )
        _dsim_hash_rehash( h, DSIM_HASH_INITIAL_HASH_SIZE );

    /*
    const uint32_t i = hash_internal::find_or_make(h, key);
    */

    if( _dsim_hash_is_full(h) )
        _dsim_hash_rehash( h, 2*h->_hash.count );
}

dsim_index_t dsim_hash_insert_multi( dsim_hash *h, dsim_id_t key )
{
    if( !h->_hash.count )
        _dsim_hash_rehash( h, DSIM_HASH_INITIAL_HASH_SIZE );

    const dsim_index_t i = _dsim_hash_make( h, key );
    if( _dsim_hash_is_full(h) )
        _dsim_hash_rehash( h, 2*h->_hash.count );

    return i;
}

void dsim_hash_reset( dsim_hash *h )
{
    dsim_index_array_reset( &h->_hash );
    dsim_id_array_reset( &h->_keys );
    dsim_index_array_reset( &h->_next );
}
