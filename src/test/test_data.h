
#pragma once

#include "test_context.h"

DSIM_BEGIN_HEADER

unsigned test_gen_uint( struct dsim_test_context *ctx, unsigned min, unsigned max );
void test_gen_array( struct dsim_test_context *ctx, void **ptr, size_t *count, size_t elem_size, size_t min_count, size_t max_count );

#define GIVEN_UINT(name,min,max) unsigned name = test_gen_uint( _ctx, min, max );

#define GIVEN_ARRAY(type, name, min_count) \
    type *name##_data; size_t name##_count; \
    test_gen_array( _ctx, (void **)&name##_data, &name##_count, sizeof(type), min_count, 10 ); \

DSIM_END_HEADER
