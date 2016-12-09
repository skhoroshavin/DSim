
#pragma once

#include "storage/storage.h"
#include "reflection/ddl_reader.h"

typedef struct vec4 {
    float x;
    float y;
    float z;
    float w;
} vec4;

inline static uint8_t *test_int_data( struct dsim_storage *s, uint32_t block ) { return (uint8_t *)dsim_storage_data( s, block, 0 ); }
inline static float *test_float_data( struct dsim_storage *s, uint32_t block ) { return (float *)dsim_storage_data( s, block, 1 ); }
inline static vec4 *test_vec_data( struct dsim_storage *s, uint32_t block ) { return (vec4 *)dsim_storage_data( s, block, 2 ); }

struct _ddl_test
{
    dsim_ddl_type_table_t type_uint8;
    dsim_ddl_type_table_t type_float;
    dsim_ddl_type_table_t type_vec4;
    dsim_ddl_layout_table_t layout_test;
};
extern const struct _ddl_test *const ddl_test;

void dsim_ddl_init_test();

