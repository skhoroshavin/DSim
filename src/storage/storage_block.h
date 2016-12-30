
#pragma once

#include "storage_array.h"

struct dsim_storage_block
{
    struct dsim_storage_array *arrays;
    struct dsim_allocator *alloc;
    dsim_ddl_layout_table_t layout;
};

void dsim_storage_block_init( struct dsim_storage_block *sb, dsim_ddl_layout_table_t layout, struct dsim_allocator *alloc );
int dsim_storage_block_can_modify( const struct dsim_storage_block *sb );
void dsim_storage_block_resize( struct dsim_storage_block *sb, uint32_t count );
void dsim_storage_block_push_back( struct dsim_storage_block *sb, const void *const *data, uint32_t count );
uint32_t dsim_storage_block_update( struct dsim_storage_block *sb, const void *const *data, uint32_t src_offset, uint32_t dst_offset, uint32_t count );
void dsim_storage_block_remove_fast( struct dsim_storage_block *sb, uint32_t pos, uint32_t count );
void dsim_storage_block_done( struct dsim_storage_block *sb );

const void *dsim_storage_block_begin_read( struct dsim_storage_block *sb, uint32_t arr );
unsigned dsim_storage_block_end_read( struct dsim_storage_block *sb, const void *data );
void *dsim_storage_block_begin_write( struct dsim_storage_block *sb, uint32_t arr, enum dsim_storage_write_mode mode );
unsigned dsim_storage_block_end_write( struct dsim_storage_block *sb, void *data );
