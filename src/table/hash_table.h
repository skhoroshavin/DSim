
#pragma once

#include "table.h"
#include "table_column.h"

extern struct dsim_table_operations dsim_hash_table_ops;

struct dsim_hash_table
{
    struct dsim_table table;
    struct dsim_uint64_array ids;
    uint32_t column_count;
    struct dsim_table_column * columns;
};

#define dsim_hash_table_static_init(column_count,columns,alloc) { \
    /* .table = */        dsim_table_static_init(&dsim_hash_table_ops), \
    /* .ids = */          dsim_array_static_init(alloc), \
    /* .column_count = */ column_count, \
    /* .columns = */      columns }
