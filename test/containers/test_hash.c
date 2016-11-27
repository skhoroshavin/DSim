
#include "test_hash.h"
#include "test_allocator.h"

#include <time.h>
#include <stdlib.h>
#include <memory.h>

static struct dsim_hash hash = dsim_hash_static_init(&dsim_test_allocator);

/*
 * Utility
 */

static int _find( uint64_t value, const uint64_t * values, uint32_t count )
{
    for( uint32_t i = 0; i < count; ++i )
        if( value == values[i] )
            return 1;
    return 0;
}

static uint64_t rand_not_in_array( uint64_t max_value, const uint64_t * values, uint32_t count )
{
    while( 1 )
    {
        uint64_t result = rand() % max_value;
        if( !_find(result, values, count) )
            return result;
    }
}

static int _less_uint64( const void * pa, const void * pb )
{
    uint64_t a = *((uint64_t*)pa);
    uint64_t b = *((uint64_t*)pb);
    if( b > a ) return -1;
    if( b < a ) return 1;
    return 0;
}

void TEST_ASSERT_HASH_CONSISTENT( const struct dsim_hash *h )
{
    TEST_ASSERT_EQUAL( dsim_hash_find_next( h, DSIM_INVALID_INDEX ), DSIM_INVALID_INDEX );

    struct dsim_array_uint64 keys = dsim_array_static_init(&dsim_test_allocator);
    dsim_array_uint64_push_back_n( &keys, h->keys.data, h->keys.count );
    qsort( keys.data, keys.count, sizeof(uint64_t), _less_uint64 );

    for( uint32_t count, i = 0; i < h->keys.count; i += count )
    {
        uint64_t key = keys.at[i];

        count = 0;
        for( uint32_t j = dsim_hash_find( h, key ); j != DSIM_INVALID_INDEX; j = dsim_hash_find_next( h, j ) )
        {
            TEST_ASSERT_EQUAL( h->keys.at[j], key );
            TEST_ASSERT_EQUAL( keys.at[i+count], key );
            ++count;
        }
    }

    for( uint32_t i = 0; i < 100; ++i )
    {
        uint64_t key = rand_not_in_array( 50, keys.data, keys.count );
        TEST_ASSERT_EQUAL( dsim_hash_find( h, key ), DSIM_INVALID_INDEX );
    }

    dsim_array_uint64_reset( &keys );
}

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
    TEST_ASSERT_ARRAY_NULL( &hash.keys );
    TEST_ASSERT_HASH_CONSISTENT( &hash );
}

TEST(hash_empty, reserve)
{
    dsim_hash_reserve( &hash, 10 );

    TEST_ASSERT_ARRAY_CAPACITY( &hash.keys, 10 );
    TEST_ASSERT_HASH_CONSISTENT( &hash );
}

TEST(hash_empty, push_back)
{
    dsim_hash_push_back( &hash, 5 );

    TEST_ASSERT_ARRAY_CAPACITY( &hash.keys, 1 );
    TEST_ASSERT_EQUAL( hash.keys.count, 1 );
    TEST_ASSERT_EQUAL( hash.keys.at[0], 5 );

    TEST_ASSERT_HASH_CONSISTENT( &hash );
}

TEST(hash_empty, push_back_n)
{
    const uint64_t test_keys[] = { 2, 5, 3, 5, 9, 0 };
    dsim_hash_push_back_n( &hash, test_keys, count_of(test_keys) );

    TEST_ASSERT_ARRAY_CAPACITY( &hash.keys, count_of(test_keys) );
    TEST_ASSERT_EQUAL( hash.keys.count, count_of(test_keys) );
    TEST_ASSERT_EQUAL_MEMORY( hash.keys.data, test_keys, sizeof(test_keys) );

    TEST_ASSERT_HASH_CONSISTENT( &hash );
}

TEST(hash_empty, clear)
{
    dsim_hash_clear( &hash );

    TEST_ASSERT_ARRAY_NULL( &hash.keys );
    TEST_ASSERT_HASH_CONSISTENT( &hash );
}

TEST(hash_empty, reset)
{
    dsim_hash_reset( &hash );

    TEST_ASSERT_ARRAY_NULL( &hash.keys );
    TEST_ASSERT_HASH_CONSISTENT( &hash );
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

/*
 * Non empty array
 */

TEST_GROUP(hash_non_empty);

static uint64_t test_data[] = { 5, 3, 7, 52, 0, 12, 3, 623, 23 };

TEST_SETUP(hash_non_empty)
{
    dsim_hash_push_back_n( &hash, test_data, count_of(test_data) );
}

TEST_TEAR_DOWN(hash_non_empty)
{
    dsim_hash_reset( &hash );
}

TEST(hash_non_empty, assert_non_empty)
{
    TEST_ASSERT_ARRAY_CAPACITY( &hash.keys, count_of(test_data) );
    TEST_ASSERT_EQUAL( hash.keys.count, count_of(test_data) );
    TEST_ASSERT_EQUAL_MEMORY( hash.keys.data, test_data, sizeof(test_data) );
    TEST_ASSERT_HASH_CONSISTENT( &hash );
}

TEST(hash_non_empty, reserve_more)
{
    dsim_hash_reserve( &hash, count_of(test_data) + 4 );

    TEST_ASSERT_ARRAY_CAPACITY( &hash.keys, count_of(test_data) + 4 );
    TEST_ASSERT_EQUAL( hash.keys.count, count_of(test_data) );
    TEST_ASSERT_EQUAL_MEMORY( hash.keys.data, test_data, sizeof(test_data) );
    TEST_ASSERT_HASH_CONSISTENT( &hash );
}

TEST(hash_non_empty, reserve_less)
{
    dsim_hash_reserve( &hash, count_of(test_data) - 4 );

    TEST_ASSERT_ARRAY_CAPACITY( &hash.keys, count_of(test_data) );
    TEST_ASSERT_EQUAL( hash.keys.count, count_of(test_data) );
    TEST_ASSERT_EQUAL_MEMORY( hash.keys.data, test_data, sizeof(test_data) );
    TEST_ASSERT_HASH_CONSISTENT( &hash );
}

TEST(hash_non_empty, push_back)
{
    dsim_hash_push_back( &hash, 42 );

    TEST_ASSERT_ARRAY_CAPACITY( &hash.keys, count_of(test_data) + 1 );
    TEST_ASSERT_EQUAL( hash.keys.count, count_of(test_data) + 1 );
    TEST_ASSERT_EQUAL_MEMORY( hash.keys.data, test_data, sizeof(test_data) );
    TEST_ASSERT_EQUAL( hash.keys.at[count_of(test_data)], 42 );
    TEST_ASSERT_HASH_CONSISTENT( &hash );
}

TEST(hash_non_empty, push_back_n)
{
    const uint64_t more_data[] = { 65, 0, 123, 87, 1, 12, 45, 62, 76, 4, 2 };
    dsim_hash_push_back_n( &hash, more_data, count_of(more_data) );

    TEST_ASSERT_ARRAY_CAPACITY( &hash.keys, count_of(test_data) + count_of(more_data) );
    TEST_ASSERT_EQUAL( hash.keys.count, count_of(test_data) + count_of(more_data) );
    TEST_ASSERT_EQUAL_MEMORY( hash.keys.data, test_data, sizeof(test_data) );
    TEST_ASSERT_EQUAL_MEMORY( hash.keys.data + count_of(test_data), more_data, sizeof(more_data) );
    TEST_ASSERT_HASH_CONSISTENT( &hash );
}

TEST(hash_non_empty, remove_fast_unordered)
{
    dsim_hash_remove_fast( &hash, 3, 2 );

    TEST_ASSERT_ARRAY_REMOVE_UNORDERED( &hash.keys, 3, 2, test_data, count_of(test_data) );
    TEST_ASSERT_HASH_CONSISTENT( &hash );
}

TEST(hash_non_empty, remove_fast_ordered)
{
    dsim_hash_remove_fast( &hash, 2, 4 );

    TEST_ASSERT_ARRAY_REMOVE_ORDERED( &hash.keys, 2, 4, test_data, count_of(test_data) );
    TEST_ASSERT_HASH_CONSISTENT( &hash );
}

TEST(hash_non_empty, clear)
{
    dsim_hash_clear( &hash );

    TEST_ASSERT_ARRAY_CAPACITY( &hash.keys, count_of(test_data) );
    TEST_ASSERT_EQUAL( hash.keys.count, 0 );
    TEST_ASSERT_HASH_CONSISTENT( &hash );
}

TEST(hash_non_empty, reset)
{
    dsim_hash_reset( &hash );

    TEST_ASSERT_ARRAY_NULL( &hash.keys );
    TEST_ASSERT_HASH_CONSISTENT( &hash );
}

TEST_GROUP_RUNNER(hash_non_empty)
{
    RUN_TEST_CASE(hash_non_empty, assert_non_empty);
    RUN_TEST_CASE(hash_non_empty, reserve_more);
    RUN_TEST_CASE(hash_non_empty, reserve_less);
    RUN_TEST_CASE(hash_non_empty, push_back);
    RUN_TEST_CASE(hash_non_empty, push_back_n);
    RUN_TEST_CASE(hash_non_empty, remove_fast_unordered);
    RUN_TEST_CASE(hash_non_empty, remove_fast_ordered);
    RUN_TEST_CASE(hash_non_empty, clear);
    RUN_TEST_CASE(hash_non_empty, reset);
}

void run_test_hash()
{
    RUN_TEST_GROUP(hash_empty);
    RUN_TEST_GROUP(hash_non_empty);
}
