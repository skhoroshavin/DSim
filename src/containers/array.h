
#pragma once

#include "allocators/allocator.h"

DSIM_BEGIN_HEADER

struct _dsim_array
{
    void * data;
    uint32_t count;
    uint32_t capacity;

    struct dsim_allocator * _alloc;
};

#define dsim_array_static_init(alloc) { \
    /* .data = */     0, \
    /* .count = */    0, \
    /* .capacity = */ 0, \
    /* ._alloc = */    alloc }

void _dsim_array_reserve( struct _dsim_array *a, uint32_t capacity, uint32_t elem_size );
void _dsim_array_grow( struct _dsim_array *a, uint32_t count, uint32_t elem_size );
void _dsim_array_resize( struct _dsim_array *a, uint32_t count, uint32_t elem_size );
void _dsim_array_fill( struct _dsim_array *a, void *value, uint32_t elem_size );
void _dsim_array_push_back( struct _dsim_array *a, const void *data, uint32_t count, uint32_t elem_size );
void _dsim_array_remove( struct _dsim_array *a, uint32_t pos, uint32_t count, uint32_t elem_size );
void _dsim_array_remove_fast( struct _dsim_array *a, uint32_t pos, uint32_t count, uint32_t elem_size );
void _dsim_array_reset( struct _dsim_array *a, uint32_t elem_size );

#define DSIM_DEFINE_ARRAY(type,short_type) \
    struct dsim_##short_type##_array \
    { \
        union { type * data; type * at; }; \
        uint32_t count; \
        uint32_t capacity; \
        struct dsim_allocator * _alloc; \
    }; \
    inline static void dsim_##short_type##_array##_reserve( struct dsim_##short_type##_array *a, uint32_t count ) \
    { _dsim_array_reserve( (struct _dsim_array*)a, count, sizeof(type)); } \
    inline static void dsim_##short_type##_array_resize( struct dsim_##short_type##_array *a, uint32_t count ) \
    { _dsim_array_resize( (struct _dsim_array*)a, count, sizeof(type) ); } \
    inline static void dsim_##short_type##_array_fill( struct dsim_##short_type##_array *a, type value ) \
    { _dsim_array_fill( (struct _dsim_array*)a, &value, sizeof(type) ); } \
    inline static void dsim_##short_type##_array_push_back( struct dsim_##short_type##_array *a, type value ) \
    { if( a->capacity == a->count ) _dsim_array_grow( (struct _dsim_array*)a, 8, sizeof(type) ); a->at[a->count] = value; ++a->count; } \
    inline static void dsim_##short_type##_array_push_back_n( struct dsim_##short_type##_array *a, const type * data, uint32_t count ) \
    { _dsim_array_push_back( (struct _dsim_array*)a, data, count, sizeof(type) ); } \
    inline static void dsim_##short_type##_array_pop_back( struct dsim_##short_type##_array *a ) \
    { assert(a->count); --a->count; } \
    inline static void dsim_##short_type##_array_pop_back_n( struct dsim_##short_type##_array *a, uint32_t count ) \
    { assert(a->count >= count ); a->count -= count; } \
    inline static void dsim_##short_type##_array_remove( struct dsim_##short_type##_array *a, uint32_t pos, uint32_t count ) \
    { assert(a->count >= pos + count ); _dsim_array_remove( (struct _dsim_array*)a, pos, count, sizeof(type) ); } \
    inline static void dsim_##short_type##_array_remove_fast( struct dsim_##short_type##_array *a, uint32_t pos, uint32_t count ) \
    { assert(a->count >= pos + count ); _dsim_array_remove_fast( (struct _dsim_array*)a, pos, count, sizeof(type) ); } \
    inline static void dsim_##short_type##_array_clear( struct dsim_##short_type##_array *a ) \
    { a->count = 0; } \
    inline static void dsim_##short_type##_array_reset( struct dsim_##short_type##_array *a ) \
    { _dsim_array_reset( (struct _dsim_array*)a, sizeof(type) ); } \

DSIM_DEFINE_ARRAY(uint8_t,  uint8)
DSIM_DEFINE_ARRAY(uint16_t, uint16)
DSIM_DEFINE_ARRAY(uint32_t, uint32)
DSIM_DEFINE_ARRAY(uint64_t, uint64)

DSIM_END_HEADER
