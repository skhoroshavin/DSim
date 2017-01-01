
#pragma once

#include "core/array.h"

DSIM_BEGIN_HEADER

struct dsim_test_data
{
    size_t (*gen_data)( struct dsim_test_data *self, void *data, size_t size );
};

size_t dsim_test_data_gen( struct dsim_test_data *self, void *data, size_t size );

struct dsim_random_test_data
{
    struct dsim_test_data base;
    dsim_array(char) buffer;
};

void dsim_random_test_data_init( struct dsim_random_test_data *self );
void dsim_random_test_data_done( struct dsim_random_test_data *self );

DSIM_END_HEADER
