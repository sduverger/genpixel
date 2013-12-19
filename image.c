#include "image.h"
#include "utils.h"
#include "bmp.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static image_t images[IMG_N];

static void image_init(image_t *img, int modifier, int renderer)
{
   img->birth = time(NULL);

   equation_init(&img->equ.r, EQU_DEPTH);
   equation_init(&img->equ.g, EQU_DEPTH);
   equation_init(&img->equ.b, EQU_DEPTH);

   image_set_modifier(img, modifier);
   image_set_renderer(img, renderer);
}

static void image_eval(image_t *img)
{
   size_t x, y;
   for(x=0 ; x<IMG_W ; x++)
      for(y=0 ; y<IMG_H ; y++)
      {
	 img->eval_at[y][x].r = equation_eval(&img->equ.r, x, y);
	 img->eval_at[y][x].g = equation_eval(&img->equ.g, x, y);
	 img->eval_at[y][x].b = equation_eval(&img->equ.b, x, y);
      }
#ifdef DEBUG
   printf("MIN MAX\nR= %f %f\nG= %f %f\nB= %f %f\n--------\n",
	 img->equ.r.min, img->equ.r.max,
	 img->equ.g.min, img->equ.g.max,
	 img->equ.b.min, img->equ.b.max);
#endif
}

static uint8_t __modifier_mod256(equ_t *eq, double value)
{
   return (uint8_t)((uint32_t)value % 256);
}

static uint8_t __modifier_linear(equ_t *eq, double value)
{
   double diff = eq->max - eq->min;
   return (uint8_t)(((value - eq->min)/diff) * 255);
}

static void __render_rgb(image_t *img)
{
   size_t i;
   for(i=0 ; i<IMG_W*IMG_H ; i++)
   {
      img->pixels[i].r = img->modifier(&img->equ.r, img->evals[i].r);
      img->pixels[i].g = img->modifier(&img->equ.g, img->evals[i].g);
      img->pixels[i].b = img->modifier(&img->equ.b, img->evals[i].b);
   }
}

static void __render_single(image_t *img, int c)
{
   size_t i;
   for(i=0 ; i<IMG_W*IMG_H ; i++)
      img->pixels[i].r = img->pixels[i].g = img->pixels[i].b =
	 img->modifier(&img->equ.chan[c], img->evals[i].chan[c]);
}

static void __render_combined(image_t *img, int c)
{
   size_t i, color;
   for(i=0 ; i<IMG_W*IMG_H ; i++)
   {
      color = (uint32_t)img->evals[i].chan[c];
      img->pixels[i].r = (color>>16)&0xff;
      img->pixels[i].g = (color>> 8)&0xff;
      img->pixels[i].b =  color     &0xff;
   }
}

void image_set_modifier(image_t *img, int value)
{
   switch(value)
   {
   case IMG_M_256: img->modifier = __modifier_mod256; break;
   case IMG_M_LIN: img->modifier = __modifier_linear; break;

   default: error("no such modifier %d\n", value);
   }
}

void image_set_renderer(image_t *img, int value)
{
   img->renderer = value;
}

void image_render(image_t *img)
{
   switch(img->renderer)
   {
   case IMG_R_RGB: __render_rgb(img);         break;
   case IMG_R_SR:  __render_single  (img, 0); break;
   case IMG_R_SG:  __render_single  (img, 1); break;
   case IMG_R_SB:  __render_single  (img, 2); break;
   case IMG_R_CR:  __render_combined(img, 0); break;
   case IMG_R_CG:  __render_combined(img, 1); break;
   case IMG_R_CB:  __render_combined(img, 2); break;

   default: error("no such renderer %d\n", img->renderer);
   }
}

void image_new(size_t n, int modifier, int renderer)
{
   image_t *img = &images[n];

   image_init(img, modifier, renderer);
   image_eval(img);
   image_render(img);
}

image_t* image_get(size_t n)
{
   return &images[n];
}

static void __print(image_t *img, FILE* file)
{
   bmp_save(img, file);
}

void image_save(size_t n, char *dir)
{
   FILE      *file;
   image_t   *img  = &images[n];
   struct tm *t    = localtime(&img->birth);
   size_t    size  = strlen(dir) + 64;
   char      *name = malloc(size);

   if(!name)
      error("malloc file name");

   snprintf(name, size, "%s/img_%.2d_%.2d_%.4d_%.2dh_%.2dm_%.2ds_%.2d.equ",
	    dir,t->tm_mday,t->tm_mon,t->tm_year+1900,t->tm_hour,t->tm_min,t->tm_sec,n);

   file = fopen(name, "w");
   for(n=0 ; n<3 ; n++)
   {
      equation_print(&img->equ.chan[n], file);
      fputc('\n',file);
   }
   fclose(file);

   size = strlen(name) - 3;
   strcpy(&name[size], "bmp");

   file = fopen(name, "w");
   __print(img, file);
   fclose(file);
   free(name);
}
