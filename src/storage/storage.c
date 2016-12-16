
#include "storage.h"

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
