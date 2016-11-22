
#include "cmocka.h"

#include "allocators/allocator.h"
#include "allocators/stack_allocator.h"

static void test_allocate( void **state )
{
    struct dsim_allocator *alloc = *(struct dsim_allocator**)state;

    void *ptr = dsim_allocate( alloc, 25 );
    assert_non_null( ptr );
    memset( ptr, 0xaa, 25 ); // Make sure memory is accessible
    dsim_deallocate( alloc, ptr, 25 );
}

static void test_reallocate( void **state )
{
    struct dsim_allocator *alloc = *(struct dsim_allocator**)state;

    const uint32_t test_data[] = { 23, 65, 0, 12, 52 };
    void *ptr = dsim_allocate( alloc, sizeof(test_data) );

    memcpy( ptr, test_data, sizeof(test_data) );
    assert_memory_equal( ptr, test_data, sizeof(test_data) );

    ptr = dsim_reallocate( alloc, ptr, sizeof(test_data), sizeof(test_data)+64 );
    assert_non_null( ptr );
    assert_memory_equal( ptr, test_data, sizeof(test_data) );

    dsim_deallocate( alloc, ptr, sizeof(test_data)+64 );
}

static void test_reallocate_from_null( void **state )
{
    struct dsim_allocator *alloc = *(struct dsim_allocator**)state;

    void *ptr = dsim_reallocate( alloc, 0, 0, 17 );
    assert_non_null( ptr );
    memset( ptr, 0xaa, 17 ); // Make sure memory is accessible
    dsim_deallocate( alloc, ptr, 17 );
}

static void test_reallocate_twin( void **state )
{
    struct dsim_allocator *alloc = *(struct dsim_allocator**)state;

    const uint32_t test_data1[] = { 3, 53, 25, 11, 76 };
    void *ptr1 = dsim_allocate( alloc, sizeof(test_data1) );

    const uint8_t test_data2[] = { 5, 2, 4 };
    void *ptr2 = dsim_allocate( alloc, sizeof(test_data2) );

    memcpy( ptr1, test_data1, sizeof(test_data1) );
    memcpy( ptr2, test_data2, sizeof(test_data2) );

    assert_memory_equal( ptr1, test_data1, sizeof(test_data1) );
    assert_memory_equal( ptr2, test_data2, sizeof(test_data2) );

    ptr1 = dsim_reallocate( alloc, ptr1, sizeof(test_data1), sizeof(test_data1) + 33 );
    ptr2 = dsim_reallocate( alloc, ptr2, sizeof(test_data2), sizeof(test_data2) + 11 );

    assert_memory_equal( ptr1, test_data1, sizeof(test_data1) );
    assert_memory_equal( ptr2, test_data2, sizeof(test_data2) );

    dsim_deallocate( alloc, ptr1, sizeof(test_data1)+33 );
    dsim_deallocate( alloc, ptr2, sizeof(test_data2)+11 );
}

static void test_reallocate_less( void **state )
{
    struct dsim_allocator *alloc = *(struct dsim_allocator**)state;

    const uint32_t test_data[] = { 23, 65, 12, 98, 0, 3, 53 };
    void * ptr = dsim_allocate( alloc, sizeof(test_data) );
    memcpy( ptr, test_data, sizeof(test_data) );

    ptr = dsim_reallocate( alloc, ptr, sizeof(test_data), sizeof(test_data)-13 );
    assert_memory_equal( ptr, test_data, sizeof(test_data)-13 );

    dsim_deallocate( alloc, ptr, sizeof(test_data)-13 );
}

static const struct CMUnitTest allocator_tests[] =
{
    cmocka_unit_test( test_allocate ),
    cmocka_unit_test( test_reallocate ),
    cmocka_unit_test( test_reallocate_from_null ),
    cmocka_unit_test( test_reallocate_twin ),
    cmocka_unit_test( test_reallocate_less )
};

static int test_default_allocator_setup( void **state )
{
    *state = &dsim_default_allocator;
    return 0;
}

static int test_stack_allocator_setup( void **state )
{
    *state = &dsim_stack_allocator_0.alloc;
    return 0;
}

#define run_allocator_tests(name) cmocka_run_group_tests_name( #name, allocator_tests, test_##name##_setup, 0 )

int test_allocator()
{
    int result = 0;
    result |= run_allocator_tests( default_allocator );
    result |= run_allocator_tests( stack_allocator );
    return result;
}
