
#include "sample.h"
#include "sample.ddl.h"
#include "reflection/ddl_registry.h"

#include "utils/file.h"
#include "utils/log.h"

BEGIN_STORAGE(wires)
    dsim_storage_array_static_init( "state", DSIM_TYPE_INT, sizeof(uint8_t), &dsim_default_allocator ),
END_STORAGE(wires)

inline static uint8_t * strg_wires_state() { return (uint8_t*)dsim_storage_data( strg_wires, 0, 0 ); }

BEGIN_STORAGE(logic)
    dsim_storage_array_static_init( "input_1", DSIM_TYPE_INT, sizeof(uint64_t), &dsim_default_allocator ),
    dsim_storage_array_static_init( "input_2", DSIM_TYPE_INT, sizeof(uint64_t), &dsim_default_allocator ),
    dsim_storage_array_static_init( "output",  DSIM_TYPE_INT, sizeof(uint64_t), &dsim_default_allocator ),
END_STORAGE(logic)

inline static uint64_t * strg_logic_input_1() { return (uint64_t*)dsim_storage_data( strg_logic, 0, 0 ); }
inline static uint64_t * strg_logic_input_2() { return (uint64_t*)dsim_storage_data( strg_logic, 0, 1 ); }
inline static uint64_t * strg_logic_output()  { return (uint64_t*)dsim_storage_data( strg_logic, 0, 2 ); }

int main( int argc, const char * argv[] )
{
    dsim_unused(argc);
    dsim_unused(argv);

    dsim_ddl_registry_init();
    dsim_ddl_init_sample();

    lua_State * l = luaL_newstate();
    luaopen_base( l );

    dsim_lua_register_storage( l, strg_wires, "wires" );
    dsim_lua_register_storage( l, strg_logic, "logic" );

    dsim_storage_insert( strg_wires, 1, 16 );
    memset( strg_wires_state(), 0, 16 );

    dsim_storage_insert( strg_logic, 1, 3 );
    strg_logic_input_1()[0] = 1;
    strg_logic_input_2()[0] = 2;
    strg_logic_output() [0] = 3;

    strg_logic_input_1()[1] = 4;
    strg_logic_input_2()[1] = 5;
    strg_logic_output() [1] = 6;

    strg_logic_input_1()[2] = 3;
    strg_logic_input_2()[2] = 6;
    strg_logic_output() [2] = 7;

    if( luaL_dofile( l, "sample.lua" ) )
        printf( "Error: %s\n", lua_tostring( l, -1 ) );
    lua_close( l );

    dsim_storage_done( strg_wires );
    dsim_storage_done( strg_logic );

    dsim_ddl_registry_reset();

    return 0;
}
