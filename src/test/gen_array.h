
#pragma once

#include "test_gen.h"

DSIM_BEGIN_HEADER

void test_gen_array( struct dsim_test_data *data, void **ptr, size_t *count, size_t elem_size, size_t min_count, size_t max_count );

DSIM_END_HEADER
