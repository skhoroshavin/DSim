
#include "table/hash_table.h"

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include <stdio.h>

#define BEGIN_TABLE(name) \
    static struct dsim_table_column name##_columns[] = {

#define END_TABLE(name) \
    }; \
    static struct dsim_hash_table name##_hash = dsim_hash_table_static_init( \
        count_of(name##_columns), name##_columns, &dsim_default_allocator ); \
    static struct dsim_table * tbl_##name = &name##_hash.table;

BEGIN_TABLE(actuator)
    dsim_table_column_static_init( "state", sizeof(uint32_t), &dsim_default_allocator )
END_TABLE(actuator)

static void * dsim_lua_alloc( void *ud, void *ptr, size_t osize, size_t nsize )
{
    dsim_unused(ud);

    if( (osize == 0) && (nsize == 0) )
        return 0;

    if( (osize == 0) && (nsize != 0) )
        return dsim_allocate( &dsim_default_allocator, nsize );

    if( (osize != 0) && (nsize == 0) )
    {
        dsim_deallocate( &dsim_default_allocator, ptr, osize );
        return 0;
    }

    return dsim_reallocate( &dsim_default_allocator, ptr, osize, nsize );
}

int main( int argc, const char * argv[] )
{
    dsim_unused(argc);
    dsim_unused(argv);

    lua_State * lua = lua_newstate( dsim_lua_alloc, 0 );
    luaopen_base( lua );



    luaL_dofile( lua, "main.lua" );
    lua_close( lua );

    return 0;
}
