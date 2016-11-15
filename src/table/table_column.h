
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

inline static void dsim_table_column_resize( struct dsim_table_column *tc, uint32_t count )
{ _dsim_array_resize( &tc->data, count, tc->elem_size ); }
inline static void dsim_table_column_remove( struct dsim_table_column *tc, uint32_t pos, uint32_t count )
{ _dsim_array_remove( &tc->data, pos, count, tc->elem_size ); }
inline static void dsim_table_column_remove_fast( struct dsim_table_column *tc, uint32_t pos, uint32_t count )
{ _dsim_array_remove_fast( &tc->data, pos, count, tc->elem_size ); }
inline static void dsim_table_column_reset( struct dsim_table_column *tc )
{ _dsim_array_reset( &tc->data, tc->elem_size ); }
