
#pragma once

#include "core/common.h"

DSIM_BEGIN_HEADER

struct dsim_test_context;

struct dsim_test_datagen
{
    void (*example)( struct dsim_test_datagen *self, struct dsim_test_context *ctx, void *data );
    void (*print)( struct dsim_test_datagen *self, const void *data );
    size_t size;
};

void dsim_test_data_example( struct dsim_test_datagen *self, struct dsim_test_context *ctx, void *data, const char *name );
void dsim_test_data_print( struct dsim_test_datagen *self, const void *data );


unsigned test_gen_uint( struct dsim_test_context *ctx, unsigned min, unsigned max, const char *name );
void test_gen_array( struct dsim_test_context *ctx, void **ptr, size_t *count, size_t elem_size, size_t min_count, const char *name );

#define GIVEN_UINT(name,min,max) unsigned name = test_gen_uint( _ctx, min, max, #name );

#define GIVEN_ARRAY(type, name, min_count) \
    type *name##_data; size_t name##_count; \
    test_gen_array( _ctx, (void **)&name##_data, &name##_count, sizeof(type), min_count, #name ); \

DSIM_END_HEADER
