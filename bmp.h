#ifndef __BMP_H__
#define __BMP_H__

#include <stdio.h>
#include <stdint.h>

#define BMP_MAGIC 0x4d42

typedef struct bmp_header
{
   uint16_t  magic;
   uint32_t  size;
   uint32_t  rsv0;
   uint32_t  offset;

} __attribute__((packed)) bmp_hdr_t;

typedef struct bmp_dib_header
{
   uint32_t  size;
   uint32_t  width;
   uint32_t  height;
   uint16_t  plane;
   uint16_t  bits;
   uint32_t  comp;
   uint32_t  img_sz;
   uint32_t  hres;
   uint32_t  vres;
   uint32_t  col;
   uint32_t  icol;

} __attribute__((packed)) bmp_dib_t;

typedef struct bmp_file
{
   bmp_hdr_t hdr;
   bmp_dib_t dib;
   uint8_t   pixels[0];

} __attribute__((packed)) bmp_t;

struct image;
void bmp_save(struct image*, FILE*);

#endif
