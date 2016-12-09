
#include "storage_block.h"

static void dsim_storage_array_resize( struct dsim_storage_array *tc, uint32_t count )
{
    _dsim_array_resize( &tc->data, count, tc->size );
}

static void dsim_storage_array_remove_fast( struct dsim_storage_array *tc, uint32_t pos, uint32_t count )
{
    _dsim_array_remove_fast( &tc->data, pos, count, tc->size );
}

static void dsim_storage_array_reset( struct dsim_storage_array *tc )
{
    _dsim_array_reset( &tc->data, tc->size );
}

void dsim_storage_block_resize( struct dsim_storage_block *sb, uint32_t count )
{
    for( uint32_t i = 0; i < sb->array_count; ++i )
        dsim_storage_array_resize( sb->arrays + i, count );
}

void dsim_storage_block_remove_fast( struct dsim_storage_block *sb, uint32_t pos, uint32_t count )
{
    for( uint32_t i = 0; i < sb->array_count; ++i )
        dsim_storage_array_remove_fast( sb->arrays + i, pos, count );
}

void dsim_storage_block_reset( struct dsim_storage_block *sb )
{
    for( uint32_t i = 0; i < sb->array_count; ++i )
        dsim_storage_array_reset( sb->arrays + i );
}
