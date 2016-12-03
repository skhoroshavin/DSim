
#include "dsim_lua.h"
#include <memory.h>

/*
 *  Reflection utility
 */

static uint32_t dsim_table_column_by_name( const struct dsim_table * tbl, const char * name )
{
    uint32_t count = dsim_table_column_count( tbl );
    for( uint32_t i = 0; i < count; ++i )
        if( strcmp( name, dsim_table_column_name( tbl, i ) ) == 0 )
            return i;
    return DSIM_INVALID_INDEX;
}

static void dsim_lua_read( lua_State * l, int index, enum dsim_type type, uint32_t size, void *data )
{
    if( type == DSIM_TYPE_STRUCT )
    {
        memcpy( data, lua_touserdata( l, index ), size );
    }
    else
    {
        lua_Number value = lua_tonumber( l, index );

        if( type == DSIM_TYPE_INT )
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
        else if( type == DSIM_TYPE_FLOAT )
        {
            if( size == 4 )
                *((float*)data) = value;
            else if( size == 8 )
                *((double*)data) = value;
            else
                printf( "Unsupported float size %d!\n", size );
        }
    }
}

static void dsim_lua_push( lua_State * l, enum dsim_type type, uint32_t size, const void *data )
{
    if( type == DSIM_TYPE_STRUCT )
    {
        void * value = lua_newuserdata( l, size );
        memcpy( value, data, size );
    }
    else
    {
        if( type == DSIM_TYPE_INT )
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
        else if( type == DSIM_TYPE_FLOAT )
        {
            if( size == 4 )
                lua_pushnumber( l, *((float*)data) );
            else if( size == 8 )
                lua_pushnumber( l, *((double*)data) );
            else
                printf( "Unsupported float size %d!\n", size );
        }
    }
}

/*
 *  Lua bindings
 */

static int lua_table_set_by_id( lua_State * l )
{
    const char * name = lua_tostring( l, lua_upvalueindex(1) );
    struct dsim_table * tbl = *(struct dsim_table**)lua_touserdata( l, 1 );
    uint32_t col = dsim_table_column_by_name( tbl, name );
    if( col == DSIM_INVALID_INDEX )
        return 0;

    uint64_t id = lua_tonumber( l, 2 );
    dsim_table_index idx = dsim_table_find( tbl, id );

    enum dsim_type type = dsim_table_column_type( tbl, col );
    uint32_t size = dsim_table_column_size(tbl,col);
    char * data = (char*)dsim_table_data( tbl, idx.block, col ) + size * idx.index;

    dsim_lua_read( l, 3, type, size, data );

    return 0;
}

static int lua_table_get_by_id( lua_State * l )
{
    const char * name = lua_tostring( l, lua_upvalueindex(1) );
    struct dsim_table * tbl = *(struct dsim_table**)lua_touserdata( l, 1 );
    uint32_t col = dsim_table_column_by_name( tbl, name );
    if( col == DSIM_INVALID_INDEX )
        return 0;

    uint64_t id = lua_tonumber( l, 2 );
    dsim_table_index idx = dsim_table_find( tbl, id );

    enum dsim_type type = dsim_table_column_type( tbl, col );
    uint32_t size = dsim_table_column_size(tbl,col);
    char * data = (char*)dsim_table_data( tbl, idx.block, col ) + size * idx.index;

    dsim_lua_push( l, type, size, data );

    return 1;
}

static int lua_table_data( lua_State * l )
{
    const char * name = lua_tostring( l, 2 );
    if( strncmp( name, "set_", 4 ) == 0 )
    {
        name += 4;
        lua_pushstring( l, name );
        lua_pushcclosure( l, lua_table_set_by_id, 1 );
    }
    else
    {
        lua_pushstring( l, name );
        lua_pushcclosure( l, lua_table_get_by_id, 1 );
    }
    return 1;
}

static int lua_table_size( lua_State * l )
{
    struct dsim_table * tbl = *(struct dsim_table**)lua_touserdata( l, 1 );
    lua_pushnumber( l, dsim_table_block_size( tbl, 0) );
    return 1;
}

static int lua_table_get_id( lua_State * l )
{
    struct dsim_table * tbl = *(struct dsim_table**)lua_touserdata( l, 1 );
    uint32_t index = lua_tonumber( l, 2 );
    lua_pushnumber( l, dsim_table_id_data( tbl, 0 )[index-1] );
    return 1;
}

static void lua_push_table_meta( lua_State * l )
{
    if( luaL_newmetatable( l, "dsim_table") )
    {
        lua_createtable( l, 0, 2 );
        {
            lua_pushcfunction( l, lua_table_size );
            lua_setfield( l, -2, "count" );

            lua_pushcfunction( l, lua_table_get_id );
            lua_setfield( l, -2, "get_id" );

            lua_createtable( l, 0, 1 );
            {
                lua_pushcfunction( l, lua_table_data );
                lua_setfield( l, -2, "__index" );
            }
            lua_setmetatable( l, -2 );
        }
        lua_setfield( l, -2, "__index" );
    }
}

void dsim_lua_register_table( lua_State * l, struct dsim_table * tbl, const char * name )
{
    struct dsim_table** ud = (struct dsim_table**)lua_newuserdata( l, sizeof(struct dsim_table*) );
    *ud = tbl;

    lua_push_table_meta( l );
    lua_setmetatable( l, -2 );

    lua_setglobal( l, name );
}
