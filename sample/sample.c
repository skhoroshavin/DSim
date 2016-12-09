
#include "sample.h"
#include "sample.ddl.h"
#include "reflection/ddl_registry.h"

#include "utils/file.h"
#include "utils/log.h"

BEGIN_TABLE(wires)
    dsim_table_column_static_init( "state", DSIM_TYPE_INT, sizeof(uint8_t), &dsim_default_allocator ),
END_TABLE(wires)

inline static uint8_t * tbl_wires_state() { return (uint8_t*)dsim_table_data( tbl_wires, 0, 0 ); }

BEGIN_TABLE(logic)
    dsim_table_column_static_init( "input_1", DSIM_TYPE_INT, sizeof(uint64_t), &dsim_default_allocator ),
    dsim_table_column_static_init( "input_2", DSIM_TYPE_INT, sizeof(uint64_t), &dsim_default_allocator ),
    dsim_table_column_static_init( "output",  DSIM_TYPE_INT, sizeof(uint64_t), &dsim_default_allocator ),
END_TABLE(logic)

inline static uint64_t * tbl_logic_input_1() { return (uint64_t*)dsim_table_data( tbl_logic, 0, 0 ); }
inline static uint64_t * tbl_logic_input_2() { return (uint64_t*)dsim_table_data( tbl_logic, 0, 1 ); }
inline static uint64_t * tbl_logic_output()  { return (uint64_t*)dsim_table_data( tbl_logic, 0, 2 ); }

int main( int argc, const char * argv[] )
{
    dsim_unused(argc);
    dsim_unused(argv);

    dsim_ddl_registry_init();
    dsim_ddl_init_sample();

    lua_State * l = luaL_newstate();
    luaopen_base( l );

    dsim_lua_register_table( l, tbl_wires, "wires" );
    dsim_lua_register_table( l, tbl_logic, "logic" );

    dsim_table_insert( tbl_wires, 1, 16 );
    memset( tbl_wires_state(), 0, 16 );

    dsim_table_insert( tbl_logic, 1, 3 );
    tbl_logic_input_1()[0] = 1;
    tbl_logic_input_2()[0] = 2;
    tbl_logic_output() [0] = 3;

    tbl_logic_input_1()[1] = 4;
    tbl_logic_input_2()[1] = 5;
    tbl_logic_output() [1] = 6;

    tbl_logic_input_1()[2] = 3;
    tbl_logic_input_2()[2] = 6;
    tbl_logic_output() [2] = 7;

    if( luaL_dofile( l, "sample.lua" ) )
        printf( "Error: %s\n", lua_tostring( l, -1 ) );
    lua_close( l );

    dsim_table_reset( tbl_wires );
    dsim_table_reset( tbl_logic );

    dsim_ddl_registry_reset();

    return 0;
}
