#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <stdint.h>
#include <time.h>
#include "equation.h"

#define IMG_N  6
#define IMG_W  200
#define IMG_H  200

#define IMG_M_256  0
#define IMG_M_LIN  1

#define IMG_R_RGB  0
#define IMG_R_SR   1
#define IMG_R_SG   2
#define IMG_R_SB   3
#define IMG_R_CR   4
#define IMG_R_CG   5
#define IMG_R_CB   6

#define build_channel(name, type) 			\
   typedef union __##name  {				\
      struct { type r,g,b; } __attribute__((packed));	\
      type chan[3];					\
   } name

build_channel(pixel_t, uint8_t);
build_channel(eval_t, double);
build_channel(equations_t, equ_t);

typedef uint8_t (*modifier_t)(equ_t*, double);

typedef struct image
{
   union
   {
      pixel_t pixel_at[IMG_H][IMG_W];
      pixel_t pixels[IMG_W*IMG_H];
      uint8_t bytes[IMG_W*IMG_H*sizeof(pixel_t)];
   };

   union
   {
      eval_t eval_at[IMG_H][IMG_W];
      eval_t evals[IMG_W*IMG_H];
   };

   modifier_t   modifier;
   int          renderer;
   time_t       birth;
   equations_t  equ;
   void        *gui;

} image_t;

void     image_new(size_t, int, int);
image_t* image_get(size_t);
void     image_set_modifier(image_t*, int);
void     image_set_renderer(image_t*, int);
void     image_render(image_t*);
void     image_save(size_t, char*);

#endif
