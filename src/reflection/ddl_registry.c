
#include "ddl_registry.h"
#include "containers/array.h"
#include "utils/log.h"

DSIM_DEFINE_ARRAY(dsim_type_table_t, type)

static struct dsim_array_type types = dsim_array_static_init(&dsim_default_allocator);

dsim_type_table_t dsim_type( const char * str )
{
    for( uint32_t i = 0; i != types.count; ++i )
    {
        dsim_type_table_t type = types.at[i];
        if( strcmp( dsim_type_name(type), str ) == 0  )
            return type;
    }
    return 0;
}

void dsim_register_type( dsim_type_table_t type )
{
    if( dsim_type( dsim_type_name(type) ) ) // LCOV_EXCL_START
    {
        dsim_error( "Double register of type %s", dsim_type_name(type) );
        return;
    } // LCOV_EXCL_STOP
    dsim_array_type_push_back( &types, type );
}

void dsim_ddl_register( const void * data )
{
    dsim_ddl_table_t ddl = dsim_ddl_as_root(data);

    dsim_type_vec_t types = dsim_ddl_types( ddl );
    for( size_t i = 0; i != dsim_type_vec_len(types); ++i )
        dsim_register_type( dsim_type_vec_at(types, i) );
}

void dsim_ddl_registry_init()
{

}

void dsim_ddl_registry_reset()
{
    dsim_array_type_reset( &types );
}
