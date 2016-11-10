
#include "array.h"
#include <memory.h>

static dsim_index_t _dsim_array_grow( dsim_index_t new_size, dsim_index_t elem_size )
{
    return dsim_next_pow_2( new_size*elem_size ) / elem_size;
}

void _dsim_array_reserve( _dsim_array *a, dsim_index_t capacity, dsim_index_t elem_size )
{
    if( a->capacity >= capacity ) return;
    a->data = dsim_reallocate( a->alloc, a->data, a->capacity*elem_size, capacity*elem_size, a->count*elem_size );
    a->capacity = capacity;
}

void _dsim_array_resize( _dsim_array *a, dsim_index_t count, dsim_index_t elem_size )
{
    if( a->capacity < count )
        _dsim_array_reserve( a, count, elem_size );
    a->count = count;
}

void _dsim_array_fill( _dsim_array *a, void* value, dsim_index_t elem_size )
{
    if( !a->data ) return;
    for( dsim_index_t i = 0; i < a->count; ++i )
        memcpy( (char*)a->data + i*elem_size, value, elem_size );
}

void _dsim_array_push_back( _dsim_array *a, const void *data, dsim_index_t count, dsim_index_t elem_size )
{
    if( a->capacity < a->count + count )
        _dsim_array_reserve( a, _dsim_array_grow( a->count + count, elem_size ), elem_size );
    memcpy( (char*)a->data + a->count*elem_size, data, count*elem_size );
    a->count += count;
}

void _dsim_array_reset( _dsim_array *a, dsim_index_t elem_size )
{
    if( a->data )
    {
        dsim_deallocate( a->alloc, a->data, a->capacity*elem_size );
        a->data = 0;
        a->count = 0;
        a->capacity = 0;
    }
}
