
#pragma once

#include "table/hash_table.h"

#include "dsim_lua.h"

#include "config_reader.h"
#include "config_json_parser.h"


#include <stdio.h>
#include <memory.h>

#define BEGIN_TABLE(name) \
    static struct dsim_table_column name##_columns[] = {

#define END_TABLE(name) \
    }; \
    static struct dsim_hash_table name##_hash = dsim_hash_table_static_init( \
        count_of(name##_columns), name##_columns, &dsim_default_allocator ); \
    static struct dsim_table * tbl_##name = &name##_hash.table;
