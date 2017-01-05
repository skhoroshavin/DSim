
#pragma once

#include "core/array.h"
#include "core/stack_allocator.h"

DSIM_BEGIN_HEADER

typedef void (*dsim_test_dtor)(void*);
struct dsim_test_object
{
    void *data;
    dsim_test_dtor dtor;
};

struct dsim_test_datagen;
struct dsim_test_input
{
    const char *name;
    struct dsim_test_datagen *gen;
    void *data;
};

struct dsim_test_context
{
    size_t (*gen_data)( struct dsim_test_context *self, void *data, size_t size );

    struct dsim_stack_allocator alloc;
    dsim_array(struct dsim_test_object) objects;

    dsim_array(struct dsim_test_input) inputs;
};

void dsim_test_context_init( struct dsim_test_context *self, struct dsim_allocator *alloc );
void dsim_test_context_done( struct dsim_test_context *self );
size_t dsim_test_context_gen_data( struct dsim_test_context *self, void *data, size_t size );
void *dsim_test_context_alloc( struct dsim_test_context *self, size_t size, dsim_test_dtor dtor );
void dsim_test_context_register_input( struct dsim_test_context *self, const char *name, struct dsim_test_datagen *gen, const void *data );
void dsim_test_context_dump( struct dsim_test_context *self );

struct dsim_random_test_context
{
    struct dsim_test_context base;
    dsim_byte_array buffer;
};

void dsim_random_test_context_init( struct dsim_random_test_context *self, struct dsim_allocator *alloc );
void dsim_random_test_context_done( struct dsim_random_test_context *self );

DSIM_END_HEADER
