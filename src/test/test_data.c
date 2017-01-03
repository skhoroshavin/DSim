
#include "test_data.h"

void dsim_test_data_example( struct dsim_test_data *self, struct dsim_test_context *ctx, void *data )
{
    self->example( self, ctx, data );
}



static void dsim_test_uint_in_range_data_example( struct dsim_test_data *self, struct dsim_test_context *ctx, void *data )
{
    struct dsim_test_uint_in_range_data *params = container_of(self, struct dsim_test_uint_in_range_data, base);

    unsigned result;
    dsim_test_context_gen_data( ctx, &result, sizeof(result) );
    if( (unsigned)(params->max_value - params->min_value) < UINT32_MAX )
        result %= params->max_value - params->min_value + 1;
    result += params->min_value;
    *(unsigned *)data = result;
}

void dsim_test_uint_in_range_data_init( struct dsim_test_uint_in_range_data *self, unsigned min_value, unsigned max_value )
{
    self->base.example = dsim_test_uint_in_range_data_example;
    self->base.size = sizeof(unsigned);
    self->min_value = min_value;
    self->max_value = max_value;
}



static void dsim_test_array_data_example( struct dsim_test_data *self, struct dsim_test_context *ctx, void *data )
{
    struct dsim_test_array_data *params = container_of(self, struct dsim_test_array_data, base);

    struct dsim_test_array result;
    result.count = test_gen_uint( ctx, params->min_count, 16 );
    size_t size = result.count * params->elem->size;
    if( !size )
    {
        result.data = 0;
        goto end;
    }

    result.data = malloc( size );
    dsim_test_context_register_ptr( ctx, result.data );
    for( size_t i = 0; i != result.count; ++i )
        dsim_test_data_example( params->elem, ctx, (char*)result.data + i*params->elem->size );

end:
    *(struct dsim_test_array *)data = result;
}

void dsim_test_array_data_init( struct dsim_test_array_data *self, struct dsim_test_data *elem, size_t min_count )
{
    self->base.example = dsim_test_array_data_example;
    self->base.size = sizeof(struct dsim_test_array);
    self->elem = elem;
    self->min_count = min_count;
}




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
