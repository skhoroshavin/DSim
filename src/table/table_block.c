
#include "table_block.h"

static void dsim_table_column_resize( struct dsim_table_column *tc, uint32_t count )
{
    _dsim_array_resize( &tc->data, count, tc->elem_size );
}

static void dsim_table_column_remove( struct dsim_table_column *tc, uint32_t pos, uint32_t count )
{
    _dsim_array_remove( &tc->data, pos, count, tc->elem_size );
}

static void dsim_table_column_remove_fast( struct dsim_table_column *tc, uint32_t pos, uint32_t count )
{
    _dsim_array_remove_fast( &tc->data, pos, count, tc->elem_size );
}

static void dsim_table_column_reset( struct dsim_table_column *tc )
{
    _dsim_array_reset( &tc->data, tc->elem_size );
}

void dsim_table_block_resize( struct dsim_table_block *tb, uint32_t count )
{
    for( uint32_t i = 0; i < tb->col_count; ++i )
        dsim_table_column_resize( tb->columns + i, count );
}

void dsim_table_block_remove( struct dsim_table_block *tb, uint32_t pos, uint32_t count )
{
    for( uint32_t i = 0; i < tb->col_count; ++i )
        dsim_table_column_remove( tb->columns + i, pos, count );
}

void dsim_table_block_remove_fast( struct dsim_table_block *tb, uint32_t pos, uint32_t count )
{
    for( uint32_t i = 0; i < tb->col_count; ++i )
        dsim_table_column_remove_fast( tb->columns + i, pos, count );
}

void dsim_table_block_reset( struct dsim_table_block *tb )
{
    for( uint32_t i = 0; i < tb->col_count; ++i )
        dsim_table_column_reset( tb->columns + i );
}
