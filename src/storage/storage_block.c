
#include "storage_block.h"

void dsim_storage_block_init( struct dsim_storage_block *sb, dsim_ddl_layout_table_t layout, struct dsim_allocator *alloc )
{
    sb->layout = layout;
    sb->alloc = alloc;

    dsim_ddl_array_vec_t arrays = dsim_ddl_layout_arrays(layout);
    size_t array_count = dsim_ddl_array_vec_len(arrays);
    sb->arrays = (struct dsim_storage_array*)dsim_allocate( alloc, array_count*sizeof(struct dsim_storage_array) );
    for( size_t i = 0; i != array_count; ++i )
    {
        dsim_ddl_array_table_t array = dsim_ddl_array_vec_at(arrays,i);
        dsim_ddl_type_table_t type = dsim_ddl_type( dsim_ddl_array_type(array) );
        dsim_storage_array_init( sb->arrays + i, type, alloc );
    }
}

int dsim_storage_block_can_modify( const struct dsim_storage_block *sb )
{
    dsim_ddl_array_vec_t arrays = dsim_ddl_layout_arrays(sb->layout);
    size_t array_count = dsim_ddl_array_vec_len(arrays);
    for( uint32_t i = 0; i < array_count; ++i )
        if( !dsim_storage_array_can_modify( sb->arrays + i) )
            return 0;
    return 1;
}

void dsim_storage_block_resize( struct dsim_storage_block *sb, uint32_t count )
{
    dsim_ddl_array_vec_t arrays = dsim_ddl_layout_arrays(sb->layout);
    size_t array_count = dsim_ddl_array_vec_len(arrays);
    for( uint32_t i = 0; i < array_count; ++i )
        dsim_storage_array_resize( sb->arrays + i, count );
}

void dsim_storage_block_push_back( struct dsim_storage_block *sb, const void *const *data, uint32_t count )
{
    dsim_ddl_array_vec_t arrays = dsim_ddl_layout_arrays(sb->layout);
    size_t array_count = dsim_ddl_array_vec_len(arrays);
    for( uint32_t i = 0; i < array_count; ++i )
    {
        if( data[i] )
            dsim_storage_array_push_back( sb->arrays + i, data[i], count );
        else
            dsim_storage_array_resize( sb->arrays + i, sb->arrays[i].main.count + count );
    }
}

void dsim_storage_block_update( struct dsim_storage_block *sb, const void *const *data, uint32_t src_offset, uint32_t dst_offset, uint32_t count )
{
    dsim_ddl_array_vec_t arrays = dsim_ddl_layout_arrays(sb->layout);
    size_t array_count = dsim_ddl_array_vec_len(arrays);
    for( uint32_t i = 0; i < array_count; ++i )
    {
        if( !data[i] ) continue;
        dsim_storage_array_update( sb->arrays + i, data[i], src_offset, dst_offset, count );
    }
}

void dsim_storage_block_remove_fast( struct dsim_storage_block *sb, uint32_t pos, uint32_t count )
{
    dsim_ddl_array_vec_t arrays = dsim_ddl_layout_arrays(sb->layout);
    size_t array_count = dsim_ddl_array_vec_len(arrays);
    for( uint32_t i = 0; i < array_count; ++i )
        dsim_storage_array_remove_fast( sb->arrays + i, pos, count );
}

void dsim_storage_block_done( struct dsim_storage_block *sb )
{
    dsim_ddl_array_vec_t arrays = dsim_ddl_layout_arrays(sb->layout);
    size_t array_count = dsim_ddl_array_vec_len(arrays);
    for( uint32_t i = 0; i < array_count; ++i )
        dsim_storage_array_reset( sb->arrays + i );
    dsim_deallocate( sb->alloc, sb->arrays );
}

const void *dsim_storage_block_begin_read( struct dsim_storage_block *sb, uint32_t arr )
{
    dsim_ddl_array_vec_t arrays = dsim_ddl_layout_arrays(sb->layout);
    assert( arr < dsim_ddl_array_vec_len(arrays) );
    return dsim_storage_array_begin_read( sb->arrays + arr );
}

unsigned dsim_storage_block_end_read( struct dsim_storage_block *sb, const void *data )
{
    dsim_ddl_array_vec_t arrays = dsim_ddl_layout_arrays(sb->layout);
    for( size_t i = 0; i != dsim_ddl_array_vec_len(arrays); ++i )
        if( dsim_storage_array_end_read(sb->arrays+i, data) != DSIM_INVALID_INDEX )
            return i;
    return DSIM_INVALID_INDEX;
}

void *dsim_storage_block_begin_write( struct dsim_storage_block *sb, uint32_t arr, enum dsim_storage_write_mode mode )
{
    dsim_ddl_array_vec_t arrays = dsim_ddl_layout_arrays(sb->layout);
    assert( arr < dsim_ddl_array_vec_len(arrays) );
    return dsim_storage_array_begin_write( sb->arrays + arr, mode );
}

unsigned dsim_storage_block_end_write( struct dsim_storage_block *sb, void *data )
{
    dsim_ddl_array_vec_t arrays = dsim_ddl_layout_arrays(sb->layout);
    for( size_t i = 0; i != dsim_ddl_array_vec_len(arrays); ++i )
        if( dsim_storage_array_end_write( sb->arrays+i, data ) != DSIM_INVALID_INDEX )
            return i;
    return DSIM_INVALID_INDEX;
}
