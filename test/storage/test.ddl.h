
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
    dsim_ddl_type_table_t type_uint64;
    dsim_ddl_type_table_t type_float;
    dsim_ddl_type_table_t type_vec4;
    dsim_ddl_layout_table_t layout_layout;
    struct dsim_storage *storage_data;
};
extern const struct _ddl_test *const ddl_test;

/////////////////////////////////////////////////////////////
// test data
/////////////////////////////////////////////////////////////

inline static void test_data_insert( const uint64_t *ids, const uint8_t *i, const float *f, const vec4 *v, uint32_t count )
{ const void *_data[] = { i, f, v }; dsim_storage_insert( ddl_test->storage_data, ids, _data, count ); }
inline static void test_data_update( const uint64_t *ids, const uint8_t *i, const float *f, const vec4 *v, uint32_t count )
{ const void *_data[] = { i, f, v }; dsim_storage_update( ddl_test->storage_data, ids, _data, count ); }
inline static void test_data_remove( const uint64_t *ids, uint32_t count )
{ dsim_storage_remove( ddl_test->storage_data, ids, count ); }
inline static void test_data_select( const uint64_t *ids, dsim_storage_addr *addr, uint32_t count )
{ dsim_storage_select_buf( ddl_test->storage_data, ids, addr, count ); }
inline static int test_data_can_modify( uint32_t arr )
{ return dsim_storage_can_modify( ddl_test->storage_data, 0, arr ); }

inline static const uint64_t *test_data_id_data()
{ return (const uint64_t *)dsim_storage_id_data( ddl_test->storage_data, 0 ); }

inline static const uint8_t *test_data_i_begin_read() { return (const uint8_t *)dsim_storage_begin_read( ddl_test->storage_data, 0, 0 ); }
inline static uint8_t *test_data_i_begin_write( enum dsim_storage_write_mode mode ) { return (uint8_t *)dsim_storage_begin_write( ddl_test->storage_data, 0, 0, mode ); }

inline static const float *test_data_f_begin_read() { return (const float *)dsim_storage_begin_read( ddl_test->storage_data, 0, 1 ); }
inline static float *test_data_f_begin_write( enum dsim_storage_write_mode mode ) { return (float *)dsim_storage_begin_write( ddl_test->storage_data, 0, 1, mode ); }

inline static const vec4 *test_data_v_begin_read() { return (const vec4 *)dsim_storage_begin_read( ddl_test->storage_data, 0, 2 ); }
inline static vec4 *test_data_v_begin_write( enum dsim_storage_write_mode mode ) { return (vec4 *)dsim_storage_begin_write( ddl_test->storage_data, 0, 2, mode ); }

inline static void test_data_end_read( const void *data ) { dsim_storage_end_read( ddl_test->storage_data, data ); }
inline static void test_data_end_write( void *data ) { dsim_storage_end_write( ddl_test->storage_data, data ); }


void dsim_ddl_init_test();
void dsim_ddl_done_test();

