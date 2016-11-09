
#include "allocator.h"
#include <stdio.h>
#include <memory.h>

void * default_allocate( void *self, size_t size )
{
    return malloc( size );
}

void default_deallocate( void *self, void *data, size_t size )
{
    free( data );
}

void * default_reallocate( void *self, void *data, size_t old_size, size_t new_size, size_t save_size )
{
    assert( save_size <= old_size );
    assert( save_size <= new_size );

    void *new_data = 0;

    if( new_size )
        new_data = default_allocate( self, new_size );

    if( save_size )
        memcpy( new_data, data, save_size );

    if( data )
        default_deallocate( self, data, old_size );

    return new_data;
}

static dsim_allocator_operations default_allocator_ops =
{
    .allocate = &default_allocate,
    .reallocate = &default_reallocate,
    .deallocate = &default_deallocate
};

dsim_allocator dsim_default_allocator =
{
    ._ops = &default_allocator_ops
};
