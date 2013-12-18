#include "image.h"
#include <string.h>

image_t images[IMG_N];

void init_image(int n)
{
   int i,j,c,v;

   v = rand()%255;
   c = rand()%3;

   memset(images[n].pixels, 0, sizeof(images[n].pixels));

   for(i=0;i<IMG_H;i++)
      for(j=0;j<IMG_W;j++)
	 //images[n].pixel_at[i][j].chan[c] = 255;
	 //images[n].pixel_at[i][j].chan[c] = v;
	 //images[n].pixel_at[i][j].chan[c] = rand()%255;
	 images[n].pixel_at[i][j].chan[rand()%3] = rand()%255;
}
