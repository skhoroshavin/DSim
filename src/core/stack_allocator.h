
#pragma once

#include "allocator.h"

struct dsim_stack_allocator
{
    struct dsim_allocator alloc;

    struct dsim_allocator *base_alloc;
    void *data;
    size_t allocated;
    size_t capacity;

    void *last_data;
    size_t last_size;
};

#define dsim_stack_allocator_static_init(_alloc) { \
    /* .alloc = */ { ._ops = &stack_allocator_ops }, \
    \
    /* .base_alloc = */ _alloc, \
    /* .data = */ 0, \
    /* .allocated = */ 0, \
    /* .capacity = */ 0, \
    \
    /* .last_data = */ 0, \
    /* .last_size = */ 0 }

void dsim_stack_allocator_init( struct dsim_stack_allocator *a, struct dsim_allocator *base );
void dsim_stack_allocator_reset( struct dsim_stack_allocator *a );

extern struct dsim_allocator_operations stack_allocator_ops;
