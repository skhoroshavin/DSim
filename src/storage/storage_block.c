
#include "storage_block.h"

static void dsim_storage_array_init( struct dsim_storage_array *sa, dsim_ddl_type_table_t type, struct dsim_allocator *alloc )
{
    sa->type = type;
    _dsim_array_init( &sa->array, alloc );
}

static void dsim_storage_array_resize( struct dsim_storage_array *sa, uint32_t count )
{
    _dsim_array_resize( &sa->array, count, dsim_ddl_type_size(sa->type) );
}

static void dsim_storage_array_remove_fast( struct dsim_storage_array *sa, uint32_t pos, uint32_t count )
{
    _dsim_array_remove_fast( &sa->array, pos, count, dsim_ddl_type_size(sa->type) );
}

static void dsim_storage_array_reset( struct dsim_storage_array *sa )
{
    _dsim_array_reset( &sa->array );
}

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

void dsim_storage_block_resize( struct dsim_storage_block *sb, uint32_t count )
{
    dsim_ddl_array_vec_t arrays = dsim_ddl_layout_arrays(sb->layout);
    size_t array_count = dsim_ddl_array_vec_len(arrays);
    for( uint32_t i = 0; i < array_count; ++i )
        dsim_storage_array_resize( sb->arrays + i, count );
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
