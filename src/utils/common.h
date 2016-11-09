
#pragma once

#include <assert.h>
#include <stdlib.h>
#include <stdint.h>

#ifdef __cplusplus
#define DSIM_BEGIN_HEADER extern "C" {
#define DSIM_END_HEADER }
#else // __cplusplus
#define DSIM_BEGIN_HEADER
#define DSIM_END_HEADER
#endif // __cplusplus

DSIM_BEGIN_HEADER

#ifndef count_of
#define count_of(a) (sizeof(a)/sizeof((a)[0]))
#endif // count_of

#ifndef container_of
#define container_of(ptr,type,member) ((type*)((char*)ptr - offsetof(type,member)))
#endif // container_of

#ifndef max
#define max(a,b) ((a > b) ? a : b)
#endif // max

#ifndef min
#define min(a,b) ((a < b) ? a : b)
#endif // min

typedef uint32_t dsim_index_t;
#define DSIM_INVALID_INDEX ((dsim_index_t)-1)

typedef uint64_t dsim_id_t;

uint64_t dsim_hash64( uint64_t key );
size_t dsim_next_pow_2( size_t value );

DSIM_END_HEADER
