
#include "hash_storage.h"
#include "allocators/stack_allocator.h"
#include <memory.h>

static uint32_t dsim_hash_storage_array_count( const struct dsim_storage *self )
{
    const struct dsim_hash_storage *s = container_of( self, const struct dsim_hash_storage, storage );

    return s->data.array_count;
}

static uint32_t dsim_hash_storage_array_size( const struct dsim_storage *self, uint32_t col )
{
    const struct dsim_hash_storage *s = container_of( self, const struct dsim_hash_storage, storage );

    assert( col < s->data.array_count ); // LCOV_EXCL_BR_LINE
    return s->data.arrays[col].size;
}

static const char * dsim_hash_storage_array_name( const struct dsim_storage *self, uint32_t col )
{
    const struct dsim_hash_storage *s = container_of( self, const struct dsim_hash_storage, storage );

    assert( col < s->data.array_count ); // LCOV_EXCL_BR_LINE
    return s->data.arrays[col].name;
}

static enum dsim_type dsim_hash_storage_array_type( const struct dsim_storage *self, uint32_t col )
{
    const struct dsim_hash_storage *s = container_of( self, const struct dsim_hash_storage, storage );

    assert( col < s->data.array_count ); // LCOV_EXCL_BR_LINE
    return s->data.arrays[col].type;
}

static uint32_t dsim_hash_storage_block_count( const struct dsim_storage *self )
{
    dsim_unused(self);
    return 1;
}

static uint32_t dsim_hash_storage_block_size( const struct dsim_storage *self, uint32_t block )
{
    dsim_unused(block);

    const struct dsim_hash_storage *s = container_of( self, const struct dsim_hash_storage, storage );

    assert( block == 0 ); // LCOV_EXCL_BR_LINE
    return s->ids.keys.count;
}

static const uint64_t *dsim_hash_storage_id_data( const struct dsim_storage *self, uint32_t block )
{
    dsim_unused(block);

    const struct dsim_hash_storage *s = container_of( self, const struct dsim_hash_storage, storage );

    assert( block == 0 ); // LCOV_EXCL_BR_LINE
    return s->ids.keys.data;
}

static void *dsim_hash_storage_data( struct dsim_storage *self, uint32_t block, uint32_t col )
{
    dsim_unused(block);

    struct dsim_hash_storage *s = container_of( self, struct dsim_hash_storage, storage );

    assert( block == 0 ); // LCOV_EXCL_BR_LINE
    assert( col < s->data.array_count ); // LCOV_EXCL_BR_LINE
    return s->data.arrays[col].data.data;
}

static dsim_storage_index dsim_hash_storage_find( const struct dsim_storage *self, uint64_t id )
{
    const struct dsim_hash_storage *s = container_of( self, const struct dsim_hash_storage, storage );

    dsim_storage_index result =
    {
        .block = 0,
        .index = dsim_hash_find( &s->ids, id )
    };
    return result;
}

static void dsim_hash_storage_find_range( const struct dsim_storage *self, uint64_t start_id, uint32_t count, struct dsim_array_storage_range * result )
{
    struct dsim_storage_range range =
    {
        .block = 0,
        .start_index = 0,
        .count = 0
    };

    for( uint32_t i = 0; i < count; ++i )
    {
        dsim_storage_index pos = dsim_hash_storage_find( self, start_id + i );

        range.start_index = pos.index;
        range.count = count - i;
        i = count-1;
        for( uint32_t j = 1; j < count-i; ++j )
            if( (pos.index + j >= dsim_hash_storage_block_size( self, 0 )) ||
                 (dsim_hash_storage_id_data( self, 0 )[pos.index+j] != start_id+i+j) )
            {
                range.count = j;
                i = j-1;
                break;
            }

        dsim_array_storage_range_push_back( result, range );
    }
}

static void dsim_hash_storage_insert( struct dsim_storage *self, uint64_t start_id, uint32_t count )
{
    struct dsim_hash_storage *s = container_of( self, struct dsim_hash_storage, storage );

    size_t new_count = s->ids.keys.count + count;

    dsim_hash_reserve( &s->ids, new_count );
    for( uint32_t i = 0; i < count; ++i )
        dsim_hash_push_back( &s->ids, start_id + i );
    dsim_storage_block_resize( &s->data, new_count );

    dsim_storage_log_cmd_push_back( &self->log, 0, start_id, count );
}

static void dsim_hash_storage_remove_range( struct dsim_storage *self, uint32_t pos, uint32_t count )
{
    struct dsim_hash_storage *s = container_of( self, struct dsim_hash_storage, storage );

    dsim_hash_remove_fast( &s->ids, pos, count );
    dsim_storage_block_remove_fast( &s->data, pos, count );

    dsim_storage_log_cmd_remove_fast( &self->log, 0, pos, count );
}

static void dsim_hash_storage_remove( struct dsim_storage *self, uint64_t start_id, uint32_t count )
{
    dsim_stack_alloc_save(0);
    struct dsim_array_storage_range range_list = dsim_array_static_init( dsim_stack_alloc(0) );

    dsim_hash_storage_find_range( self, start_id, count, &range_list );
    for( uint32_t i = 0; i != range_list.count; ++i )
        dsim_hash_storage_remove_range( self, range_list.at[i].start_index, range_list.at[i].count );

    dsim_array_storage_range_reset( &range_list );
    dsim_stack_alloc_restore(0);
}

static void dsim_hash_storage_reset( struct dsim_storage *self )
{
    struct dsim_hash_storage *s = container_of( self, struct dsim_hash_storage, storage );

    dsim_hash_reset( &s->ids );
    dsim_storage_block_reset( &s->data );
};

struct dsim_storage_operations dsim_hash_storage_ops =
{
    .array_count = dsim_hash_storage_array_count,
    .array_size = dsim_hash_storage_array_size,
    .array_name = dsim_hash_storage_array_name,
    .array_type = dsim_hash_storage_array_type,

    .block_count = dsim_hash_storage_block_count,
    .block_size = dsim_hash_storage_block_size,

    .id_data = dsim_hash_storage_id_data,
    .data = dsim_hash_storage_data,

    .find       = dsim_hash_storage_find,
    .find_range = dsim_hash_storage_find_range,

    .insert = dsim_hash_storage_insert,
    .remove = dsim_hash_storage_remove,
    .reset  = dsim_hash_storage_reset
};
