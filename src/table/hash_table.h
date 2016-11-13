
#pragma once

#include "table.h"
#include "containers/array.h"

struct dsim_table_column
{
    uint32_t width;
    struct _dsim_array data;
};

#define dsim_table_column_init(width,alloc) { \
    /* .width = */ width, \
    /* .data = */  dsim_array_init(alloc) }

extern struct dsim_table_operations dsim_hash_table_ops;

struct dsim_hash_table
{
    struct dsim_table table;
    struct dsim_uint64_array ids;
    uint32_t column_count;
    struct dsim_table_column * columns;
};

#define dsim_hash_table_init(column_count,columns,alloc) { \
    /* .table = */        dsim_table_init(&dsim_hash_table_ops), \
    /* .ids = */          dsim_array_init(alloc), \
    /* .column_count = */ column_count, \
    /* .columns = */      columns }
