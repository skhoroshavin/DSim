
#include "ddl_types.h"
#include "containers/array.h"
#include "reflection/ddl_builder.h"

#include <stdio.h>

DSIM_DEFINE_ARRAY(dsim_type_table_t, type)

static struct dsim_array_type types = dsim_array_static_init(&dsim_default_allocator);

void dsim_register_type( dsim_type_table_t type )
{
    dsim_array_type_push_back( &types, type );
}

dsim_type_table_t dsim_type( const char * str )
{
    for( uint32_t i = 0; i != types.count; ++i )
    {
        dsim_type_table_t type = types.at[i];
        if( strcmp( dsim_type_name(type), str ) == 0  )
            return type;
    }
    return 0;
}

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

int struct_size( dsim_type_table_t type )
{
    if( dsim_type_size_is_present(type) )
        return dsim_type_size(type);

    int result = 0;

    dsim_struct_type_table_t stype = (dsim_struct_type_table_t)dsim_type_data( type );
    dsim_struct_field_vec_t fields = dsim_struct_type_fields( stype );
    for( size_t i = 0; i != dsim_struct_field_vec_len(fields); ++i )
    {
        dsim_struct_field_table_t field = dsim_struct_field_vec_at(fields,i);
        dsim_type_table_t ftype = dsim_type( dsim_struct_field_type(field) );
        result += dsim_type_size(ftype);
    }

    return result;
}

int struct_align( dsim_type_table_t type )
{
    if( dsim_type_size_is_present(type) )
        return dsim_type_size(type);

    int result = 0;

    dsim_struct_type_table_t stype = (dsim_struct_type_table_t)dsim_type_data( type );
    dsim_struct_field_vec_t fields = dsim_struct_type_fields( stype );
    for( size_t i = 0; i != dsim_struct_field_vec_len(fields); ++i )
    {
        dsim_struct_field_table_t field = dsim_struct_field_vec_at(fields,i);
        dsim_type_table_t ftype = dsim_type( dsim_struct_field_type(field) );
        result = max( result, dsim_type_align(ftype) );
    }

    return result;
}

static void rebuild_struct_type( struct flatcc_builder * b, dsim_type_table_t type )
{
    dsim_struct_type_table_t stype = (dsim_struct_type_table_t)dsim_type_data( type );

    dsim_type_size_add( b, struct_size(type) );
    dsim_type_align_add( b, struct_align(type) );

    dsim_type_data_struct_type_start( b );

    int offset = 0;
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

        offset += dsim_type_size( dsim_type( dsim_struct_field_type(field) ) );

        dsim_struct_type_fields_push_end( b );
    }
    dsim_struct_type_fields_end( b );

    dsim_type_data_struct_type_end( b );
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

int dsim_can_rebuild_type( dsim_type_table_t type )
{
    if( dsim_type_data_type( type ) == dsim_any_type_struct_type )
    {
        dsim_struct_type_table_t stype = (dsim_struct_type_table_t)dsim_type_data( type );
        dsim_struct_field_vec_t fields = dsim_struct_type_fields(stype);

        for( size_t i = 0; i != dsim_struct_field_vec_len(fields); ++i )
        {
            dsim_struct_field_table_t field = dsim_struct_field_vec_at(fields, i);
            if( !dsim_type( dsim_struct_field_type(field) ) )
                return 0;
        }

        return 1;
    }

    return 1;
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
