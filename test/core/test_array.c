
#include "test_array.h"

/*
 * Utility
 */

enum greatest_test_res _assert_array_null( const void *a_data, uint32_t a_count, uint32_t a_capacity )
{
    ASSERT( !a_data );
    ASSERT_INT_EQ( a_count, 0 );
    ASSERT_INT_EQ( a_capacity, 0 );
    PASS();
}

enum greatest_test_res _assert_array_capacity( const void *a_data, uint32_t a_count, uint32_t a_capacity, uint32_t capacity )
{
    ASSERT( a_data );
    ASSERT( a_capacity >= capacity );
    ASSERT( a_count <= a_capacity );
    PASS();
}

enum greatest_test_res _assert_array_remove_ordered( const void *a_data, uint32_t a_count, uint32_t a_capacity, uint32_t a_elemsize, uint32_t pos, uint32_t count, const uint64_t *old_data, uint32_t old_count )
{
    CHECK_CALL(_assert_array_capacity( a_data, a_count, a_capacity, old_count ));
    ASSERT_INT_EQ( a_count, old_count - count );
    ASSERT_MEM_EQ( a_data, old_data, pos*a_elemsize );
    if( old_count > pos + count )
        ASSERT_MEM_EQ( (const char*)a_data + pos*a_elemsize,
                       (const char*)old_data + (pos + count)*a_elemsize,
                       (old_count - pos - count)*a_elemsize );
    PASS();
}

enum greatest_test_res _assert_array_remove_unordered( const void *a_data, uint32_t a_count, uint32_t a_capacity, uint32_t a_elemsize, uint32_t pos, uint32_t count, const uint64_t *old_data, uint32_t old_count )
{
    CHECK_CALL(_assert_array_capacity( a_data, a_count, a_capacity, old_count ));
    ASSERT_INT_EQ( a_count, old_count - count );
    ASSERT_MEM_EQ( a_data, old_data, pos*a_elemsize );
    ASSERT_MEM_EQ( (const char*)a_data + pos*a_elemsize,
                   (const char*)old_data + (old_count - count)*a_elemsize,
                   count*a_elemsize );
    ASSERT_MEM_EQ( (const char*)a_data + (pos + count)*a_elemsize,
                   (const char*)old_data + (pos + count)*a_elemsize,
                   (old_count - pos - 2*count)*a_elemsize );
    PASS();
}

void _test_gen_dsim_array( struct dsim_test_data *data, struct _dsim_array **result )
{
    *result = (struct _dsim_array*)malloc( sizeof(struct _dsim_array) );
    dsim_test_data_register_ptr( data, *result );

    _dsim_array_init( *result, &dsim_default_allocator );
    dsim_test_data_register_dtor( data, (dsim_test_dtor)_dsim_array_reset, *result );
}

#define test_gen_dsim_array(data,res) _test_gen_dsim_array(data,(struct _dsim_array**)&(res) );

#define test_setup_dsim_array(type,min,max) \
    type *test_data; size_t test_count; \
    dsim_array(type) *test_array; \
    test_gen_array( data, (void **)&test_data, &test_count, sizeof(type), min, max ); \
    test_gen_dsim_array( data, test_array ); \
    dsim_array_push_back_n( test_array, test_data, test_count );

#define ASSERT_DSIM_ARRAY_INVARIANTS() do { \
    ASSERT( test_array->capacity >= test_array->count ); \
    if( test_array->capacity ) ASSERT( test_array->data ); \
    else ASSERT( !test_array->data ); \
    } while(0)

/*
 * Tests
 */

TEST array_invariants( struct dsim_test_data *data )
{
    test_setup_dsim_array(uint64_t, 0, 10);

    ASSERT_DSIM_ARRAY_INVARIANTS();
    PASS();
}

TEST array_reserve( struct dsim_test_data *data )
{
    test_setup_dsim_array(uint64_t, 0, 10);

    uint32_t old_capacity = test_array->capacity;
    uint32_t capacity = test_gen_uint( data, 0, 10 );
    dsim_array_reserve( test_array, capacity );

    ASSERT_DSIM_ARRAY_INVARIANTS();
    ASSERT( test_array->capacity >= old_capacity );
    ASSERT( test_array->count == test_count );
    ASSERT_MEM_EQ( test_array->data, test_data, test_count*sizeof(uint64_t) );

    PASS();
}

TEST array_resize( struct dsim_test_data *data )
{
    test_setup_dsim_array(uint64_t, 0, 10);

    uint32_t new_size = test_gen_uint( data, 0, 10 );
    dsim_array_resize( test_array, new_size );

    ASSERT_DSIM_ARRAY_INVARIANTS();
    ASSERT( test_array->count == new_size );
    ASSERT_MEM_EQ( test_array->data, test_data, min(new_size,test_count)*sizeof(uint64_t) );

    PASS();
}

TEST array_push_back( struct dsim_test_data *data )
{
    test_setup_dsim_array(uint64_t, 0, 10);

    unsigned value = test_gen_uint( data, 0, UINT32_MAX );
    dsim_array_push_back( test_array, value );

    ASSERT_DSIM_ARRAY_INVARIANTS();
    ASSERT( test_array->count == test_count + 1 );
    ASSERT_MEM_EQ( test_array->data, test_data, test_count*sizeof(uint64_t) );
    ASSERT_INT_EQ( test_array->at[test_count], value );

    PASS();
}

TEST array_push_back_n( struct dsim_test_data *data )
{
    test_setup_dsim_array(uint64_t, 0, 10);

    uint64_t *more_data; size_t more_count;
    test_gen_array( data, (void **)&more_data, &more_count, sizeof(uint64_t), 1, 10 );
    dsim_array_push_back_n( test_array, more_data, more_count );

    ASSERT_DSIM_ARRAY_INVARIANTS();
    ASSERT( test_array->count == test_count + more_count );
    ASSERT_MEM_EQ( test_array->data, test_data, test_count*sizeof(uint64_t) );
    ASSERT_MEM_EQ( test_array->data + test_count, more_data, more_count*sizeof(uint64_t) );

    PASS();
}

TEST array_pop_back( struct dsim_test_data *data )
{
    test_setup_dsim_array(uint64_t, 1, 10);

    dsim_array_pop_back( test_array );

    ASSERT_DSIM_ARRAY_INVARIANTS();
    ASSERT( test_array->count == test_count - 1 );
    ASSERT_MEM_EQ( test_array->data, test_data, (test_count-1)*sizeof(uint64_t) );

    PASS();
}

TEST array_pop_back_n( struct dsim_test_data *data )
{
    test_setup_dsim_array(uint64_t, 1, 10);

    uint32_t pop_count = test_gen_uint( data, 0, test_count );
    dsim_array_pop_back_n( test_array, pop_count );

    ASSERT_DSIM_ARRAY_INVARIANTS();
    ASSERT( test_array->count == test_count - pop_count );
    ASSERT_MEM_EQ( test_array->data, test_data, (test_count-pop_count)*sizeof(uint64_t) );

    PASS();
}

TEST array_remove( struct dsim_test_data *data )
{
    test_setup_dsim_array(uint64_t, 1, 10);

    unsigned pos = test_gen_uint( data, 0, test_count - 1 );
    unsigned count = test_gen_uint( data, 0, test_count - pos - 1 );
    dsim_array_remove( test_array, pos, count );

    ASSERT_DSIM_ARRAY_INVARIANTS();
    CHECK_CALL(assert_array_remove_ordered( test_array, pos, count, test_data, test_count ));
    PASS();
}

TEST array_remove_fast( struct dsim_test_data *data )
{
    test_setup_dsim_array(uint64_t, 1, 10);

    unsigned pos = test_gen_uint( data, 0, test_count - 1 );
    unsigned count = test_gen_uint( data, 0, test_count - pos - 1 );
    dsim_array_remove_fast( test_array, pos, count );

    ASSERT_DSIM_ARRAY_INVARIANTS();
    if( test_count - pos - count < count )
        CHECK_CALL(assert_array_remove_ordered( test_array, pos, count, test_data, test_count ));
    else
        CHECK_CALL(assert_array_remove_unordered( test_array, pos, count, test_data, test_count ));
    PASS();
}

TEST array_clear( struct dsim_test_data *data )
{
    test_setup_dsim_array(uint64_t, 0, 10);

    dsim_array_clear( test_array );

    ASSERT_DSIM_ARRAY_INVARIANTS();
    ASSERT( test_array->capacity >= test_count );
    PASS();
}

TEST array_reset( struct dsim_test_data *data )
{
    test_setup_dsim_array(uint64_t, 0, 10);

    dsim_array_reset( test_array );

    ASSERT_DSIM_ARRAY_INVARIANTS();
    ASSERT( test_array->capacity == 0 );
    PASS();
}

SUITE(test_array)
{
    RUN_TEST_RAND(array_invariants);
    RUN_TEST_RAND(array_reserve);
    RUN_TEST_RAND(array_resize);
    RUN_TEST_RAND(array_push_back);
    RUN_TEST_RAND(array_push_back_n);
    RUN_TEST_RAND(array_pop_back);
    RUN_TEST_RAND(array_pop_back_n);
    RUN_TEST_RAND(array_remove);
    RUN_TEST_RAND(array_remove_fast);
    RUN_TEST_RAND(array_clear);
    RUN_TEST_RAND(array_reset);
}
