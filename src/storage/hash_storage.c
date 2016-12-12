
#include "hash_storage.h"
#include "utils/log.h"
#include <memory.h>

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

static const void *dsim_hash_storage_data( struct dsim_storage *self, uint32_t block, uint32_t i )
{
    dsim_unused(block);

    struct dsim_hash_storage *s = container_of( self, struct dsim_hash_storage, storage );

    assert( block == 0 ); // LCOV_EXCL_BR_LINE
    assert( i < dsim_ddl_array_vec_len( dsim_ddl_layout_arrays(self->layout) ) ); // LCOV_EXCL_BR_LINE
    return s->data.arrays[i].array.data;
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

static void dsim_hash_storage_insert( struct dsim_storage *self, const uint64_t *ids, const void *const *data, uint32_t count )
{
    struct dsim_hash_storage *s = container_of( self, struct dsim_hash_storage, storage );

    dsim_hash_push_back_n( &s->ids, ids, count );
    if( data )
        dsim_storage_block_push_back( &s->data, data, count );
    else
        dsim_storage_block_resize( &s->data, s->ids.keys.count + count );

    //dsim_storage_log_cmd_push_back( &self->log, 0, start_id, count );
}

static void dsim_hash_storage_remove( struct dsim_storage *self, const uint64_t *ids, uint32_t count )
{
    struct dsim_hash_storage *s = container_of( self, struct dsim_hash_storage, storage );

    size_t i = 0;
    while( i < count )
    {
        uint32_t begin = dsim_hash_find( &s->ids, ids[i] );
        ++i;
        if( begin == DSIM_INVALID_INDEX )
        {
            dsim_warning( "Trying to remove nonexistant ID from storage %s", self->name );
            continue;
        }
        uint32_t end = begin+1;
        while( i < count )
        {
            if( s->ids.keys.at[end] != ids[i] )
                break;
            ++end;
            ++i;
        }

        dsim_hash_remove_fast( &s->ids, begin, end-begin );
        dsim_storage_block_remove_fast( &s->data, begin, end-begin );
    }

    //dsim_array_storage_range_reset( &range_list );
}

static void dsim_hash_storage_done( struct dsim_storage *self )
{
    struct dsim_hash_storage *s = container_of( self, struct dsim_hash_storage, storage );

    dsim_hash_reset( &s->ids );
    dsim_storage_block_done( &s->data );
};

struct dsim_storage_operations dsim_hash_storage_ops =
{
    .block_count = dsim_hash_storage_block_count,
    .block_size = dsim_hash_storage_block_size,

    .id_data = dsim_hash_storage_id_data,
    .data = dsim_hash_storage_data,

    .find       = dsim_hash_storage_find,

    .insert = dsim_hash_storage_insert,
    .remove = dsim_hash_storage_remove,
    .done  = dsim_hash_storage_done
};

void dsim_hash_storage_init( struct dsim_hash_storage *storage, const char *name, dsim_ddl_layout_table_t layout, struct dsim_allocator *alloc )
{
    dsim_storage_init( &storage->storage, &dsim_hash_storage_ops, name, layout, alloc );
    dsim_hash_init( &storage->ids, alloc );
    dsim_storage_block_init( &storage->data, layout, alloc );
}
