
#include "ddl_registry.h"
#include "containers/array.h"
#include "utils/log.h"

static struct dsim_array_cptr blocks = dsim_array_static_init(&dsim_default_allocator);

dsim_type_table_t dsim_types_scan_by_name( dsim_type_vec_t types, const char * name )
{
    for( size_t i = 0; i != dsim_type_vec_len(types); ++i )
    {
        dsim_type_table_t type = dsim_type_vec_at(types, i);
        if( strcmp(dsim_type_name(type),name) == 0 )
            return type;
    }
    return 0;
}

dsim_layout_table_t dsim_layouts_scan_by_name( dsim_layout_vec_t layouts, const char * name )
{
    for( size_t i = 0; i != dsim_layout_vec_len(layouts); ++i )
    {
        dsim_layout_table_t layout = dsim_layout_vec_at(layouts, i);
        if( strcmp(dsim_layout_name(layout),name) == 0 )
            return layout;
    }
    return 0;
}

dsim_type_table_t dsim_type( const char *name )
{
    for( uint32_t i = 0; i != blocks.count; ++i )
    {
        dsim_ddl_table_t ddl = dsim_ddl_as_root(blocks.at[i]);
        dsim_type_table_t type = dsim_types_scan_by_name( dsim_ddl_types(ddl), name );
        if( type ) return type;
    }
    return 0;
}

dsim_layout_table_t dsim_layout( const char *name )
{
    for( uint32_t i = 0; i != blocks.count; ++i )
    {
        dsim_ddl_table_t ddl = dsim_ddl_as_root(blocks.at[i]);
        dsim_layout_table_t layout = dsim_layouts_scan_by_name( dsim_ddl_layouts(ddl), name );
        if( layout ) return layout;
    }
    return 0;
}

void dsim_ddl_register( const void * data )
{
    dsim_array_cptr_push_back( &blocks, data );
}

void dsim_ddl_registry_init()
{

}

void dsim_ddl_registry_reset()
{
    dsim_array_cptr_reset( &blocks );
}
