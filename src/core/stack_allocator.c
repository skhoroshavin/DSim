
#include "stack_allocator.h"
#include <memory.h>

static void stack_initialize( struct dsim_stack_allocator *alloc )
{
    alloc->capacity = 4*1024*1024;
    alloc->data = dsim_allocate( alloc->base_alloc, alloc->capacity );
}

static void *stack_allocate( struct dsim_allocator *self, size_t size )
{
    struct dsim_stack_allocator *alloc = container_of(self, struct dsim_stack_allocator, alloc);

    if( alloc->data == 0 )
        stack_initialize( alloc );

    // LCOV_EXCL_START
    if( alloc->allocated + size > alloc->capacity )
        return 0;
    // LCOV_EXCL_STOP

    void *data = (uint8_t*)alloc->data + alloc->allocated;
    alloc->allocated += size;
    alloc->last_data = data;
    alloc->last_size = size;
    return data;
}

static void *stack_reallocate( struct dsim_allocator *self, void *data, size_t size )
{
    if( data == 0 )
        return stack_allocate( self, size );

    struct dsim_stack_allocator *alloc = container_of(self, struct dsim_stack_allocator, alloc);

    if( data == alloc->last_data )
    {
        // LCOV_EXCL_START
        if( alloc->allocated + size - alloc->last_size > alloc->capacity )
            return 0;
        // LCOV_EXCL_STOP

        alloc->allocated += size - alloc->last_size;
        alloc->last_size = size;
        return data;
    }

    void *new_data = stack_allocate( self, size );
    if( new_data ) // LCOV_EXCL_BR_LINE
        memmove( new_data, data, size );
    return new_data;
}

static void stack_deallocate( struct dsim_allocator *self, void *data )
{
    if( data == 0 )
        return;

    struct dsim_stack_allocator *alloc = container_of(self, struct dsim_stack_allocator, alloc);

    if( data == alloc->last_data )
    {
        alloc->allocated -= alloc->last_size;
        alloc->last_data = 0;
        alloc->last_size = 0;
    }
}

void dsim_stack_allocator_reset( struct dsim_stack_allocator *a )
{
    if( a->data )
        dsim_deallocate( a->base_alloc, a->data );
    a->data = 0;
    a->capacity = 0;
    a->last_data = 0;
    a->last_size = 0;
}

struct dsim_allocator_operations stack_allocator_ops =
{
    .allocate   = &stack_allocate,
    .reallocate = &stack_reallocate,
    .deallocate = &stack_deallocate
};
