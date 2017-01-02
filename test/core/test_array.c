
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

#define GIVEN_DSIM_ARRAY(type,name,min_count) \
    GIVEN_ARRAY(type,name,min_count); \
    dsim_array(type) *name; \
    test_gen_dsim_array( _ctx, name ); \
    dsim_array_push_back_n( name, name##_data, name##_count );

#define ASSERT_DSIM_ARRAY_INVARIANTS(name) do { \
    ASSERT( name->capacity >= name->count ); \
    if( name->capacity ) ASSERT( name->data ); \
    else ASSERT( !name->data ); \
    } while(0)

/*
 * Tests
 */

DSIM_TEST(array_invariants)
{
    GIVEN_DSIM_ARRAY(uint64_t, test, 0);

    ASSERT_DSIM_ARRAY_INVARIANTS(test);
    PASS();
}

DSIM_TEST(array_reserve)
{
    GIVEN_DSIM_ARRAY(uint64_t, test, 0);
    uint32_t old_capacity = test->capacity;
    GIVEN_UINT(capacity,0,old_capacity*2);

    dsim_array_reserve( test, capacity );

    ASSERT_DSIM_ARRAY_INVARIANTS(test);
    ASSERT( test->capacity >= old_capacity );
    ASSERT_INT_EQ( test->count, test_count );
    ASSERT_MEM_EQ( test->data, test_data, test_count*sizeof(uint64_t) );
    PASS();
}

DSIM_TEST(array_resize)
{
    GIVEN_DSIM_ARRAY(uint64_t, test, 0);
    GIVEN_UINT( new_size, 0, test->count*2 );

    dsim_array_resize( test, new_size );

    ASSERT_DSIM_ARRAY_INVARIANTS(test);
    ASSERT( test->count == new_size );
    ASSERT_MEM_EQ( test->data, test_data, min(new_size,test_count)*sizeof(uint64_t) );
    PASS();
}

DSIM_TEST(array_push_back)
{
    GIVEN_DSIM_ARRAY(uint64_t, test, 0);
    GIVEN_UINT(value, 0, UINT32_MAX);

    dsim_array_push_back( test, value );

    ASSERT_DSIM_ARRAY_INVARIANTS(test);
    ASSERT_INT_EQ( test->count, test_count + 1 );
    ASSERT_MEM_EQ( test->data, test_data, test_count*sizeof(uint64_t) );
    ASSERT_INT_EQ( test->at[test_count], value );
    PASS();
}

DSIM_TEST(array_push_back_n)
{
    GIVEN_DSIM_ARRAY(uint64_t, test, 0);
    GIVEN_ARRAY(uint64_t, more, 1 );

    dsim_array_push_back_n( test, more_data, more_count );

    ASSERT_DSIM_ARRAY_INVARIANTS(test);
    ASSERT_INT_EQ( test->count, test_count + more_count );
    ASSERT_MEM_EQ( test->data, test_data, test_count*sizeof(uint64_t) );
    ASSERT_MEM_EQ( test->data + test_count, more_data, more_count*sizeof(uint64_t) );
    PASS();
}

DSIM_TEST(array_pop_back)
{
    GIVEN_DSIM_ARRAY(uint64_t, test, 1);

    dsim_array_pop_back( test );

    ASSERT_DSIM_ARRAY_INVARIANTS(test);
    ASSERT_INT_EQ( test->count, test_count - 1 );
    ASSERT_MEM_EQ( test->data, test_data, (test_count-1)*sizeof(uint64_t) );
    PASS();
}

DSIM_TEST(array_pop_back_n)
{
    GIVEN_DSIM_ARRAY(uint64_t, test, 1);
    GIVEN_UINT( pop_count, 0, test->count );

    dsim_array_pop_back_n( test, pop_count );

    ASSERT_DSIM_ARRAY_INVARIANTS(test);
    ASSERT_INT_EQ( test->count, test_count - pop_count );
    ASSERT_MEM_EQ( test->data, test_data, (test_count-pop_count)*sizeof(uint64_t) );
    PASS();
}

DSIM_TEST(array_remove)
{
    GIVEN_DSIM_ARRAY(uint64_t, test, 1);
    GIVEN_UINT( pos, 0, test->count - 1 );
    GIVEN_UINT( count, 0, test->count - pos - 1 );

    dsim_array_remove( test, pos, count );

    ASSERT_DSIM_ARRAY_INVARIANTS(test);
    ASSERT_INT_EQ( test->count, test_count - count );
    ASSERT_MEM_EQ( test->data, test_data, pos*sizeof(uint64_t) );
    ASSERT_MEM_EQ( test->data + pos, test_data + pos + count, (test_count - pos - count)*sizeof(uint64_t) );
    PASS();
}

DSIM_TEST(array_remove_fast)
{
    GIVEN_DSIM_ARRAY(uint64_t, test, 1);
    GIVEN_UINT( pos, 0, test->count - 1 );
    GIVEN_UINT( count, 0, test->count - pos - 1 );

    dsim_array_remove_fast( test, pos, count );

    ASSERT_DSIM_ARRAY_INVARIANTS(test);
    ASSERT_INT_EQ( test->count, test_count - count );
    ASSERT_MEM_EQ( test->data, test_data, pos*sizeof(uint64_t) );
    if( test_count - pos - count < count )
        ASSERT_MEM_EQ( test->data + pos, test_data + pos + count, (test_count - pos - count)*sizeof(uint64_t) );
    else
    {
        ASSERT_MEM_EQ( test->data + pos, test_data + test_count - count, count*sizeof(uint64_t) );
        ASSERT_MEM_EQ( test->data + pos + count, test_data + pos + count, (test_count - pos - 2*count)*sizeof(uint64_t) );
    }
    PASS();
}

DSIM_TEST(array_clear)
{
    GIVEN_DSIM_ARRAY(uint64_t, test, 0);

    dsim_array_clear( test );

    ASSERT_DSIM_ARRAY_INVARIANTS(test);
    ASSERT( test->capacity >= test_count );
    PASS();
}

DSIM_TEST(array_reset)
{
    GIVEN_DSIM_ARRAY(uint64_t, test, 0);

    dsim_array_reset( test );

    ASSERT_DSIM_ARRAY_INVARIANTS(test);
    ASSERT( test->capacity == 0 );
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
