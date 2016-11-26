
#include "allocator.h"
#include <stdio.h>
#include <memory.h>

static void * default_allocate( struct dsim_allocator *self, size_t size )
{
    dsim_unused(self);
    return malloc( size );
}

static void default_deallocate( struct dsim_allocator *self, void *data, size_t size )
{
    dsim_unused(self);
    dsim_unused(size);
    free( data );
}

static void * default_reallocate( struct dsim_allocator *self, void *data, size_t old_size, size_t new_size )
{
    dsim_unused(self);
    dsim_unused(old_size);
    return realloc( data, new_size );
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
