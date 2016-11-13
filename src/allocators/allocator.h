
#pragma once

#include "utils/common.h"
#include <stdlib.h>

DSIM_BEGIN_HEADER

struct dsim_allocator;

struct dsim_allocator_operations
{
    void* (*allocate)   (struct dsim_allocator *self, size_t size);
    void* (*reallocate) (struct dsim_allocator *self, void *data, size_t old_size, size_t new_size );
    void  (*deallocate) (struct dsim_allocator *self, void *data, size_t size);
};

struct dsim_allocator
{
    struct dsim_allocator_operations * _ops;
};

static inline void * dsim_allocate( struct dsim_allocator *alloc, size_t size ) { return alloc->_ops->allocate( alloc, size ); }
static inline void * dsim_reallocate( struct dsim_allocator *alloc, void *data, size_t old_size, size_t new_size )
{ return alloc->_ops->reallocate( alloc, data, old_size, new_size ); }
static inline void dsim_deallocate( struct dsim_allocator *alloc, void * data, size_t size ) { alloc->_ops->deallocate( alloc, data, size ); }

extern struct dsim_allocator dsim_default_allocator;

DSIM_END_HEADER
