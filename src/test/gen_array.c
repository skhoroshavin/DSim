
#include "gen_array.h"
#include "gen_int.h"

void test_gen_array( struct dsim_test_data *data, void **ptr, size_t *count, size_t elem_size, size_t min_count, size_t max_count )
{
    *count = test_gen_uint( data, min_count, max_count );
    size_t size = *count * elem_size;
    if( !size )
    {
        *ptr = 0;
        return;
    }

    *ptr = malloc( size );
    dsim_test_data_gen( data, *ptr, size );
    dsim_test_data_register_ptr( data, *ptr );
}
