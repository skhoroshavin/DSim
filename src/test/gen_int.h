
#pragma once

#include "test_gen.h"

DSIM_BEGIN_HEADER

unsigned test_gen_uint( struct dsim_test_context *data, unsigned min, unsigned max );

#define GIVEN_UINT(name,min,max) unsigned name = test_gen_uint( _ctx, min, max );

DSIM_END_HEADER
