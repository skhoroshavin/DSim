
#pragma once

#include "reflection/ddl_reader.h"
#include "reflection/flatbuffers_common_builder.h"
#include "utils/common.h"

DSIM_BEGIN_HEADER

void dsim_rebuild_type( struct flatcc_builder * b, dsim_type_table_t type );

DSIM_END_HEADER
