
#include "ddl_utils.h"

dsim_ddl_type_table_t dsim_ddl_type_by_name( dsim_ddl_root_table_t root, const char *name )
{
    dsim_ddl_type_vec_t types = dsim_ddl_root_types(root);
    for( size_t i = 0; i != dsim_ddl_type_vec_len(types); ++i )
    {
        dsim_ddl_type_table_t type = dsim_ddl_type_vec_at(types, i);
        if( strcmp(dsim_ddl_type_name(type),name) == 0 )
            return type;
    }
    return 0;
}

dsim_ddl_layout_table_t dsim_ddl_layout_by_name( dsim_ddl_root_table_t root, const char *name )
{
    dsim_ddl_layout_vec_t layouts = dsim_ddl_root_layouts(root);
    for( size_t i = 0; i != dsim_ddl_layout_vec_len(layouts); ++i )
    {
        dsim_ddl_layout_table_t layout = dsim_ddl_layout_vec_at(layouts, i);
        if( strcmp(dsim_ddl_layout_name(layout),name) == 0 )
            return layout;
    }
    return 0;
}
