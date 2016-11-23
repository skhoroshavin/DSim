
#include "unity_fixture.h"

#include "utils/common.h"

TEST_GROUP(common);

TEST_SETUP(common) { }
TEST_TEAR_DOWN(common) { }

TEST(common, count_of)
{
    struct test_struct { int a; float b; };

    uint64_t arr1[23];
    int arr2[] = { 2, 5, 1, 0, 2 };
    struct test_struct arr3[] = { { 1, 2.0f }, { -10, 0.0f } };
    const char str[] = "Hello!";

    TEST_ASSERT_EQUAL( count_of(arr1), 23 );
    TEST_ASSERT_EQUAL( count_of(arr2), 5 );
    TEST_ASSERT_EQUAL( count_of(arr3), 2 );
    TEST_ASSERT_EQUAL( count_of(str),  7 );
}

TEST(common, container_of)
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

    TEST_ASSERT_EQUAL_PTR( container_of( test_x, struct A, x ), &test );
    TEST_ASSERT_EQUAL_PTR( container_of( test_y, struct A, y ), &test );
    TEST_ASSERT_EQUAL_PTR( container_of( test_z, struct A, z ), &test );

    int *test_xx = &test_x->x;
    int *test_xy = &test_x->y;

    TEST_ASSERT_EQUAL_PTR( container_of( test_xx, struct B, x ), test_x );
    TEST_ASSERT_EQUAL_PTR( container_of( test_xy, struct B, y ), test_x );

    int *test_zx = &test_z->x;
    int *test_zy = &test_z->y;

    TEST_ASSERT_EQUAL_PTR( container_of( test_zx, struct B, x ), test_z );
    TEST_ASSERT_EQUAL_PTR( container_of( test_zy, struct B, y ), test_z );

    TEST_ASSERT_EQUAL_PTR( container_of( test_xx, struct A, x.x ), &test );
    TEST_ASSERT_EQUAL_PTR( container_of( test_xy, struct A, x.y ), &test );
    TEST_ASSERT_EQUAL_PTR( container_of( test_zx, struct A, z.x ), &test );
    TEST_ASSERT_EQUAL_PTR( container_of( test_zy, struct A, z.y ), &test );
}

TEST(common, next_pow_2)
{
    TEST_ASSERT_EQUAL_HEX64( dsim_next_pow_2(0), 0 );
    TEST_ASSERT_EQUAL_HEX64( dsim_next_pow_2(1), 1 );
    TEST_ASSERT_EQUAL_HEX64( dsim_next_pow_2(2), 2 );
    TEST_ASSERT_EQUAL_HEX64( dsim_next_pow_2(3), 4 );
    TEST_ASSERT_EQUAL_HEX64( dsim_next_pow_2(4), 4 );
    TEST_ASSERT_EQUAL_HEX64( dsim_next_pow_2(5), 8 );

    TEST_ASSERT_EQUAL_HEX64( dsim_next_pow_2(0xffffffff) , 0x100000000 );
    TEST_ASSERT_EQUAL_HEX64( dsim_next_pow_2(0x100000000), 0x100000000 );
    TEST_ASSERT_EQUAL_HEX64( dsim_next_pow_2(0x100000001), 0x200000000 );

    TEST_ASSERT_EQUAL_HEX64( dsim_next_pow_2(0x7fffffffffffffff), 0x8000000000000000 );
    TEST_ASSERT_EQUAL_HEX64( dsim_next_pow_2(0x8000000000000000), 0x8000000000000000 );
    TEST_ASSERT_EQUAL_HEX64( dsim_next_pow_2(0x8000000000000001), 0 );
}

TEST_GROUP_RUNNER(common)
{
    RUN_TEST_CASE(common, count_of);
    RUN_TEST_CASE(common, container_of);
    RUN_TEST_CASE(common, next_pow_2);
}

void run_test_common()
{
    RUN_TEST_GROUP(common);
}
