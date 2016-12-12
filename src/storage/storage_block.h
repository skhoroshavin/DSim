
#pragma once

#include "containers/array.h"
#include "reflection/ddl_registry.h"

struct dsim_storage_array
{
    struct _dsim_array array;
    dsim_ddl_type_table_t type;
};

struct dsim_storage_block
{
    struct dsim_storage_array *arrays;
    struct dsim_allocator *alloc;
    dsim_ddl_layout_table_t layout;
};

void dsim_storage_block_init( struct dsim_storage_block *sb, dsim_ddl_layout_table_t layout, struct dsim_allocator *alloc );
void dsim_storage_block_resize( struct dsim_storage_block *sb, uint32_t count );
void dsim_storage_block_push_back( struct dsim_storage_block *sb, const void *const *data, uint32_t count );
void dsim_storage_block_remove_fast( struct dsim_storage_block *sb, uint32_t pos, uint32_t count );
void dsim_storage_block_done( struct dsim_storage_block *sb );
