
#include "unity_fixture.h"
#include "test_allocator.h"

#include "containers/hash.h"

static struct dsim_hash hash = dsim_hash_static_init(&dsim_test_allocator);

/*
 * Empty hash
 */

TEST_GROUP(hash_empty);

TEST_SETUP(hash_empty)
{

}

TEST_TEAR_DOWN(hash_empty)
{
    dsim_hash_reset( &hash );
}

TEST(hash_empty, assert_empty)
{
    TEST_ASSERT_NULL( hash.keys.data );
    TEST_ASSERT_EQUAL( hash.keys.count, 0 );
    TEST_ASSERT_EQUAL( hash.keys.capacity, 0 );

    TEST_ASSERT_EQUAL( dsim_hash_count_of( &hash, 0 ),  0 );
    TEST_ASSERT_EQUAL( dsim_hash_count_of( &hash, 1 ),  0 );
    TEST_ASSERT_EQUAL( dsim_hash_count_of( &hash, 5 ),  0 );
    TEST_ASSERT_EQUAL( dsim_hash_count_of( &hash, 10 ), 0 );
}

TEST(hash_empty, reserve)
{
    dsim_hash_reserve( &hash, 10 );

    TEST_ASSERT_NOT_NULL( hash.keys.data );
    TEST_ASSERT_EQUAL( hash.keys.count, 0 );
    TEST_ASSERT( hash.keys.capacity >= 10 );

    TEST_ASSERT_EQUAL( dsim_hash_count_of( &hash, 0 ),  0 );
    TEST_ASSERT_EQUAL( dsim_hash_count_of( &hash, 1 ),  0 );
    TEST_ASSERT_EQUAL( dsim_hash_count_of( &hash, 5 ),  0 );
    TEST_ASSERT_EQUAL( dsim_hash_count_of( &hash, 10 ), 0 );
}

TEST(hash_empty, push_back)
{
    dsim_hash_push_back( &hash, 5 );

    TEST_ASSERT_NOT_NULL( hash.keys.data );
    TEST_ASSERT_EQUAL( hash.keys.count, 1 );
    TEST_ASSERT( hash.keys.capacity >= 1 );
    TEST_ASSERT_EQUAL( hash.keys.at[0], 5 );

    TEST_ASSERT_EQUAL( dsim_hash_count_of( &hash, 0 ),  0 );
    TEST_ASSERT_EQUAL( dsim_hash_count_of( &hash, 1 ),  0 );
    TEST_ASSERT_EQUAL( dsim_hash_count_of( &hash, 5 ),  1 );
    TEST_ASSERT_EQUAL( dsim_hash_count_of( &hash, 10 ), 0 );

    TEST_ASSERT_EQUAL( dsim_hash_find( &hash, 5 ),  0 );
}

TEST(hash_empty, push_back_n)
{
    const uint64_t test_keys[] = { 2, 5, 3, 5, 9, 0 };
    dsim_hash_push_back_n( &hash, test_keys, count_of(test_keys) );

    TEST_ASSERT_NOT_NULL( hash.keys.data );
    TEST_ASSERT_EQUAL( hash.keys.count, count_of(test_keys) );
    TEST_ASSERT( hash.keys.capacity >= count_of(test_keys) );
    TEST_ASSERT_EQUAL_MEMORY( hash.keys.data, test_keys, sizeof(test_keys) );

    TEST_ASSERT_EQUAL( dsim_hash_count_of( &hash, 0 ),  1 );
    TEST_ASSERT_EQUAL( dsim_hash_count_of( &hash, 1 ),  0 );
    TEST_ASSERT_EQUAL( dsim_hash_count_of( &hash, 5 ),  2 );
    TEST_ASSERT_EQUAL( dsim_hash_count_of( &hash, 10 ), 0 );

    TEST_ASSERT_EQUAL( dsim_hash_find( &hash, 2 ), 0 );
    TEST_ASSERT_EQUAL( dsim_hash_find( &hash, 3 ), 2 );
    TEST_ASSERT_EQUAL( dsim_hash_find( &hash, 9 ), 4 );
    TEST_ASSERT_EQUAL( dsim_hash_find( &hash, 0 ), 5 );

    TEST_ASSERT_EQUAL( dsim_hash_find( &hash, 1  ), DSIM_INVALID_INDEX );
    TEST_ASSERT_EQUAL( dsim_hash_find( &hash, 10 ), DSIM_INVALID_INDEX );

    TEST_ASSERT_EQUAL( dsim_hash_find_next( &hash, 0 ), DSIM_INVALID_INDEX );
    TEST_ASSERT_EQUAL( dsim_hash_find_next( &hash, 2 ), DSIM_INVALID_INDEX );
    TEST_ASSERT_EQUAL( dsim_hash_find_next( &hash, 4 ), DSIM_INVALID_INDEX );
    TEST_ASSERT_EQUAL( dsim_hash_find_next( &hash, 5 ), DSIM_INVALID_INDEX );

    struct dsim_array_uint32 idx = dsim_array_static_init(&dsim_test_allocator);
    for( uint32_t i = dsim_hash_find( &hash, 5 ); i != DSIM_INVALID_INDEX; i = dsim_hash_find_next( &hash, i ) )
    {
        TEST_ASSERT_EQUAL( hash.keys.at[i], 5 );
        dsim_array_uint32_push_back( &idx, i );
    }
    TEST_ASSERT_EQUAL( idx.count, 2 );
    TEST_ASSERT_EQUAL( min( idx.at[0], idx.at[1] ), 1 );
    TEST_ASSERT_EQUAL( max( idx.at[0], idx.at[1] ), 3 );

    dsim_array_uint32_reset( &idx );
}

TEST(hash_empty, clear)
{
    dsim_hash_clear( &hash );

    TEST_ASSERT_NULL( hash.keys.data );
    TEST_ASSERT_EQUAL( hash.keys.count, 0 );
    TEST_ASSERT_EQUAL( hash.keys.capacity, 0 );

    TEST_ASSERT_EQUAL( dsim_hash_find( &hash, 0 ),  DSIM_INVALID_INDEX );
    TEST_ASSERT_EQUAL( dsim_hash_find( &hash, 1 ),  DSIM_INVALID_INDEX );
    TEST_ASSERT_EQUAL( dsim_hash_find( &hash, 5 ),  DSIM_INVALID_INDEX );
    TEST_ASSERT_EQUAL( dsim_hash_find( &hash, 10 ), DSIM_INVALID_INDEX );
}

TEST(hash_empty, reset)
{
    dsim_hash_reset( &hash );

    TEST_ASSERT_NULL( hash.keys.data );
    TEST_ASSERT_EQUAL( hash.keys.count, 0 );
    TEST_ASSERT_EQUAL( hash.keys.capacity, 0 );

    TEST_ASSERT_EQUAL( dsim_hash_find( &hash, 0 ),  DSIM_INVALID_INDEX );
    TEST_ASSERT_EQUAL( dsim_hash_find( &hash, 1 ),  DSIM_INVALID_INDEX );
    TEST_ASSERT_EQUAL( dsim_hash_find( &hash, 5 ),  DSIM_INVALID_INDEX );
    TEST_ASSERT_EQUAL( dsim_hash_find( &hash, 10 ), DSIM_INVALID_INDEX );
}

TEST_GROUP_RUNNER(hash_empty)
{
    RUN_TEST_CASE(hash_empty, assert_empty);
    RUN_TEST_CASE(hash_empty, reserve);
    RUN_TEST_CASE(hash_empty, push_back);
    RUN_TEST_CASE(hash_empty, push_back_n);
    RUN_TEST_CASE(hash_empty, clear);
    RUN_TEST_CASE(hash_empty, reset);
}

void run_test_hash()
{
    RUN_TEST_GROUP(hash_empty);
}
