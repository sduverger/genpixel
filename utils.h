#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdint.h>

void error(const char*, ...);

unsigned int randinto(unsigned int, unsigned int);

static inline unsigned int randtwo()
{
   return (unsigned int)(rand()&1);
}

#endif
