
#pragma once

#include "greatest.h"
#include "containers/array.h"

/*
 * Utility
 */

typedef dsim_array(uint64_t) dsim_array_uint64;

enum greatest_test_res assert_array_null( const dsim_array_uint64 *a );
enum greatest_test_res assert_array_capacity( const dsim_array_uint64 *a, uint32_t capacity );
enum greatest_test_res assert_array_remove_ordered( const dsim_array_uint64 *a, uint32_t pos, uint32_t count, const uint64_t *old_data, uint32_t old_count );
enum greatest_test_res assert_array_remove_unordered( const dsim_array_uint64 *a, uint32_t pos, uint32_t count, const uint64_t *old_data, uint32_t old_count );

/*
 * Runner
 */


SUITE_EXTERN(test_array);
