
#include "log.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void dsim_info( const char *fmt, ... )
{
    va_list args;
    va_start (args, fmt);
    printf( "(II) " );
    vprintf( fmt, args );
    printf( "\e[0m\n" );
    va_end (args);
}

void dsim_warning( const char *fmt, ... )
{
    va_list args;
    va_start (args, fmt);
    printf( "\e[33m(WW) " );
    vprintf( fmt, args );
    printf( "\e[0m\n" );
    va_end (args);
}

void dsim_error( const char *fmt, ... )
{
    va_list args;
    va_start (args, fmt);
    printf( "\e[91m(EE) " );
    vprintf( fmt, args );
    printf( "\e[0m\n" );
    va_end (args);
}

void dsim_fatal( const char *fmt, ... )
{
    va_list args;
    va_start (args, fmt);
    printf( "\e[101m(FATAL) " );
    vprintf( fmt, args );
    printf( "\e[0m\n" );
    va_end (args);

    abort();
}
