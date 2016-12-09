
#include "sample.h"
#include "sample.ddl.h"
#include "reflection/ddl_registry.h"

#include "utils/file.h"
#include "utils/log.h"

int main( int argc, const char * argv[] )
{
    dsim_unused(argc);
    dsim_unused(argv);

    dsim_ddl_init_sample();

    lua_State * l = luaL_newstate();
    luaopen_base( l );

    dsim_lua_register_storage( l, wires, "wires" );
    dsim_lua_register_storage( l, logic, "logic" );
    dsim_lua_register_storage( l, logic, "delay" );

    dsim_storage_insert( wires, 1, 16 );
    memset( wire_state_data( wires, 0 ), 0, 16 );

    dsim_storage_insert( logic, 1, 3 );
    logic_input_1_data( logic, 0 )[0].id = 1;
    logic_input_2_data( logic, 0 )[0].id = 2;
    logic_output_data( logic, 0 ) [0].id = 3;

    logic_input_1_data( logic, 0 )[1].id = 4;
    logic_input_2_data( logic, 0 )[1].id = 5;
    logic_output_data( logic, 0 ) [1].id = 6;

    logic_input_1_data( logic, 0 )[2].id = 3;
    logic_input_2_data( logic, 0 )[2].id = 6;
    logic_output_data( logic, 0 ) [2].id = 7;

    if( luaL_dofile( l, "sample.lua" ) )
        printf( "Error: %s\n", lua_tostring( l, -1 ) );
    lua_close( l );

    dsim_ddl_done_sample();

    return 0;
}
