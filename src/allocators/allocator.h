
#pragma once

#include "utils/common.h"
#include <stdlib.h>

DSIM_BEGIN_HEADER

typedef struct dsim_allocator_operations_
{
    void* (*allocate)   (void *self, size_t size);
    void* (*reallocate) (void *self, void *data, size_t old_size, size_t new_size, size_t save_size );
    void  (*deallocate) (void *self, void *data, size_t size);
} dsim_allocator_operations;

typedef struct dsim_allocator_
{
    dsim_allocator_operations * _ops;
} dsim_allocator;

static inline void * dsim_allocate( dsim_allocator *alloc, size_t size ) { return alloc->_ops->allocate( alloc, size ); }
static inline void * dsim_reallocate( dsim_allocator *alloc, void *data, size_t old_size, size_t new_size, size_t save_size )
{ return alloc->_ops->reallocate( alloc, data, old_size, new_size, save_size ); }
static inline void dsim_deallocate( dsim_allocator *alloc, void * data, size_t size ) { alloc->_ops->deallocate( alloc, data, size ); }

extern dsim_allocator dsim_default_allocator;

DSIM_END_HEADER
