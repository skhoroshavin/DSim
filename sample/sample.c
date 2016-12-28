
#include "sample.h"
#include "sample.ddl.h"
#include "storage/ddl_registry.h"

#include "core/file.h"
#include "core/log.h"

int main( int argc, const char *argv[] )
{
    dsim_unused(argc);
    dsim_unused(argv);

    dsim_ddl_init_sample();

    lua_State *l = luaL_newstate();
    luaopen_base( l );

    dsim_lua_register_storage( l, ddl_sample->storage_wires, "wires" );
    dsim_lua_register_storage( l, ddl_sample->storage_logic, "logic" );
    dsim_lua_register_storage( l, ddl_sample->storage_logic, "delay" );

    const uint64_t wire_ids[]    = { 1, 2, 3, 4, 5, 6, 7, 8 };
    const uint8_t  wire_states[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    sample_wires_insert( wire_ids, wire_states, count_of(wire_ids) );

    const uint64_t logic_ids[] = { 1, 2, 3 };
    const wire_ref logic_in1[] = { {1}, {2}, {3} };
    const wire_ref logic_in2[] = { {4}, {5}, {6} };
    const wire_ref logic_out[] = { {3}, {6}, {7} };
    sample_logic_insert( logic_ids, logic_in1, logic_in2, logic_out, 0, count_of(logic_ids) );

    if( luaL_dofile( l, "sample.lua" ) )
        printf( "Error: %s\n", lua_tostring( l, -1 ) );
    lua_close( l );

    dsim_ddl_done_sample();

    return 0;
}
