
#pragma once

#include "containers/array.h"

DSIM_BEGIN_HEADER

enum dsim_storage_command_type
{
    SCT_PUSH_BACK,
    SCT_REMOVE_FAST
};

struct dsim_storage_command
{
    uint32_t block;
    uint32_t count;
    uint64_t start;

    enum dsim_storage_command_type type;
};

DSIM_DEFINE_ARRAY(struct dsim_storage_command, storage_command)

struct dsim_storage_log
{
    uint64_t version;
    struct dsim_array_storage_command commands;
};

void dsim_storage_log_init( struct dsim_storage_log *sl, struct dsim_allocator *alloc );

void dsim_storage_log_cmd_push_back( struct dsim_storage_log *sl, uint32_t block, uint64_t start_id, uint32_t count );
void dsim_storage_log_cmd_remove_fast( struct dsim_storage_log *sl, uint32_t block, uint32_t start_index, uint32_t count );

void dsim_storage_log_shrink( struct dsim_storage_log *sl );
void dsim_storage_log_reset( struct dsim_storage_log *sl );

DSIM_END_HEADER
