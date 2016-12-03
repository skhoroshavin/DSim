
#include "sample.h"

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

void * dsim_load_config( const char * filename )
{
    FILE * f = fopen( filename, "r" );
    char json_buf[4096];
    size_t json_size = fread( json_buf, 1, sizeof(json_buf), f );
    fclose( f );

    struct flatcc_builder builder;
    flatcc_builder_init( &builder );

    struct flatcc_json_parser_ctx ctx;

    int err = config_parse_json( &builder, &ctx, json_buf, json_size, 0 );
    if( err )
    {
        printf( "Line %d: %s\n", ctx.line, flatcc_json_parser_error_string(err) );
        flatcc_builder_clear( &builder );
        return 0;
    }

    void *buf = flatcc_builder_finalize_buffer( &builder, 0 );
    flatcc_builder_clear( &builder );
    return buf;
}

void test()
{
    void * buf = dsim_load_config( "sample.json" );
    if( !buf ) return;

    dsimx_config_table_t config = dsimx_config_as_root( buf );
    dsimx_schema_vec_t schemas = dsimx_config_schemas( config );
    int scount = dsimx_schema_vec_len( schemas );
    for( int i = 0; i != scount; ++i )
    {
        dsimx_schema_table_t schema = dsimx_schema_vec_at( schemas, i );
        printf( "%s\n", dsimx_schema_name( schema) );

        dsimx_column_vec_t columns = dsimx_schema_columns( schema );
        int ccount = dsimx_column_vec_len( columns );
        for( int j = 0; j != ccount; ++j )
        {
            dsimx_column_table_t column = dsimx_column_vec_at( columns, j );
            printf( "  %s: %s size %d\n",
                    dsimx_column_name( column ),
                    dsimx_type_name( dsimx_column_type( column ) ),
                    dsimx_column_size( column ) );
        }
        printf( "\n" );
    }

    free( buf );
}


int main( int argc, const char * argv[] )
{
    dsim_unused(argc);
    dsim_unused(argv);

    test();

    lua_State * l = luaL_newstate();
    luaopen_base( l );

    dsim_lua_register_table( l, tbl_wires,  "wires" );
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

    return 0;
}
