
#pragma once

#include "utils/common.h"
#include <stdlib.h>

DSIM_BEGIN_HEADER

struct dsim_allocator;

struct dsim_allocator_operations
{
    void* (*allocate)   (struct dsim_allocator *self, size_t size);
    void* (*reallocate) (struct dsim_allocator *self, void *data, size_t size );
    void  (*deallocate) (struct dsim_allocator *self, void *data);
};

struct dsim_allocator
{
    const struct dsim_allocator_operations * _ops;
};

inline static void * dsim_allocate( struct dsim_allocator *alloc, size_t size )
{ return alloc->_ops->allocate( alloc, size ); }
inline static void * dsim_reallocate( struct dsim_allocator *alloc, void *data, size_t size )
{ return alloc->_ops->reallocate( alloc, data, size ); }
inline static void dsim_deallocate( struct dsim_allocator *alloc, void * data )
{ alloc->_ops->deallocate( alloc, data ); }

extern struct dsim_allocator dsim_default_allocator;

DSIM_END_HEADER
