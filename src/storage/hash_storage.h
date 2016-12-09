
#pragma once

#include "storage.h"
#include "storage_block.h"
#include "containers/hash.h"

DSIM_BEGIN_HEADER

extern struct dsim_storage_operations dsim_hash_storage_ops;

struct dsim_hash_storage
{
    struct dsim_storage storage;
    struct dsim_hash ids;
    struct dsim_storage_block data;
};

#define dsim_hash_storage_static_init(array_count,arrays,alloc) { \
    /* .storage = */ dsim_storage_static_init(&dsim_hash_storage_ops, alloc), \
    /* .ids = */     dsim_hash_static_init(alloc), \
    /* .data = */    dsim_storage_block_static_init(array_count,arrays) }

DSIM_END_HEADER
