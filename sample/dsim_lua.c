
#include "dsim_lua.h"
#include "storage/ddl_registry.h"
#include <memory.h>

/*
 *  Reflection utility
 */

static uint32_t dsim_storage_array_by_name( const struct dsim_storage *s, const char *name )
{
    dsim_ddl_array_vec_t arrays = dsim_ddl_layout_arrays(s->layout);
    uint32_t count = dsim_ddl_array_vec_len(arrays);
    for( uint32_t i = 0; i < count; ++i )
    {
        dsim_ddl_array_table_t array = dsim_ddl_array_vec_at(arrays,i);
        if( strcmp( name, dsim_ddl_array_name(array) ) == 0 )
            return i;
    }
    return DSIM_INVALID_INDEX;
}

static dsim_ddl_type_table_t dsim_storage_array_type( const struct dsim_storage *s, uint32_t arr )
{
    dsim_ddl_array_vec_t arrays = dsim_ddl_layout_arrays(s->layout);
    dsim_ddl_array_table_t array = dsim_ddl_array_vec_at(arrays,arr);
    return dsim_ddl_type( dsim_ddl_array_type(array) );
}

static void dsim_lua_read( lua_State *l, int index, dsim_ddl_type_table_t type, void *data )
{
    uint32_t size = dsim_ddl_type_size(type);
    dsim_ddl_any_type_union_type_t ttype = dsim_ddl_type_data_type(type);

    if( ttype == dsim_ddl_any_type_numeric_type )
    {
        dsim_ddl_numeric_type_table_t ntype = (dsim_ddl_numeric_type_table_t)dsim_ddl_type_data(type);
        lua_Number value = lua_tonumber( l, index );

        if( dsim_ddl_numeric_type_is_float(ntype) )
        {
            if( size == 4 )
                *((float*)data) = value;
            else if( size == 8 )
                *((double*)data) = value;
            else
                printf( "Unsupported float size %d!\n", size );
        }
        else
        {
            if( size == 1 )
                *((uint8_t*)data) = value;
            else if( size == 2 )
                *((uint16_t*)data) = value;
            else if( size == 4 )
                *((uint32_t*)data) = value;
            else if( size == 8 )
                *((uint64_t*)data) = value;
            else
                printf( "Unsupported int size %d!\n", size );
        }
    }
    else if( ttype == dsim_ddl_any_type_reference_type )
    {
        *((uint64_t*)data) = lua_tonumber( l, index );
    }
    else
    {
        memcpy( data, lua_touserdata( l, index ), size );
    }
}

static void dsim_lua_push( lua_State *l, dsim_ddl_type_table_t type, const void *data )
{
    uint32_t size = dsim_ddl_type_size(type);
    dsim_ddl_any_type_union_type_t ttype = dsim_ddl_type_data_type(type);

    if( ttype == dsim_ddl_any_type_numeric_type )
    {
        dsim_ddl_numeric_type_table_t ntype = (dsim_ddl_numeric_type_table_t)dsim_ddl_type_data(type);

        if( dsim_ddl_numeric_type_is_float(ntype) )
        {
            if( size == 4 )
                lua_pushnumber( l, *((float*)data) );
            else if( size == 8 )
                lua_pushnumber( l, *((double*)data) );
            else
                printf( "Unsupported float size %d!\n", size );
        }
        else
        {
            if( size == 1 )
                lua_pushnumber( l, *((uint8_t*)data) );
            else if( size == 2 )
                lua_pushnumber( l, *((uint16_t*)data) );
            else if( size == 4 )
                lua_pushnumber( l, *((uint32_t*)data) );
            else if( size == 8 )
                lua_pushnumber( l, *((uint64_t*)data) );
            else
                printf( "Unsupported int size %d!\n", size );
        }
    }
    else if( ttype == dsim_ddl_any_type_reference_type )
    {
        lua_pushnumber( l, *((uint64_t*)data) );
    }
    else
    {
        void *value = lua_newuserdata( l, size );
        memcpy( value, data, size );
    }
}

/*
 *  Lua bindings
 */

static int lua_storage_set_by_id( lua_State *l )
{
    const char *name = lua_tostring( l, lua_upvalueindex(1) );
    struct dsim_storage *s = *(struct dsim_storage **)lua_touserdata( l, 1 );
    uint32_t arr = dsim_storage_array_by_name( s, name );
    if( arr == DSIM_INVALID_INDEX )
        return 0;

    uint64_t id = lua_tonumber( l, 2 );
    dsim_storage_addr addr;
    dsim_storage_select_buf( s, &id, &addr, 1 );

    dsim_ddl_type_table_t type = dsim_storage_array_type( s, arr );
    uint32_t size = dsim_ddl_type_size(type);
    char *data = (char*)dsim_storage_begin_write( s, addr.block, arr, DSIM_STORAGE_WRITE_DIRECT );
    dsim_lua_read( l, 3, type, data + size * addr.index );
    dsim_storage_end_write( s, data );

    return 0;
}

static int lua_storage_get_by_id( lua_State *l )
{
    const char *name = lua_tostring( l, lua_upvalueindex(1) );
    struct dsim_storage *s = *(struct dsim_storage **)lua_touserdata( l, 1 );
    uint32_t arr = dsim_storage_array_by_name( s, name );
    if( arr == DSIM_INVALID_INDEX )
        return 0;

    uint64_t id = lua_tonumber( l, 2 );
    dsim_storage_addr addr;
    dsim_storage_select_buf( s, &id, &addr, 1 );

    dsim_ddl_type_table_t type = dsim_storage_array_type( s, arr );
    uint32_t size = dsim_ddl_type_size(type);
    char *data = (char*)dsim_storage_begin_read( s, addr.block, arr );
    dsim_lua_push( l, type, data + size * addr.index );
    dsim_storage_end_read( s, data );

    return 1;
}

static int lua_storage_data( lua_State *l )
{
    const char *name = lua_tostring( l, 2 );
    if( strncmp( name, "set_", 4 ) == 0 )
    {
        name += 4;
        lua_pushstring( l, name );
        lua_pushcclosure( l, lua_storage_set_by_id, 1 );
    }
    else
    {
        lua_pushstring( l, name );
        lua_pushcclosure( l, lua_storage_get_by_id, 1 );
    }
    return 1;
}

static int lua_storage_size( lua_State *l )
{
    struct dsim_storage *s = *(struct dsim_storage **)lua_touserdata( l, 1 );
    lua_pushnumber( l, dsim_storage_block_size( s, 0) );
    return 1;
}

static int lua_storage_get_id( lua_State *l )
{
    struct dsim_storage *s = *(struct dsim_storage **)lua_touserdata( l, 1 );
    uint32_t index = lua_tonumber( l, 2 );
    lua_pushnumber( l, dsim_storage_id_data( s, 0 )[index-1] );
    return 1;
}

static void lua_push_storage_meta( lua_State *l )
{
    if( luaL_newmetatable( l, "dsim_storage") )
    {
        lua_createtable( l, 0, 2 );
        {
            lua_pushcfunction( l, lua_storage_size );
            lua_setfield( l, -2, "count" );

            lua_pushcfunction( l, lua_storage_get_id );
            lua_setfield( l, -2, "get_id" );

            lua_createtable( l, 0, 1 );
            {
                lua_pushcfunction( l, lua_storage_data );
                lua_setfield( l, -2, "__index" );
            }
            lua_setmetatable( l, -2 );
        }
        lua_setfield( l, -2, "__index" );
    }
}

void dsim_lua_register_storage( lua_State *l, struct dsim_storage *s, const char *name )
{
    struct dsim_storage **ud = (struct dsim_storage **)lua_newuserdata( l, sizeof(struct dsim_storage*) );
    *ud = s;

    lua_push_storage_meta( l );
    lua_setmetatable( l, -2 );

    lua_setglobal( l, name );
}
