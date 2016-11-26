
#include "array.h"
#include <memory.h>

void _dsim_array_reserve( struct _dsim_array *a, uint32_t capacity, uint32_t elem_size )
{
    if( a->capacity >= capacity ) return;
    a->data = dsim_reallocate( a->_alloc, a->data, a->capacity*elem_size, capacity*elem_size );
    a->capacity = capacity;
}

void _dsim_array_grow( struct _dsim_array *a, uint32_t count, uint32_t elem_size )
{
    uint32_t capacity = a->count + count;
    capacity = dsim_next_pow_2( capacity*elem_size ) / elem_size;
    _dsim_array_reserve( a, capacity, elem_size );
}

void _dsim_array_resize( struct _dsim_array *a, uint32_t count, uint32_t elem_size )
{
    if( a->capacity < count )
        _dsim_array_reserve( a, count, elem_size );
    a->count = count;
}

void _dsim_array_fill( struct _dsim_array *a, void* value, uint32_t elem_size )
{
    if( !a->data ) return;
    for( uint32_t i = 0; i < a->count; ++i )
        memcpy( (char*)a->data + i*elem_size, value, elem_size );
}

void _dsim_array_push_back( struct _dsim_array *a, const void *data, uint32_t count, uint32_t elem_size )
{
    if( a->count + count > a->capacity )
        _dsim_array_grow( a, count, elem_size );
    memcpy( (char*)a->data + a->count*elem_size, data, count*elem_size );
    a->count += count;
}

void _dsim_array_remove( struct _dsim_array *a, uint32_t pos, uint32_t count, uint32_t elem_size )
{
    uint32_t move_count = a->count - pos - count;
    if( move_count )
        memmove( (char*)a->data + pos*elem_size, (char*)a->data + (pos + count)*elem_size, move_count*elem_size );
    a->count -= count;
}

void _dsim_array_remove_fast( struct _dsim_array *a, uint32_t pos, uint32_t count, uint32_t elem_size )
{
    if( a->count - pos - count < count )
    {
        _dsim_array_remove( a, pos, count, elem_size );
        return;
    }
    memcpy( (char*)a->data + pos*elem_size, (char*)a->data + (a->count - count)*elem_size, count*elem_size );
    a->count -= count;
}

void _dsim_array_reset( struct _dsim_array *a, uint32_t elem_size )
{
    if( a->data )
    {
        dsim_deallocate( a->_alloc, a->data, a->capacity*elem_size );
        a->data = 0;
        a->count = 0;
        a->capacity = 0;
    }
}
