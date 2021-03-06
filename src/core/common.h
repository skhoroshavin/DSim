
#pragma once

#include <assert.h>
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
#define DSIM_BEGIN_HEADER extern "C" {
#define DSIM_END_HEADER }
#else // __cplusplus
#define DSIM_BEGIN_HEADER
#define DSIM_END_HEADER
#endif // __cplusplus

DSIM_BEGIN_HEADER

#ifdef _MSC_VER
#define dsim_thread __declspec(thread)
#else
#define dsim_thread __thread
#endif

#ifndef count_of
#define count_of(a) (sizeof(a)/sizeof((a)[0]))
#endif // count_of

#ifndef container_of
#define container_of(ptr,type,member) ((type*)((char*)ptr - offsetof(type,member)))
#endif // container_of

inline static size_t min( size_t a, size_t b ) { return a < b ? a : b; }
inline static size_t max( size_t a, size_t b ) { return a > b ? a : b; }

#define DSIM_INVALID_INDEX ((uint32_t)-1)

#define dsim_unused(a) (void)a

uint64_t dsim_hash64( uint64_t key );
uint64_t dsim_next_pow_2( uint64_t value );

#define dsim_fill(data, value, count) do { for( uint32_t i = 0; i < (count); ++i ) (data)[i] = (value); } while(0)

#define dsim_find(pos, value, data, count) do { \
    for( ; pos < (count); ++pos ) \
        if( (data)[pos] == (value) ) \
            break; \
    if( pos == (count) ) \
        pos = DSIM_INVALID_INDEX; \
    } while(0)

#define dsim_count(cnt, value, data, count) do { \
    unsigned pos = 0; \
    while(1) \
    { \
        dsim_find( pos, value, data, count ); \
        if( pos == DSIM_INVALID_INDEX ) \
            break; \
        ++cnt; \
        ++pos; \
    } } while(0)

DSIM_END_HEADER
