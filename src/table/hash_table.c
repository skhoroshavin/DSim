
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
    return t->columns[col].elem_size;
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

static dsim_table_index dsim_hash_table_find( const struct dsim_table *self, uint64_t id )
{
    dsim_table_index result =
    {
        .block = 0,
        .index = DSIM_INVALID_INDEX
    };

    const struct dsim_hash_table * t = container_of( self, const struct dsim_hash_table, table );

    for( uint32_t i = 0; i < t->ids.count; ++i )
        if( t->ids.data[i] == id )
        {
            result.index = i;
            break;
        }
    return result;
}

static void dsim_hash_table_find_range( const struct dsim_table *self, uint64_t start_id, uint32_t count, struct dsim_table_range_array * result )
{
    const struct dsim_hash_table * t = container_of( self, const struct dsim_hash_table, table );

    struct dsim_table_range range =
    {
        .block = 0,
        .start_index = 0,
        .count = 0
    };

    for( uint32_t i = 0; i < count; ++i )
    {
        dsim_table_index pos = dsim_hash_table_find( self, start_id + i );

        range.start_index = pos.index;
        range.count = count - i;
        i = count-1;
        for( uint32_t j = 1; j < count-i; ++j )
            if( (pos.index + j >= t->ids.count) || (t->ids.data[pos.index+j] != start_id+i+j) )
            {
                range.count = j;
                i = j-1;
                break;
            }

        dsim_table_range_array_push_back( result, range );
    }
}

static void dsim_hash_table_insert( struct dsim_table *self, uint64_t start_id, uint32_t count )
{
    struct dsim_hash_table * t = container_of( self, struct dsim_hash_table, table );

    size_t new_count = t->ids.count + count;

    dsim_uint64_array_reserve( &t->ids, new_count );
    for( uint32_t i = 0; i < count; ++i )
        dsim_uint64_array_push_back( &t->ids, start_id + i );

    for( uint32_t i = 0; i != t->column_count; ++i )
        dsim_table_column_resize( t->columns + i, new_count );

    dsim_table_log_cmd_push_back( &self->log, 0, start_id, count );
}

static void dsim_hash_table_remove_range( struct dsim_table *self, uint32_t pos, uint32_t count )
{
    struct dsim_hash_table * t = container_of( self, struct dsim_hash_table, table );

    dsim_uint64_array_remove_fast( &t->ids, pos, count );
    for( uint32_t i = 0; i < t->column_count; ++i )
        dsim_table_column_remove_fast( t->columns + i, pos, count );

    dsim_table_log_cmd_remove_fast( &self->log, 0, pos, count );
}

static void dsim_hash_table_remove( struct dsim_table *self, uint64_t start_id, uint32_t count )
{
    struct dsim_table_range_array range_list = dsim_array_static_init(&dsim_default_allocator);

    dsim_hash_table_find_range( self, start_id, count, &range_list );
    for( uint32_t i = 0; i != range_list.count; ++i )
        dsim_hash_table_remove_range( self, range_list.data[i].start_index, range_list.data[i].count );

    dsim_table_range_array_reset( &range_list );
}

static void dsim_hash_table_reset( struct dsim_table *self )
{
    struct dsim_hash_table * t = container_of( self, struct dsim_hash_table, table );

    dsim_uint64_array_reset( &t->ids );
    for( uint32_t i = 0; i != t->column_count; ++i )
        dsim_table_column_reset( t->columns + i );
};

struct dsim_table_operations dsim_hash_table_ops =
{
    .column_count = dsim_hash_table_column_count,
    .column_size = dsim_hash_table_column_size,
    .block_count = dsim_hash_table_block_count,
    .block_size = dsim_hash_table_block_size,

    .id_data = dsim_hash_table_id_data,
    .data = dsim_hash_table_data,

    .find       = dsim_hash_table_find,
    .find_range = dsim_hash_table_find_range,

    .insert = dsim_hash_table_insert,
    .remove = dsim_hash_table_remove,
    .reset  = dsim_hash_table_reset
};
