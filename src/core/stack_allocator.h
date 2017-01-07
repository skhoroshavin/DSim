
#pragma once

#include "allocator.h"

struct dsim_stack_allocator
{
    struct dsim_allocator alloc;

    void *data;
    size_t capacity;
    size_t allocated;

    void *last_data;
    size_t last_size;
};

#define dsim_stack_allocator_static_init(_data,_size) { \
    /* .alloc = */ { ._ops = &stack_allocator_ops }, \
    \
    /* .data = */ _data, \
    /* .capacity = */ _size, \
    /* .allocated = */ 0, \
    \
    /* .last_data = */ 0, \
    /* .last_size = */ 0 }

void dsim_stack_allocator_init( struct dsim_stack_allocator *a, void *data, size_t size );
void dsim_stack_allocator_reset( struct dsim_stack_allocator *a );

extern struct dsim_allocator_operations stack_allocator_ops;
