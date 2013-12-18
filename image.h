#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <stdint.h>

#define IMG_N  6
#define IMG_W  200
#define IMG_H  200

typedef union pixel
{
   struct { uint8_t r,g,b; } __attribute__((packed));
   uint8_t chan[3];

} pixel_t;

typedef struct image
{
   union
   {
      pixel_t pixel_at[IMG_W][IMG_H];
      pixel_t pixels[IMG_W*IMG_H];
   };

   void *gui;

} image_t;

void init_image(int);

#endif
