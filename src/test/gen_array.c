
#include "gen_array.h"
#include "test_context.h"
#include <stdio.h>

struct dsim_test_array_data
{
    struct dsim_test_datagen base;
    struct dsim_test_datagen *elem;
    size_t min_count;
};

static void dsim_test_array_data_example( struct dsim_test_datagen *self, struct dsim_test_context *ctx, void *data )
{
    struct dsim_test_array_data *params = container_of(self, struct dsim_test_array_data, base);

    struct dsim_test_array result;
    result.count = test_gen_uint( ctx, params->min_count, 16, 0 );
    size_t size = result.count * params->elem->size;
    if( !size )
    {
        result.data = 0;
        goto end;
    }

    result.data = dsim_test_context_alloc( ctx, size, 0 );
    for( size_t i = 0; i != result.count; ++i )
        dsim_test_data_example( params->elem, ctx, (char*)result.data + i*params->elem->size, 0 );

end:
    *(struct dsim_test_array *)data = result;
}

static void dsim_test_array_data_print( struct dsim_test_datagen *self, const void *data )
{
    struct dsim_test_array_data *params = container_of(self, struct dsim_test_array_data, base);

    printf( "( " );
    struct dsim_test_array *array = (struct dsim_test_array *)data;
    for( size_t i = 0; i < array->count; ++i )
    {
        void *elem = (char*)array->data + i*params->elem->size;
        dsim_test_data_print( params->elem, elem );
        printf( " " );
    }
    printf( ")" );
}

struct dsim_test_datagen *dsim_test_array_data( struct dsim_test_context *ctx, struct dsim_test_datagen *elem, size_t min_count )
{
    struct dsim_test_array_data *result = (struct dsim_test_array_data*)dsim_test_context_alloc( ctx, sizeof(struct dsim_test_array_data), 0 );
    result->base.example = dsim_test_array_data_example;
    result->base.print = dsim_test_array_data_print;
    result->base.size = sizeof(struct dsim_test_array);
    result->elem = elem;
    result->min_count = min_count;
    return &result->base;
}
