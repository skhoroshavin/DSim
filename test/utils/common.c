
#include "cmocka.h"

#include "utils/common.h"

static void test_count_of( void **state )
{
    struct test_struct { int a; float b; };

    uint64_t arr1[23];
    int arr2[] = { 2, 5, 1, 0, 2 };
    struct test_struct arr3[] = { { 1, 2.0f }, { -10, 0.0f } };
    const char str[] = "Hello!";

    assert_int_equal( count_of(arr1), 23 );
    assert_int_equal( count_of(arr2), 5 );
    assert_int_equal( count_of(arr3), 2 );
    assert_int_equal( count_of(str),  7 );
}

static void test_container_of( void **state )
{
    struct B
    {
        int x;
        int y;
    };

    struct A
    {
        struct B x;
        int      y;
        struct B z;
    };

    struct A test;

    struct B *test_x = &test.x;
    int      *test_y = &test.y;
    struct B *test_z = &test.z;

    assert_ptr_equal( container_of( test_x, struct A, x ), &test );
    assert_ptr_equal( container_of( test_y, struct A, y ), &test );
    assert_ptr_equal( container_of( test_z, struct A, z ), &test );

    int *test_xx = &test_x->x;
    int *test_xy = &test_x->y;

    assert_ptr_equal( container_of( test_xx, struct B, x ), test_x );
    assert_ptr_equal( container_of( test_xy, struct B, y ), test_x );

    int *test_zx = &test_z->x;
    int *test_zy = &test_z->y;

    assert_ptr_equal( container_of( test_zx, struct B, x ), test_z );
    assert_ptr_equal( container_of( test_zy, struct B, y ), test_z );

    assert_ptr_equal( container_of( test_xx, struct A, x.x ), &test );
    assert_ptr_equal( container_of( test_xy, struct A, x.y ), &test );
    assert_ptr_equal( container_of( test_zx, struct A, z.x ), &test );
    assert_ptr_equal( container_of( test_zy, struct A, z.y ), &test );
}

static void test_next_pow_2( void **state )
{
    assert_int_equal( dsim_next_pow_2(0), 0 );
    assert_int_equal( dsim_next_pow_2(1), 1 );
    assert_int_equal( dsim_next_pow_2(2), 2 );
    assert_int_equal( dsim_next_pow_2(3), 4 );
    assert_int_equal( dsim_next_pow_2(4), 4 );
    assert_int_equal( dsim_next_pow_2(5), 8 );

    assert_int_equal( dsim_next_pow_2(0xffffffff),  0x100000000 );
    assert_int_equal( dsim_next_pow_2(0x100000000), 0x100000000 );
    assert_int_equal( dsim_next_pow_2(0x100000001), 0x200000000 );

    assert_int_equal( dsim_next_pow_2(0x7fffffffffffffff), 0x8000000000000000 );
    assert_int_equal( dsim_next_pow_2(0x8000000000000000), 0x8000000000000000 );
    assert_int_equal( dsim_next_pow_2(0x8000000000000001), 0 );
}

int test_common()
{
    static const struct CMUnitTest common[] =
    {
        cmocka_unit_test(test_count_of),
        cmocka_unit_test(test_container_of),
        cmocka_unit_test(test_next_pow_2 )
    };
    return cmocka_run_group_tests( common, 0, 0 );
}
