
#include "unity_fixture.h"
#include <memory.h>

#include "allocators/allocator.h"
#include "allocators/stack_allocator.h"

static void test_allocate( struct dsim_allocator *alloc )
{
    void *ptr = dsim_allocate( alloc, 25 );
    TEST_ASSERT_NOT_NULL( ptr );
    memset( ptr, 0xaa, 25 ); // Make sure memory is accessible
    dsim_deallocate( alloc, ptr, 25 );
}

static void test_reallocate( struct dsim_allocator *alloc )
{
    const uint32_t test_data[] = { 23, 65, 0, 12, 52 };
    void *ptr = dsim_allocate( alloc, sizeof(test_data) );

    memcpy( ptr, test_data, sizeof(test_data) );
    TEST_ASSERT_EQUAL_MEMORY( ptr, test_data, sizeof(test_data) );

    ptr = dsim_reallocate( alloc, ptr, sizeof(test_data), sizeof(test_data)+64 );
    TEST_ASSERT_NOT_NULL( ptr );
    TEST_ASSERT_EQUAL_MEMORY( ptr, test_data, sizeof(test_data) );

    dsim_deallocate( alloc, ptr, sizeof(test_data)+64 );
}

static void test_reallocate_from_null( struct dsim_allocator *alloc )
{
    void *ptr = dsim_reallocate( alloc, 0, 0, 17 );
    TEST_ASSERT_NOT_NULL( ptr );
    memset( ptr, 0xaa, 17 ); // Make sure memory is accessible
    dsim_deallocate( alloc, ptr, 17 );
}

static void test_reallocate_twin( struct dsim_allocator *alloc )
{
    const uint32_t test_data1[] = { 3, 53, 25, 11, 76 };
    void *ptr1 = dsim_allocate( alloc, sizeof(test_data1) );

    const uint8_t test_data2[] = { 5, 2, 4 };
    void *ptr2 = dsim_allocate( alloc, sizeof(test_data2) );

    memcpy( ptr1, test_data1, sizeof(test_data1) );
    memcpy( ptr2, test_data2, sizeof(test_data2) );

    TEST_ASSERT_EQUAL_MEMORY( ptr1, test_data1, sizeof(test_data1) );
    TEST_ASSERT_EQUAL_MEMORY( ptr2, test_data2, sizeof(test_data2) );

    ptr1 = dsim_reallocate( alloc, ptr1, sizeof(test_data1), sizeof(test_data1) + 33 );
    ptr2 = dsim_reallocate( alloc, ptr2, sizeof(test_data2), sizeof(test_data2) + 11 );

    TEST_ASSERT_EQUAL_MEMORY( ptr1, test_data1, sizeof(test_data1) );
    TEST_ASSERT_EQUAL_MEMORY( ptr2, test_data2, sizeof(test_data2) );

    dsim_deallocate( alloc, ptr1, sizeof(test_data1)+33 );
    dsim_deallocate( alloc, ptr2, sizeof(test_data2)+11 );
}

static void test_reallocate_less( struct dsim_allocator *alloc )
{
    const uint32_t test_data[] = { 23, 65, 12, 98, 0, 3, 53 };
    void * ptr = dsim_allocate( alloc, sizeof(test_data) );
    memcpy( ptr, test_data, sizeof(test_data) );

    ptr = dsim_reallocate( alloc, ptr, sizeof(test_data), sizeof(test_data)-13 );
    TEST_ASSERT_EQUAL_MEMORY( ptr, test_data, sizeof(test_data)-13 );

    dsim_deallocate( alloc, ptr, sizeof(test_data)-13 );
}

#define TEST_ALLOCATOR(name,alloc) \
    TEST_GROUP(name); \
    TEST_SETUP(name) { } \
    TEST_TEAR_DOWN(name) { } \
    TEST(name, allocate)             { test_allocate( alloc ); } \
    TEST(name, reallocate)           { test_reallocate( alloc ); } \
    TEST(name, reallocate_from_null) { test_reallocate_from_null( alloc ); } \
    TEST(name, reallocate_twin)      { test_reallocate_twin( alloc ); } \
    TEST(name, reallocate_less)      { test_reallocate_less( alloc ); } \
    TEST_GROUP_RUNNER(name) { \
        RUN_TEST_CASE( name, allocate ); \
        RUN_TEST_CASE( name, reallocate ); \
        RUN_TEST_CASE( name, reallocate_from_null ); \
        RUN_TEST_CASE( name, reallocate_twin ); \
        RUN_TEST_CASE( name, reallocate_less ); \
    } \
    static void run_test_##name()    { RUN_TEST_GROUP(name); }

TEST_ALLOCATOR(default_allocator, &dsim_default_allocator)
TEST_ALLOCATOR(stack_allocator, &dsim_stack_allocator_0.alloc)

void run_test_allocator()
{
    RUN_TEST_GROUP(default_allocator);
    RUN_TEST_GROUP(stack_allocator);
}
