#include "gui.h"

extern image_t  images[IMG_N];
static gtkgen_t gtk_gen;

void generate()
{
   unsigned int n;
   for(n=0 ; n<IMG_N ; n++)
   {
      image_new(n);
      gtk_widget_queue_draw(gtk_gen.images[n].img);
   }
}

void evolve()
{
   printf("evolve\n");
}

gboolean key_press(GtkWidget *w, GdkEventKey *event, gpointer data)
{
   //event->state & (GDK_SHIFT_MASK | GDK_CONTROL_MASK)
   if(event->keyval ==  GDK_KEY_Escape)
      gtk_main_quit();
   return FALSE;
}

void destroy_pixbuf(guchar *pixels, gpointer image)
{
   printf("!WRN! destroy pixbuf %p %p\n", pixels, image);
}

gboolean image_clicked(GtkWidget *w, GdkEventButton *event, gpointer data)
{
   gboolean active;
   unsigned int n = (unsigned int)data;

   if(!gtk_widget_get_sensitive(gtk_gen.images[n].chk))
      return FALSE;

   active = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtk_gen.images[n].chk));
   gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtk_gen.images[n].chk), !active);
   return TRUE;
}

void check_toggled(GtkToggleButton *b, gpointer data)
{
   unsigned int n = (unsigned int)data;

   gtk_gen.images[n].sts = gtk_toggle_button_get_active(b);

   if(gtk_gen.images[n].sts)
   {
      if(++gtk_gen.count == 3)
      {
	 gtk_widget_set_sensitive(gtk_gen.eb, FALSE);
	 for(n=0 ; n<IMG_N ; n++)
	    if(!gtk_gen.images[n].sts)
	       gtk_widget_set_sensitive(gtk_gen.images[n].chk, FALSE);
      }
   }
   else if(gtk_gen.count-- == 3)
   {
      gtk_widget_set_sensitive(gtk_gen.eb, TRUE);
      for(n=0 ; n<IMG_N ; n++)
	 if(!gtk_widget_get_sensitive(gtk_gen.images[n].chk))
	    gtk_widget_set_sensitive(gtk_gen.images[n].chk, TRUE);
   }
}

int init_gui(int argc, char **argv)
{
   GtkWidget *window;
   GtkWidget *box;
   GtkWidget *grid;
   GtkWidget *button;
   GtkWidget *ebox;
   GtkWidget *sep;
   GdkPixbuf *pix;
   int        i;

   gtk_init(&argc, &argv);
   window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
   gtk_window_set_title(GTK_WINDOW(window), "genpixel");
   gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
   g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
   g_signal_connect(window, "key_press_event", G_CALLBACK(key_press), NULL);
   gtk_container_set_border_width(GTK_CONTAINER(window), 10);

   box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
   gtk_container_add(GTK_CONTAINER(window), box);
   gtk_box_set_spacing(GTK_BOX(box), 10);

   grid = gtk_grid_new();
   gtk_container_add(GTK_CONTAINER(box), grid);
   gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
   gtk_grid_set_column_spacing(GTK_GRID(grid), 5);

   for(i=0 ; i<IMG_N ; i++)
   {
      const guchar *pixels;
      struct image *image = &images[i];

      gtk_gen.images[i].cnt = (void*)image;
      image->gui = (void*)&gtk_gen.images[i];
      pixels = (const guchar *)image->pixels;

      pix = gdk_pixbuf_new_from_data(pixels, GDK_COLORSPACE_RGB, FALSE,
				     8, IMG_W, IMG_H, 3*IMG_W,
				     destroy_pixbuf, &gtk_gen.images[i]);

      gtk_gen.images[i].img = gtk_image_new_from_pixbuf(pix);

      ebox = gtk_event_box_new();
      gtk_container_add(GTK_CONTAINER(ebox), gtk_gen.images[i].img);
      g_signal_connect(ebox, "button-press-event", G_CALLBACK(image_clicked), (gpointer)i);
      gtk_grid_attach(GTK_GRID(grid), ebox, i%3, i/3, 1, 1);
   }

   sep = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
   gtk_container_add(GTK_CONTAINER(box), sep);

   grid = gtk_grid_new();
   gtk_container_add(GTK_CONTAINER(box), grid);

   button = gtk_button_new_with_label("generate");
   g_signal_connect(button, "clicked", G_CALLBACK(generate), NULL);
   gtk_grid_attach(GTK_GRID(grid), button, 0, 0, 1, 1);

   gtk_gen.eb = gtk_button_new_with_label("evolve");
   g_signal_connect(gtk_gen.eb, "clicked", G_CALLBACK(evolve), NULL);
   gtk_grid_attach(GTK_GRID(grid), gtk_gen.eb, 0, 1, 1, 1);

   for(i=0 ; i<IMG_N ; i++)
   {
      gtk_gen.images[i].chk = gtk_check_button_new();
      gtk_grid_attach(GTK_GRID(grid), gtk_gen.images[i].chk, 1+(i%3), i/3, 1, 1);
      g_signal_connect(gtk_gen.images[i].chk, "toggled", G_CALLBACK(check_toggled), (gpointer)i);
   }

   gtk_widget_show_all(window);
   gtk_main();
   return 0;
}
