
#include "hash.h"
#include <memory.h>

static void _dsim_hash_set_prev( struct dsim_hash *h, uint32_t pos, uint32_t dst )
{
    uint32_t hash_i = dsim_hash64( h->keys.at[pos] ) % h->_hash.count;
    uint32_t prev_i = DSIM_INVALID_INDEX;

    uint32_t i = h->_hash.at[hash_i];
    while( i != pos )
    {
        prev_i = i;
        i = h->_next.at[i];

        if( i == DSIM_INVALID_INDEX )
        {
            assert( 0 ); // Should never reach this!
            return;
        }
    }

    if( prev_i == DSIM_INVALID_INDEX )
        h->_hash.at[hash_i] = dst;
    else
        h->_next.at[prev_i] = dst;
}

static void _dsim_hash_key_insert( struct dsim_hash *h, uint32_t pos )
{
    uint32_t hash_i = dsim_hash64( h->keys.at[pos] ) % h->_hash.count;
    h->_next.at[pos] = h->_hash.at[hash_i];
    h->_hash.at[hash_i] = pos;
}

inline static void _dsim_hash_key_delete( struct dsim_hash *h, uint32_t pos )
{
    _dsim_hash_set_prev( h, pos, h->_next.at[pos] );
}

static void _dsim_hash_key_move_n( struct dsim_hash *h, uint32_t dst, uint32_t src, uint32_t count )
{
    for( uint32_t i = 0; i < count; ++i )
        _dsim_hash_key_delete( h, dst + i );

    count = min( count, h->keys.count - src );
    for( uint32_t i = 0; i < count; ++i )
        _dsim_hash_set_prev( h, src + i, dst + i );

    memcpy( h->keys.data + dst,  h->keys.data + src, count*sizeof(h->keys.at[0]) );
    memcpy( h->_next.data + dst, h->_next.data + src, count*sizeof(h->_next.at[0]) );
}

static void _dsim_hash_rehash( struct dsim_hash *h, uint32_t count )
{
    dsim_uint32_array_resize( &h->_hash, count );
    dsim_fill( h->_hash.data, DSIM_INVALID_INDEX, count );

    for( uint32_t i = 0; i < h->keys.count; ++i )
        _dsim_hash_key_insert( h, i );
}

uint32_t dsim_hash_find( const struct dsim_hash *h, uint64_t key )
{
    if( h->_hash.count == 0 )
        return DSIM_INVALID_INDEX;

    uint32_t hash_i = dsim_hash64( key ) % h->_hash.count;
    for( uint32_t i = h->_hash.at[hash_i]; i != DSIM_INVALID_INDEX; i = h->_next.at[i] )
        if( h->keys.at[i] == key )
            return i;

    return DSIM_INVALID_INDEX;
}

uint32_t dsim_hash_find_next( const struct dsim_hash *h, uint32_t pos )
{
    if( pos == DSIM_INVALID_INDEX )
        return DSIM_INVALID_INDEX;

    uint64_t key = h->keys.at[pos];
    for( uint32_t i = h->_next.at[pos]; i != DSIM_INVALID_INDEX; i = h->_next.at[i] )
        if( h->keys.at[i] == key )
            return i;

    return DSIM_INVALID_INDEX;
}

uint32_t dsim_hash_count_of( const struct dsim_hash *h, uint64_t key )
{
    uint32_t result = 0;
    for( uint32_t i = dsim_hash_find( h, key ); i != DSIM_INVALID_INDEX; i = dsim_hash_find_next( h, i ) )
        ++result;
    return result;
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

void dsim_hash_push_back( struct dsim_hash *h, uint64_t key )
{
    if( h->keys.count == h->keys.capacity )
        dsim_hash_reserve( h, dsim_next_pow_2(h->keys.count + 8) );

    dsim_uint64_array_push_back( &h->keys, key );
    dsim_uint32_array_push_back( &h->_next, DSIM_INVALID_INDEX );
    _dsim_hash_key_insert( h, h->keys.count-1 );
}

void dsim_hash_push_back_n( struct dsim_hash *h, const uint64_t * keys, uint32_t count )
{
    if( h->keys.count + count > h->keys.capacity )
        dsim_hash_reserve( h, dsim_next_pow_2(h->keys.count + count) );

    uint32_t start_pos = h->keys.count;
    dsim_uint64_array_push_back_n( &h->keys, keys, count );
    dsim_uint32_array_resize( &h->_next, start_pos + count );
    dsim_fill( h->_next.data + start_pos, DSIM_INVALID_INDEX, count );

    for( uint32_t i = 0; i < count; ++i )
        _dsim_hash_key_insert( h, start_pos + i );
}

void dsim_hash_remove_fast( struct dsim_hash *h, uint32_t pos, uint32_t count )
{
    if( h->keys.count - pos - count < count )
        _dsim_hash_key_move_n( h, pos, pos + count, count );
    else
        _dsim_hash_key_move_n( h, pos, h->keys.count - count, count );

    dsim_uint64_array_pop_back_n( &h->keys, count );
    dsim_uint32_array_pop_back_n( &h->_next, count );
}

void dsim_hash_clear( struct dsim_hash *h )
{
    dsim_uint64_array_clear( &h->keys );
    dsim_uint32_array_clear( &h->_next );
    dsim_fill( h->_hash.data, DSIM_INVALID_INDEX, h->_hash.count );
}

void dsim_hash_reset( struct dsim_hash *h )
{
    dsim_uint64_array_reset( &h->keys );
    dsim_uint32_array_reset( &h->_hash );
    dsim_uint32_array_reset( &h->_next );
}
