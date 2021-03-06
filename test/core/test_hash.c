
#include "test_hash.h"
#include "core/hash.h"
#include "test/test_context.h"

/*
 * Utility
 */

static void test_gen_dsim_hash( struct dsim_test_context *_ctx, struct dsim_hash **result )
{
    *result = (struct dsim_hash*)dsim_test_context_alloc( _ctx, sizeof(struct dsim_hash), (dsim_test_dtor)dsim_hash_reset );
    dsim_hash_init( *result, &dsim_default_allocator );
}

#define GIVEN_DSIM_HASH(name,min_count) \
    GIVEN_ARRAY(uint64_t,name,min_count); \
    struct dsim_hash *name; \
    test_gen_dsim_hash( _ctx, &name ); \
    dsim_hash_push_back_n( name, name##_data, name##_count );

TEST assert_dsim_hash_invariants( const struct dsim_hash *h )
{
    dsim_unused(h);
    PASS();
}

/*
 * Tests
 */

DSIM_TEST(hash_invariants)
{
    GIVEN_DSIM_HASH( test, 0 );
    CHECK_CALL(assert_dsim_hash_invariants(test));
    PASS();
}

DSIM_TEST(hash_find)
{
    GIVEN_DSIM_HASH( test, 0 );
    GIVEN_UINT( key, 0, 10 );

    uint32_t pos = dsim_hash_find( test, key );

    if( pos == DSIM_INVALID_INDEX )
    {
        uint32_t test_pos = 0;
        dsim_find( test_pos, key, test_data, test_count );
        ASSERT_INT_EQ( test_pos, DSIM_INVALID_INDEX );
    }
    else
        ASSERT_INT_EQ( test->keys.at[pos], key );
    PASS();
}

DSIM_TEST(hash_find_next)
{
    GIVEN_DSIM_HASH( test, 1 );
    GIVEN_UINT( pos, 0, test->keys.count - 1 );

    uint32_t next_pos = dsim_hash_find_next( test, pos );

    if( next_pos != DSIM_INVALID_INDEX )
    {
        uint64_t test_key = test->keys.at[pos];
        ASSERT_INT_EQ( test->keys.at[next_pos], test_key );
    }
    PASS();
}

DSIM_TEST(hash_count)
{
    GIVEN_DSIM_HASH( test, 0 );
    GIVEN_UINT( key, 0, 10 );

    unsigned count = 0;
    for( uint32_t pos = dsim_hash_find( test, key ); pos != DSIM_INVALID_INDEX; pos = dsim_hash_find_next( test, pos ) )
        ++count;

    unsigned simple_count = 0;
    dsim_count( simple_count, key, test_data, test_count );

    ASSERT_INT_EQ( count, simple_count );
    PASS();
}

DSIM_TEST(hash_reserve)
{
    GIVEN_DSIM_HASH( test, 0 );
    GIVEN_UINT( new_capacity, 0, 2*test->keys.count );

    dsim_hash_reserve( test, new_capacity );

    CHECK_CALL(assert_dsim_hash_invariants(test));
    ASSERT_INT_EQ( test->keys.count, test_count );
    ASSERT_MEM_EQ( test->keys.data, test_data, test_count*sizeof(uint64_t) );
    PASS();
}

DSIM_TEST(hash_push_back)
{
    GIVEN_DSIM_HASH( test, 0 );
    GIVEN_UINT( value, 0, 10 );

    dsim_hash_push_back( test, value );

    CHECK_CALL(assert_dsim_hash_invariants(test));
    ASSERT_INT_EQ( test->keys.count, test_count + 1 );
    ASSERT_MEM_EQ( test->keys.data, test_data, test_count*sizeof(uint64_t) );
    ASSERT_INT_EQ( test->keys.at[test_count], value );
    PASS();
}

DSIM_TEST(hash_push_back_n)
{
    GIVEN_DSIM_HASH( test, 0 );
    GIVEN_ARRAY(uint64_t, more, 1 );

    dsim_hash_push_back_n( test, more_data, more_count );

    CHECK_CALL(assert_dsim_hash_invariants(test));
    ASSERT_INT_EQ( test->keys.count, test_count + more_count );
    ASSERT_MEM_EQ( test->keys.data, test_data, test_count*sizeof(uint64_t) );
    ASSERT_MEM_EQ( test->keys.data + test_count, more_data, more_count*sizeof(uint64_t) );
    PASS();
}

DSIM_TEST(hash_remove_fast)
{
    GIVEN_DSIM_HASH( test, 1 );
    GIVEN_UINT( pos, 0, test->keys.count - 1 );
    GIVEN_UINT( count, 0, test->keys.count - pos - 1 );

    dsim_array(uint64_t) oracle = dsim_array_static_init(test->keys._alloc);
    dsim_array_push_back_n( &oracle, test_data, test_count );

    dsim_hash_remove_fast( test, pos, count );
    dsim_array_remove_fast( &oracle, pos, count );

    CHECK_CALL(assert_dsim_hash_invariants(test));
    ASSERT_INT_EQ( test->keys.count, oracle.count );
    ASSERT_MEM_EQ( test->keys.data, oracle.data, test->keys.count*sizeof(uint64_t) );
    dsim_array_reset( &oracle );
    PASS();
}

DSIM_TEST(hash_clear)
{
    GIVEN_DSIM_HASH( test, 0 );

    dsim_hash_clear( test );

    CHECK_CALL(assert_dsim_hash_invariants(test));
    ASSERT_INT_EQ( test->keys.count, 0 );
    ASSERT( test->keys.capacity >= test_count );
    PASS();
}

DSIM_TEST(hash_reset)
{
    GIVEN_DSIM_HASH( test, 0 );

    dsim_hash_reset( test );

    CHECK_CALL(assert_dsim_hash_invariants(test));
    ASSERT_INT_EQ( test->keys.capacity, 0 );
    PASS();
}


SUITE(test_hash)
{
    DSIM_RUN_TEST(hash_invariants);
    DSIM_RUN_TEST(hash_find);
    DSIM_RUN_TEST(hash_find_next);
    DSIM_RUN_TEST(hash_count);
    DSIM_RUN_TEST(hash_reserve);
    DSIM_RUN_TEST(hash_push_back);
    DSIM_RUN_TEST(hash_push_back_n);
    DSIM_RUN_TEST(hash_remove_fast);
    DSIM_RUN_TEST(hash_clear);
    DSIM_RUN_TEST(hash_reset);
}
