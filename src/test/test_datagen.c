
#include "test_datagen.h"
#include "test_context.h"
#include "gen_int.h"
#include "gen_array.h"
#include <stdio.h>

void dsim_test_data_example( struct dsim_test_datagen *self, struct dsim_test_context *ctx, void *data, const char *name )
{
    self->example( self, ctx, data );
    if( name )
        dsim_test_context_register_input( ctx, name, self, data );
}

void dsim_test_data_print( struct dsim_test_datagen *self, const void *data )
{
    self->print( self, data );
}

unsigned test_gen_uint( struct dsim_test_context *ctx, unsigned min, unsigned max, const char *name )
{
    unsigned res;
    dsim_test_data_example( dsim_test_uint_in_range( ctx, sizeof(res), min, max ), ctx, &res, name );
    return res;
}

void test_gen_array( struct dsim_test_context *ctx, void **ptr, size_t *count, size_t elem_size, size_t min_count, const char *name )
{
    struct dsim_test_array res;
    dsim_test_data_example( dsim_test_array_data( ctx, dsim_test_uint_in_range( ctx, elem_size, 0, 10 ), min_count ), ctx, &res, name );
    *count = res.count;
    *ptr = res.data;
}
