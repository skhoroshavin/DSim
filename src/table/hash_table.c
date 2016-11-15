
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
    struct dsim_hash_table * t = container_of( self, struct dsim_hash_table, table );

    uint32_t begin = 0;
    uint32_t end = 0;

    for( uint32_t i = 0; i < count; ++i )
    {
        uint32_t pos = dsim_hash_table_find( self, start_id + i );

        if( begin == end )
        {
            begin = pos;
            end = begin + 1;
            continue;
        }

        if( end == pos )
        {
            ++end;
            continue;
        }

        dsim_hash_table_remove_range( self, begin, end - begin );
        begin = pos;
        end = begin + 1;
    }

    if( end > begin )
        dsim_hash_table_remove_range( self, begin, end - begin );
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

    .find   = dsim_hash_table_find,
    .insert = dsim_hash_table_insert,
    .remove = dsim_hash_table_remove,
    .reset  = dsim_hash_table_reset
};
