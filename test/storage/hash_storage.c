
#include <memory.h>

#include "test/greatest.h"
#include "test.ddl.h"

TEST assert_hash_storage_scheme()
{
    ASSERT_INT_EQ( dsim_storage_block_count( ddl_test->storage_data ), 1 );
    PASS();
}

#define not_found { DSIM_INVALID_INDEX, DSIM_INVALID_INDEX }

TEST assert_hash_storage_empty()
{
    ASSERT_INT_EQ( dsim_storage_block_size( ddl_test->storage_data, 0 ), 0 );

    ASSERT( !test_data_id_data() );
//    ASSERT( !test_data_i_begin_read() ); test_data_end_read();
//    ASSERT( !test_data_f_begin_read() ); test_data_end_read();
//    ASSERT( !test_data_v_begin_read() ); test_data_end_read();

    uint64_t test_ids[] = { 0, 1, 25, 30 };
    dsim_storage_addr expected_addr[] = { not_found, not_found, not_found, not_found };
    dsim_storage_addr addr[count_of(test_ids)];
    test_data_select( test_ids, addr, count_of(test_ids) );
    ASSERT_MEM_EQ( addr, expected_addr, sizeof(addr) );

    ASSERT_INT_EQ( ddl_test->storage_data->log.version, 0 );
    ASSERT_INT_EQ( ddl_test->storage_data->log.commands.count, 0 );
    PASS();
}

TEST assert_hash_storage_count( uint32_t count )
{
    ASSERT_INT_EQ( dsim_storage_block_size( ddl_test->storage_data, 0 ), count );

    ASSERT( test_data_id_data() );
//    ASSERT( test_data_i_begin_read() ); test_data_end_read();
//    ASSERT( test_data_f_begin_read() ); test_data_end_read();
//    ASSERT( test_data_v_begin_read() ); test_data_end_read();
    PASS();
}

/*
 * Empty ddl_test->storage_test
 */

TEST hash_storage_empty_assert_empty()
{
    CHECK_CALL(assert_hash_storage_scheme());
    CHECK_CALL(assert_hash_storage_empty());
    PASS();
}

TEST hash_storage_empty_insert()
{
    const uint64_t ids[] = { 100, 101, 102, 103 };
    test_data_insert( ids, 0, 0, 0, count_of(ids) );

    CHECK_CALL(assert_hash_storage_scheme());
    CHECK_CALL(assert_hash_storage_count( count_of(ids) ));
    ASSERT_MEM_EQ( test_data_id_data(), ids, sizeof(ids) );

//    ASSERT_INT_EQ( ddl_test->storage_test->log.commands.count, 1 );
//    ASSERT_INT_EQ( ddl_test->storage_test->log.commands.at[0].type, SCT_PUSH_BACK );
//    ASSERT_INT_EQ( ddl_test->storage_test->log.commands.at[0].block, 0 );
//    ASSERT_INT_EQ( ddl_test->storage_test->log.commands.at[0].start, 100 );
//    ASSERT_INT_EQ( ddl_test->storage_test->log.commands.at[0].count, 4 );
    PASS();
}

/*
 * Non empty ddl_test->storage_test
 */

static const uint64_t test_data_ids[10] = { 20, 21, 22, 23, 24, 25, 26, 27, 28, 29 };

static const uint8_t  test_data_i[10] = { 12, 64, 23, 76, 43, 0, 2, 9, 72, 36 };
static const float    test_data_f[10] = { 0, -1, 23.2, 0.64, -0.0001, -0, -2634.6, 54, 0, 6024 };
static const vec4     test_data_v[10] = {
    { 1, 2, 3, 4 },
    { 5, 6, 7, 8 },
    { 12, 13, 14, 15 },
    { 0.1, 0.2, 0.3, 0.4 },
    { -1, -2, -3, -4 },
    { 5, -6, -7, 8 },
    { -12, -13, 14, 15 },
    { 0.1, 0.2, -0.3, -0.4 },
    { 12, 13, -14, 15 },
    { 0.1, 0.2, -0.3, 0.4 }
};

TEST assert_hash_storage_data()
{
//    ASSERT_MEM_EQ( test_data_i_begin_read(), test_data_i, sizeof(test_data_i) ); test_data_i_end_read();
//    ASSERT_MEM_EQ( test_data_f_begin_read(), test_data_f, sizeof(test_data_f) ); test_data_f_end_read();
//    ASSERT_MEM_EQ( test_data_v_begin_read(), test_data_v, sizeof(test_data_v) ); test_data_v_end_read();

    uint64_t test_ids[] = { 0, 1, 20, 27, 324 };
    dsim_storage_addr expected_addr[] = { not_found, not_found, { 0, 0 }, { 0, 7 }, not_found };
    dsim_storage_addr addr[count_of(test_ids)];
    test_data_select( test_ids, addr, count_of(test_ids) );
    ASSERT_MEM_EQ( addr, expected_addr, sizeof(addr) );
    PASS();
}

TEST hash_storage_non_empty_assert_non_empty()
{
    CHECK_CALL(assert_hash_storage_scheme());
    CHECK_CALL(assert_hash_storage_count( 10 ));
    CHECK_CALL(assert_hash_storage_data());

//    ASSERT_INT_EQ( ddl_test->storage_test->log.commands.count, 1 );
//    ASSERT_INT_EQ( ddl_test->storage_test->log.commands.at[0].type, SCT_PUSH_BACK );
//    ASSERT_INT_EQ( ddl_test->storage_test->log.commands.at[0].block, 0 );
//    ASSERT_INT_EQ( ddl_test->storage_test->log.commands.at[0].start, 20 );
//    ASSERT_INT_EQ( ddl_test->storage_test->log.commands.at[0].count, 10 );
    PASS();
}

TEST hash_storage_non_empty_insert_more()
{
    const uint64_t ids[] = { 15, 16, 17 };
    test_data_insert( ids, 0, 0, 0, count_of(ids) );

    CHECK_CALL(assert_hash_storage_scheme());
    CHECK_CALL(assert_hash_storage_count( 13 ));
    CHECK_CALL(assert_hash_storage_data());

    uint64_t test_ids[] = { 15, 17, 18 };
    dsim_storage_addr expected_addr[] = { { 0, 10 }, { 0, 12 }, not_found };
    dsim_storage_addr addr[count_of(test_ids)];
    test_data_select( test_ids, addr, count_of(test_ids) );
    ASSERT_MEM_EQ( addr, expected_addr, sizeof(addr) );

//    ASSERT_INT_EQ( ddl_test->storage_test->log.commands.count, 2 );
//    ASSERT_INT_EQ( ddl_test->storage_test->log.commands.at[0].type, SCT_PUSH_BACK );
//    ASSERT_INT_EQ( ddl_test->storage_test->log.commands.at[0].block, 0 );
//    ASSERT_INT_EQ( ddl_test->storage_test->log.commands.at[0].start, 20 );
//    ASSERT_INT_EQ( ddl_test->storage_test->log.commands.at[0].count, 10 );
//    ASSERT_INT_EQ( ddl_test->storage_test->log.commands.at[1].type, SCT_PUSH_BACK );
//    ASSERT_INT_EQ( ddl_test->storage_test->log.commands.at[1].block, 0 );
//    ASSERT_INT_EQ( ddl_test->storage_test->log.commands.at[1].start, 15 );
//    ASSERT_INT_EQ( ddl_test->storage_test->log.commands.at[1].count, 3 );
    PASS();
}

TEST hash_storage_non_empty_remove_fast()
{
    const uint64_t ids[] = { 22, 23, 24 };
    test_data_remove( ids, count_of(ids) );

    CHECK_CALL(assert_hash_storage_scheme());
    CHECK_CALL(assert_hash_storage_count( 7 ));

//    ASSERT_MEM_EQ( test_data_i_begin_read(),     test_data_i,          2*sizeof(test_data_i[0]) ); test_data_i_end_read();
//    ASSERT_MEM_EQ( test_data_i_begin_read() + 2, test_data_i + 10 - 3, 3*sizeof(test_data_i[0]) ); test_data_i_end_read();
//    ASSERT_MEM_EQ( test_data_i_begin_read() + 5, test_data_i + 5,      2*sizeof(test_data_i[0]) ); test_data_i_end_read();

//    ASSERT_MEM_EQ( test_data_f_begin_read(),     test_data_f,          2*sizeof(test_data_f[0]) ); test_data_f_end_read();
//    ASSERT_MEM_EQ( test_data_f_begin_read() + 2, test_data_f + 10 - 3, 3*sizeof(test_data_f[0]) ); test_data_f_end_read();
//    ASSERT_MEM_EQ( test_data_f_begin_read() + 5, test_data_f + 5,      2*sizeof(test_data_f[0]) ); test_data_f_end_read();

//    ASSERT_MEM_EQ( test_data_v_begin_read(),     test_data_v,          2*sizeof(test_data_v[0]) ); test_data_v_end_read();
//    ASSERT_MEM_EQ( test_data_v_begin_read() + 2, test_data_v + 10 - 3, 3*sizeof(test_data_v[0]) ); test_data_v_end_read();
//    ASSERT_MEM_EQ( test_data_v_begin_read() + 5, test_data_v + 5,      2*sizeof(test_data_v[0]) ); test_data_v_end_read();

    uint64_t test_ids[] = { 0, 15, 20, 21, 23, 26, 28 };
    dsim_storage_addr expected_addr[] = { not_found, not_found, { 0, 0 }, { 0, 1 }, not_found, { 0, 6 }, { 0, 3 } };
    dsim_storage_addr addr[count_of(test_ids)];
    test_data_select( test_ids, addr, count_of(test_ids) );
    ASSERT_MEM_EQ( addr, expected_addr, sizeof(addr) );

//    ASSERT_INT_EQ( ddl_test->storage_test->log.commands.count, 2 );
//    ASSERT_INT_EQ( ddl_test->storage_test->log.commands.at[0].type, SCT_PUSH_BACK );
//    ASSERT_INT_EQ( ddl_test->storage_test->log.commands.at[0].block, 0 );
//    ASSERT_INT_EQ( ddl_test->storage_test->log.commands.at[0].start, 20 );
//    ASSERT_INT_EQ( ddl_test->storage_test->log.commands.at[0].count, 10 );
//    ASSERT_INT_EQ( ddl_test->storage_test->log.commands.at[1].type, SCT_REMOVE_FAST );
//    ASSERT_INT_EQ( ddl_test->storage_test->log.commands.at[1].block, 0 );
//    ASSERT_INT_EQ( ddl_test->storage_test->log.commands.at[1].start, 2 );
//    ASSERT_INT_EQ( ddl_test->storage_test->log.commands.at[1].count, 3 );
    PASS();
}

TEST hash_storage_non_empty_remove_ordered()
{
    const uint64_t ids[] = { 23, 24, 25, 26 };
    test_data_remove( ids, count_of(ids) );

    CHECK_CALL(assert_hash_storage_scheme());
    CHECK_CALL(assert_hash_storage_count( 6 ));

//    ASSERT_MEM_EQ( test_data_i_begin_read(),     test_data_i,     3*sizeof(test_data_i[0]) ); test_data_i_end_read();
//    ASSERT_MEM_EQ( test_data_i_begin_read() + 3, test_data_i + 7, 3*sizeof(test_data_i[0]) ); test_data_i_end_read();

//    ASSERT_MEM_EQ( test_data_f_begin_read(),     test_data_f,     3*sizeof(test_data_f[0]) ); test_data_f_end_read();
//    ASSERT_MEM_EQ( test_data_f_begin_read() + 3, test_data_f + 7, 3*sizeof(test_data_f[0]) ); test_data_f_end_read();

//    ASSERT_MEM_EQ( test_data_v_begin_read(),     test_data_v,     3*sizeof(test_data_v[0]) ); test_data_v_end_read();
//    ASSERT_MEM_EQ( test_data_v_begin_read() + 3, test_data_v + 7, 3*sizeof(test_data_v[0]) ); test_data_v_end_read();

    uint64_t test_ids[] = { 0, 15, 20, 22, 24, 27, 29 };
    dsim_storage_addr expected_addr[] = { not_found, not_found, { 0, 0 }, { 0, 2 }, not_found, { 0, 3 }, { 0, 5 } };
    dsim_storage_addr addr[count_of(test_ids)];
    test_data_select( test_ids, addr, count_of(test_ids) );
    ASSERT_MEM_EQ( addr, expected_addr, sizeof(addr) );

//    ASSERT_INT_EQ( ddl_test->storage_test->log.commands.count, 2 );
//    ASSERT_INT_EQ( ddl_test->storage_test->log.commands.at[0].type, SCT_PUSH_BACK );
//    ASSERT_INT_EQ( ddl_test->storage_test->log.commands.at[0].block, 0 );
//    ASSERT_INT_EQ( ddl_test->storage_test->log.commands.at[0].start, 20 );
//    ASSERT_INT_EQ( ddl_test->storage_test->log.commands.at[0].count, 10 );
//    ASSERT_INT_EQ( ddl_test->storage_test->log.commands.at[1].type, SCT_REMOVE_FAST );
//    ASSERT_INT_EQ( ddl_test->storage_test->log.commands.at[1].block, 0 );
//    ASSERT_INT_EQ( ddl_test->storage_test->log.commands.at[1].start, 3 );
//    ASSERT_INT_EQ( ddl_test->storage_test->log.commands.at[1].count, 4 );
    PASS();
}

TEST hash_storage_non_empty_update_ordered()
{
    const uint64_t ids[] = { 22, 23, 24 };
    const float f_data[] = { 11, 22, 33 };
    test_data_update( ids, 0, f_data, 0, count_of(ids) );

//    ASSERT_MEM_EQ( test_data_i_begin_read(), test_data_i, sizeof(test_data_i) ); test_data_i_end_read();
//    ASSERT_MEM_EQ( test_data_v_begin_read(), test_data_v, sizeof(test_data_v) ); test_data_v_end_read();

    const float * f_test = test_data_f_begin_read();
    ASSERT_MEM_EQ( f_test,     test_data_f,     2*sizeof(test_data_f[0]) );
    ASSERT_MEM_EQ( f_test + 2, f_data,          3*sizeof(test_data_f[0]) );
    ASSERT_MEM_EQ( f_test + 5, test_data_f + 5, 5*sizeof(test_data_f[0]) );
    test_data_end_read( f_test );
    PASS();
}

TEST hash_storage_non_empty_update_unordered()
{
    const uint64_t ids[] = { 24, 23, 22 };
    const float f_data[] = { 11, 22, 33 };
    test_data_update( ids, 0, f_data, 0, count_of(ids) );

    //ASSERT_MEM_EQ( test_data_i_begin_read(), test_data_i, sizeof(test_data_i) ); test_data_end_read();
    //ASSERT_MEM_EQ( test_data_v_begin_read(), test_data_v, sizeof(test_data_v) ); test_data_end_read();

    const float * f_test = test_data_f_begin_read();
    ASSERT_MEM_EQ( f_test,     test_data_f,     2*sizeof(test_data_f[0]) );
    ASSERT_MEM_EQ( f_test + 5, test_data_f + 5, 5*sizeof(test_data_f[0]) );
    ASSERT_INT_EQ( f_test[2], f_data[2] );
    ASSERT_INT_EQ( f_test[3], f_data[1] );
    ASSERT_INT_EQ( f_test[4], f_data[0] );
    test_data_end_read( f_test );
    PASS();
}

static void setup_hash_storage_empty( void *ctx )
{
    dsim_ddl_init_test();
}

static void setup_hash_storage_non_empty( void *ctx )
{
    dsim_ddl_init_test();
    test_data_insert( test_data_ids, test_data_i, test_data_f, test_data_v, count_of(test_data_ids) );
}

static void teardown_hash_storage( void *ctx )
{
    dsim_ddl_done_test();
}

SUITE(test_hash_storage)
{
    SET_SETUP(setup_hash_storage_empty, 0);
    SET_TEARDOWN(teardown_hash_storage, 0);
    RUN_TEST(hash_storage_empty_assert_empty);
    RUN_TEST(hash_storage_empty_insert);

    SET_SETUP(setup_hash_storage_non_empty, 0);
    RUN_TEST(hash_storage_non_empty_assert_non_empty);
    RUN_TEST(hash_storage_non_empty_insert_more);
    RUN_TEST(hash_storage_non_empty_remove_fast);
    RUN_TEST(hash_storage_non_empty_remove_ordered);
    RUN_TEST(hash_storage_non_empty_update_ordered);
    RUN_TEST(hash_storage_non_empty_update_unordered);
}
