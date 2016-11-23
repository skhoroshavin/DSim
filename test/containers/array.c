
#include "unity_fixture.h"

#include "containers/array.h"
#include "test_allocator.h"

static struct dsim_uint64_array array = dsim_array_static_init(&dsim_test_allocator);

/*
 * Empty array
 */

TEST_GROUP(array_empty);

TEST_SETUP(array_empty)
{

}

TEST_TEAR_DOWN(array_empty)
{
    dsim_uint64_array_reset( &array );
}

TEST(array_empty, assert_empty)
{
    TEST_ASSERT_NULL( array.data );
    TEST_ASSERT_EQUAL( array.count, 0 );
    TEST_ASSERT_EQUAL( array.capacity, 0 );
}

TEST(array_empty, reserve)
{
    dsim_uint64_array_reserve( &array, 10 );
    TEST_ASSERT_NOT_NULL( array.data );
    TEST_ASSERT_EQUAL( array.count, 0 );
    TEST_ASSERT( array.capacity >= 10 );
}

TEST(array_empty, resize)
{
    dsim_uint64_array_resize( &array, 10 );
    TEST_ASSERT_NOT_NULL( array.data );
    TEST_ASSERT_EQUAL( array.count, 10 );
    TEST_ASSERT( array.capacity >= 10 );
}

TEST(array_empty, push_back)
{
    dsim_uint64_array_push_back( &array, 5 );
    TEST_ASSERT_NOT_NULL( array.data );
    TEST_ASSERT_EQUAL( array.count, 1 );
    TEST_ASSERT( array.capacity >= 1 );
    TEST_ASSERT_EQUAL( array.data[0], 5 );
}

TEST(array_empty, push_back_n)
{
    const uint64_t data[] = { 43, 3, 16, 5 };
    dsim_uint64_array_push_back_n( &array, data, count_of(data) );
    TEST_ASSERT_NOT_NULL( array.data );
    TEST_ASSERT_EQUAL( array.count, count_of(data) );
    TEST_ASSERT( array.capacity >= count_of(data) );
    TEST_ASSERT_EQUAL_MEMORY( array.data,data,sizeof(data) );
}

TEST(array_empty, clear)
{
    dsim_uint64_array_clear( &array );
    TEST_ASSERT_NULL( array.data );
    TEST_ASSERT_EQUAL( array.count, 0 );
    TEST_ASSERT_EQUAL( array.capacity, 0 );
}

TEST(array_empty, reset)
{
    dsim_uint64_array_reset( &array );
    TEST_ASSERT_NULL( array.data );
    TEST_ASSERT_EQUAL( array.count, 0 );
    TEST_ASSERT_EQUAL( array.capacity, 0 );
}

TEST_GROUP_RUNNER(array_empty)
{
    RUN_TEST_CASE(array_empty, assert_empty);
    RUN_TEST_CASE(array_empty, reserve);
    RUN_TEST_CASE(array_empty, resize);
    RUN_TEST_CASE(array_empty, push_back);
    RUN_TEST_CASE(array_empty, push_back_n);
    RUN_TEST_CASE(array_empty, clear);
    RUN_TEST_CASE(array_empty, reset);
}

/*
 * Non empty array
 */

TEST_GROUP(array_non_empty);

static uint64_t test_data[] = { 5, 3, 7, 52, 0, 12, 3, 623, 23 };

TEST_SETUP(array_non_empty)
{
    dsim_uint64_array_push_back_n( &array, test_data, count_of(test_data) );
}

TEST_TEAR_DOWN(array_non_empty)
{
    dsim_uint64_array_reset( &array );
}

TEST(array_non_empty, assert_non_empty)
{
    TEST_ASSERT_NOT_NULL( array.data );
    TEST_ASSERT_EQUAL( array.count, count_of(test_data) );
    TEST_ASSERT( array.capacity >= count_of(test_data) );
    TEST_ASSERT_EQUAL_MEMORY( array.data, test_data, sizeof(test_data) );
}

TEST(array_non_empty, reserve_more)
{
    dsim_uint64_array_reserve( &array, count_of(test_data) + 4 );
    TEST_ASSERT_NOT_NULL( array.data );
    TEST_ASSERT_EQUAL( array.count, count_of(test_data) );
    TEST_ASSERT( array.capacity >= count_of(test_data) + 4 );
    TEST_ASSERT_EQUAL_MEMORY( array.data, test_data, sizeof(test_data) );
}

TEST(array_non_empty, reserve_less)
{
    dsim_uint64_array_reserve( &array, count_of(test_data) - 4 );
    TEST_ASSERT_NOT_NULL( array.data );
    TEST_ASSERT_EQUAL( array.count, count_of(test_data) );
    TEST_ASSERT( array.capacity >= count_of(test_data) );
    TEST_ASSERT_EQUAL_MEMORY( array.data, test_data, sizeof(test_data) );
}

TEST(array_non_empty, resize_more)
{
    dsim_uint64_array_resize( &array, count_of(test_data) + 4 );
    TEST_ASSERT_NOT_NULL( array.data );
    TEST_ASSERT_EQUAL( array.count, count_of(test_data) + 4 );
    TEST_ASSERT( array.capacity >= count_of(test_data) + 4 );
    TEST_ASSERT_EQUAL_MEMORY( array.data, test_data, sizeof(test_data) );
}

TEST(array_non_empty, resize_less)
{
    dsim_uint64_array_resize( &array, count_of(test_data) - 4 );
    TEST_ASSERT_NOT_NULL( array.data );
    TEST_ASSERT_EQUAL( array.count, count_of(test_data) - 4 );
    TEST_ASSERT( array.capacity >= count_of(test_data) );
    TEST_ASSERT_EQUAL_MEMORY( array.data, test_data, array.count*sizeof(test_data[0]) );
}

TEST(array_non_empty, fill)
{
    dsim_uint64_array_fill( &array, 0xdeadbeefcafe );
    TEST_ASSERT_NOT_NULL( array.data );
    TEST_ASSERT_EQUAL( array.count, count_of(test_data) );
    TEST_ASSERT( array.capacity >= count_of(test_data) );
    for( size_t i = 0; i < array.count; ++i )
        TEST_ASSERT_EQUAL_HEX64( array.data[i], 0xdeadbeefcafe );
}

TEST(array_non_empty, push_back)
{
    dsim_uint64_array_push_back( &array, 42 );
    TEST_ASSERT_NOT_NULL( array.data );
    TEST_ASSERT_EQUAL( array.count, count_of(test_data) + 1 );
    TEST_ASSERT( array.capacity >= count_of(test_data) + 1 );
    TEST_ASSERT_EQUAL_MEMORY( array.data, test_data, sizeof(test_data) );
    TEST_ASSERT_EQUAL( array.data[count_of(test_data)], 42 );
}

TEST(array_non_empty, push_back_n)
{
    const uint64_t more_data[] = { 65, 0, 123, 87, 1 };
    dsim_uint64_array_push_back_n( &array, more_data, count_of(more_data) );
    TEST_ASSERT_NOT_NULL( array.data );
    TEST_ASSERT_EQUAL( array.count, count_of(test_data) + count_of(more_data) );
    TEST_ASSERT( array.capacity >= count_of(test_data) + count_of(more_data) );
    TEST_ASSERT_EQUAL_MEMORY( array.data, test_data, sizeof(test_data) );
    TEST_ASSERT_EQUAL_MEMORY( array.data + count_of(test_data), more_data, sizeof(more_data) );
}

TEST(array_non_empty, pop_back)
{
    dsim_uint64_array_pop_back( &array );
    TEST_ASSERT_NOT_NULL( array.data );
    TEST_ASSERT_EQUAL( array.count, count_of(test_data) - 1 );
    TEST_ASSERT( array.capacity >= count_of(test_data) );
    TEST_ASSERT_EQUAL_MEMORY( array.data, test_data, array.count*sizeof(test_data[0]) );
}

TEST(array_non_empty, pop_back_n)
{
    dsim_uint64_array_pop_back_n( &array, 3 );
    TEST_ASSERT_NOT_NULL( array.data );
    TEST_ASSERT_EQUAL( array.count, count_of(test_data) - 3 );
    TEST_ASSERT( array.capacity >= count_of(test_data) );
    TEST_ASSERT_EQUAL_MEMORY( array.data, test_data, array.count*sizeof(test_data[0]) );
}

TEST(array_non_empty, remove)
{
    dsim_uint64_array_remove( &array, 2, 3 );
    TEST_ASSERT_NOT_NULL( array.data );
    TEST_ASSERT_EQUAL( array.count, count_of(test_data) - 3 );
    TEST_ASSERT( array.capacity >= count_of(test_data) );
    TEST_ASSERT_EQUAL_MEMORY( array.data, test_data, 2*sizeof(test_data[0]) );
    TEST_ASSERT_EQUAL_MEMORY( array.data + 2, test_data + 5, (count_of(test_data) - 5)*sizeof(test_data[0]) );
}

TEST(array_non_empty, remove_fast_unordered)
{
    dsim_uint64_array_remove_fast( &array, 3, 2 );
    TEST_ASSERT_NOT_NULL( array.data );
    TEST_ASSERT_EQUAL( array.count, count_of(test_data) - 2 );
    TEST_ASSERT( array.capacity >= count_of(test_data) );
    TEST_ASSERT_EQUAL_MEMORY( array.data, test_data, 3*sizeof(test_data[0]) );
    TEST_ASSERT_EQUAL_MEMORY( array.data + 3, test_data + count_of(test_data) - 2, 2*sizeof(test_data[0]) );
    TEST_ASSERT_EQUAL_MEMORY( array.data + 5, test_data + 5, (array.count - 5)*sizeof(test_data[0]) );
}

TEST(array_non_empty, remove_fast_ordered)
{
    dsim_uint64_array_remove_fast( &array, 2, 4 );
    TEST_ASSERT_NOT_NULL( array.data );
    TEST_ASSERT_EQUAL( array.count, count_of(test_data) - 4 );
    TEST_ASSERT( array.capacity >= count_of(test_data) );
    TEST_ASSERT_EQUAL_MEMORY( array.data, test_data, 2*sizeof(test_data[0]) );
    TEST_ASSERT_EQUAL_MEMORY( array.data + 2, test_data + 6, (count_of(test_data) - 6)*sizeof(test_data[0]) );
}

TEST(array_non_empty, clear)
{
    dsim_uint64_array_clear( &array );
    TEST_ASSERT_NOT_NULL( array.data );
    TEST_ASSERT_EQUAL( array.count, 0 );
    TEST_ASSERT( array.capacity >= count_of(test_data) );
}

TEST(array_non_empty, reset)
{
    dsim_uint64_array_reset( &array );
    TEST_ASSERT_NULL( array.data );
    TEST_ASSERT_EQUAL( array.count, 0 );
    TEST_ASSERT_EQUAL( array.capacity, 0 );
}

TEST_GROUP_RUNNER(array_non_empty)
{
    RUN_TEST_CASE(array_non_empty, assert_non_empty);
    RUN_TEST_CASE(array_non_empty, reserve_more);
    RUN_TEST_CASE(array_non_empty, reserve_less);
    RUN_TEST_CASE(array_non_empty, resize_more);
    RUN_TEST_CASE(array_non_empty, resize_less);
    RUN_TEST_CASE(array_non_empty, fill);
    RUN_TEST_CASE(array_non_empty, push_back);
    RUN_TEST_CASE(array_non_empty, push_back_n);
    RUN_TEST_CASE(array_non_empty, pop_back);
    RUN_TEST_CASE(array_non_empty, pop_back_n);
    RUN_TEST_CASE(array_non_empty, remove);
    RUN_TEST_CASE(array_non_empty, remove_fast_unordered);
    RUN_TEST_CASE(array_non_empty, remove_fast_ordered);
    RUN_TEST_CASE(array_non_empty, clear);
    RUN_TEST_CASE(array_non_empty, reset);
}

void run_test_array()
{
    RUN_TEST_GROUP(array_empty);
    RUN_TEST_GROUP(array_non_empty);
}
