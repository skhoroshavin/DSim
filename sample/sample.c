
#include "sample.h"
#include "sample.ddl.h"
#include "reflection/ddl_registry.h"
#include "utils/file.h"

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

static void dump_ddl( dsim_ddl_table_t ddl )
{
    printf( "Types:\n" );
    dsim_type_vec_t types = dsim_ddl_types( ddl );
    for( size_t i = 0; i != dsim_type_vec_len(types); ++i )
    {
        dsim_type_table_t type = dsim_type_vec_at(types, i);
        printf( "  %s, %s, %s, size %d, align %d\n",
                dsim_type_name(type),
                dsim_type_ctype(type),
                dsim_any_type_type_name( dsim_type_data_type(type) ),
                dsim_type_size(type),
                dsim_type_align(type) );
        switch( dsim_type_data_type(type) )
        {
        case dsim_any_type_numeric_type:
        {
            dsim_numeric_type_table_t ntype = (dsim_numeric_type_table_t)dsim_type_data( type );
            if( dsim_numeric_type_is_float(ntype) )
                printf( "    Floating point\n" );
            else
                printf( "    Integer\n" );
            break;
        }
        case dsim_any_type_struct_type:
        {
            dsim_struct_type_table_t stype = (dsim_struct_type_table_t)dsim_type_data( type );
            dsim_struct_field_vec_t fields = dsim_struct_type_fields( stype );
            for( size_t j = 0; j != dsim_struct_field_vec_len(fields); ++j )
            {
                dsim_struct_field_table_t field = dsim_struct_field_vec_at(fields,j);
                printf( "    %s: %s, offset %d\n",
                        dsim_struct_field_name(field),
                        dsim_struct_field_type(field),
                        dsim_struct_field_offset(field) );
            }
            break;
        }
        case dsim_any_type_enum_type:
        {
            dsim_enum_type_table_t stype = (dsim_enum_type_table_t)dsim_type_data( type );
            flatbuffers_string_vec_t values = dsim_enum_type_values( stype );
            for( size_t j = 0; j != flatbuffers_string_vec_len(values); ++j )
                printf( "    %s\n", flatbuffers_string_vec_at(values, j) );
            break;
        }
        case dsim_any_type_reference_type:
        {
            dsim_reference_type_table_t rtype = (dsim_reference_type_table_t)dsim_type_data( type );
            printf( "    Target: %s\n", dsim_reference_type_target(rtype) );
            break;
        }
        }
    }

    printf( "Layouts:\n" );
    dsim_layout_vec_t layouts = dsim_ddl_layouts( ddl );
    for( size_t i = 0; i != dsim_layout_vec_len(layouts); ++i )
    {
        dsim_layout_table_t layout = dsim_layout_vec_at(layouts, i);
        printf( "  %s\n", dsim_layout_name(layout) );
        dsim_column_vec_t columns = dsim_layout_columns(layout);
        for( size_t j = 0; j != dsim_column_vec_len(columns); ++j )
        {
            dsim_column_table_t column = dsim_column_vec_at(columns, j);
            printf( "    %s: %s\n",
                    dsim_column_name(column),
                    dsim_column_type(column) );
        }

    }
}

int main( int argc, const char * argv[] )
{
    dsim_unused(argc);
    dsim_unused(argv);

    dump_ddl( dsim_ddl_as_root(dsim_ddl_sample_data) );
    dsim_ddl_register_sample();

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
