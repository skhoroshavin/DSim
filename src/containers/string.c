
#include "string.h"
#include <memory.h>

static void dsim_string_grow( struct dsim_string *s, uint32_t count )
{
    uint32_t capacity = s->size + count + 1;
    capacity = dsim_next_pow_2( capacity ) - 1;
    dsim_string_reserve( s, capacity );
}

void dsim_string_reserve( struct dsim_string *s, uint32_t count )
{
    if( s->capacity > count ) return;
    s->data = (char*)dsim_reallocate( s->_alloc, s->data, s->capacity, count+1 );
    s->capacity = count+1;
}

void dsim_string_append( struct dsim_string *s, const char * data )
{
    size_t count = strlen(data);
    if( s->size + count + 1 > s->capacity )
        dsim_string_grow( s, count );
    memcpy( s->data + s->size, data, count+1 );
    s->size += count;
}

void dsim_string_remove( struct dsim_string *s, uint32_t pos, uint32_t count )
{
    assert( s->size >= pos + count ); // LCOV_EXCL_BR_LINE
    uint32_t move_count = s->size + 1 - pos - count;
    memmove( s->data + pos, s->data + pos + count, move_count );
    s->size -= count;
}

void dsim_string_clear( struct dsim_string *s )
{
    s->size = 0;
    if( s->data )
        *s->data = 0;
}

void dsim_string_reset( struct dsim_string *s )
{
    if( s->data )
    {
        dsim_deallocate( s->_alloc, s->data, s->capacity );
        s->data = 0;
        s->size = 0;
        s->capacity = 0;
    }
}


