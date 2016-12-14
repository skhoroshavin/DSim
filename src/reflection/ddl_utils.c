
#include "ddl_utils.h"

dsim_ddl_type_table_t dsim_ddl_type_by_name( dsim_ddl_root_table_t root, const char *name )
{
    dsim_ddl_type_vec_t types = dsim_ddl_root_types(root);
    size_t i = dsim_ddl_type_vec_scan( types, name );
    if( i == flatbuffers_not_found ) return 0;
    return dsim_ddl_type_vec_at(types, i);
}

dsim_ddl_layout_table_t dsim_ddl_layout_by_name( dsim_ddl_root_table_t root, const char *name )
{
    dsim_ddl_layout_vec_t layouts = dsim_ddl_root_layouts(root);
    size_t i = dsim_ddl_layout_vec_scan( layouts, name );
    if( i == flatbuffers_not_found ) return 0;
    return dsim_ddl_layout_vec_at(layouts, i);
}
