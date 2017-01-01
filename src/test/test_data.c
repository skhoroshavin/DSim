
#include "test_data.h"

size_t dsim_test_data_gen( struct dsim_test_data *self, void *data, size_t size )
{
    return self->gen_data( self, data, size );
}

static size_t gen_random( struct dsim_test_data *self, void *data, size_t size )
{
    struct dsim_random_test_data *ctx = container_of(self, struct dsim_random_test_data, base);

    for( size_t i = 0; i != size; ++i )
        ((char*)data)[i] = rand();
    dsim_array_push_back_n( &ctx->buffer, data, size );

    return size;
}

void dsim_random_test_data_init( struct dsim_random_test_data *self )
{
    self->base.gen_data = gen_random;
    dsim_array_init( &self->buffer, &dsim_default_allocator );
}

void dsim_random_test_data_done( struct dsim_random_test_data *self )
{
    dsim_array_reset( &self->buffer );
}
