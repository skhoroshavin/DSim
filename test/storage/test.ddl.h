
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
    struct dsim_storage *storage_test;
};
extern const struct _ddl_test *const ddl_test;

inline static void test_test_insert( const uint64_t *ids, const uint8_t *i, const float *f, const vec4 *v, uint32_t count )
{ const void *_data[] = { i, f, v }; dsim_storage_insert( ddl_test->storage_test, ids, _data, count ); }
inline static void test_test_remove( const uint64_t *ids, uint32_t count )
{ dsim_storage_remove( ddl_test->storage_test, ids, count ); }
inline static dsim_storage_index test_test_find( uint64_t id )
{ return dsim_storage_find( ddl_test->storage_test, id ); }
inline static const uint8_t *test_test_i_data( uint32_t block ){ return (const uint8_t *)dsim_storage_data( ddl_test->storage_test, block, 0 ); }
inline static const float *test_test_f_data( uint32_t block ){ return (const float *)dsim_storage_data( ddl_test->storage_test, block, 1 ); }
inline static const vec4 *test_test_v_data( uint32_t block ){ return (const vec4 *)dsim_storage_data( ddl_test->storage_test, block, 2 ); }

void dsim_ddl_init_test();
void dsim_ddl_done_test();

