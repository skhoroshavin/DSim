
#include "test_storage_array.h"
#include "test_allocator.h"
#include <memory.h>

#include "containers/test_array.h"
#include "test.ddl.h"

struct dsim_storage_array array;

static void TEST_ASSERT_STORAGE_ARRAY_NULL( const struct dsim_storage_array *a )
{
    TEST_ASSERT_ARRAY_NULL( (struct dsim_array_uint64*)&a->current );
    TEST_ASSERT_ARRAY_NULL( (struct dsim_array_uint64*)&a->next );
    TEST_ASSERT_EQUAL( dsim_storage_array_can_modify(a), 1 );
}

static void TEST_ASSERT_STORAGE_ARRAY_SIZE( const struct dsim_storage_array *a, size_t size )
{
    TEST_ASSERT_ARRAY_CAPACITY( (struct dsim_array_uint64*)&a->current, size );
    TEST_ASSERT_ARRAY_NULL( (struct dsim_array_uint64*)&a->next );
    TEST_ASSERT_EQUAL( a->current.count, size );
    TEST_ASSERT_EQUAL( dsim_storage_array_can_modify(a), 1 );
}

/*
 * Empty array
 */

TEST_GROUP(storage_array_empty);

TEST_SETUP(storage_array_empty)
{
    dsim_ddl_init_test();
    dsim_storage_array_init( &array, ddl_test->type_uint64, &dsim_test_allocator );
}

TEST_TEAR_DOWN(storage_array_empty)
{
    dsim_storage_array_reset( &array );
    dsim_ddl_done_test();
}

TEST(storage_array_empty, assert_empty)
{
    TEST_ASSERT_STORAGE_ARRAY_NULL( &array );
}

TEST(storage_array_empty, resize)
{
    dsim_storage_array_resize( &array, 10 );

    TEST_ASSERT_STORAGE_ARRAY_SIZE( &array, 10 );
}

TEST(storage_array_empty, push_back)
{
    const uint64_t data[] = { 43, 3, 16, 5 };
    dsim_storage_array_push_back( &array, data, count_of(data) );

    TEST_ASSERT_STORAGE_ARRAY_SIZE( &array, count_of(data) );
    TEST_ASSERT_EQUAL_MEMORY( array.current.data, data, sizeof(data) );
}

TEST(storage_array_empty, reset)
{
    dsim_storage_array_reset( &array );

    TEST_ASSERT_STORAGE_ARRAY_NULL( &array );
}

TEST_GROUP_RUNNER(storage_array_empty)
{
    RUN_TEST_CASE(storage_array_empty, assert_empty);
    RUN_TEST_CASE(storage_array_empty, resize);
    RUN_TEST_CASE(storage_array_empty, push_back);
    RUN_TEST_CASE(storage_array_empty, reset);
}

/*
 * Non empty array
 */

TEST_GROUP(storage_array_non_empty);

static uint64_t test_data[] = { 5, 3, 7, 52, 0, 12, 3, 623, 23 };

TEST_SETUP(storage_array_non_empty)
{
    dsim_ddl_init_test();
    dsim_storage_array_init( &array, ddl_test->type_uint64, &dsim_test_allocator );
    dsim_storage_array_push_back( &array, test_data, count_of(test_data) );
}

TEST_TEAR_DOWN(storage_array_non_empty)
{
    dsim_storage_array_reset( &array );
    dsim_ddl_done_test();
}

TEST(storage_array_non_empty, assert_non_empty)
{
    TEST_ASSERT_STORAGE_ARRAY_SIZE( &array, count_of(test_data) );
    TEST_ASSERT_EQUAL_MEMORY( array.current.data, test_data, sizeof(test_data) );
}
TEST(storage_array_non_empty, resize_more)
{
    dsim_storage_array_resize( &array, count_of(test_data) + 4 );

    TEST_ASSERT_STORAGE_ARRAY_SIZE( &array, count_of(test_data) + 4 );
    TEST_ASSERT_EQUAL_MEMORY( array.current.data, test_data, sizeof(test_data) );
}

TEST(storage_array_non_empty, resize_less)
{
    dsim_storage_array_resize( &array, count_of(test_data) - 4 );

    TEST_ASSERT_STORAGE_ARRAY_SIZE( &array, count_of(test_data) - 4 );
    TEST_ASSERT_EQUAL_MEMORY( array.current.data, test_data, array.current.count*sizeof(test_data[0]) );
}

TEST(storage_array_non_empty, push_back)
{
    const uint64_t more_data[] = { 65, 0, 123, 87, 1 };
    dsim_storage_array_push_back( &array, more_data, count_of(more_data) );

    TEST_ASSERT_STORAGE_ARRAY_SIZE( &array, count_of(test_data) + count_of(more_data) );
    TEST_ASSERT_EQUAL_MEMORY( (uint64_t*)array.current.data, test_data, sizeof(test_data) );
    TEST_ASSERT_EQUAL_MEMORY( (uint64_t*)array.current.data + count_of(test_data), more_data, sizeof(more_data) );
}

TEST(storage_array_non_empty, remove)
{
    dsim_storage_array_remove_fast( &array, 3, 2 );

    TEST_ASSERT_ARRAY_REMOVE_UNORDERED( (struct dsim_array_uint64*)&array, 3, 2, test_data, count_of(test_data) );
}

TEST(storage_array_non_empty, reset)
{
    dsim_storage_array_reset( &array );

    TEST_ASSERT_STORAGE_ARRAY_NULL( &array );
}

TEST_GROUP_RUNNER(storage_array_non_empty)
{
    RUN_TEST_CASE(storage_array_non_empty, assert_non_empty);
    RUN_TEST_CASE(storage_array_non_empty, resize_more);
    RUN_TEST_CASE(storage_array_non_empty, resize_less);
    RUN_TEST_CASE(storage_array_non_empty, push_back);
    RUN_TEST_CASE(storage_array_non_empty, remove);
    //    RUN_TEST_CASE(storage_array_non_empty, update);
    RUN_TEST_CASE(storage_array_non_empty, reset);
}

void run_test_storage_array()
{
    RUN_TEST_GROUP(storage_array_empty);
    RUN_TEST_GROUP(storage_array_non_empty);
}

