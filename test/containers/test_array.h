
#pragma once

#include "unity_fixture.h"
#include "containers/array.h"

/*
 * Utility
 */

void TEST_ASSERT_ARRAY_NULL( const struct dsim_array_uint64 *a );
void TEST_ASSERT_ARRAY_CAPACITY( const struct dsim_array_uint64 *a, uint32_t capacity );
void TEST_ASSERT_ARRAY_REMOVE_ORDERED( const struct dsim_array_uint64 *a, uint32_t pos, uint32_t count, const uint64_t *old_data, uint32_t old_count );
void TEST_ASSERT_ARRAY_REMOVE_UNORDERED( const struct dsim_array_uint64 *a, uint32_t pos, uint32_t count, const uint64_t *old_data, uint32_t old_count );

/*
 * Runner
 */

void run_test_array();
