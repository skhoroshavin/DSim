
#include "test_context.h"
#include "test_datagen.h"
#include <stdio.h>
#include <string.h>

void dsim_test_context_init( struct dsim_test_context *self, struct dsim_allocator *alloc )
{
    dsim_stack_allocator_init( &self->alloc, alloc );
    dsim_array_init( &self->objects, alloc );

    dsim_array_init( &self->inputs, alloc );
}

void dsim_test_context_done( struct dsim_test_context *self )
{
    dsim_array_reset( &self->inputs );

    for( uint32_t i = self->objects.count; i != 0; --i )
    {
        struct dsim_test_object *obj = &self->objects.at[i-1];
        obj->dtor( obj->data );
    }
    dsim_array_reset( &self->objects );
    dsim_stack_allocator_reset( &self->alloc );
}

size_t dsim_test_context_gen_data( struct dsim_test_context *self, void *data, size_t size )
{
    return self->gen_data( self, data, size );
}

void *dsim_test_context_alloc( struct dsim_test_context *self, size_t size, dsim_test_dtor dtor )
{
    void *result = dsim_allocate( &self->alloc.alloc, size );
    if( dtor )
    {
        struct dsim_test_object obj =
        {
            .data = result,
            .dtor = dtor
        };
        dsim_array_push_back( &self->objects, obj );
    }
    return result;
}

void dsim_test_context_register_input( struct dsim_test_context *self, const char *name, struct dsim_test_datagen *gen, const void *data )
{
    struct dsim_test_input in;
    in.name = name;
    in.gen = gen;
    in.data = dsim_test_context_alloc( self, gen->size, 0 );
    memcpy( in.data, data, gen->size );
    dsim_array_push_back( &self->inputs, in );
}

void dsim_test_context_dump( struct dsim_test_context *self )
{
    for( unsigned i = 0; i != self->inputs.count; ++i )
    {
        printf( "  %s: ", self->inputs.at[i].name );
        dsim_test_data_print( self->inputs.at[i].gen, self->inputs.at[i].data );
        printf( "\n" );
    }
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
