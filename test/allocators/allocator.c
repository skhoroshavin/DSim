
#include "greatest.h"
#include <memory.h>

#include "allocators/allocator.h"
#include "allocators/stack_allocator.h"

TEST test_allocate( struct dsim_allocator *alloc )
{
    void *ptr = dsim_allocate( alloc, 25 );
    ASSERT( ptr );
    memset( ptr, 0xaa, 25 ); // Make sure memory is accessible
    dsim_deallocate( alloc, ptr );
    PASS();
}

TEST test_reallocate( struct dsim_allocator *alloc )
{
    const uint32_t test_data[] = { 23, 65, 0, 12, 52 };
    void *ptr = dsim_allocate( alloc, sizeof(test_data) );

    memcpy( ptr, test_data, sizeof(test_data) );
    ASSERT_MEM_EQ( ptr, test_data, sizeof(test_data) );

    ptr = dsim_reallocate( alloc, ptr, sizeof(test_data)+64 );
    ASSERT( ptr );
    ASSERT_MEM_EQ( ptr, test_data, sizeof(test_data) );

    dsim_deallocate( alloc, ptr );
    PASS();
}

TEST test_reallocate_from_null( struct dsim_allocator *alloc )
{
    void *ptr = dsim_reallocate( alloc, 0, 17 );
    ASSERT( ptr );
    memset( ptr, 0xaa, 17 ); // Make sure memory is accessible
    dsim_deallocate( alloc, ptr );
    PASS();
}

TEST test_reallocate_twin( struct dsim_allocator *alloc )
{
    const uint32_t test_data1[] = { 3, 53, 25, 11, 76 };
    void *ptr1 = dsim_allocate( alloc, sizeof(test_data1) );

    const uint8_t test_data2[] = { 5, 2, 4 };
    void *ptr2 = dsim_allocate( alloc, sizeof(test_data2) );

    memcpy( ptr1, test_data1, sizeof(test_data1) );
    memcpy( ptr2, test_data2, sizeof(test_data2) );

    ASSERT_MEM_EQ( ptr1, test_data1, sizeof(test_data1) );
    ASSERT_MEM_EQ( ptr2, test_data2, sizeof(test_data2) );

    ptr1 = dsim_reallocate( alloc, ptr1, sizeof(test_data1) + 33 );
    ptr2 = dsim_reallocate( alloc, ptr2, sizeof(test_data2) + 11 );

    ASSERT_MEM_EQ( ptr1, test_data1, sizeof(test_data1) );
    ASSERT_MEM_EQ( ptr2, test_data2, sizeof(test_data2) );

    dsim_deallocate( alloc, ptr1 );
    dsim_deallocate( alloc, ptr2 );
    PASS();
}

TEST test_reallocate_less( struct dsim_allocator *alloc )
{
    const uint32_t test_data[] = { 23, 65, 12, 98, 0, 3, 53 };
    void *ptr = dsim_allocate( alloc, sizeof(test_data) );
    memcpy( ptr, test_data, sizeof(test_data) );

    ptr = dsim_reallocate( alloc, ptr, sizeof(test_data)-13 );
    ASSERT_MEM_EQ( test_data, ptr, sizeof(test_data)-13 );

    dsim_deallocate( alloc, ptr );
    PASS();
}

static void test_allocator_p( struct dsim_allocator *alloc )
{
    RUN_TESTp( test_allocate,             alloc );
    RUN_TESTp( test_reallocate,           alloc );
    RUN_TESTp( test_reallocate_from_null, alloc );
    RUN_TESTp( test_reallocate_twin,      alloc );
    RUN_TESTp( test_reallocate_less,      alloc );
}

SUITE(test_allocator)
{
    test_allocator_p( &dsim_default_allocator );
    test_allocator_p( &dsim_stack_allocator_0.alloc );
}
