
#pragma once

#include "allocator.h"

DSIM_BEGIN_HEADER

struct _dsim_array
{
    union { void *data; uint8_t *at; };
    uint32_t count;
    uint32_t capacity;

    struct dsim_allocator *_alloc;
};

void _dsim_array_init( struct _dsim_array *a, struct dsim_allocator *alloc );
void _dsim_array_reserve( struct _dsim_array *a, uint32_t capacity, uint32_t elem_size );
void _dsim_array_grow( struct _dsim_array *a, uint32_t count, uint32_t elem_size );
void _dsim_array_resize( struct _dsim_array *a, uint32_t count, uint32_t elem_size );
void _dsim_array_push_back( struct _dsim_array *a, const void *data, uint32_t count, uint32_t elem_size );
void _dsim_array_remove( struct _dsim_array *a, uint32_t pos, uint32_t count, uint32_t elem_size );
void _dsim_array_remove_fast( struct _dsim_array *a, uint32_t pos, uint32_t count, uint32_t elem_size );
void _dsim_array_reset( struct _dsim_array *a );

#define dsim_array(type) \
    struct \
    { \
        union { type *data; type *at; }; \
        uint32_t count; \
        uint32_t capacity; \
        struct dsim_allocator *_alloc; \
    }

#define dsim_array_static_init(alloc) { \
    /* .data = */     { 0 }, \
    /* .count = */    0, \
    /* .capacity = */ 0, \
    /* ._alloc = */    alloc }

#define dsim_array_init(a, alloc) do { \
    (a)->data = 0; \
    (a)->count = 0; \
    (a)->capacity = 0; \
    (a)->_alloc = alloc; \
    } while(0)

#define dsim_array_reserve(a, count) do { \
    _dsim_array_reserve( (struct _dsim_array*)(a), count, sizeof((a)->at[0]) ); \
    } while(0)

#define dsim_array_resize(a, count) do { \
    _dsim_array_resize( (struct _dsim_array*)(a), count, sizeof((a)->at[0]) ); \
    } while(0)

#define dsim_array_push_back(a, value) do { \
    if( (a)->capacity == (a)->count ) \
        _dsim_array_grow( (struct _dsim_array*)(a), 8, sizeof((a)->at[0]) ); \
    (a)->at[(a)->count] = value; ++(a)->count; \
    } while(0)

#define dsim_array_push_back_n(a, p, n) do { \
    _dsim_array_push_back( (struct _dsim_array*)(a), p, n, sizeof((a)->at[0]) ); \
    } while(0)

#define dsim_array_pop_back(a) do { \
    assert((a)->count); \
    --(a)->count; \
    } while(0)

#define dsim_array_pop_back_n(a, n) do { \
    assert((a)->count >= n); \
    (a)->count -= n; \
    } while(0)

#define dsim_array_remove(a, pos, n) do { \
    assert((a)->count >= pos + n ); \
    _dsim_array_remove( (struct _dsim_array*)(a), pos, n, sizeof((a)->at[0]) ); \
    } while(0)

#define dsim_array_remove_fast(a, pos, n) do { \
    assert((a)->count >= pos + n ); \
    _dsim_array_remove_fast( (struct _dsim_array*)(a), pos, n, sizeof((a)->at[0]) ); \
    } while(0)

#define dsim_array_clear(a) do { (a)->count = 0; } while(0)
#define dsim_array_reset(a) do { _dsim_array_reset( (struct _dsim_array*)(a) ); } while(0)

DSIM_END_HEADER
