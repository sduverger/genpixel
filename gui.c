#include "gui.h"

static gtkgen_t gtk_gen;

void generate()
{
   unsigned int n;
   for(n=0 ; n<IMG_N ; n++)
   {
      image_new(n);
      gtk_widget_queue_draw(gtk_gen.images[n].view);
   }
}

void evolve()
{
   printf("evolve\n");
}

void save()
{
   int       n;
   char      *dir;
   GtkWidget *dialog;

   dialog = gtk_file_chooser_dialog_new("Directory to save image(s)",
					GTK_WINDOW(gtk_gen.win),
					GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
					"cancel", GTK_RESPONSE_CANCEL,
					"select", GTK_RESPONSE_ACCEPT,
					NULL);

   if(gtk_dialog_run(GTK_DIALOG(dialog)) != GTK_RESPONSE_ACCEPT)
      goto __leave;

   dir = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER(dialog));

   for(n=0 ; n<IMG_N ; n++)
      if(gtk_gen.images[n].active)
	 image_save(n, dir);

   g_free(dir);

__leave:
   gtk_widget_destroy(dialog);
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

   active = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtk_gen.images[n].check));
   gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtk_gen.images[n].check), !active);
   return TRUE;
}

void check_toggled(GtkToggleButton *b, gpointer data)
{
   unsigned int n = (unsigned int)data;

   gtk_gen.images[n].active = gtk_toggle_button_get_active(b);

   if(gtk_gen.images[n].active)
      gtk_gen.count++;
   else
      gtk_gen.count--;

   gtk_widget_set_sensitive(gtk_gen.sv, (gtk_gen.count != 0));
   gtk_widget_set_sensitive(gtk_gen.eb, (gtk_gen.count == 3));
}



int init_gui(int argc, char **argv)
{
   GtkWidget *box;
   GtkWidget *grid;
   GtkWidget *button;
   GtkWidget *ebox;
   GtkWidget *sep;
   GdkPixbuf *pix;
   int        i;

   gtk_init(&argc, &argv);
   gtk_gen.win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
   gtk_window_set_title(GTK_WINDOW(gtk_gen.win), "genpixel");
   gtk_window_set_resizable(GTK_WINDOW(gtk_gen.win), FALSE);
   g_signal_connect(gtk_gen.win, "destroy", G_CALLBACK(gtk_main_quit), NULL);
   g_signal_connect(gtk_gen.win, "key_press_event", G_CALLBACK(key_press), NULL);
   gtk_container_set_border_width(GTK_CONTAINER(gtk_gen.win), 10);

   box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
   gtk_container_add(GTK_CONTAINER(gtk_gen.win), box);
   gtk_box_set_spacing(GTK_BOX(box), 10);

   grid = gtk_grid_new();
   gtk_container_add(GTK_CONTAINER(box), grid);
   gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
   gtk_grid_set_column_spacing(GTK_GRID(grid), 5);

   for(i=0 ; i<IMG_N ; i++)
   {
      const guchar *pixels;
      image_t *image = image_get(i);

      gtk_gen.images[i].img = (void*)image;
      image->gui = (void*)&gtk_gen.images[i];
      pixels = (const guchar *)image->pixels;

      pix = gdk_pixbuf_new_from_data(pixels, GDK_COLORSPACE_RGB, FALSE,
				     8, IMG_W, IMG_H, 3*IMG_W,
				     destroy_pixbuf, &gtk_gen.images[i]);

      gtk_gen.images[i].view = gtk_image_new_from_pixbuf(pix);

      ebox = gtk_event_box_new();
      gtk_container_add(GTK_CONTAINER(ebox), gtk_gen.images[i].view);
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

   gtk_gen.sv = gtk_button_new_with_label("save");
   g_signal_connect(gtk_gen.sv, "clicked", G_CALLBACK(save), NULL);
   gtk_grid_attach(GTK_GRID(grid), gtk_gen.sv, 0, 2, 1, 1);

   for(i=0 ; i<IMG_N ; i++)
   {
      gtk_gen.images[i].check = gtk_check_button_new();
      gtk_grid_attach(GTK_GRID(grid), gtk_gen.images[i].check, 1+(i%3), i/3, 1, 1);
      g_signal_connect(gtk_gen.images[i].check, "toggled", G_CALLBACK(check_toggled), (gpointer)i);
   }

   gtk_widget_set_sensitive(gtk_gen.eb, FALSE);
   gtk_widget_set_sensitive(gtk_gen.sv, FALSE);

   gtk_widget_show_all(gtk_gen.win);
   gtk_main();
   return 0;
}
