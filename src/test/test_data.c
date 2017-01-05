
#include "test_data.h"
#include "test_context.h"

void dsim_test_data_example( struct dsim_test_datagen *self, struct dsim_test_context *ctx, void *data )
{
    self->example( self, ctx, data );
}



static void dsim_test_uint_in_range_data_example( struct dsim_test_datagen *self, struct dsim_test_context *ctx, void *data )
{
    struct dsim_test_uint_in_range_data *params = container_of(self, struct dsim_test_uint_in_range_data, base);

    uint64_t result;
    dsim_test_context_gen_data( ctx, &result, sizeof(result) );
    if( (unsigned)(params->max_value - params->min_value) < UINT32_MAX )
        result %= params->max_value - params->min_value + 1;
    result += params->min_value;
    switch( self->size )
    {
    case 1: *(uint8_t *)data  = result; break;
    case 2: *(uint16_t *)data = result; break;
    case 4: *(uint32_t *)data = result; break;
    case 8: *(uint64_t *)data = result; break;
    }
}

void dsim_test_uint_in_range_data_init( struct dsim_test_uint_in_range_data *self, unsigned size, unsigned min_value, unsigned max_value )
{
    self->base.example = dsim_test_uint_in_range_data_example;
    self->base.size = size;
    self->min_value = min_value;
    self->max_value = max_value;
}



static void dsim_test_array_data_example( struct dsim_test_datagen *self, struct dsim_test_context *ctx, void *data )
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

void dsim_test_array_data_init( struct dsim_test_array_data *self, struct dsim_test_datagen *elem, size_t min_count )
{
    self->base.example = dsim_test_array_data_example;
    self->base.size = sizeof(struct dsim_test_array);
    self->elem = elem;
    self->min_count = min_count;
}




unsigned test_gen_uint( struct dsim_test_context *ctx, unsigned min, unsigned max )
{
    unsigned res;

    struct dsim_test_uint_in_range_data data;
    dsim_test_uint_in_range_data_init( &data, sizeof(res), min, max );
    dsim_test_data_example( &data.base, ctx, &res );

    return res;
}

void test_gen_array( struct dsim_test_context *ctx, void **ptr, size_t *count, size_t elem_size, size_t min_count )
{
    struct dsim_test_array res;

    struct dsim_test_uint_in_range_data elem;
    dsim_test_uint_in_range_data_init( &elem, elem_size, 0, 100000 );
    struct dsim_test_array_data data;
    dsim_test_array_data_init( &data, &elem.base, min_count );
    dsim_test_data_example( &data.base, ctx, &res );

    *count = res.count;
    *ptr = res.data;
}
