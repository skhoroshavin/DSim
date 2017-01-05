
#include "gen_int.h"
#include "test_context.h"
#include <stdio.h>

struct dsim_test_uint_in_range_data
{
    struct dsim_test_datagen base;
    unsigned min_value;
    unsigned max_value;
};

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

static void dsim_test_uint_in_range_data_print( struct dsim_test_datagen *self, const void *data )
{
#if 1
    switch( self->size )
    {
    case 1: printf( "%hhu", *(const uint8_t  *)data );
    case 2: printf( "%hu",  *(const uint16_t *)data );
    case 4: printf( "%u",   *(const uint32_t *)data );
    case 8: printf( "%lu",  *(const uint64_t *)data );
    }
#else
    dsim_unused( self );
    dsim_unused( data );
    printf( "x" );
#endif
}

struct dsim_test_datagen *dsim_test_uint_in_range( struct dsim_test_context *ctx, unsigned size, unsigned min_value, unsigned max_value )
{
    struct dsim_test_uint_in_range_data *result = (struct dsim_test_uint_in_range_data*)dsim_test_context_alloc( ctx, sizeof(struct dsim_test_uint_in_range_data), 0 );
    result->base.example = dsim_test_uint_in_range_data_example;
    result->base.print = dsim_test_uint_in_range_data_print;
    result->base.size = size;
    result->min_value = min_value;
    result->max_value = max_value;
    return &result->base;
}
