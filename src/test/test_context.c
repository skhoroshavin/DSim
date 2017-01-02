
#include "test_context.h"

void dsim_test_context_init( struct dsim_test_context *self, struct dsim_allocator *alloc )
{
    dsim_array_init( &self->dtors, alloc );
    dsim_array_init( &self->ptrs, alloc );
}

void dsim_test_context_done( struct dsim_test_context *self )
{
    assert( self->dtors.count == self->ptrs.count ); //  LCOV_EXCL_BR_LINE
    for( uint32_t i = self->dtors.count; i != 0; --i )
        self->dtors.at[i-1]( self->ptrs.at[i-1] );
    dsim_array_reset( &self->dtors );
    dsim_array_reset( &self->ptrs );
}

size_t dsim_test_context_gen_data( struct dsim_test_context *self, void *data, size_t size )
{
    return self->gen_data( self, data, size );
}

void dsim_test_context_register_dtor( struct dsim_test_context *self, dsim_test_dtor dtor, void *ptr )
{
    dsim_array_push_back( &self->dtors, dtor );
    dsim_array_push_back( &self->ptrs, ptr );
}

void dsim_test_context_register_ptr( struct dsim_test_context *self, void *ptr )
{
    dsim_test_context_register_dtor( self, free, ptr );
}

static size_t gen_random( struct dsim_test_context *self, void *data, size_t size )
{
    struct dsim_random_test_context *ctx = container_of(self, struct dsim_random_test_context, base);

    for( size_t i = 0; i != size; ++i )
        ((char*)data)[i] = rand();
    dsim_array_push_back_n( &ctx->buffer, data, size );

    return size;
}

void dsim_random_test_context_init( struct dsim_random_test_context *self, struct dsim_allocator *alloc )
{
    dsim_test_context_init( &self->base, alloc );

    self->base.gen_data = gen_random;
    dsim_array_init( &self->buffer, alloc );
}

void dsim_random_test_context_done( struct dsim_random_test_context *self )
{
    dsim_array_reset( &self->buffer );
    dsim_test_context_done( &self->base );
}
