
#pragma once

#include "storage/storage.h"
#include "reflection/ddl_reader.h"

typedef struct vec4 {
    float x;
    float y;
    float z;
    float w;
} vec4;

struct _ddl_test
{
    dsim_ddl_type_table_t type_uint8;
    dsim_ddl_type_table_t type_float;
    dsim_ddl_type_table_t type_vec4;
    dsim_ddl_layout_table_t layout_test;
    struct dsim_storage * storage_test;
};
extern const struct _ddl_test *const ddl_test;

inline static uint8_t *test_test_i_data( uint32_t block ) { return (uint8_t *)dsim_storage_data( ddl_test->storage_test, block, 0 ); }
inline static float *test_test_f_data( uint32_t block ) { return (float *)dsim_storage_data( ddl_test->storage_test, block, 1 ); }
inline static vec4 *test_test_v_data( uint32_t block ) { return (vec4 *)dsim_storage_data( ddl_test->storage_test, block, 2 ); }
void dsim_ddl_init_test();
void dsim_ddl_done_test();

