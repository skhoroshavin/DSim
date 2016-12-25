
#include "storage_array.h"

void dsim_storage_array_init( struct dsim_storage_array *sa, dsim_ddl_type_table_t type, struct dsim_allocator *alloc )
{
    sa->type = type;
    _dsim_array_init( &sa->current, alloc );
    _dsim_array_init( &sa->next, alloc );
    sa->current_read_count = 0;
    sa->next_read_count = 0;
    sa->write_mode = DSIM_WRITE_NONE;
}

int dsim_storage_array_can_modify( const struct dsim_storage_array *sa )
{
    return !(sa->current_read_count || sa->next_read_count || sa->write_mode);
}

void dsim_storage_array_resize( struct dsim_storage_array *sa, uint32_t count )
{
    assert( dsim_storage_array_can_modify(sa) );

    _dsim_array_resize( &sa->current, count, dsim_ddl_type_size(sa->type) );
}

void dsim_storage_array_push_back( struct dsim_storage_array *sa, const void *data, uint32_t count )
{
    assert( dsim_storage_array_can_modify(sa) );

    _dsim_array_push_back( &sa->current, data, count, dsim_ddl_type_size(sa->type) );
}

void dsim_storage_array_update( struct dsim_storage_array *sa, const void *data, uint32_t src_offset, uint32_t dst_offset, uint32_t count )
{
    assert( dsim_storage_array_can_modify(sa) );

    size_t elem_size = dsim_ddl_type_size(sa->type);
    memcpy( (char *)sa->current.data + dst_offset*elem_size,
            (const char *)data + src_offset*elem_size,
            count*elem_size );
}

void dsim_storage_array_remove_fast( struct dsim_storage_array *sa, uint32_t pos, uint32_t count )
{
    assert( dsim_storage_array_can_modify(sa) );

    _dsim_array_remove_fast( &sa->current, pos, count, dsim_ddl_type_size(sa->type) );
}

void dsim_storage_array_reset( struct dsim_storage_array *sa )
{
    assert( dsim_storage_array_can_modify(sa) );

    _dsim_array_reset( &sa->current );
    _dsim_array_reset( &sa->next );
}

const void *dsim_storage_array_begin_read( struct dsim_storage_array *sa )
{
    if( sa->write_mode == DSIM_WRITE_DIRECT )
        return 0;

    ++sa->current_read_count;
    return sa->current.data;
}

int dsim_storage_array_end_read( struct dsim_storage_array *sa, const void *data )
{
    if( data == sa->current.data )
    {
        assert( sa->current_read_count );
        --sa->current_read_count;
        return 1;
    }

    if( data == sa->next.data )
    {
        assert( sa->next_read_count );
        --sa->next_read_count;
        return 1;
    }

    return 0;
}

void *dsim_storage_array_begin_write( struct dsim_storage_array *sa, enum dsim_storage_write_mode mode )
{
    if( mode == DSIM_WRITE_NONE )
        return 0;

    if( mode == DSIM_WRITE_DIRECT )
    {
        if( !dsim_storage_array_can_modify(sa) )
            return 0;
        sa->write_mode = mode;
        return sa->current.data;
    }

    if( mode == DSIM_WRITE_BUFFERED )
    {
        if( (sa->write_mode) || (sa->next_read_count) )
            return 0;
        sa->write_mode = mode;
        if( sa->next.count != sa->current.count )
        {
            _dsim_array_reset( &sa->next );
            _dsim_array_resize( &sa->next, sa->current.count, dsim_ddl_type_size(sa->type) );
        }
        return sa->next.data;
    }

    return 0;
}

int dsim_storage_array_end_write( struct dsim_storage_array *sa, void *data )
{
    if( data == sa->current.data )
    {
        assert( sa->write_mode == DSIM_WRITE_DIRECT );
        sa->write_mode = DSIM_WRITE_NONE;
        return 1;
    }

    if( data == sa->next.data )
    {
        assert( sa->write_mode == DSIM_WRITE_BUFFERED );
        sa->write_mode = DSIM_WRITE_NONE;

        void *tmp_data = sa->current.data;
        sa->current.data = sa->next.data;
        sa->next.data = tmp_data;

        uint32_t tmp_count = sa->current_read_count;
        sa->current_read_count = sa->next_read_count;
        sa->next_read_count = tmp_count;

        return 1;
    }

    return 0;
}
