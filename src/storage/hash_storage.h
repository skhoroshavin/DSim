
#pragma once

#include "storage.h"
#include "storage_block.h"
#include "containers/hash.h"

DSIM_BEGIN_HEADER

struct dsim_hash_storage
{
    struct dsim_storage storage;
    struct dsim_hash ids;
    struct dsim_storage_block data;
};

void dsim_hash_storage_init( struct dsim_hash_storage *storage, const char *name, dsim_ddl_layout_table_t layout, struct dsim_allocator *alloc );

DSIM_END_HEADER
