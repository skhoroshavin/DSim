
#pragma once

#include "test_gen.h"

DSIM_BEGIN_HEADER

void test_gen_array( struct dsim_test_data *data, void **ptr, size_t *count, size_t elem_size, size_t min_count, size_t max_count );

#define GIVEN_ARRAY(type, name, min_count) \
    type *name##_data; size_t name##_count; \
    test_gen_array( _ctx, (void **)&name##_data, &name##_count, sizeof(type), min_count, 10 ); \


DSIM_END_HEADER
