
#pragma once

#include "utils/common.h"
#include "ddl_reader.h"

DSIM_BEGIN_HEADER

dsim_ddl_type_table_t   dsim_ddl_type_by_name( dsim_ddl_root_table_t root, const char *name );
dsim_ddl_layout_table_t dsim_ddl_layout_by_name( dsim_ddl_root_table_t root, const char *name );

DSIM_END_HEADER
