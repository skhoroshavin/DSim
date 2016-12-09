
#pragma once

#include "containers/array.h"
#include "reflection.h"

struct dsim_storage_array
{
    const char * name;
    enum dsim_type type;
    uint32_t size;
    struct _dsim_array data;
};

#define dsim_storage_array_static_init(name,type,size,alloc) { \
    /* .name = */      name, \
    /* .type = */      type, \
    /* .size = */      size, \
    /* .data = */      dsim_array_static_init(alloc) }

struct dsim_storage_block
{
    uint32_t array_count;
    struct dsim_storage_array *arrays;
};

#define dsim_storage_block_static_init(count,arrays) { \
    /* .count = */ count, \
    /* .arrays = */ arrays }

void dsim_storage_block_resize( struct dsim_storage_block *sb, uint32_t count );
void dsim_storage_block_remove_fast( struct dsim_storage_block *sb, uint32_t pos, uint32_t count );
void dsim_storage_block_reset( struct dsim_storage_block *sb );
