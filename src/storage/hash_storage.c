
#include "hash_storage.h"
#include "core/log.h"
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

const void *dsim_hash_storage_begin_read( struct dsim_storage *self, uint32_t block, uint32_t arr )
{
    dsim_unused(block);

    struct dsim_hash_storage *s = container_of( self, struct dsim_hash_storage, storage );

    assert( block == 0 ); // LCOV_EXCL_BR_LINE
    return dsim_storage_block_begin_read( &s->data, arr );

}

unsigned dsim_hash_storage_end_read( struct dsim_storage *self, const void *data )
{
    struct dsim_hash_storage *s = container_of( self, struct dsim_hash_storage, storage );
    return dsim_storage_block_end_read( &s->data, data );
}

void *dsim_hash_storage_begin_write( struct dsim_storage *self, uint32_t block, uint32_t arr, enum dsim_storage_write_mode mode )
{
    dsim_unused(block);

    struct dsim_hash_storage *s = container_of( self, struct dsim_hash_storage, storage );

    assert( block == 0 ); // LCOV_EXCL_BR_LINE
    return dsim_storage_block_begin_write( &s->data, arr, mode );
}

unsigned dsim_hash_storage_end_write( struct dsim_storage *self, void *data )
{
    struct dsim_hash_storage *s = container_of( self, struct dsim_hash_storage, storage );
    return dsim_storage_block_end_write( &s->data, data );
}

static int dsim_hash_storage_can_modify( const struct dsim_storage *self, uint32_t block, uint32_t arr )
{
    dsim_unused(block);

    struct dsim_hash_storage *s = container_of( self, struct dsim_hash_storage, storage );
    if( arr == DSIM_INVALID_INDEX )
        return dsim_storage_block_can_modify( &s->data );
    if( !s->data.arrays )
        return 1;

    assert( arr < dsim_ddl_array_vec_len( dsim_ddl_layout_arrays(self->layout) ) );
    return dsim_storage_array_can_modify( s->data.arrays + arr );
}

static void dsim_hash_storage_select( struct dsim_storage *self, const uint64_t *ids, uint32_t count, dsim_storage_select_cb cb, void *context )
{
    struct dsim_hash_storage *s = container_of( self, struct dsim_hash_storage, storage );

    size_t i = 0;
    size_t pos = 0;
    while( i < count )
    {
        uint32_t begin = dsim_hash_find( &s->ids, ids[i] );
        ++i;
        if( begin == DSIM_INVALID_INDEX )
        {
            cb( context, pos, DSIM_INVALID_INDEX, DSIM_INVALID_INDEX, 1 );
            pos = i;
            continue;
        }
        uint32_t end = begin+1;
        while( (i < count) && (end < s->ids.keys.count) )
        {
            if( s->ids.keys.at[end] != ids[i] )
                break;
            ++end;
            ++i;
        }

        cb( context, pos, 0, begin, end-begin );
        pos = i;
    }
}

static void dsim_hash_storage_insert( struct dsim_storage *self, const uint64_t *ids, const void *const *data, uint32_t count )
{
    assert( dsim_hash_storage_can_modify( self, DSIM_INVALID_INDEX, DSIM_INVALID_INDEX ) );
    struct dsim_hash_storage *s = container_of( self, struct dsim_hash_storage, storage );

    dsim_hash_push_back_n( &s->ids, ids, count );
    if( data )
        dsim_storage_block_push_back( &s->data, data, count );
    else
        dsim_storage_block_resize( &s->data, s->ids.keys.count + count );

    //dsim_storage_log_cmd_push_back( &self->log, 0, start_id, count );
}

struct _hash_update_context
{
    struct dsim_hash_storage *s;
    const uint64_t *ids;
    const void *const *data;
};

static void _hash_update_process( void *context, uint32_t pos, uint32_t block, uint32_t block_pos, uint32_t count )
{
    struct _hash_update_context *ctx = (struct _hash_update_context*)context;

    if( pos != DSIM_INVALID_INDEX )
        dsim_storage_block_update( &ctx->s->data, ctx->data, pos, block_pos, count );
}

static void dsim_hash_storage_update( struct dsim_storage *self, const uint64_t *ids, const void *const *data, uint32_t count )
{
    for( size_t i = 0; i < dsim_ddl_array_vec_len( dsim_ddl_layout_arrays(self->layout) ); ++i )
        if( data[i] )
            assert( dsim_hash_storage_can_modify( self, DSIM_INVALID_INDEX, i ) );

    struct dsim_hash_storage *s = container_of( self, struct dsim_hash_storage, storage );

    struct _hash_update_context ctx = { s, ids, data };
    dsim_hash_storage_select( self, ids, count, _hash_update_process, &ctx );
}

struct _hash_remove_context
{
    struct dsim_hash_storage *s;
    const uint64_t *ids;
};

static void _hash_remove_process( void *context, uint32_t pos, uint32_t block, uint32_t block_pos, uint32_t count )
{
    dsim_unused( pos );
    struct _hash_remove_context *ctx = (struct _hash_remove_context*)context;

    if( pos != DSIM_INVALID_INDEX )
    {
        dsim_hash_remove_fast( &ctx->s->ids, block_pos, count );
        dsim_storage_block_remove_fast( &ctx->s->data, block_pos, count );
    }
}

static void dsim_hash_storage_remove( struct dsim_storage *self, const uint64_t *ids, uint32_t count )
{
    assert( dsim_hash_storage_can_modify( self, DSIM_INVALID_INDEX, DSIM_INVALID_INDEX ) );
    struct dsim_hash_storage *s = container_of( self, struct dsim_hash_storage, storage );

    struct _hash_remove_context ctx = { s, ids };
    dsim_hash_storage_select( self, ids, count, _hash_remove_process, &ctx );
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

    .id_data       = dsim_hash_storage_id_data,
    .begin_read    = dsim_hash_storage_begin_read,
    .end_read      = dsim_hash_storage_end_read,
    .begin_write   = dsim_hash_storage_begin_write,
    .end_write     = dsim_hash_storage_end_write,

    .can_modify = dsim_hash_storage_can_modify,
    .select = dsim_hash_storage_select,
    .insert = dsim_hash_storage_insert,
    .update = dsim_hash_storage_update,
    .remove = dsim_hash_storage_remove,

    .done = dsim_hash_storage_done
};

void dsim_hash_storage_init( struct dsim_hash_storage *storage, const char *name, dsim_ddl_layout_table_t layout, struct dsim_allocator *alloc )
{
    dsim_storage_init( &storage->storage, &dsim_hash_storage_ops, name, layout, alloc );
    dsim_hash_init( &storage->ids, alloc );
    dsim_storage_block_init( &storage->data, layout, alloc );
}
