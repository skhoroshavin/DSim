
#include "test_hash.h"

#include <time.h>
#include <stdlib.h>
#include <memory.h>

static struct dsim_hash hash = dsim_hash_static_init(&dsim_default_allocator);

#define SMALL_COUNT 10
#define LARGE_COUNT 100

static uint64_t test_small[SMALL_COUNT];
static uint64_t test_large[LARGE_COUNT];
static uint64_t more_small[SMALL_COUNT];
static uint64_t more_large[LARGE_COUNT];

static void setup_hash_non_empty( void *ctx )
{
    dsim_unused(ctx);
    dsim_hash_push_back_n( &hash, test_large, count_of(test_large) );
}

static void teardown_hash( void *ctx )
{
    dsim_unused(ctx);
    dsim_hash_reset( &hash );
}

/*
 * Utility
 */

static uint64_t rand_not_in_array( uint64_t max_value, const uint64_t *values, uint32_t count )
{
    while( 1 )
    {
        uint64_t result = rand() % max_value;
        uint32_t pos = 0;
        dsim_find( pos, result, values, count );
        if( pos == DSIM_INVALID_INDEX )
            return result;
    }
}

//static void fill_unique_rand( uint64_t max_value, uint64_t *values, uint32_t count )
//{
//    for( uint32_t i = 0; i < count; ++i )
//        values[i] = rand_not_in_array( max_value, values, i );
//}

static int _less_uint64( const void *pa, const void *pb )
{
    uint64_t a = *((uint64_t*)pa);
    uint64_t b = *((uint64_t*)pb);
    if( b > a ) return -1;
    if( b < a ) return 1;
    return 0;
}

TEST assert_hash_consistent( const struct dsim_hash *h )
{
    ASSERT_INT_EQ( dsim_hash_find_next( h, DSIM_INVALID_INDEX ), DSIM_INVALID_INDEX );

    dsim_array_uint64 keys = dsim_array_static_init(&dsim_default_allocator);
    dsim_array_push_back_n( &keys, h->keys.data, h->keys.count );
    qsort( keys.data, keys.count, sizeof(uint64_t), _less_uint64 );

    for( uint32_t count, i = 0; i < h->keys.count; i += count )
    {
        uint64_t key = keys.at[i];

        count = 0;
        for( uint32_t j = dsim_hash_find( h, key ); j != DSIM_INVALID_INDEX; j = dsim_hash_find_next( h, j ) )
        {
            ASSERT_INT_EQ( h->keys.at[j], key );
            ASSERT_INT_EQ( keys.at[i+count], key );
            ++count;
        }
    }

    for( uint32_t i = 0; i < 100; ++i )
    {
        uint64_t key = rand_not_in_array( 100, keys.data, keys.count );
        ASSERT_INT_EQ( dsim_hash_find( h, key ), DSIM_INVALID_INDEX );
    }

    dsim_array_reset( &keys );
    PASS();
}

/*
 * Empty hash
 */

TEST hash_empty_assert_empty()
{
    CHECK_CALL(assert_array_null( (const dsim_array_uint64*)&hash.keys ));
    CHECK_CALL(assert_hash_consistent( &hash ));
    PASS();
}

TEST hash_empty_reserve()
{
    dsim_hash_reserve( &hash, 10 );

    CHECK_CALL(assert_array_capacity( (const dsim_array_uint64*)&hash.keys, 10 ));
    CHECK_CALL(assert_hash_consistent( &hash ));
    PASS();
}

TEST hash_empty_push_back()
{
    dsim_hash_push_back( &hash, 5 );

    CHECK_CALL(assert_array_capacity( (const dsim_array_uint64*)&hash.keys, 1 ));
    ASSERT_INT_EQ( hash.keys.count, 1 );
    ASSERT_INT_EQ( hash.keys.at[0], 5 );

    CHECK_CALL(assert_hash_consistent( &hash ));
    PASS();
}

TEST hash_empty_push_back_n()
{
    dsim_hash_push_back_n( &hash, test_small, count_of(test_small) );

    CHECK_CALL(assert_array_capacity( (const dsim_array_uint64*)&hash.keys, count_of(test_small) ));
    ASSERT_INT_EQ( hash.keys.count, count_of(test_small) );
    ASSERT_MEM_EQ( hash.keys.data, test_small, sizeof(test_small) );

    CHECK_CALL(assert_hash_consistent( &hash ));
    PASS();
}

TEST hash_empty_clear()
{
    dsim_hash_clear( &hash );

    CHECK_CALL(assert_array_null( (const dsim_array_uint64*)&hash.keys ));
    CHECK_CALL(assert_hash_consistent( &hash ));
    PASS();
}

TEST hash_empty_reset()
{
    dsim_hash_reset( &hash );

    CHECK_CALL(assert_array_null( (const dsim_array_uint64*)&hash.keys ));
    CHECK_CALL(assert_hash_consistent( &hash ));
    PASS();
}

/*
 * Non empty array
 */


TEST hash_non_empty_assert_non_empty()
{
    CHECK_CALL(assert_array_capacity( (const dsim_array_uint64*)&hash.keys, count_of(test_large) ));
    ASSERT_INT_EQ( hash.keys.count, count_of(test_large) );
    ASSERT_MEM_EQ( hash.keys.data, test_large, sizeof(test_large) );
    CHECK_CALL(assert_hash_consistent( &hash ));
    PASS();
}

TEST hash_non_empty_reserve_more()
{
    dsim_hash_reserve( &hash, count_of(test_large) + 4 );

    CHECK_CALL(assert_array_capacity( (const dsim_array_uint64*)&hash.keys, count_of(test_large) + 4 ));
    ASSERT_INT_EQ( hash.keys.count, count_of(test_large) );
    ASSERT_MEM_EQ( hash.keys.data, test_large, sizeof(test_large) );
    CHECK_CALL(assert_hash_consistent( &hash ));
    PASS();
}

TEST hash_non_empty_reserve_less()
{
    dsim_hash_reserve( &hash, count_of(test_large) - 4 );

    CHECK_CALL(assert_array_capacity( (const dsim_array_uint64*)&hash.keys, count_of(test_large) ));
    ASSERT_INT_EQ( hash.keys.count, count_of(test_large) );
    ASSERT_MEM_EQ( hash.keys.data, test_large, sizeof(test_large) );
    CHECK_CALL(assert_hash_consistent( &hash ));
    PASS();
}

TEST hash_non_empty_push_back()
{
    dsim_hash_push_back( &hash, 42 );

    CHECK_CALL(assert_array_capacity( (const dsim_array_uint64*)&hash.keys, count_of(test_large) + 1 ));
    ASSERT_INT_EQ( hash.keys.count, count_of(test_large) + 1 );
    ASSERT_MEM_EQ( hash.keys.data, test_large, sizeof(test_large) );
    ASSERT_INT_EQ( hash.keys.at[count_of(test_large)], 42 );
    CHECK_CALL(assert_hash_consistent( &hash ));
    PASS();
}

TEST hash_non_empty_push_back_n_small()
{
    dsim_hash_push_back_n( &hash, more_small, count_of(more_small) );

    CHECK_CALL(assert_array_capacity( (const dsim_array_uint64*)&hash.keys, count_of(test_large) + count_of(more_small) ));
    ASSERT_INT_EQ( hash.keys.count, count_of(test_large) + count_of(more_small) );
    ASSERT_MEM_EQ( hash.keys.data, test_large, sizeof(test_large) );
    ASSERT_MEM_EQ( hash.keys.data + count_of(test_large), more_small, sizeof(more_small) );
    CHECK_CALL(assert_hash_consistent( &hash ));
    PASS();
}

TEST hash_non_empty_push_back_n_large()
{
    dsim_hash_push_back_n( &hash, more_large, count_of(more_large) );

    CHECK_CALL(assert_array_capacity( (const dsim_array_uint64*)&hash.keys, count_of(test_large) + count_of(more_large) ));
    ASSERT_INT_EQ( hash.keys.count, count_of(test_large) + count_of(more_large) );
    ASSERT_MEM_EQ( hash.keys.data, test_large, sizeof(test_large) );
    ASSERT_MEM_EQ( hash.keys.data + count_of(test_large), more_large, sizeof(more_large) );
    CHECK_CALL(assert_hash_consistent( &hash ));
    PASS();
}

TEST hash_non_empty_remove_fast_unordered()
{
    uint32_t pos = hash.keys.count / 3;
    uint32_t count = hash.keys.count / 6;
    dsim_hash_remove_fast( &hash, pos, count );

    CHECK_CALL(assert_array_remove_unordered( (const dsim_array_uint64*)&hash.keys, pos, count, test_large, count_of(test_large) ));
    CHECK_CALL(assert_hash_consistent( &hash ));
    PASS();
}

TEST hash_non_empty_remove_fast_ordered()
{
    uint32_t pos = hash.keys.count / 3;
    uint32_t count = hash.keys.count / 2;
    dsim_hash_remove_fast( &hash, pos, count );

    CHECK_CALL(assert_array_remove_ordered( (const dsim_array_uint64*)&hash.keys, pos, count, test_large, count_of(test_large) ));
    CHECK_CALL(assert_hash_consistent( &hash ));
    PASS();
}

TEST hash_non_empty_clear()
{
    dsim_hash_clear( &hash );

    CHECK_CALL(assert_array_capacity( (const dsim_array_uint64*)&hash.keys, count_of(test_large) ));
    ASSERT_INT_EQ( hash.keys.count, 0 );
    CHECK_CALL(assert_hash_consistent( &hash ));
    PASS();
}

TEST hash_non_empty_reset()
{
    dsim_hash_reset( &hash );

    CHECK_CALL(assert_array_null( (const dsim_array_uint64*)&hash.keys ));
    CHECK_CALL(assert_hash_consistent( &hash ));
    PASS();
}

SUITE(test_hash)
{
    dsim_fill( test_small, rand() % 10, count_of(test_small) );
    dsim_fill( test_large, rand() % 10, count_of(test_large) );
    dsim_fill( more_small, rand() % 10, count_of(more_small) );
    dsim_fill( more_large, rand() % 10, count_of(more_large) );

    SET_TEARDOWN(teardown_hash, 0);
    RUN_TEST(hash_empty_assert_empty);
    RUN_TEST(hash_empty_reserve);
    RUN_TEST(hash_empty_push_back);
    RUN_TEST(hash_empty_push_back_n);
    RUN_TEST(hash_empty_clear);
    RUN_TEST(hash_empty_reset);

    SET_SETUP(setup_hash_non_empty, 0);
    RUN_TEST(hash_non_empty_assert_non_empty);
    RUN_TEST(hash_non_empty_reserve_more);
    RUN_TEST(hash_non_empty_reserve_less);
    RUN_TEST(hash_non_empty_push_back);
    RUN_TEST(hash_non_empty_push_back_n_small);
    RUN_TEST(hash_non_empty_push_back_n_large);
    RUN_TEST(hash_non_empty_remove_fast_unordered);
    RUN_TEST(hash_non_empty_remove_fast_ordered);
    RUN_TEST(hash_non_empty_clear);
    RUN_TEST(hash_non_empty_reset);
}
