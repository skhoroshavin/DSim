
#pragma once

#include "allocators/allocator.h"

DSIM_BEGIN_HEADER

struct _dsim_array
{
    union { void *data; uint8_t *at; };
    uint32_t count;
    uint32_t capacity;

    struct dsim_allocator *_alloc;
};

#define dsim_array_static_init(alloc) { \
    /* .data = */     { 0 }, \
    /* .count = */    0, \
    /* .capacity = */ 0, \
    /* ._alloc = */    alloc }

void _dsim_array_init( struct _dsim_array *a, struct dsim_allocator *alloc );
void _dsim_array_reserve( struct _dsim_array *a, uint32_t capacity, uint32_t elem_size );
void _dsim_array_grow( struct _dsim_array *a, uint32_t count, uint32_t elem_size );
void _dsim_array_resize( struct _dsim_array *a, uint32_t count, uint32_t elem_size );
void _dsim_array_push_back( struct _dsim_array *a, const void *data, uint32_t count, uint32_t elem_size );
void _dsim_array_remove( struct _dsim_array *a, uint32_t pos, uint32_t count, uint32_t elem_size );
void _dsim_array_remove_fast( struct _dsim_array *a, uint32_t pos, uint32_t count, uint32_t elem_size );
void _dsim_array_reset( struct _dsim_array *a );

#define DSIM_DEFINE_ARRAY(type,short_type) \
    struct dsim_array_##short_type \
    { \
        union { type *data; type *at; }; \
        uint32_t count; \
        uint32_t capacity; \
        struct dsim_allocator *_alloc; \
    }; \
    inline static void dsim_array_##short_type##_init( struct dsim_array_##short_type *a, struct dsim_allocator *alloc ) \
    { a->data = 0; a->count = 0; a->capacity = 0; a->_alloc = alloc; } \
    inline static void dsim_array_##short_type##_reserve( struct dsim_array_##short_type *a, uint32_t count ) \
    { _dsim_array_reserve( (struct _dsim_array*)a, count, sizeof(type)); } \
    inline static void dsim_array_##short_type##_resize( struct dsim_array_##short_type *a, uint32_t count ) \
    { _dsim_array_resize( (struct _dsim_array*)a, count, sizeof(type) ); } \
    inline static void dsim_array_##short_type##_push_back( struct dsim_array_##short_type *a, type value ) \
    { if( a->capacity == a->count ) _dsim_array_grow( (struct _dsim_array*)a, 8, sizeof(type) ); a->at[a->count] = value; ++a->count; } \
    inline static void dsim_array_##short_type##_push_back_n( struct dsim_array_##short_type *a, const type *data, uint32_t count ) \
    { _dsim_array_push_back( (struct _dsim_array*)a, data, count, sizeof(type) ); } \
    inline static void dsim_array_##short_type##_pop_back( struct dsim_array_##short_type *a ) \
    { assert(a->count); --a->count; } \
    inline static void dsim_array_##short_type##_pop_back_n( struct dsim_array_##short_type *a, uint32_t count ) \
    { assert(a->count >= count ); a->count -= count; } \
    inline static void dsim_array_##short_type##_remove( struct dsim_array_##short_type *a, uint32_t pos, uint32_t count ) \
    { assert(a->count >= pos + count ); _dsim_array_remove( (struct _dsim_array*)a, pos, count, sizeof(type) ); } \
    inline static void dsim_array_##short_type##_remove_fast( struct dsim_array_##short_type *a, uint32_t pos, uint32_t count ) \
    { assert(a->count >= pos + count ); _dsim_array_remove_fast( (struct _dsim_array*)a, pos, count, sizeof(type) ); } \
    inline static void dsim_array_##short_type##_clear( struct dsim_array_##short_type *a ) \
    { a->count = 0; } \
    inline static void dsim_array_##short_type##_reset( struct dsim_array_##short_type *a ) \
    { _dsim_array_reset( (struct _dsim_array*)a ); } \

DSIM_DEFINE_ARRAY(char,        char)    // LCOV_EXCL_BR_LINE
DSIM_DEFINE_ARRAY(uint8_t,     uint8)   // LCOV_EXCL_BR_LINE
DSIM_DEFINE_ARRAY(uint16_t,    uint16)  // LCOV_EXCL_BR_LINE
DSIM_DEFINE_ARRAY(uint32_t,    uint32)  // LCOV_EXCL_BR_LINE
DSIM_DEFINE_ARRAY(uint64_t,    uint64)  // LCOV_EXCL_BR_LINE
DSIM_DEFINE_ARRAY(void*,       ptr)     // LCOV_EXCL_BR_LINE
DSIM_DEFINE_ARRAY(const void*, cptr)     // LCOV_EXCL_BR_LINE

DSIM_END_HEADER
