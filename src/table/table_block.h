
#pragma once

#include "containers/array.h"

struct dsim_table_column
{
    uint32_t elem_size;
    struct _dsim_array data;
};

#define dsim_table_column_static_init(elem_size,alloc) { \
    /* .elem_size = */ elem_size, \
    /* .data = */      dsim_array_static_init(alloc) }

struct dsim_table_block
{
    uint32_t col_count;
    struct dsim_table_column *columns;
};

#define dsim_table_block_static_init(count,columns) { \
    /* .count = */ count, \
    /* .columns = */ columns }

void dsim_table_block_resize( struct dsim_table_block *tb, uint32_t count );
void dsim_table_block_remove_fast( struct dsim_table_block *tb, uint32_t pos, uint32_t count );
void dsim_table_block_reset( struct dsim_table_block *tb );
