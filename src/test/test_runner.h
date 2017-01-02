
#pragma once

#include "test_data.h"
#include "greatest.h"

DSIM_BEGIN_HEADER

typedef enum greatest_test_res (*dsim_rand_test)( struct dsim_test_data *data );
#define DSIM_TEST(name) TEST name( struct dsim_test_data *_ctx )

void dsim_run_test( const char *name, dsim_rand_test test );

#define RUN_TEST_RAND(TEST) dsim_run_test(#TEST, TEST)

DSIM_END_HEADER
