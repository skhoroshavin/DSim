
#pragma once

#include "core/array.h"

DSIM_BEGIN_HEADER

typedef void (*dsim_test_dtor)(void*);

struct dsim_test_datagen;

struct dsim_test_input
{
    const char *name;
    void *data;
    struct dsim_test_datagen *gen;
};

struct dsim_test_context
{
    size_t (*gen_data)( struct dsim_test_context *self, void *data, size_t size );

    dsim_array(struct dsim_test_input) inputs;

    dsim_array(dsim_test_dtor) dtors;
    dsim_array(void*) ptrs;
};

void dsim_test_context_init( struct dsim_test_context *self, struct dsim_allocator *alloc );
void dsim_test_context_done( struct dsim_test_context *self );
size_t dsim_test_context_gen_data( struct dsim_test_context *self, void *data, size_t size );
void dsim_test_context_register_dtor( struct dsim_test_context *self, dsim_test_dtor dtor, void *ptr );
void dsim_test_context_register_ptr( struct dsim_test_context *self, void *ptr );

struct dsim_random_test_context
{
    struct dsim_test_context base;
    dsim_array(char) buffer;
};

void dsim_random_test_context_init( struct dsim_random_test_context *self, struct dsim_allocator *alloc );
void dsim_random_test_context_done( struct dsim_random_test_context *self );

DSIM_END_HEADER
