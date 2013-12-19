#include "bmp.h"
#include "image.h"
#include <stdio.h>
#include <stdint.h>

void bmp_save(image_t *img, FILE *file)
{
   bmp_t   bmp;
   ssize_t line;
   size_t  col, row, width, mod, pad=0;

   width = (sizeof(img->bytes)/IMG_H);
   mod = width % 4;
   if(mod) pad = 4 - mod;
   row = width + pad;

   bmp.hdr.magic = BMP_MAGIC;
   bmp.hdr.size = sizeof(bmp) + row*IMG_H;
   bmp.hdr.offset = sizeof(bmp);

   bmp.dib.size =sizeof(bmp.dib);
   bmp.dib.width = IMG_W;
   bmp.dib.height = IMG_H;
   bmp.dib.plane = 1;
   bmp.dib.bits = 24;
   bmp.dib.comp = 0;
   bmp.dib.img_sz = bmp.hdr.size - sizeof(bmp);
   bmp.dib.hres = 0xb13;
   bmp.dib.vres = 0xb13;
   bmp.dib.col = 0;
   bmp.dib.icol = 0;

   fwrite((void*)&bmp, sizeof(bmp), 1, file);

   for(line=IMG_H-1 ; line>=0 ; line--)
   {
      for(col=0 ; col<IMG_W ; col++)
	 fprintf(file, "%c%c%c",
		 img->pixel_at[line][col].b,
		 img->pixel_at[line][col].g,
		 img->pixel_at[line][col].r);
      fwrite("\x00\x00\x00", pad, 1, file);
   }
}
