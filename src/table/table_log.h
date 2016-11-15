
#pragma once

#include "containers/array.h"

DSIM_BEGIN_HEADER

enum dsim_table_command_type
{
    TCT_PUSH_BACK,
    TCT_REMOVE_FAST
};

struct dsim_table_command
{
    uint32_t block;
    uint32_t count;
    uint64_t start;

    enum dsim_table_command_type type;
};

DSIM_DEFINE_ARRAY(struct dsim_table_command, table_command)

struct dsim_table_log
{
    uint64_t version;
    struct dsim_table_command_array commands;
};

#define dsim_table_log_static_init(alloc) { \
    /* .version = */ 0, \
    /* .commands = */ dsim_array_static_init(alloc) }

void dsim_table_log_cmd_push_back( struct dsim_table_log *tl, uint32_t block, uint64_t start_id, uint32_t count );
void dsim_table_log_cmd_remove_fast( struct dsim_table_log *tl, uint32_t block, uint32_t start_index, uint32_t count );

void dsim_table_log_shrink( struct dsim_table_log *tl );
void dsim_table_log_reset( struct dsim_table_log *tl );

DSIM_END_HEADER
