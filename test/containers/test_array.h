
#pragma once

#include "greatest.h"
#include "containers/array.h"

/*
 * Utility
 */

enum greatest_test_res assert_array_null( const struct dsim_array_uint64 *a );
enum greatest_test_res assert_array_capacity( const struct dsim_array_uint64 *a, uint32_t capacity );
enum greatest_test_res assert_array_remove_ordered( const struct dsim_array_uint64 *a, uint32_t pos, uint32_t count, const uint64_t *old_data, uint32_t old_count );
enum greatest_test_res assert_array_remove_unordered( const struct dsim_array_uint64 *a, uint32_t pos, uint32_t count, const uint64_t *old_data, uint32_t old_count );

/*
 * Runner
 */


SUITE_EXTERN(test_array);
