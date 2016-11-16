
#include "table_block.h"

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
