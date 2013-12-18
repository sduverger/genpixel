#include "image.h"
#include <string.h>

image_t images[IMG_N];

void image_init(image_t *img)
{
   equation_init(&img->equ.r, EQU_DEPTH);
   equation_init(&img->equ.g, EQU_DEPTH);
   equation_init(&img->equ.b, EQU_DEPTH);
}

void image_eval(image_t *img)
{
   unsigned int x, y;
   for(x=0 ; x<IMG_W ; x++)
      for(y=0 ; y<IMG_H ; y++)
      {
	 img->eval_at[y][x].r = equation_eval(&img->equ.r, x, y);
	 img->eval_at[y][x].g = equation_eval(&img->equ.g, x, y);
	 img->eval_at[y][x].b = equation_eval(&img->equ.b, x, y);
      }
}

void __render_mod256(image_t *img)
{
   unsigned int i;
   for(i=0 ; i<IMG_W*IMG_H ; i++)
   {
      img->pixels[i].r = (uint8_t)((uint32_t)img->evals[i].r % 256);
      img->pixels[i].g = (uint8_t)((uint32_t)img->evals[i].g % 256);
      img->pixels[i].b = (uint8_t)((uint32_t)img->evals[i].b % 256);
   }
}

void __render_single(image_t *img, int c)
{
   unsigned int i, color;
   for(i=0 ; i<IMG_W*IMG_H ; i++)
   {
      color = (uint32_t)img->evals[i].chan[c];
      img->pixels[i].r = (color>>16)&0xff;
      img->pixels[i].g = (color>> 8)&0xff;
      img->pixels[i].b =  color     &0xff;
   }
}

void __render_linear(image_t *img)
{
   unsigned int	i;
   double diffr = img->equ.r.max - img->equ.r.min;
   double diffg = img->equ.g.max - img->equ.g.min;
   double diffb = img->equ.b.max - img->equ.b.min;

   for(i=0 ; i<IMG_W*IMG_H ; i++)
   {
      img->pixels[i].r = (uint8_t)(((img->evals[i].r - img->equ.r.min)/diffr) * 255);
      img->pixels[i].g = (uint8_t)(((img->evals[i].g - img->equ.g.min)/diffg) * 255);
      img->pixels[i].b = (uint8_t)(((img->evals[i].b - img->equ.b.min)/diffb) * 255);
   }
}

void image_render(image_t *img)
{
   __render_mod256(img);
   //__render_single(img, 0);
   //__render_single(img, 1);
   //__render_single(img, 2);
   //__render_linear(img);
}

void image_new(int n)
{
   image_t *img = &images[n];

   image_init(img);
   image_eval(img);
   image_render(img);
}
