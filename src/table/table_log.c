
#include "table_log.h"

void dsim_table_log_cmd_push_back( struct dsim_table_log *tl, uint32_t block, uint64_t start_id, uint32_t count )
{
    struct dsim_table_command cmd =
    {
        .type  = TCT_PUSH_BACK,
        .block = block,
        .start = start_id,
        .count = count
    };
    dsim_table_command_array_push_back( &tl->commands, cmd );
}

void dsim_table_log_cmd_remove_fast( struct dsim_table_log *tl, uint32_t block, uint32_t start_index, uint32_t count )
{
    struct dsim_table_command cmd =
    {
        .type  = TCT_REMOVE_FAST,
        .block = block,
        .start = start_index,
        .count = count
    };
    dsim_table_command_array_push_back( &tl->commands, cmd );
}

void dsim_table_log_shrink( struct dsim_table_log *tl )
{
    tl->version += tl->commands.count;
    tl->commands.count = 0;
}

void dsim_table_log_reset( struct dsim_table_log *tl )
{
    dsim_table_command_array_reset( &tl->commands );
}
