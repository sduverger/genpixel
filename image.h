#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <stdint.h>
#include "equation.h"

#define IMG_N  6
#define IMG_W  200
#define IMG_H  200

#define build_channel(name, type) 			\
   typedef union __##name  {				\
      struct { type r,g,b; } __attribute__((packed));	\
      type chan[3];					\
   } name

build_channel(pixel_t, uint8_t);
build_channel(eval_t, double);
build_channel(equations_t, equ_t);

typedef struct image
{
   union
   {
      pixel_t pixel_at[IMG_H][IMG_W];
      pixel_t pixels[IMG_W*IMG_H];
   };

   union
   {
      eval_t eval_at[IMG_H][IMG_W];
      eval_t evals[IMG_W*IMG_H];
   };

   equations_t  equ;
   void        *gui;

} image_t;

void image_new(int);

#endif
