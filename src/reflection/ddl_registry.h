
#pragma once

#include "ddl_reader.h"
#include "utils/common.h"

DSIM_BEGIN_HEADER

dsim_type_table_t dsim_type( const char * str );

void dsim_register_type( dsim_type_table_t type );

void dsim_ddl_register( const void * data );

void dsim_ddl_registry_init();
void dsim_ddl_registry_reset();

DSIM_END_HEADER
