
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

    TEST_ASSERT_EQUAL( dsim_hash_find( &hash, 0 ),  DSIM_INVALID_INDEX );
    TEST_ASSERT_EQUAL( dsim_hash_find( &hash, 1 ),  DSIM_INVALID_INDEX );
    TEST_ASSERT_EQUAL( dsim_hash_find( &hash, 5 ),  DSIM_INVALID_INDEX );
    TEST_ASSERT_EQUAL( dsim_hash_find( &hash, 10 ), DSIM_INVALID_INDEX );
}

TEST(hash_empty, reserve)
{
    dsim_hash_reserve( &hash, 10 );

    TEST_ASSERT_NOT_NULL( hash.keys.data );
    TEST_ASSERT_EQUAL( hash.keys.count, 0 );
    TEST_ASSERT( hash.keys.capacity >= 10 );

    TEST_ASSERT_EQUAL( dsim_hash_find( &hash, 0 ),  DSIM_INVALID_INDEX );
    TEST_ASSERT_EQUAL( dsim_hash_find( &hash, 1 ),  DSIM_INVALID_INDEX );
    TEST_ASSERT_EQUAL( dsim_hash_find( &hash, 5 ),  DSIM_INVALID_INDEX );
    TEST_ASSERT_EQUAL( dsim_hash_find( &hash, 10 ), DSIM_INVALID_INDEX );
}

TEST(hash_empty, push_back_one)
{
    dsim_hash_push_back( &hash, 5, 1 );

    TEST_ASSERT_NOT_NULL( hash.keys.data );
    TEST_ASSERT_EQUAL( hash.keys.count, 1 );
    TEST_ASSERT( hash.keys.capacity >= 1 );

    TEST_ASSERT_EQUAL( dsim_hash_find( &hash, 0 ),  DSIM_INVALID_INDEX );
    TEST_ASSERT_EQUAL( dsim_hash_find( &hash, 1 ),  DSIM_INVALID_INDEX );
    TEST_ASSERT_EQUAL( dsim_hash_find( &hash, 5 ),  0 );
    TEST_ASSERT_EQUAL( dsim_hash_find( &hash, 10 ), DSIM_INVALID_INDEX );
}

TEST(hash_empty, push_back_multi)
{
    dsim_hash_push_back( &hash, 3, 5 );

    TEST_ASSERT_NOT_NULL( hash.keys.data );
    TEST_ASSERT_EQUAL( hash.keys.count, 5 );
    TEST_ASSERT( hash.keys.capacity >= 5 );

    TEST_ASSERT_EQUAL( dsim_hash_find( &hash, 0 ),  DSIM_INVALID_INDEX );
    TEST_ASSERT_EQUAL( dsim_hash_find( &hash, 1 ),  DSIM_INVALID_INDEX );
    TEST_ASSERT_EQUAL( dsim_hash_find( &hash, 5 ),  2 );
    TEST_ASSERT_EQUAL( dsim_hash_find( &hash, 7 ),  4 );
    TEST_ASSERT_EQUAL( dsim_hash_find( &hash, 10 ), DSIM_INVALID_INDEX );
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
    RUN_TEST_CASE(hash_empty, push_back_one);
    RUN_TEST_CASE(hash_empty, push_back_multi);
    RUN_TEST_CASE(hash_empty, clear);
    RUN_TEST_CASE(hash_empty, reset);
}

void run_test_hash()
{
    RUN_TEST_GROUP(hash_empty);
}
