
#pragma once

#include "table.h"
#include "table_block.h"
#include "containers/hash.h"

DSIM_BEGIN_HEADER

extern struct dsim_table_operations dsim_hash_table_ops;

struct dsim_hash_table
{
    struct dsim_table table;
    struct dsim_hash  ids;
    struct dsim_table_block data;
};

#define dsim_hash_table_static_init(column_count,columns,alloc) { \
    /* .table = */        dsim_table_static_init(&dsim_hash_table_ops, alloc), \
    /* .ids = */          dsim_hash_static_init(alloc), \
    /* .data = */         dsim_table_block_static_init(column_count,columns) }

DSIM_END_HEADER
