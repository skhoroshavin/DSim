
#include "storage_log.h"

void dsim_storage_log_cmd_push_back( struct dsim_storage_log *sl, uint32_t block, uint64_t start_id, uint32_t count )
{
    struct dsim_storage_command cmd =
    {
        .type  = SCT_PUSH_BACK,
        .block = block,
        .start = start_id,
        .count = count
    };
    dsim_array_storage_command_push_back( &sl->commands, cmd );
}

void dsim_storage_log_cmd_remove_fast( struct dsim_storage_log *sl, uint32_t block, uint32_t start_index, uint32_t count )
{
    struct dsim_storage_command cmd =
    {
        .type  = SCT_REMOVE_FAST,
        .block = block,
        .start = start_index,
        .count = count
    };
    dsim_array_storage_command_push_back( &sl->commands, cmd );
}

void dsim_storage_log_shrink( struct dsim_storage_log *sl )
{
    sl->version += sl->commands.count;
    sl->commands.count = 0;
}

void dsim_storage_log_reset( struct dsim_storage_log *sl )
{
    sl->version = 0;
    dsim_array_storage_command_reset( &sl->commands );
}
