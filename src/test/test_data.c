
#include "test_data.h"

unsigned test_gen_uint( struct dsim_test_context *ctx, unsigned min, unsigned max )
{
    assert( min <= max ); //  LCOV_EXCL_BR_LINE

    unsigned result;
    dsim_test_context_gen_data( ctx, &result, sizeof(result) );
    if( (unsigned)(max - min) < UINT32_MAX )
        result %= max - min + 1;
    result += min;
    return result;
}

void test_gen_array( struct dsim_test_context *ctx, void **ptr, size_t *count, size_t elem_size, size_t min_count, size_t max_count )
{
    *count = test_gen_uint( ctx, min_count, max_count );
    size_t size = *count * elem_size;
    if( !size )
    {
        *ptr = 0;
        return;
    }

    *ptr = malloc( size );
    dsim_test_context_gen_data( ctx, *ptr, size );
    dsim_test_context_register_ptr( ctx, *ptr );
}
