
#pragma once

#include "greatest.h"
#include "containers/array.h"

/*
 * Utility
 */

enum greatest_test_res _assert_array_null( const void *a_data, uint32_t a_count, uint32_t a_capacity );
enum greatest_test_res _assert_array_capacity( const void *a_data, uint32_t a_count, uint32_t a_capacity, uint32_t capacity );
enum greatest_test_res _assert_array_remove_ordered( const void *a_data, uint32_t a_count, uint32_t a_capacity, uint32_t a_elemsize, uint32_t pos, uint32_t count, const uint64_t *old_data, uint32_t old_count );
enum greatest_test_res _assert_array_remove_unordered( const void *a_data, uint32_t a_count, uint32_t a_capacity, uint32_t a_elemsize, uint32_t pos, uint32_t count, const uint64_t *old_data, uint32_t old_count );

#define assert_array_null(a) _assert_array_null( (a)->data, (a)->count, (a)->capacity )
#define assert_array_capacity(a,n) _assert_array_capacity( (a)->data, (a)->count, (a)->capacity, n )
#define assert_array_remove_ordered(a,pos,n,old_data,old_count) \
    _assert_array_remove_ordered( (a)->data, (a)->count, (a)->capacity, sizeof((a)->at[0]), pos, n, old_data, old_count )
#define assert_array_remove_unordered(a,pos,n,old_data,old_count) \
    _assert_array_remove_unordered( (a)->data, (a)->count, (a)->capacity, sizeof((a)->at[0]), pos, n, old_data, old_count )


/*
 * Runner
 */


SUITE_EXTERN(test_array);
