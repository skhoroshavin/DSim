
#pragma once

#include "common.h"
#include "containers/array.h"

DSIM_BEGIN_HEADER

typedef dsim_array(char) dsim_byte_array;

void dsim_read_file( const char *filename, dsim_byte_array *buf );

DSIM_END_HEADER
