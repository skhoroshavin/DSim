
#include <criterion/criterion.h>

#include "allocators/allocator.h"

Test(allocator, default_allocator)
{
    const uint32_t test_data[] = { 23, 65, 0, 12, 52 };
    void *ptr = dsim_allocate( &dsim_default_allocator, sizeof(test_data) );
    cr_assert( ptr != 0 );

    memcpy( ptr, test_data, sizeof(test_data) );
    cr_assert( memcmp( ptr, test_data, sizeof(test_data) ) == 0 );

    ptr = dsim_reallocate( &dsim_default_allocator, ptr, sizeof(test_data), sizeof(test_data)+64 );
    cr_assert( memcmp( ptr, test_data, sizeof(test_data) ) == 0 );

    dsim_deallocate( &dsim_default_allocator, ptr, sizeof(test_data)+64 );
}
