
#include "common.h"

uint64_t dsim_hash64( uint64_t key )
{
    key ^= (key >> 30);
    key *= 0xbf58476d1ce4e5b9;
    key ^= (key >> 27);
    key *= 0x94d049bb133111eb;
    key ^= (key >> 31);
    return key;
}

size_t dsim_next_pow_2( size_t value )
{
    value--;
    value |= value >> 1;
    value |= value >> 2;
    value |= value >> 4;
    value |= value >> 8;
    value |= value >> 16;
    value |= value >> 32;
    value++;
    return value;
}
