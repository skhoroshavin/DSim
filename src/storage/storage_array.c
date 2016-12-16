
#include "storage_array.h"

void dsim_storage_array_init( struct dsim_storage_array *sa, dsim_ddl_type_table_t type, struct dsim_allocator *alloc )
{
    sa->type = type;
    _dsim_array_init( &sa->current, alloc );
    _dsim_array_init( &sa->next, alloc );
    sa->read_count = 0;
    sa->write_direct = 0;
    sa->write_buffered = 0;
}

int dsim_storage_array_can_modify( const struct dsim_storage_array *sa )
{
    return !(sa->read_count || sa->write_buffered || sa->write_direct);
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
    memcpy( (char *)&sa->current.data + dst_offset*elem_size,
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

const void *dsim_storage_array_read_begin( struct dsim_storage_array *sa )
{
    if( sa->write_direct )
        return 0;

    ++sa->read_count;
    return sa->current.data;
}

void dsim_storage_array_read_end( struct dsim_storage_array *sa )
{
    assert( sa->read_count );
    --sa->read_count;
}

void *dsim_storage_array_write_direct_begin( struct dsim_storage_array *sa )
{
    if( !dsim_storage_array_can_modify(sa) )
        return 0;

    sa->write_direct = 1;
    return sa->current.data;
}

void dsim_storage_array_write_direct_end( struct dsim_storage_array *sa )
{
    assert( sa->write_direct );
    sa->write_direct = 0;
}

void *dsim_storage_array_write_buffered_begin( struct dsim_storage_array *sa )
{
    if( sa->write_buffered || sa->write_direct )
        return 0;

    sa->write_buffered = 1;
    if( sa->next.count != sa->current.count )
    {
        _dsim_array_reset( &sa->next );
        _dsim_array_resize( &sa->next, sa->current.count, dsim_ddl_type_size(sa->type) );
    }
    return sa->next.data;
}

void dsim_storage_array_write_buffered_end( struct dsim_storage_array *sa )
{
    assert( sa->write_buffered );

    void *tmp = sa->current.data;
    sa->current.data = sa->next.data;
    sa->next.data = tmp;
    sa->write_buffered = 0;
}

