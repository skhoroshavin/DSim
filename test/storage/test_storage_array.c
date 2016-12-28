
#include "test_storage_array.h"
#include <memory.h>

#include "containers/test_array.h"
#include "test.ddl.h"

struct dsim_storage_array array;

TEST assert_storage_array_null( const struct dsim_storage_array *a )
{
    CHECK_CALL(assert_array_null( (struct dsim_array_uint64*)&a->main ));
    CHECK_CALL(assert_array_null( (struct dsim_array_uint64*)&a->back ));
    ASSERT_INT_EQ( dsim_storage_array_can_modify(a), 1 );
    PASS();
}

TEST TEST_ASSERT_STORAGE_ARRAY_SIZE( const struct dsim_storage_array *a, size_t size )
{
    CHECK_CALL(assert_array_capacity( (struct dsim_array_uint64*)&a->main, size ));
    CHECK_CALL(assert_array_null( (struct dsim_array_uint64*)&a->back ));
    ASSERT_INT_EQ( a->main.count, size );
    ASSERT_INT_EQ( dsim_storage_array_can_modify(a), 1 );
    PASS();
}

/*
 * Empty array
 */

TEST storage_array_empty_assert_empty()
{
    CHECK_CALL(assert_storage_array_null( &array ));
    PASS();
}

TEST storage_array_empty_resize()
{
    dsim_storage_array_resize( &array, 10 );

    TEST_ASSERT_STORAGE_ARRAY_SIZE( &array, 10 );
    PASS();
}

TEST storage_array_empty_push_back()
{
    const uint64_t data[] = { 43, 3, 16, 5 };
    dsim_storage_array_push_back( &array, data, count_of(data) );

    TEST_ASSERT_STORAGE_ARRAY_SIZE( &array, count_of(data) );
    ASSERT_MEM_EQ( array.main.data, data, sizeof(data) );
    PASS();
}

TEST storage_array_empty_reset()
{
    dsim_storage_array_reset( &array );

    CHECK_CALL(assert_storage_array_null( &array ));
    PASS();
}

/*
 * Non empty array
 */

static uint64_t test_data[] = { 5, 3, 7, 52, 0, 12, 3, 623, 23 };

TEST storage_array_non_empty_assert_non_empty()
{
    TEST_ASSERT_STORAGE_ARRAY_SIZE( &array, count_of(test_data) );
    ASSERT_MEM_EQ( array.main.data, test_data, sizeof(test_data) );
    PASS();
}

TEST storage_array_non_empty_resize_more()
{
    dsim_storage_array_resize( &array, count_of(test_data) + 4 );

    TEST_ASSERT_STORAGE_ARRAY_SIZE( &array, count_of(test_data) + 4 );
    ASSERT_MEM_EQ( array.main.data, test_data, sizeof(test_data) );
    PASS();
}

TEST storage_array_non_empty_resize_less()
{
    dsim_storage_array_resize( &array, count_of(test_data) - 4 );

    TEST_ASSERT_STORAGE_ARRAY_SIZE( &array, count_of(test_data) - 4 );
    ASSERT_MEM_EQ( array.main.data, test_data, array.main.count*sizeof(test_data[0]) );
    PASS();
}

TEST storage_array_non_empty_push_back()
{
    const uint64_t more_data[] = { 65, 0, 123, 87, 1 };
    dsim_storage_array_push_back( &array, more_data, count_of(more_data) );

    TEST_ASSERT_STORAGE_ARRAY_SIZE( &array, count_of(test_data) + count_of(more_data) );
    ASSERT_MEM_EQ( (uint64_t*)array.main.data, test_data, sizeof(test_data) );
    ASSERT_MEM_EQ( (uint64_t*)array.main.data + count_of(test_data), more_data, sizeof(more_data) );
    PASS();
}

TEST storage_array_non_empty_remove()
{
    dsim_storage_array_remove_fast( &array, 3, 2 );

    CHECK_CALL(assert_array_remove_unordered( (struct dsim_array_uint64*)&array, 3, 2, test_data, count_of(test_data) ));
    PASS();
}

TEST storage_array_non_empty_update()
{
    const uint64_t more_data[] = { 65, 0, 123, 87, 1 };
    dsim_storage_array_update( &array, more_data, 0, 3, count_of(more_data) );

    TEST_ASSERT_STORAGE_ARRAY_SIZE( &array, count_of(test_data) );
    ASSERT_MEM_EQ( (uint64_t*)array.main.data, test_data, 3*sizeof(test_data[0]) );
    ASSERT_MEM_EQ( (uint64_t*)array.main.data + 3, more_data, sizeof(more_data) );
    ASSERT_MEM_EQ( (uint64_t*)array.main.data + 8, test_data + 8, 1*sizeof(test_data[0]) );
    PASS();
}

TEST storage_array_non_empty_read()
{
    const uint64_t *data = (const uint64_t *)dsim_storage_array_begin_read( &array );

    ASSERT( data );
    ASSERT_INT_EQ( dsim_storage_array_can_modify(&array), 0 );
    ASSERT_MEM_EQ( data, test_data, sizeof(test_data) );

    const uint64_t *data2 = (const uint64_t *)dsim_storage_array_begin_read( &array );

    ASSERT( data2 );
    ASSERT_INT_EQ( dsim_storage_array_can_modify(&array), 0 );
    ASSERT_MEM_EQ( data2, test_data, sizeof(test_data) );

    dsim_storage_array_end_read( &array, data );
    ASSERT_INT_EQ( dsim_storage_array_can_modify(&array), 0 );

    ASSERT( !dsim_storage_array_begin_write(&array, DSIM_STORAGE_WRITE_DIRECT) );

    dsim_storage_array_end_read( &array, data2 );
    ASSERT_INT_EQ( dsim_storage_array_can_modify(&array), 1 );
    PASS();
}

TEST storage_array_non_empty_write_buffered()
{
    uint64_t more_data[count_of(test_data)];
    for( size_t i = 0; i != count_of(more_data); ++i )
        more_data[i] = rand();

    const uint64_t *prev_data = (const uint64_t *)dsim_storage_array_begin_read( &array );
    ASSERT( prev_data );
    ASSERT_INT_EQ( dsim_storage_array_can_modify(&array), 0 );
    ASSERT_MEM_EQ( prev_data, test_data, sizeof(test_data) );

    uint64_t *data = (uint64_t *)dsim_storage_array_begin_write( &array, DSIM_STORAGE_WRITE_BUFFERED );
    ASSERT( data );
    ASSERT_INT_EQ( dsim_storage_array_can_modify(&array), 0 );

    memcpy( data, more_data, sizeof(more_data) );
    ASSERT_MEM_EQ( prev_data, test_data, sizeof(test_data) );
    ASSERT_MEM_EQ( data, more_data, sizeof(more_data) );

    dsim_storage_array_end_write( &array, data );
    ASSERT_MEM_EQ( prev_data, test_data, sizeof(test_data) );

    const uint64_t *next_data = (const uint64_t *)dsim_storage_array_begin_read( &array );
    ASSERT( next_data );
    ASSERT_INT_EQ( dsim_storage_array_can_modify(&array), 0 );
    ASSERT_MEM_EQ( prev_data, test_data, sizeof(test_data) );
    ASSERT_MEM_EQ( next_data, more_data, sizeof(more_data) );

    dsim_storage_array_end_read( &array, next_data );
    dsim_storage_array_end_read( &array, prev_data );
    PASS();
}

TEST storage_array_non_empty_reset()
{
    dsim_storage_array_reset( &array );

    CHECK_CALL(assert_storage_array_null( &array ));
    PASS();
}

static void setup_storage_array_empty( void *ctx )
{
    dsim_unused(ctx);
    dsim_ddl_init_test();
    dsim_storage_array_init( &array, ddl_test->type_uint64, &dsim_default_allocator );
}

static void setup_storage_array_non_empty( void *ctx )
{
    dsim_unused(ctx);
    dsim_ddl_init_test();
    dsim_storage_array_init( &array, ddl_test->type_uint64, &dsim_default_allocator );
    dsim_storage_array_push_back( &array, test_data, count_of(test_data) );
}

static void teardown_storage_array( void *ctx )
{
    dsim_unused(ctx);
    dsim_storage_array_reset( &array );
    dsim_ddl_done_test();
}

SUITE(test_storage_array)
{
    SET_SETUP(setup_storage_array_empty, 0);
    SET_TEARDOWN(teardown_storage_array, 0);
    RUN_TEST(storage_array_empty_assert_empty);
    RUN_TEST(storage_array_empty_resize);
    RUN_TEST(storage_array_empty_push_back);
    RUN_TEST(storage_array_empty_reset);

    SET_SETUP(setup_storage_array_non_empty, 0);
    RUN_TEST(storage_array_non_empty_assert_non_empty);
    RUN_TEST(storage_array_non_empty_resize_more);
    RUN_TEST(storage_array_non_empty_resize_less);
    RUN_TEST(storage_array_non_empty_push_back);
    RUN_TEST(storage_array_non_empty_remove);
    RUN_TEST(storage_array_non_empty_update);
    RUN_TEST(storage_array_non_empty_read);
    RUN_TEST(storage_array_non_empty_write_buffered);
    RUN_TEST(storage_array_non_empty_reset);
}

