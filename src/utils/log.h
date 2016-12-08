
#pragma once

#include "common.h"

DSIM_BEGIN_HEADER

void dsim_info( const char * fmt, ... );
void dsim_warning( const char * fmt, ... );
void dsim_error( const char * fmt, ... );
void dsim_fatal( const char * fmt, ... );

DSIM_END_HEADER
