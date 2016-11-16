
#include <criterion/criterion.h>

#include "allocators/allocator.h"
#include "allocators/stack_allocator.h"

static void test_allocate( struct dsim_allocator *alloc )
{
    void *ptr = dsim_allocate( alloc, 25 );
    cr_assert( ptr != 0 );
    memset( ptr, 0xaa, 25 ); // Make sure memory is accessible
    dsim_deallocate( alloc, ptr, 25 );
}

static void test_reallocate( struct dsim_allocator *alloc )
{
    const uint32_t test_data[] = { 23, 65, 0, 12, 52 };
    void *ptr = dsim_allocate( alloc, sizeof(test_data) );

    memcpy( ptr, test_data, sizeof(test_data) );
    cr_assert( memcmp( ptr, test_data, sizeof(test_data) ) == 0 );

    ptr = dsim_reallocate( alloc, ptr, sizeof(test_data), sizeof(test_data)+64 );
    cr_assert( ptr != 0 );
    cr_assert( memcmp( ptr, test_data, sizeof(test_data) ) == 0 );

    dsim_deallocate( alloc, ptr, sizeof(test_data)+64 );
}

static void test_reallocate_from_null( struct dsim_allocator *alloc )
{
    void *ptr = dsim_reallocate( alloc, 0, 0, 17 );
    cr_assert( ptr != 0 );
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

    cr_assert( memcmp( ptr1, test_data1, sizeof(test_data1) ) == 0 );
    cr_assert( memcmp( ptr2, test_data2, sizeof(test_data2) ) == 0 );

    ptr1 = dsim_reallocate( alloc, ptr1, sizeof(test_data1), sizeof(test_data1) + 33 );
    ptr2 = dsim_reallocate( alloc, ptr2, sizeof(test_data2), sizeof(test_data2) + 11 );

    cr_assert( memcmp( ptr1, test_data1, sizeof(test_data1) ) == 0 );
    cr_assert( memcmp( ptr2, test_data2, sizeof(test_data2) ) == 0 );

    dsim_deallocate( alloc, ptr1, sizeof(test_data1)+33 );
    dsim_deallocate( alloc, ptr2, sizeof(test_data2)+11 );
}

static void test_reallocate_less( struct dsim_allocator *alloc )
{
    const uint32_t test_data[] = { 23, 65, 12, 98, 0, 3, 53 };
    void * ptr = dsim_allocate( alloc, sizeof(test_data) );
    memcpy( ptr, test_data, sizeof(test_data) );

    ptr = dsim_reallocate( alloc, ptr, sizeof(test_data), sizeof(test_data)-13 );
    cr_assert( memcmp( ptr, test_data, sizeof(test_data)-13 ) == 0 );

    dsim_deallocate( alloc, ptr, sizeof(test_data)-13 );
}

#define TEST_ALLOCATOR(name,alloc) \
    Test(name, allocate) { test_allocate( alloc ); } \
    Test(name, reallocate) { test_reallocate( alloc ); } \
    Test(name, reallocate_from_null) { test_reallocate_from_null( alloc ); } \
    Test(name, reallocate_twin) { test_reallocate_twin( alloc ); } \
    Test(name, reallocate_less) { test_reallocate_less( alloc ); } \

TEST_ALLOCATOR(default_allocator, &dsim_default_allocator)
TEST_ALLOCATOR(stack_allocator, &dsim_stack_allocator_0.alloc)
