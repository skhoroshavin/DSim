
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
    dsim_ddl_layout_table_t layout_layout;
    struct dsim_storage *storage_storage;
};
extern const struct _ddl_test *const ddl_test;

/////////////////////////////////////////////////////////////
// test storage
/////////////////////////////////////////////////////////////

inline static void test_storage_insert( const uint64_t *ids, const uint8_t *i, const float *f, const vec4 *v, uint32_t count )
{ const void *_data[] = { i, f, v }; dsim_storage_insert( ddl_test->storage_storage, ids, _data, count ); }
inline static void test_storage_update( const uint64_t *ids, const uint8_t *i, const float *f, const vec4 *v, uint32_t count )
{ const void *_data[] = { i, f, v }; dsim_storage_update( ddl_test->storage_storage, ids, _data, count ); }
inline static void test_storage_remove( const uint64_t *ids, uint32_t count )
{ dsim_storage_remove( ddl_test->storage_storage, ids, count ); }
inline static void test_storage_select( const uint64_t *ids, dsim_storage_addr *addr, uint32_t count )
{ dsim_storage_select_buf( ddl_test->storage_storage, ids, addr, count ); }
inline static int test_storage_can_modify( uint32_t arr )
{ return dsim_storage_can_modify( ddl_test->storage_storage, 0, arr ); }

inline static const uint64_t *test_storage_id_data()
{ return (const uint64_t *)dsim_storage_id_data( ddl_test->storage_storage, 0 ); }

inline static const uint8_t *test_storage_i_read_begin() { return (const uint8_t *)dsim_storage_read_begin( ddl_test->storage_storage, 0, 0 ); }
inline static void test_storage_i_read_end() { dsim_storage_read_end( ddl_test->storage_storage, 0, 0 ); }
inline static uint8_t *test_storage_i_write_direct_begin() { return (uint8_t *)dsim_storage_write_direct_begin( ddl_test->storage_storage, 0, 0 ); }
inline static void test_storage_i_write_direct_end() { dsim_storage_write_direct_end( ddl_test->storage_storage, 0, 0 ); }
inline static uint8_t *test_storage_i_write_buffered_begin() { return (uint8_t *)dsim_storage_write_buffered_begin( ddl_test->storage_storage, 0, 0 ); }
inline static void test_storage_i_write_buffered_end() { dsim_storage_write_buffered_end( ddl_test->storage_storage, 0, 0 ); }

inline static const float *test_storage_f_read_begin() { return (const float *)dsim_storage_read_begin( ddl_test->storage_storage, 0, 1 ); }
inline static void test_storage_f_read_end() { dsim_storage_read_end( ddl_test->storage_storage, 0, 1 ); }
inline static float *test_storage_f_write_direct_begin() { return (float *)dsim_storage_write_direct_begin( ddl_test->storage_storage, 0, 1 ); }
inline static void test_storage_f_write_direct_end() { dsim_storage_write_direct_end( ddl_test->storage_storage, 0, 1 ); }
inline static float *test_storage_f_write_buffered_begin() { return (float *)dsim_storage_write_buffered_begin( ddl_test->storage_storage, 0, 1 ); }
inline static void test_storage_f_write_buffered_end() { dsim_storage_write_buffered_end( ddl_test->storage_storage, 0, 1 ); }

inline static const vec4 *test_storage_v_read_begin() { return (const vec4 *)dsim_storage_read_begin( ddl_test->storage_storage, 0, 2 ); }
inline static void test_storage_v_read_end() { dsim_storage_read_end( ddl_test->storage_storage, 0, 2 ); }
inline static vec4 *test_storage_v_write_direct_begin() { return (vec4 *)dsim_storage_write_direct_begin( ddl_test->storage_storage, 0, 2 ); }
inline static void test_storage_v_write_direct_end() { dsim_storage_write_direct_end( ddl_test->storage_storage, 0, 2 ); }
inline static vec4 *test_storage_v_write_buffered_begin() { return (vec4 *)dsim_storage_write_buffered_begin( ddl_test->storage_storage, 0, 2 ); }
inline static void test_storage_v_write_buffered_end() { dsim_storage_write_buffered_end( ddl_test->storage_storage, 0, 2 ); }



void dsim_ddl_init_test();
void dsim_ddl_done_test();

