
#include "test_array.h"

static dsim_array_uint64 array = dsim_array_static_init(&dsim_default_allocator);

static uint64_t test_data[] = { 5, 3, 7, 52, 0, 12, 3, 623, 23 };

static void setup_array_non_empty( void *ctx )
{
    dsim_unused(ctx);
    dsim_array_push_back_n( &array, test_data, count_of(test_data) );
}

static void teardown_array( void *ctx )
{
    dsim_unused(ctx);
    dsim_array_reset( &array );
}

/*
 * Utility
 */

enum greatest_test_res assert_array_null( const dsim_array_uint64 *a )
{
    ASSERT( !a->data );
    ASSERT_INT_EQ( a->count, 0 );
    ASSERT_INT_EQ( a->capacity, 0 );
    PASS();
}

enum greatest_test_res assert_array_capacity( const dsim_array_uint64 *a, uint32_t capacity )
{
    ASSERT( a->data );
    ASSERT( a->capacity >= capacity );
    PASS();
}

enum greatest_test_res assert_array_remove_ordered( const dsim_array_uint64 *a, uint32_t pos, uint32_t count, const uint64_t *old_data, uint32_t old_count )
{
    CHECK_CALL(assert_array_capacity( a, old_count ));
    ASSERT_INT_EQ( a->count, old_count - count );
    ASSERT_MEM_EQ( a->data, old_data, pos*sizeof(a->data[0]) );
    if( old_count > pos + count )
        ASSERT_MEM_EQ( a->data + pos, old_data + pos + count, (old_count - pos - count)*sizeof(a->data[0]) );
    PASS();
}

enum greatest_test_res assert_array_remove_unordered( const dsim_array_uint64 *a, uint32_t pos, uint32_t count, const uint64_t *old_data, uint32_t old_count )
{
    CHECK_CALL(assert_array_capacity( a, old_count ));
    ASSERT_INT_EQ( a->count, old_count - count );
    ASSERT_MEM_EQ( a->data, old_data, pos*sizeof(a->data[0]) );
    ASSERT_MEM_EQ( a->data + pos, old_data + old_count - count, count*sizeof(a->data[0]) );
    ASSERT_MEM_EQ( a->data + pos + count, old_data + pos + count, (old_count - pos - 2*count)*sizeof(a->data[0]) );
    PASS();
}

/*
 * Empty array
 */

TEST array_empty_assert_empty()
{
    CHECK_CALL(assert_array_null( &array ));
    PASS();
}

TEST array_empty_reserve()
{
    dsim_array_reserve( &array, 10 );

    CHECK_CALL(assert_array_capacity(&array, 10 ));
    ASSERT_INT_EQ( array.count, 0 );
    PASS();
}

TEST array_empty_resize()
{
    dsim_array_resize( &array, 10 );

    CHECK_CALL(assert_array_capacity(&array, 10 ));
    ASSERT_INT_EQ( array.count, 10 );
    PASS();
}

TEST array_empty_push_back()
{
    dsim_array_push_back( &array, 5 );

    CHECK_CALL(assert_array_capacity(&array, 1 ));
    ASSERT_INT_EQ( array.count, 1 );
    ASSERT_INT_EQ( array.at[0], 5 );
    PASS();
}

TEST array_empty_push_back_n()
{
    const uint64_t data[] = { 43, 3, 16, 5 };
    dsim_array_push_back_n( &array, data, count_of(data) );

    CHECK_CALL(assert_array_capacity(&array, count_of(data) ));
    ASSERT_INT_EQ( array.count, count_of(data) );
    ASSERT_MEM_EQ( array.data, data, sizeof(data) );
    PASS();
}

TEST array_empty_clear()
{
    dsim_array_clear( &array );

    CHECK_CALL(assert_array_null( &array ));
    PASS();
}

TEST array_empty_reset()
{
    dsim_array_reset( &array );

    CHECK_CALL(assert_array_null( &array ));
    PASS();
}

/*
 * Non empty array
 */

TEST array_non_empty_assert_non_empty()
{
    CHECK_CALL(assert_array_capacity(&array, count_of(test_data) ));
    ASSERT_INT_EQ( array.count, count_of(test_data) );
    ASSERT_MEM_EQ( array.data, test_data, sizeof(test_data) );
    PASS();
}

TEST array_non_empty_reserve_more()
{
    dsim_array_reserve( &array, count_of(test_data) + 4 );

    CHECK_CALL(assert_array_capacity(&array, count_of(test_data) + 4 ));
    ASSERT_INT_EQ( array.count, count_of(test_data) );
    ASSERT_MEM_EQ( array.data, test_data, sizeof(test_data) );
    PASS();
}

TEST array_non_empty_reserve_less()
{
    dsim_array_reserve( &array, count_of(test_data) - 4 );

    CHECK_CALL(assert_array_capacity(&array, count_of(test_data) ));
    ASSERT_INT_EQ( array.count, count_of(test_data) );
    ASSERT_MEM_EQ( array.data, test_data, sizeof(test_data) );
    PASS();
}

TEST array_non_empty_resize_more()
{
    dsim_array_resize( &array, count_of(test_data) + 4 );

    CHECK_CALL(assert_array_capacity(&array, count_of(test_data) + 4 ));
    ASSERT_INT_EQ( array.count, count_of(test_data) + 4 );
    ASSERT_MEM_EQ( array.data, test_data, sizeof(test_data) );
    PASS();
}

TEST array_non_empty_resize_less()
{
    dsim_array_resize( &array, count_of(test_data) - 4 );

    CHECK_CALL(assert_array_capacity(&array, count_of(test_data) ));
    ASSERT_INT_EQ( array.count, count_of(test_data) - 4 );
    ASSERT_MEM_EQ( array.data, test_data, array.count*sizeof(test_data[0]) );
    PASS();
}

TEST array_non_empty_push_back()
{
    dsim_array_push_back( &array, 42 );

    CHECK_CALL(assert_array_capacity(&array, count_of(test_data) + 1 ));
    ASSERT_INT_EQ( array.count, count_of(test_data) + 1 );
    ASSERT_MEM_EQ( array.data, test_data, sizeof(test_data) );
    ASSERT_INT_EQ( array.at[count_of(test_data)], 42 );
    PASS();
}

TEST array_non_empty_push_back_n()
{
    const uint64_t more_data[] = { 65, 0, 123, 87, 1 };
    dsim_array_push_back_n( &array, more_data, count_of(more_data) );

    CHECK_CALL(assert_array_capacity(&array, count_of(test_data) + count_of(more_data) ));
    ASSERT_INT_EQ( array.count, count_of(test_data) + count_of(more_data) );
    ASSERT_MEM_EQ( array.data, test_data, sizeof(test_data) );
    ASSERT_MEM_EQ( array.data + count_of(test_data), more_data, sizeof(more_data) );
    PASS();
}

TEST array_non_empty_pop_back()
{
    dsim_array_pop_back( &array );

    CHECK_CALL(assert_array_capacity(&array, count_of(test_data) ));
    ASSERT_INT_EQ( array.count, count_of(test_data) - 1 );
    ASSERT_MEM_EQ( array.data, test_data, array.count*sizeof(test_data[0]) );
    PASS();
}

TEST array_non_empty_pop_back_n()
{
    dsim_array_pop_back_n( &array, 3 );

    CHECK_CALL(assert_array_capacity(&array, count_of(test_data) ));
    ASSERT_INT_EQ( array.count, count_of(test_data) - 3 );
    ASSERT_MEM_EQ( array.data, test_data, array.count*sizeof(test_data[0]) );
    PASS();
}

TEST array_non_empty_remove()
{
    dsim_array_remove( &array, 2, 3 );

    CHECK_CALL(assert_array_remove_ordered(&array, 2, 3, test_data, count_of(test_data) ));
    PASS();
}

TEST array_non_empty_remove_from_end()
{
    dsim_array_remove( &array, array.count-4, 4 );

    CHECK_CALL(assert_array_remove_ordered(&array, array.count, 4, test_data, count_of(test_data) ));
    PASS();
}

TEST array_non_empty_remove_fast_unordered()
{
    dsim_array_remove_fast( &array, 3, 2 );

    CHECK_CALL(assert_array_remove_unordered(&array, 3, 2, test_data, count_of(test_data) ));
    PASS();
}

TEST array_non_empty_remove_fast_ordered()
{
    dsim_array_remove_fast( &array, 2, 4 );

    CHECK_CALL(assert_array_remove_ordered(&array, 2, 4, test_data, count_of(test_data) ));
    PASS();
}

TEST array_non_empty_clear()
{
    dsim_array_clear( &array );

    CHECK_CALL(assert_array_capacity(&array, count_of(test_data) ));
    ASSERT_INT_EQ( array.count, 0 );
    PASS();
}

TEST array_non_empty_reset()
{
    dsim_array_reset( &array );

    CHECK_CALL(assert_array_null( &array ));
    PASS();
}

SUITE(test_array)
{
    SET_TEARDOWN(teardown_array, 0);
    RUN_TEST(array_empty_assert_empty);
    RUN_TEST(array_empty_reserve);
    RUN_TEST(array_empty_resize);
    RUN_TEST(array_empty_push_back);
    RUN_TEST(array_empty_push_back_n);
    RUN_TEST(array_empty_clear);
    RUN_TEST(array_empty_reset);


    SET_SETUP(setup_array_non_empty, 0);
    RUN_TEST(array_non_empty_assert_non_empty);
    RUN_TEST(array_non_empty_reserve_more);
    RUN_TEST(array_non_empty_reserve_less);
    RUN_TEST(array_non_empty_resize_more);
    RUN_TEST(array_non_empty_resize_less);
    RUN_TEST(array_non_empty_push_back);
    RUN_TEST(array_non_empty_push_back_n);
    RUN_TEST(array_non_empty_pop_back);
    RUN_TEST(array_non_empty_pop_back_n);
    RUN_TEST(array_non_empty_remove);
    RUN_TEST(array_non_empty_remove_from_end);
    RUN_TEST(array_non_empty_remove_fast_unordered);
    RUN_TEST(array_non_empty_remove_fast_ordered);
    RUN_TEST(array_non_empty_clear);
    RUN_TEST(array_non_empty_reset);
}
