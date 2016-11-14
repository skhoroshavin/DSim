
#include <criterion/criterion.h>

#include "containers/array.h"

static struct dsim_uint64_array array = dsim_array_static_init(&dsim_default_allocator);

/*
 * Empty array
 */

void init_array_empty()
{

}

void done_array_empty()
{
    dsim_uint64_array_reset( &array );
}

TestSuite(array_empty, .init = init_array_empty, .fini = done_array_empty );

Test(array_empty, assert_empty)
{
    cr_assert( array.data == 0 );
    cr_assert( array.count == 0 );
    cr_assert( array.capacity == 0 );
}

Test(array_empty, reserve)
{
    dsim_uint64_array_reserve( &array, 10 );
    cr_assert( array.data != 0 );
    cr_assert( array.count == 0 );
    cr_assert( array.capacity >= 10 );
}

Test(array_empty, resize)
{
    dsim_uint64_array_resize( &array, 10 );
    cr_assert( array.data != 0 );
    cr_assert( array.count == 10 );
    cr_assert( array.capacity >= 10 );
}

Test(array_empty, push_back)
{
    dsim_uint64_array_push_back( &array, 5 );
    cr_assert( array.data != 0 );
    cr_assert( array.count == 1 );
    cr_assert( array.capacity >= 1 );
    cr_assert( array.data[0] == 5 );
}

Test(array_empty, push_back_n)
{
    const uint64_t data[] = { 43, 3, 16, 5 };
    dsim_uint64_array_push_back_n( &array, data, count_of(data) );
    cr_assert( array.data != 0 );
    cr_assert( array.count == count_of(data) );
    cr_assert( array.capacity >= count_of(data) );
    cr_assert( memcmp(array.data,data,sizeof(data)) == 0 );
}

Test(array_empty, clear)
{
    dsim_uint64_array_clear( &array );
    cr_assert( array.data == 0 );
    cr_assert( array.count == 0 );
    cr_assert( array.capacity == 0 );
}

Test(array_empty, reset)
{
    dsim_uint64_array_reset( &array );
    cr_assert( array.data == 0 );
    cr_assert( array.count == 0 );
    cr_assert( array.capacity == 0 );
}

/*
 * Non empty array
 */

static uint64_t test_data[] = { 5, 3, 7, 52, 0, 12, 3, 623, 23 };

void init_array_non_empty()
{
    dsim_uint64_array_push_back_n( &array, test_data, count_of(test_data) );
}

void done_array_non_empty()
{
    dsim_uint64_array_reset( &array );
}

TestSuite(array_non_empty, .init = init_array_non_empty, .fini = done_array_non_empty );

Test(array_non_empty, assert_non_empty)
{
    cr_assert( array.data != 0 );
    cr_assert( array.count == count_of(test_data) );
    cr_assert( array.capacity >= count_of(test_data) );
    cr_assert( memcmp(array.data, test_data, sizeof(test_data)) == 0 );
}

Test(array_non_empty, reserve_more)
{
    dsim_uint64_array_reserve( &array, count_of(test_data) + 4 );
    cr_assert( array.data != 0 );
    cr_assert( array.count == count_of(test_data) );
    cr_assert( array.capacity >= count_of(test_data) + 4 );
    cr_assert( memcmp(array.data, test_data, sizeof(test_data)) == 0 );
}

Test(array_non_empty, reserve_less)
{
    dsim_uint64_array_reserve( &array, count_of(test_data) - 4 );
    cr_assert( array.data != 0 );
    cr_assert( array.count == count_of(test_data) );
    cr_assert( array.capacity >= count_of(test_data) );
    cr_assert( memcmp(array.data, test_data, sizeof(test_data)) == 0 );
}

Test(array_non_empty, resize_more)
{
    dsim_uint64_array_resize( &array, count_of(test_data) + 4 );
    cr_assert( array.data != 0 );
    cr_assert( array.count == count_of(test_data) + 4 );
    cr_assert( array.capacity >= count_of(test_data) + 4 );
    cr_assert( memcmp(array.data, test_data, sizeof(test_data)) == 0 );
}

Test(array_non_empty, resize_less)
{
    dsim_uint64_array_resize( &array, count_of(test_data) - 4 );
    cr_assert( array.data != 0 );
    cr_assert( array.count == count_of(test_data) - 4 );
    cr_assert( array.capacity >= count_of(test_data) );
    cr_assert( memcmp(array.data, test_data, array.count*sizeof(test_data[0])) == 0 );
}

Test(array_non_empty, fill)
{
    dsim_uint64_array_fill( &array, 0xdeadbeefcafe );
    cr_assert( array.data != 0 );
    cr_assert( array.count == count_of(test_data) );
    cr_assert( array.capacity >= count_of(test_data) );
    for( size_t i = 0; i < array.count; ++i )
        cr_assert( array.data[i] == 0xdeadbeefcafe );
}

Test(array_non_empty, push_back)
{
    dsim_uint64_array_push_back( &array, 42 );
    cr_assert( array.data != 0 );
    cr_assert( array.count == count_of(test_data) + 1 );
    cr_assert( array.capacity >= count_of(test_data) + 1 );
    cr_assert( memcmp(array.data, test_data, sizeof(test_data)) == 0 );
    cr_assert( array.data[count_of(test_data)] == 42 );
}

Test(array_non_empty, push_back_n)
{
    const uint64_t more_data[] = { 65, 0, 123, 87, 1 };
    dsim_uint64_array_push_back_n( &array, more_data, count_of(more_data) );
    cr_assert( array.data != 0 );
    cr_assert( array.count == count_of(test_data) + count_of(more_data) );
    cr_assert( array.capacity >= count_of(test_data) + count_of(more_data) );
    cr_assert( memcmp(array.data, test_data, sizeof(test_data)) == 0 );
    cr_assert( memcmp(array.data + count_of(test_data), more_data, sizeof(more_data)) == 0 );
}

Test(array_non_empty, pop_back)
{
    dsim_uint64_array_pop_back( &array );
    cr_assert( array.data != 0 );
    cr_assert( array.count == count_of(test_data) - 1 );
    cr_assert( array.capacity >= count_of(test_data) );
    cr_assert( memcmp(array.data, test_data, array.count*sizeof(test_data[0])) == 0 );
}

Test(array_non_empty, pop_back_n)
{
    dsim_uint64_array_pop_back_n( &array, 3 );
    cr_assert( array.data != 0 );
    cr_assert( array.count == count_of(test_data) - 3 );
    cr_assert( array.capacity >= count_of(test_data) );
    cr_assert( memcmp(array.data, test_data, array.count*sizeof(test_data[0])) == 0 );
}

Test(array_non_empty, remove)
{
    dsim_uint64_array_remove( &array, 2, 3 );
    cr_assert( array.data != 0 );
    cr_assert( array.count == count_of(test_data) - 3 );
    cr_assert( array.capacity >= count_of(test_data) );
    cr_assert( memcmp(array.data, test_data, 2*sizeof(test_data[0])) == 0 );
    cr_assert( memcmp(array.data + 2, test_data + 5, (count_of(test_data) - 5)*sizeof(test_data[0])) == 0 );
}

Test(array_non_empty, remove_fast_unordered)
{
    dsim_uint64_array_remove_fast( &array, 3, 2 );
    cr_assert( array.data != 0 );
    cr_assert( array.count == count_of(test_data) - 2 );
    cr_assert( array.capacity >= count_of(test_data) );
    cr_assert( memcmp(array.data, test_data, 3*sizeof(test_data[0])) == 0 );
    cr_assert( memcmp(array.data + 3, test_data + count_of(test_data) - 2, 2*sizeof(test_data[0])) == 0 );
    cr_assert( memcmp(array.data + 5, test_data + 5, (array.count - 5)*sizeof(test_data[0])) == 0 );
}

Test(array_non_empty, remove_fast_ordered)
{
    dsim_uint64_array_remove_fast( &array, 2, 4 );
    cr_assert( array.data != 0 );
    cr_assert( array.count == count_of(test_data) - 4 );
    cr_assert( array.capacity >= count_of(test_data) );
    cr_assert( memcmp(array.data, test_data, 2*sizeof(test_data[0])) == 0 );
    cr_assert( memcmp(array.data + 2, test_data + 6, (count_of(test_data) - 6)*sizeof(test_data[0])) == 0 );
}

Test(array_non_empty, clear)
{
    dsim_uint64_array_clear( &array );
    cr_assert( array.data != 0 );
    cr_assert( array.count == 0 );
    cr_assert( array.capacity >= count_of(test_data) );
}

Test(array_non_empty, reset)
{
    dsim_uint64_array_reset( &array );
    cr_assert( array.data == 0 );
    cr_assert( array.count == 0 );
    cr_assert( array.capacity == 0 );
}
