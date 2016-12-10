
#include "stack_allocator.h"
#include <memory.h>

static void stack_initialize( struct dsim_stack_allocator *alloc )
{
    alloc->capacity = 4*1024*1024;
    alloc->data = dsim_allocate( alloc->base_alloc, alloc->capacity );
}

static void * stack_allocate( struct dsim_allocator *self, size_t size )
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

static void * stack_reallocate( struct dsim_allocator *self, void *data, size_t size )
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

    struct dsim_stack_allocator * alloc = container_of(self, struct dsim_stack_allocator, alloc);

    if( data == alloc->last_data )
    {
        alloc->allocated -= alloc->last_size;
        alloc->last_data = 0;
        alloc->last_size = 0;
    }
}

static struct dsim_allocator_operations stack_allocator_ops =
{
    .allocate   = &stack_allocate,
    .reallocate = &stack_reallocate,
    .deallocate = &stack_deallocate
};

#define dsim_stack_allocator_static_init() { \
    .alloc = { ._ops = &stack_allocator_ops }, \
    \
    .base_alloc = &dsim_default_allocator, \
    .data = 0, \
    .allocated = 0, \
    .capacity = 0, \
    \
    .last_data = 0, \
    .last_size = 0 }

dsim_thread struct dsim_stack_allocator dsim_stack_allocator_0 = dsim_stack_allocator_static_init();
dsim_thread struct dsim_stack_allocator dsim_stack_allocator_1 = dsim_stack_allocator_static_init();
