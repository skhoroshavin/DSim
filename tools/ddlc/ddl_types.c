
#include "ddl_types.h"
#include "containers/array.h"
#include "reflection/ddl_builder.h"
#include "reflection/ddl_registry.h"

#include <stdio.h>

static void rebuild_numeric_type( struct flatcc_builder * b, dsim_type_table_t type )
{
    dsim_numeric_type_table_t ntype = (dsim_numeric_type_table_t)dsim_type_data( type );

    if( !dsim_type_size_is_present(type) )
    {
        printf( "Numeric type %s must have size!\n", dsim_type_name(type) );
        abort();
    }
    int size = dsim_type_size(type);
    dsim_type_size_add( b, size );

    int align = dsim_type_align(type);
    if( !dsim_type_align_is_present(type) )
        align = min( 16, dsim_next_pow_2(size) );
    dsim_type_align_add( b, align );

    dsim_type_data_numeric_type_start( b );
    dsim_numeric_type_is_float_add( b, dsim_numeric_type_is_float(ntype) );

    dsim_type_data_numeric_type_end( b );
}

static void rebuild_struct_type( struct flatcc_builder * b, dsim_type_table_t type )
{
    dsim_struct_type_table_t stype = (dsim_struct_type_table_t)dsim_type_data( type );

    dsim_type_data_struct_type_start( b );

    int offset = 0;
    int align = 0;
    dsim_struct_field_vec_t fields = dsim_struct_type_fields( stype );
    dsim_struct_type_fields_start( b );
    for( size_t i = 0; i != dsim_struct_field_vec_len(fields); ++i )
    {
        dsim_struct_type_fields_push_start( b );

        dsim_struct_field_table_t field = dsim_struct_field_vec_at(fields,i);
        dsim_struct_field_name_clone( b, dsim_struct_field_name(field) );
        dsim_struct_field_type_clone( b, dsim_struct_field_type(field) );

        if( dsim_struct_field_offset_is_present(field) )
            offset = dsim_struct_field_offset(field);
        dsim_struct_field_offset_add( b, offset );

        dsim_type_table_t type = dsim_type( dsim_struct_field_type(field) );

        offset += dsim_type_size(type);
        align = max( align, dsim_type_align(type) );

        dsim_struct_type_fields_push_end( b );
    }
    dsim_struct_type_fields_end( b );

    int size = offset;
    while( size & align )
        ++size;

    dsim_type_data_struct_type_end( b );

    if( dsim_type_size_is_present(type) )
        size = dsim_type_size(type);

    if( dsim_type_align_is_present(type) )
        align = dsim_type_align(type);

    dsim_type_size_add( b, size );
    dsim_type_align_add( b, align );
}

static void rebuild_enum_type( struct flatcc_builder * b, dsim_type_table_t type )
{
    dsim_enum_type_table_t etype = (dsim_enum_type_table_t)dsim_type_data( type );

    dsim_type_size_add( b, 4 );
    dsim_type_align_add( b, 4 );

    dsim_type_data_enum_type_start( b );

    flatbuffers_string_vec_t values = dsim_enum_type_values( etype );
    dsim_enum_type_values_start( b );
    for( size_t i = 0; i != flatbuffers_string_vec_len(values); ++i )
        dsim_enum_type_values_push_clone( b, flatbuffers_string_vec_at(values, i) );
    dsim_enum_type_values_end( b );

    dsim_type_data_enum_type_end( b );
}

static void rebuild_reference_type( struct flatcc_builder * b, dsim_type_table_t type )
{
    dsim_reference_type_table_t rtype = (dsim_reference_type_table_t)dsim_type_data( type );

    dsim_type_size_add( b, 8 );
    dsim_type_align_add( b, 8 );

    dsim_type_data_reference_type_start( b );
    dsim_reference_type_target_clone( b, dsim_reference_type_target(rtype) );
    dsim_type_data_reference_type_end( b );
}

void dsim_rebuild_type( struct flatcc_builder * b, dsim_type_table_t type )
{
    dsim_type_name_clone( b, dsim_type_name(type) );
    if( dsim_type_ctype_is_present(type) )
        dsim_type_ctype_clone( b, dsim_type_ctype(type) );

    switch( dsim_type_data_type(type) )
    {
    case dsim_any_type_numeric_type:
        rebuild_numeric_type( b, type );
        break;
    case dsim_any_type_struct_type:
        rebuild_struct_type( b, type );
        break;
    case dsim_any_type_enum_type:
        rebuild_enum_type( b, type );
        break;
    case dsim_any_type_reference_type:
        rebuild_reference_type( b, type );
        break;
    }
}
