
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

void * default_reallocate( void *self, void *data, size_t old_size, size_t new_size )
{
    return realloc( data, new_size );
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
