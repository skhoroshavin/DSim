
#pragma once

#include "table/table.h"
#include "reflection/ddl_reader.h"

typedef struct vec4 {
    float x;
    float y;
    float z;
    float w;
} vec4;

inline static uint8_t *test_int_data( struct dsim_table *t, uint32_t block ) { return (uint8_t *)dsim_table_data( t, block, 0 ); }
inline static float *test_float_data( struct dsim_table *t, uint32_t block ) { return (float *)dsim_table_data( t, block, 1 ); }
inline static vec4 *test_vec_data( struct dsim_table *t, uint32_t block ) { return (vec4 *)dsim_table_data( t, block, 2 ); }

struct _ddl_test
{
    dsim_type_table_t type_uint8;
    dsim_type_table_t type_float;
    dsim_type_table_t type_vec4;
    dsim_layout_table_t layout_test;
};
extern const struct _ddl_test *const ddl_test;

void dsim_ddl_init_test();

