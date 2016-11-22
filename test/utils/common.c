
/*
#include <criterion/criterion.h>

#include "utils/common.h"

Test(utils_common, count_of)
{
    struct test_struct { int a; float b; };

    uint64_t arr1[23];
    int arr2[] = { 2, 5, 1, 0, 2 };
    struct test_struct arr3[] = { { 1, 2.0f }, { -10, 0.0f } };
    const char str[] = "Hello!";

    cr_assert( count_of(arr1) == 23 );
    cr_assert( count_of(arr2) == 5 );
    cr_assert( count_of(arr3) == 2 );
    cr_assert( count_of(str) == 7 );
}

Test(utils_common, container_of)
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

    cr_assert( container_of( test_x, struct A, x ) == &test );
    cr_assert( container_of( test_y, struct A, y ) == &test );
    cr_assert( container_of( test_z, struct A, z ) == &test );

    int *test_xx = &test_x->x;
    int *test_xy = &test_x->y;

    cr_assert( container_of( test_xx, struct B, x ) == test_x );
    cr_assert( container_of( test_xy, struct B, y ) == test_x );

    int *test_zx = &test_z->x;
    int *test_zy = &test_z->y;

    cr_assert( container_of( test_zx, struct B, x ) == test_z );
    cr_assert( container_of( test_zy, struct B, y ) == test_z );

    cr_assert( container_of( test_xx, struct A, x.x ) == &test );
    cr_assert( container_of( test_xy, struct A, x.y ) == &test );
    cr_assert( container_of( test_zx, struct A, z.x ) == &test );
    cr_assert( container_of( test_zy, struct A, z.y ) == &test );
}

Test(utils_common, next_pow_2)
{
    cr_assert( dsim_next_pow_2(0) == 0 );
    cr_assert( dsim_next_pow_2(1) == 1 );
    cr_assert( dsim_next_pow_2(2) == 2 );
    cr_assert( dsim_next_pow_2(3) == 4 );
    cr_assert( dsim_next_pow_2(4) == 4 );
    cr_assert( dsim_next_pow_2(5) == 8 );

    cr_assert( dsim_next_pow_2(0xffffffff)  == 0x100000000 );
    cr_assert( dsim_next_pow_2(0x100000000) == 0x100000000 );
    cr_assert( dsim_next_pow_2(0x100000001) == 0x200000000 );

    cr_assert( dsim_next_pow_2(0x7fffffffffffffff) == 0x8000000000000000 );
    cr_assert( dsim_next_pow_2(0x8000000000000000) == 0x8000000000000000 );
    cr_assert( dsim_next_pow_2(0x8000000000000001) == 0 );
}
*/
