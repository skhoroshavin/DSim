
#include "storage_array.h"

void dsim_storage_array_init( struct dsim_storage_array *sa, dsim_ddl_type_table_t type, struct dsim_allocator *alloc )
{
    _dsim_array_init( &sa->main, alloc );
    _dsim_array_init( &sa->back, alloc );
    sa->type = type;
    sa->main_read_count = 0;
    sa->back_read_count = 0;
    sa->write_mode = DSIM_STORAGE_WRITE_NONE;
}

int dsim_storage_array_can_modify( const struct dsim_storage_array *sa )
{
    return !(sa->main_read_count || sa->back_read_count || sa->write_mode);
}

void dsim_storage_array_resize( struct dsim_storage_array *sa, uint32_t count )
{
    assert( dsim_storage_array_can_modify(sa) );

    _dsim_array_resize( &sa->main, count, dsim_ddl_type_size(sa->type) );
}

void dsim_storage_array_push_back( struct dsim_storage_array *sa, const void *data, uint32_t count )
{
    assert( dsim_storage_array_can_modify(sa) );

    _dsim_array_push_back( &sa->main, data, count, dsim_ddl_type_size(sa->type) );
}

void dsim_storage_array_update( struct dsim_storage_array *sa, const void *data, uint32_t src_offset, uint32_t dst_offset, uint32_t count )
{
    assert( dsim_storage_array_can_modify(sa) );

    size_t elem_size = dsim_ddl_type_size(sa->type);
    memcpy( (char *)sa->main.data + dst_offset*elem_size,
            (const char *)data + src_offset*elem_size,
            count*elem_size );
}

void dsim_storage_array_remove_fast( struct dsim_storage_array *sa, uint32_t pos, uint32_t count )
{
    assert( dsim_storage_array_can_modify(sa) );

    _dsim_array_remove_fast( &sa->main, pos, count, dsim_ddl_type_size(sa->type) );
}

void dsim_storage_array_reset( struct dsim_storage_array *sa )
{
    assert( dsim_storage_array_can_modify(sa) );

    _dsim_array_reset( &sa->main );
    _dsim_array_reset( &sa->back );
}

const void *dsim_storage_array_begin_read( struct dsim_storage_array *sa )
{
    if( sa->write_mode == DSIM_STORAGE_WRITE_DIRECT )
        return 0;

    ++sa->main_read_count;
    return sa->main.data;
}

unsigned dsim_storage_array_end_read( struct dsim_storage_array *sa, const void *data )
{
    if( data == sa->main.data )
    {
        assert( sa->main_read_count );
        --sa->main_read_count;
        return 0;
    }

    if( data == sa->back.data )
    {
        assert( sa->back_read_count );
        --sa->back_read_count;
        return 0;
    }

    return DSIM_INVALID_INDEX;
}

void *dsim_storage_array_begin_write( struct dsim_storage_array *sa, enum dsim_storage_write_mode mode )
{
    assert( mode != DSIM_STORAGE_WRITE_NONE );

    if( mode == DSIM_STORAGE_WRITE_DIRECT )
    {
        if( !dsim_storage_array_can_modify(sa) )
            return 0;
        sa->write_mode = mode;
        return sa->main.data;
    }

    if( mode == DSIM_STORAGE_WRITE_BUFFERED )
    {
        if( (sa->write_mode) || (sa->back_read_count) )
            return 0;
        sa->write_mode = mode;
        if( sa->back.count != sa->main.count )
        {
            _dsim_array_reset( &sa->back );
            _dsim_array_resize( &sa->back, sa->main.count, dsim_ddl_type_size(sa->type) );
        }
        return sa->back.data;
    }

    return 0;
}

unsigned dsim_storage_array_end_write( struct dsim_storage_array *sa, void *data )
{
    if( data == sa->main.data )
    {
        assert( sa->write_mode == DSIM_STORAGE_WRITE_DIRECT );
        sa->write_mode = DSIM_STORAGE_WRITE_NONE;
        return 0;
    }

    if( data == sa->back.data )
    {
        assert( sa->write_mode == DSIM_STORAGE_WRITE_BUFFERED );
        sa->write_mode = DSIM_STORAGE_WRITE_NONE;

        void *tmp_data = sa->main.data;
        sa->main.data = sa->back.data;
        sa->back.data = tmp_data;

        uint32_t tmp_count = sa->main_read_count;
        sa->main_read_count = sa->back_read_count;
        sa->back_read_count = tmp_count;

        return 0;
    }

    return DSIM_INVALID_INDEX;
}
