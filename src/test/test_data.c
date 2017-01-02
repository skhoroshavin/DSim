
#include "test_data.h"

void dsim_test_data_init( struct dsim_test_data *self, struct dsim_allocator *alloc )
{
    dsim_array_init( &self->dtors, alloc );
    dsim_array_init( &self->ptrs, alloc );
}

void dsim_test_data_done( struct dsim_test_data *self )
{
    assert( self->dtors.count == self->ptrs.count );
    for( uint32_t i = self->dtors.count; i != 0; --i )
        self->dtors.at[i-1]( self->ptrs.at[i-1] );
    dsim_array_reset( &self->dtors );
    dsim_array_reset( &self->ptrs );
}

size_t dsim_test_data_gen( struct dsim_test_data *self, void *data, size_t size )
{
    return self->gen_data( self, data, size );
}

void dsim_test_data_register_dtor( struct dsim_test_data *self, dsim_test_dtor dtor, void *ptr )
{
    dsim_array_push_back( &self->dtors, dtor );
    dsim_array_push_back( &self->ptrs, ptr );
}

void dsim_test_data_register_ptr( struct dsim_test_data *self, void *ptr )
{
    dsim_test_data_register_dtor( self, free, ptr );
}

static size_t gen_random( struct dsim_test_data *self, void *data, size_t size )
{
    struct dsim_random_test_data *ctx = container_of(self, struct dsim_random_test_data, base);

    for( size_t i = 0; i != size; ++i )
        ((char*)data)[i] = rand();
    dsim_array_push_back_n( &ctx->buffer, data, size );

    return size;
}

void dsim_random_test_data_init( struct dsim_random_test_data *self, struct dsim_allocator *alloc )
{
    dsim_test_data_init( &self->base, alloc );

    self->base.gen_data = gen_random;
    dsim_array_init( &self->buffer, alloc );
}

void dsim_random_test_data_done( struct dsim_random_test_data *self )
{
    dsim_array_reset( &self->buffer );
    dsim_test_data_done( &self->base );
}
