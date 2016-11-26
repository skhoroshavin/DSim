
#pragma once

#include "unity_fixture.h"
#include "containers/array.h"

/*
 * Utility
 */

void TEST_ASSERT_ARRAY_NULL( const struct dsim_array_uint64 *a );
void TEST_ASSERT_ARRAY_CAPACITY( const struct dsim_array_uint64 *a, uint32_t capacity );

/*
 * Runner
 */

void run_test_array();
