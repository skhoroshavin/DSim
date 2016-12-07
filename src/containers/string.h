
#pragma once

#include "allocators/allocator.h"

DSIM_BEGIN_HEADER

struct dsim_string
{
    union { char * data; char * at; };
    uint32_t size;
    uint32_t capacity;
    struct dsim_allocator * _alloc;
};

#define dsim_string_static_init(alloc) { \
    /* .data = */     { 0 }, \
    /* .count = */    0, \
    /* .capacity = */ 0, \
    /* ._alloc = */    alloc }

void dsim_string_reserve( struct dsim_string *s, uint32_t count );
void dsim_string_append( struct dsim_string *s, const char * data );
void dsim_string_remove( struct dsim_string *s, uint32_t pos, uint32_t count );
void dsim_string_clear( struct dsim_string *s );
void dsim_string_reset( struct dsim_string *s );

DSIM_END_HEADER
