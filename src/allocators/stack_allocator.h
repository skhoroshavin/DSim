
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

#define dsim_stack_alloc_save(num)    size_t _dsim_stack_pos_##num = dsim_stack_allocator_##num.allocated
#define dsim_stack_alloc_restore(num) dsim_stack_allocator_##num.allocated = _dsim_stack_pos_##num
#define dsim_stack_alloc(num)         &dsim_stack_allocator_##num.alloc

extern dsim_thread struct dsim_stack_allocator dsim_stack_allocator_0;
extern dsim_thread struct dsim_stack_allocator dsim_stack_allocator_1;
