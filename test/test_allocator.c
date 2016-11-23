
#include "test_allocator.h"

#include "unity_fixture.h"

static void * test_allocate( struct dsim_allocator *self, size_t size )
{
    return unity_malloc( size );
}

static void test_deallocate( struct dsim_allocator *self, void *data, size_t size )
{
    unity_free( data );
}

static void * test_reallocate( struct dsim_allocator *self, void *data, size_t old_size, size_t new_size )
{
    return unity_realloc( data, new_size );
}

static struct dsim_allocator_operations test_allocator_ops =
{
    .allocate = &test_allocate,
    .reallocate = &test_reallocate,
    .deallocate = &test_deallocate
};

struct dsim_allocator dsim_test_allocator =
{
    ._ops = &test_allocator_ops
};
