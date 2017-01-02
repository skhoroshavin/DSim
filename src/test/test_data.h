
#pragma once

#include "core/array.h"

DSIM_BEGIN_HEADER

typedef void (*dsim_test_dtor)(void*);

struct dsim_test_data
{
    size_t (*gen_data)( struct dsim_test_data *self, void *data, size_t size );
    dsim_array(dsim_test_dtor) dtors;
    dsim_array(void*) ptrs;
};

void dsim_test_data_init( struct dsim_test_data *self, struct dsim_allocator *alloc );
void dsim_test_data_done( struct dsim_test_data *self );
size_t dsim_test_data_gen( struct dsim_test_data *self, void *data, size_t size );
void dsim_test_data_register_dtor( struct dsim_test_data *self, dsim_test_dtor dtor, void *ptr );
void dsim_test_data_register_ptr( struct dsim_test_data *self, void *ptr );

struct dsim_random_test_data
{
    struct dsim_test_data base;
    dsim_array(char) buffer;
};

void dsim_random_test_data_init( struct dsim_random_test_data *self, struct dsim_allocator *alloc );
void dsim_random_test_data_done( struct dsim_random_test_data *self );

DSIM_END_HEADER
