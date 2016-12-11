
#include "unity_fixture.h"
#include "test_allocator.h"
#include <memory.h>

#include "test.ddl.h"

static void test_hash_storage_scheme()
{
    TEST_ASSERT_EQUAL( dsim_storage_block_count( ddl_test->storage_test ), 1 );
}

inline static void check_index( dsim_storage_index ti, uint32_t block, uint32_t index )
{
    TEST_ASSERT_EQUAL( ti.block, block );
    TEST_ASSERT_EQUAL( ti.index, index );
}

static void test_hash_storage_empty()
{
    TEST_ASSERT_EQUAL( dsim_storage_block_size( ddl_test->storage_test, 0 ), 0 );

    TEST_ASSERT_NULL( dsim_storage_id_data( ddl_test->storage_test, 0 ) );
    for( uint32_t i = 0; i < dsim_storage_array_count( ddl_test->storage_test ); ++i )
        TEST_ASSERT_NULL( dsim_storage_data( ddl_test->storage_test, 0, i ) );

    check_index( dsim_storage_find( ddl_test->storage_test, 0 ),  0, DSIM_INVALID_INDEX );
    check_index( dsim_storage_find( ddl_test->storage_test, 1 ),  0, DSIM_INVALID_INDEX );
    check_index( dsim_storage_find( ddl_test->storage_test, 25 ), 0, DSIM_INVALID_INDEX );
    check_index( dsim_storage_find( ddl_test->storage_test, 30 ), 0, DSIM_INVALID_INDEX );

    TEST_ASSERT_EQUAL( ddl_test->storage_test->log.version, 0 );
    TEST_ASSERT_EQUAL( ddl_test->storage_test->log.commands.count, 0 );
}

static void test_hash_storage_count( uint32_t count )
{
    TEST_ASSERT_EQUAL( dsim_storage_block_size( ddl_test->storage_test, 0 ), count );

    TEST_ASSERT_NOT_NULL( dsim_storage_id_data( ddl_test->storage_test, 0 ) );
    for( uint32_t i = 0; i < dsim_storage_array_count( ddl_test->storage_test ); ++i )
        TEST_ASSERT_NOT_NULL( dsim_storage_data( ddl_test->storage_test, 0, i ) );
}

/*
 * Empty ddl_test->storage_test
 */

TEST_GROUP(hash_storage_empty);

TEST_SETUP(hash_storage_empty)
{
    dsim_ddl_init_test();
}

TEST_TEAR_DOWN(hash_storage_empty)
{
    dsim_ddl_done_test();
}

TEST(hash_storage_empty, assert_empty)
{
    test_hash_storage_scheme();
    test_hash_storage_empty();
}

TEST(hash_storage_empty, insert)
{
    dsim_storage_insert( ddl_test->storage_test, 100, 4 );

    test_hash_storage_scheme();
    test_hash_storage_count( 4 );

    for( int i = 0; i < 4; ++i )
        TEST_ASSERT_EQUAL( dsim_storage_id_data( ddl_test->storage_test, 0 )[i], 100 + i );

    TEST_ASSERT_EQUAL( ddl_test->storage_test->log.commands.count, 1 );
    TEST_ASSERT_EQUAL( ddl_test->storage_test->log.commands.at[0].type, SCT_PUSH_BACK );
    TEST_ASSERT_EQUAL( ddl_test->storage_test->log.commands.at[0].block, 0 );
    TEST_ASSERT_EQUAL( ddl_test->storage_test->log.commands.at[0].start, 100 );
    TEST_ASSERT_EQUAL( ddl_test->storage_test->log.commands.at[0].count, 4 );
}

TEST_GROUP_RUNNER(hash_storage_empty)
{
    RUN_TEST_CASE(hash_storage_empty, assert_empty);
    RUN_TEST_CASE(hash_storage_empty, insert);
}

/*
 * Non empty ddl_test->storage_test
 */

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

static void test_hash_storage_data()
{
    TEST_ASSERT_EQUAL_MEMORY( test_test_i_data( 0 ), test_data_i, sizeof(test_data_i) );
    TEST_ASSERT_EQUAL_MEMORY( test_test_f_data( 0 ), test_data_f, sizeof(test_data_f) );
    TEST_ASSERT_EQUAL_MEMORY( test_test_v_data( 0 ), test_data_v, sizeof(test_data_v) );

    check_index( dsim_storage_find( ddl_test->storage_test, 0 ),   0, DSIM_INVALID_INDEX );
    check_index( dsim_storage_find( ddl_test->storage_test, 1 ),   0, DSIM_INVALID_INDEX );
    check_index( dsim_storage_find( ddl_test->storage_test, 20 ),  0, 0 );
    check_index( dsim_storage_find( ddl_test->storage_test, 27 ),  0, 7 );
    check_index( dsim_storage_find( ddl_test->storage_test, 324 ), 0, DSIM_INVALID_INDEX );
}

TEST_GROUP(hash_storage_non_empty);

TEST_SETUP(hash_storage_non_empty)
{
    dsim_ddl_init_test();

    TEST_ASSERT_EQUAL( count_of(test_data_i), 10 );
    TEST_ASSERT_EQUAL( count_of(test_data_f), 10 );
    TEST_ASSERT_EQUAL( count_of(test_data_v), 10 );

    dsim_storage_insert( ddl_test->storage_test, 20, 10 );
    memcpy( test_test_i_data( 0 ), test_data_i, sizeof(test_data_i) );
    memcpy( test_test_f_data( 0 ), test_data_f, sizeof(test_data_f) );
    memcpy( test_test_v_data( 0 ), test_data_v, sizeof(test_data_v) );
}

TEST_TEAR_DOWN(hash_storage_non_empty)
{
    dsim_ddl_done_test();
}

TEST(hash_storage_non_empty, assert_non_empty)
{
    test_hash_storage_scheme();
    test_hash_storage_count( 10 );
    test_hash_storage_data();

    TEST_ASSERT_EQUAL( ddl_test->storage_test->log.commands.count, 1 );
    TEST_ASSERT_EQUAL( ddl_test->storage_test->log.commands.at[0].type, SCT_PUSH_BACK );
    TEST_ASSERT_EQUAL( ddl_test->storage_test->log.commands.at[0].block, 0 );
    TEST_ASSERT_EQUAL( ddl_test->storage_test->log.commands.at[0].start, 20 );
    TEST_ASSERT_EQUAL( ddl_test->storage_test->log.commands.at[0].count, 10 );
}

TEST(hash_storage_non_empty, insert_more)
{
    dsim_storage_insert( ddl_test->storage_test, 15, 3 );

    test_hash_storage_scheme();
    test_hash_storage_count( 13 );
    test_hash_storage_data();

    check_index( dsim_storage_find( ddl_test->storage_test, 15 ), 0, 10 );
    check_index( dsim_storage_find( ddl_test->storage_test, 17 ), 0, 12 );
    check_index( dsim_storage_find( ddl_test->storage_test, 18 ), 0, DSIM_INVALID_INDEX );

    TEST_ASSERT_EQUAL( ddl_test->storage_test->log.commands.count, 2 );
    TEST_ASSERT_EQUAL( ddl_test->storage_test->log.commands.at[0].type, SCT_PUSH_BACK );
    TEST_ASSERT_EQUAL( ddl_test->storage_test->log.commands.at[0].block, 0 );
    TEST_ASSERT_EQUAL( ddl_test->storage_test->log.commands.at[0].start, 20 );
    TEST_ASSERT_EQUAL( ddl_test->storage_test->log.commands.at[0].count, 10 );
    TEST_ASSERT_EQUAL( ddl_test->storage_test->log.commands.at[1].type, SCT_PUSH_BACK );
    TEST_ASSERT_EQUAL( ddl_test->storage_test->log.commands.at[1].block, 0 );
    TEST_ASSERT_EQUAL( ddl_test->storage_test->log.commands.at[1].start, 15 );
    TEST_ASSERT_EQUAL( ddl_test->storage_test->log.commands.at[1].count, 3 );
}

TEST(hash_storage_non_empty, remove_fast)
{
    dsim_storage_remove( ddl_test->storage_test, 22, 3 );

    test_hash_storage_scheme();
    test_hash_storage_count( 7 );

    TEST_ASSERT_EQUAL_MEMORY( test_test_i_data( 0 ),     test_data_i,          2*sizeof(test_data_i[0]) );
    TEST_ASSERT_EQUAL_MEMORY( test_test_i_data( 0 ) + 2, test_data_i + 10 - 3, 3*sizeof(test_data_i[0]) );
    TEST_ASSERT_EQUAL_MEMORY( test_test_i_data( 0 ) + 5, test_data_i + 5,      2*sizeof(test_data_i[0]) );

    TEST_ASSERT_EQUAL_MEMORY( test_test_f_data( 0 ),     test_data_f,          2*sizeof(test_data_f[0]) );
    TEST_ASSERT_EQUAL_MEMORY( test_test_f_data( 0 ) + 2, test_data_f + 10 - 3, 3*sizeof(test_data_f[0]) );
    TEST_ASSERT_EQUAL_MEMORY( test_test_f_data( 0 ) + 5, test_data_f + 5,      2*sizeof(test_data_f[0]) );

    TEST_ASSERT_EQUAL_MEMORY( test_test_v_data( 0 ),     test_data_v,          2*sizeof(test_data_v[0]) );
    TEST_ASSERT_EQUAL_MEMORY( test_test_v_data( 0 ) + 2, test_data_v + 10 - 3, 3*sizeof(test_data_v[0]) );
    TEST_ASSERT_EQUAL_MEMORY( test_test_v_data( 0 ) + 5, test_data_v + 5,      2*sizeof(test_data_v[0]) );

    check_index( dsim_storage_find( ddl_test->storage_test, 0 ) , 0, DSIM_INVALID_INDEX );
    check_index( dsim_storage_find( ddl_test->storage_test, 15 ), 0, DSIM_INVALID_INDEX );
    check_index( dsim_storage_find( ddl_test->storage_test, 20 ), 0, 0 );
    check_index( dsim_storage_find( ddl_test->storage_test, 21 ), 0, 1 );
    check_index( dsim_storage_find( ddl_test->storage_test, 23 ), 0, DSIM_INVALID_INDEX );
    check_index( dsim_storage_find( ddl_test->storage_test, 26 ), 0, 6 );
    check_index( dsim_storage_find( ddl_test->storage_test, 28 ), 0, 3 );

    TEST_ASSERT_EQUAL( ddl_test->storage_test->log.commands.count, 2 );
    TEST_ASSERT_EQUAL( ddl_test->storage_test->log.commands.at[0].type, SCT_PUSH_BACK );
    TEST_ASSERT_EQUAL( ddl_test->storage_test->log.commands.at[0].block, 0 );
    TEST_ASSERT_EQUAL( ddl_test->storage_test->log.commands.at[0].start, 20 );
    TEST_ASSERT_EQUAL( ddl_test->storage_test->log.commands.at[0].count, 10 );
    TEST_ASSERT_EQUAL( ddl_test->storage_test->log.commands.at[1].type, SCT_REMOVE_FAST );
    TEST_ASSERT_EQUAL( ddl_test->storage_test->log.commands.at[1].block, 0 );
    TEST_ASSERT_EQUAL( ddl_test->storage_test->log.commands.at[1].start, 2 );
    TEST_ASSERT_EQUAL( ddl_test->storage_test->log.commands.at[1].count, 3 );
}

TEST(hash_storage_non_empty, remove_ordered)
{
    dsim_storage_remove( ddl_test->storage_test, 23, 4 );

    test_hash_storage_scheme();
    test_hash_storage_count( 6 );

    TEST_ASSERT_EQUAL_MEMORY( test_test_i_data( 0 ),     test_data_i,     3*sizeof(test_data_i[0]) );
    TEST_ASSERT_EQUAL_MEMORY( test_test_i_data( 0 ) + 3, test_data_i + 7, 3*sizeof(test_data_i[0]) );

    TEST_ASSERT_EQUAL_MEMORY( test_test_f_data( 0 ),     test_data_f,     3*sizeof(test_data_f[0]) );
    TEST_ASSERT_EQUAL_MEMORY( test_test_f_data( 0 ) + 3, test_data_f + 7, 3*sizeof(test_data_f[0]) );

    TEST_ASSERT_EQUAL_MEMORY( test_test_v_data( 0 ),     test_data_v,     3*sizeof(test_data_v[0]) );
    TEST_ASSERT_EQUAL_MEMORY( test_test_v_data( 0 ) + 3, test_data_v + 7, 3*sizeof(test_data_v[0]) );

    check_index( dsim_storage_find( ddl_test->storage_test, 0 ), 0, DSIM_INVALID_INDEX );
    check_index( dsim_storage_find( ddl_test->storage_test, 15 ), 0, DSIM_INVALID_INDEX );
    check_index( dsim_storage_find( ddl_test->storage_test, 20 ), 0, 0 );
    check_index( dsim_storage_find( ddl_test->storage_test, 22 ), 0, 2 );
    check_index( dsim_storage_find( ddl_test->storage_test, 24 ), 0, DSIM_INVALID_INDEX );
    check_index( dsim_storage_find( ddl_test->storage_test, 27 ), 0, 3 );
    check_index( dsim_storage_find( ddl_test->storage_test, 29 ), 0, 5 );

    TEST_ASSERT_EQUAL( ddl_test->storage_test->log.commands.count, 2 );
    TEST_ASSERT_EQUAL( ddl_test->storage_test->log.commands.at[0].type, SCT_PUSH_BACK );
    TEST_ASSERT_EQUAL( ddl_test->storage_test->log.commands.at[0].block, 0 );
    TEST_ASSERT_EQUAL( ddl_test->storage_test->log.commands.at[0].start, 20 );
    TEST_ASSERT_EQUAL( ddl_test->storage_test->log.commands.at[0].count, 10 );
    TEST_ASSERT_EQUAL( ddl_test->storage_test->log.commands.at[1].type, SCT_REMOVE_FAST );
    TEST_ASSERT_EQUAL( ddl_test->storage_test->log.commands.at[1].block, 0 );
    TEST_ASSERT_EQUAL( ddl_test->storage_test->log.commands.at[1].start, 3 );
    TEST_ASSERT_EQUAL( ddl_test->storage_test->log.commands.at[1].count, 4 );
}

TEST_GROUP_RUNNER(hash_storage_non_empty)
{
    RUN_TEST_CASE(hash_storage_non_empty, assert_non_empty);
    RUN_TEST_CASE(hash_storage_non_empty, insert_more);
    RUN_TEST_CASE(hash_storage_non_empty, remove_fast);
    RUN_TEST_CASE(hash_storage_non_empty, remove_ordered);
}

void run_test_hash_storage()
{
    RUN_TEST_GROUP(hash_storage_empty);
    RUN_TEST_GROUP(hash_storage_non_empty);
}
