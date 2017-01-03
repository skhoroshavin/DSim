
#pragma once

#include "test_data.h"
#include "greatest.h"

DSIM_BEGIN_HEADER

typedef enum greatest_test_res (*dsim_rand_test)( struct dsim_test_context *data );
#define DSIM_TEST(name) TEST name( struct dsim_test_context *_ctx )

void dsim_run_test( const char *name, dsim_rand_test test );

#define DSIM_RUN_TEST(TEST) dsim_run_test(#TEST, TEST)

DSIM_END_HEADER
