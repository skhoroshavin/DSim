
#include "storage.h"

void dsim_storage_emit_insert( struct dsim_storage *storage, uint32_t block, uint32_t pos, uint32_t count )
{
    if( !storage->listener ) return;
    if( !storage->listener->on_insert ) return;
    storage->listener->on_insert( storage->listener, storage, block, pos, count );
}

void dsim_storage_emit_remove( struct dsim_storage *storage, uint32_t block, uint32_t pos, uint32_t count )
{
    if( !storage->listener ) return;
    if( !storage->listener->on_remove ) return;
    storage->listener->on_remove( storage->listener, storage, block, pos, count );
}

void dsim_storage_emit_update( struct dsim_storage *storage, uint32_t array, uint32_t block, uint32_t pos, uint32_t count )
{
    if( !storage->listener ) return;
    if( !storage->listener->on_update ) return;
    storage->listener->on_update( storage->listener, storage, array, block, pos, count );
}

static void _select_buf_process( void *context, uint32_t pos, uint32_t block, uint32_t block_pos, uint32_t count )
{
    struct dsim_storage_addr *addr = (struct dsim_storage_addr*)context;

    if( block == DSIM_INVALID_INDEX )
    {
        for( unsigned i = 0; i < count; ++i )
        {
            addr[pos+i].block = DSIM_INVALID_INDEX;
            addr[pos+i].index = DSIM_INVALID_INDEX;
        }
    }
    else
    {
        for( unsigned i = 0; i < count; ++i )
        {
            addr[pos+i].block = block;
            addr[pos+i].index = block_pos + i;
        }
    }
}

void dsim_storage_select_buf( struct dsim_storage *storage, const uint64_t *ids, dsim_storage_addr *addr, uint32_t count )
{
    dsim_storage_select( storage, ids, count, _select_buf_process, addr );
}
