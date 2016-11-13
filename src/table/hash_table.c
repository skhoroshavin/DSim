
#include "hash_table.h"
#include <memory.h>

static uint32_t dsim_hash_table_column_count( const struct dsim_table *self )
{
    const struct dsim_hash_table * t = container_of( self, const struct dsim_hash_table, table );

    return t->column_count;
}

static uint32_t dsim_hash_table_column_size( const struct dsim_table *self, uint32_t col )
{
    const struct dsim_hash_table * t = container_of( self, const struct dsim_hash_table, table );

    assert( col < t->column_count );
    return t->columns[col].width;
}

static uint32_t dsim_hash_table_block_count( const struct dsim_table *self )
{
    return 1;
}

static uint32_t dsim_hash_table_block_size( const struct dsim_table *self, uint32_t block )
{
    const struct dsim_hash_table * t = container_of( self, const struct dsim_hash_table, table );

    assert( block == 0 );
    return t->ids.count;
}

static const uint64_t *dsim_hash_table_id_data( const struct dsim_table *self, uint32_t block )
{
    const struct dsim_hash_table * t = container_of( self, const struct dsim_hash_table, table );

    assert( block == 0 );
    return t->ids.data;
}

static void *dsim_hash_table_data( struct dsim_table *self, uint32_t block, uint32_t col )
{
    struct dsim_hash_table * t = container_of( self, struct dsim_hash_table, table );

    assert( block == 0 );
    assert( col < t->column_count );
    return t->columns[col].data.data;
}

static uint32_t dsim_hash_table_find( const struct dsim_table *self, uint64_t id )
{
    const struct dsim_hash_table * t = container_of( self, const struct dsim_hash_table, table );

    for( uint32_t i = 0; i < t->ids.count; ++i )
        if( t->ids.data[i] == id )
            return i;
    return DSIM_INVALID_INDEX;
}

static void dsim_hash_table_insert( struct dsim_table *self, uint64_t start_id, uint32_t count )
{
    struct dsim_hash_table * t = container_of( self, struct dsim_hash_table, table );

    size_t new_count = t->ids.count + count;

    dsim_uint64_array_reserve( &t->ids, new_count );
    for( uint32_t i = 0; i < count; ++i )
        dsim_uint64_array_push_back( &t->ids, start_id + i );

    for( uint32_t i = 0; i != t->column_count; ++i )
        _dsim_array_resize( &t->columns[i].data, new_count, t->columns[i].width );
}

static void dsim_hash_table_remove( struct dsim_table *self, uint64_t start_id, uint32_t count )
{
    struct dsim_hash_table * t = container_of( self, struct dsim_hash_table, table );

    for( uint32_t i = 0; i < count; ++i )
    {
        uint32_t src_i = t->ids.count - count + i;
        uint32_t dst_i = dsim_hash_table_find( self, start_id + i );
        assert( dst_i != DSIM_INVALID_INDEX );
        if( src_i == dst_i ) continue;

        t->ids.data[dst_i] = t->ids.data[src_i];
        for( uint32_t j = 0; j < t->column_count; ++j )
        {
            uint8_t * data = (uint8_t*)t->columns[j].data.data;
            uint32_t elem_size = t->columns[j].width;
            memcpy( data + dst_i*elem_size, data + dst_i*elem_size, elem_size );
        }
    }

    t->ids.count -= count;
    for( uint32_t i = 0; i <t->column_count; ++i )
        t->columns[i].data.count -= count;
}

static void dsim_hash_table_reset( struct dsim_table *self )
{
    struct dsim_hash_table * t = container_of( self, struct dsim_hash_table, table );

    dsim_uint64_array_reset( &t->ids );
    for( uint32_t i = 0; i != t->column_count; ++i )
        _dsim_array_reset( &t->columns[i].data, t->columns[i].width );
};

struct dsim_table_operations dsim_hash_table_ops =
{
    .column_count = dsim_hash_table_column_count,
    .column_size = dsim_hash_table_column_size,
    .block_count = dsim_hash_table_block_count,
    .block_size = dsim_hash_table_block_size,

    .id_data = dsim_hash_table_id_data,
    .data = dsim_hash_table_data,

    .find   = dsim_hash_table_find,
    .insert = dsim_hash_table_insert,
    .remove = dsim_hash_table_remove,
    .reset  = dsim_hash_table_reset
};
