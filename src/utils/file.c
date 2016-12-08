
#include "file.h"
#include <stdio.h>

void dsim_read_file( const char * filename, struct dsim_array_char *buf )
{
    const int chunk_size = 4096;
    char chunk[chunk_size];

    FILE * f = fopen( filename, "rb" );
    int read_size;
    do
    {
        read_size = fread( chunk, 1, chunk_size, f );
        dsim_array_char_push_back_n( buf, chunk, read_size );
    } while( read_size == chunk_size );
    fclose( f );
}
