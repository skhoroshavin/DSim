
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

    if( alloc->allocated + size > alloc->capacity )
        return 0;

    void *data = (uint8_t*)alloc->data + alloc->allocated;
    alloc->allocated += size;
    return data;
}

static void * stack_reallocate( struct dsim_allocator *self, void *data, size_t old_size, size_t new_size )
{
    if( data == 0 )
        return stack_allocate( self, new_size );

    struct dsim_stack_allocator *alloc = container_of(self, struct dsim_stack_allocator, alloc);

    if( (uint8_t*)data + old_size == (uint8_t*)alloc->data + alloc->allocated )
    {
        if( alloc->allocated + new_size - old_size > alloc->capacity )
            return 0;

        alloc->allocated += new_size - old_size;
        return data;
    }

    void *new_data = stack_allocate( self, new_size );
    if( new_data )
        memcpy( new_data, data, old_size );
    return new_data;
}

static void stack_deallocate( struct dsim_allocator *self, void *data, size_t size )
{
    struct dsim_stack_allocator * alloc = container_of(self, struct dsim_stack_allocator, alloc);

    if( (uint8_t*)data + size == (uint8_t*)alloc->data + alloc->allocated )
        alloc->allocated -= size;
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
    .capacity = 0 }

dsim_thread struct dsim_stack_allocator dsim_stack_allocator_0 = dsim_stack_allocator_static_init();
dsim_thread struct dsim_stack_allocator dsim_stack_allocator_1 = dsim_stack_allocator_static_init();
