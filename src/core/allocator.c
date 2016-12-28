
#include "allocator.h"
#include <stdio.h>
#include <memory.h>

static void *default_allocate( struct dsim_allocator *self, size_t size )
{
    dsim_unused(self);
    return malloc( size );
}

static void default_deallocate( struct dsim_allocator *self, void *data )
{
    dsim_unused(self);
    free( data );
}

static void *default_reallocate( struct dsim_allocator *self, void *data, size_t size )
{
    dsim_unused(self);
    return realloc( data, size );
}

static struct dsim_allocator_operations default_allocator_ops =
{
    .allocate = &default_allocate,
    .reallocate = &default_reallocate,
    .deallocate = &default_deallocate
};

struct dsim_allocator dsim_default_allocator =
{
    ._ops = &default_allocator_ops
};
