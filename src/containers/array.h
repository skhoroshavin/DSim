
#pragma once

#include "allocators/allocator.h"

DSIM_BEGIN_HEADER

typedef struct _dsim_array_
{
    void * data;
    uint32_t count;
    uint32_t capacity;
    dsim_allocator * alloc;
} _dsim_array;

#define dsim_array_init(allocator) { \
    /* .data = */     0, \
    /* .count = */    0, \
    /* .capacity = */ 0, \
    /* .alloc = */    allocator }

void _dsim_array_reserve(_dsim_array *a, uint32_t capacity, uint32_t elem_size );
void _dsim_array_resize( _dsim_array *a, uint32_t count, uint32_t elem_size );
void _dsim_array_fill( _dsim_array *a, void *value, uint32_t elem_size );
void _dsim_array_push_back( _dsim_array *a, const void *data, uint32_t count, uint32_t elem_size );
void _dsim_array_reset( _dsim_array *a, uint32_t elem_size );

#define DSIM_DEFINE_ARRAY(type,name) \
    typedef struct dsim_array_##type##_ \
    { \
        type * data; \
        uint32_t count; \
        uint32_t capacity; \
        dsim_allocator * alloc; \
        } name; \
    inline static void name##_reserve( name *array, uint32_t count ) \
    { _dsim_array_reserve( (_dsim_array*)array, count, sizeof(type)); } \
    inline static void name##_resize( name *array, uint32_t count ) \
    { _dsim_array_resize( (_dsim_array*)array, count, sizeof(type) ); } \
    inline static void name##_fill( name *array, type value ) \
    { _dsim_array_fill( (_dsim_array*)array, &value, sizeof(type) ); } \
    inline static void name##_push_back( name *array, type value ) \
    { _dsim_array_push_back( (_dsim_array*)array, &value, 1, sizeof(type) ); } \
    inline static void name##_push_back_n( name *array, const type * data, uint32_t count ) \
    { _dsim_array_push_back( (_dsim_array*)array, data, count, sizeof(type) ); } \
    inline static void name##_pop_back( name *array ) \
    { assert(array->count); --array->count; } \
    inline static void name##_pop_back_n( name *array, uint32_t count ) \
    { assert(array->count >= count ); array->count -= count; } \
    inline static void name##_clear( name *array ) \
    { array->count = 0; } \
    inline static void name##_reset( name *array ) \
    { _dsim_array_reset( (_dsim_array*)array, sizeof(type) ); } \

DSIM_DEFINE_ARRAY(uint32_t,dsim_uint32_array)
DSIM_DEFINE_ARRAY(uint64_t,dsim_uint64_array)

DSIM_END_HEADER
