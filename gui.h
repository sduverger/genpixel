#ifndef __GUI_H__
#define __GUI_H__

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#include "image.h"

typedef struct gtk_image
{
   GtkWidget     *view;
   GtkWidget     *check;
   gboolean       active;
   struct image  *img;

} gtkimg_t;

typedef struct gtk_gen
{
   gtkimg_t     images[IMG_N];
   unsigned int count, modifier, renderer;
   GtkWidget    *win, *eb, *sv;

} gtkgen_t;

int init_gui(int, char**);

#endif
