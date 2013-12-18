#include "utils.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>

void error(const char *format, ...)
{
   va_list params;
   va_start(params, format);
   vfprintf(stderr, format, params);
   va_end(params);
   fputc('\n', stderr);
   exit(1);
}

unsigned int randinto(unsigned int min, unsigned int max)
{
   return (unsigned int)(rand()/(double)RAND_MAX*((max+1)-min)+min);
}
