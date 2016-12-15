
#include "storage.h"

struct _select_buf_context
{
    struct dsim_storage_addr *addr;
    uint32_t pos;
};

static void _select_buf_process( void *context, uint32_t block, uint32_t pos, uint32_t count )
{
    struct _select_buf_context *ctx = (struct _select_buf_context*)context;

    if( pos == DSIM_INVALID_INDEX )
    {
        for( unsigned i = 0; i < count; ++i )
        {
            ctx->addr[ctx->pos+i].block = DSIM_INVALID_INDEX;
            ctx->addr[ctx->pos+i].index = DSIM_INVALID_INDEX;
        }
    }
    else
    {
        for( unsigned i = 0; i < count; ++i )
        {
            ctx->addr[ctx->pos+i].block = block;
            ctx->addr[ctx->pos+i].index = pos + i;
        }
    }

    ctx->pos += count;
}

void dsim_storage_select_buf( struct dsim_storage *storage, const uint64_t *ids, dsim_storage_addr *addr, uint32_t count )
{
    struct _select_buf_context ctx = { addr, 0 };
    dsim_storage_select( storage, ids, count, _select_buf_process, &ctx );
}
