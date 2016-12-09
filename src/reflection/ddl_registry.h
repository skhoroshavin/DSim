
#pragma once

#include "ddl_reader.h"
#include "utils/common.h"

DSIM_BEGIN_HEADER

dsim_ddl_type_table_t dsim_ddl_types_scan_by_name( dsim_ddl_type_vec_t types, const char *name );

dsim_ddl_type_table_t dsim_ddl_type(const char *name );
dsim_ddl_layout_table_t dsim_ddl_layout(const char *name );

void dsim_ddl_register( const void *data );

void dsim_ddl_registry_init();
void dsim_ddl_registry_reset();

DSIM_END_HEADER
