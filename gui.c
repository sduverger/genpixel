#include "gui.h"

static gtkgen_t gtk_gen;

static void generate()
{
   size_t n;
   for(n=0 ; n<IMG_N ; n++)
      if(gtk_gen.images[n].active)
      {
	 image_new(n, gtk_gen.modifier, gtk_gen.renderer);
	 gtk_widget_queue_draw(gtk_gen.images[n].view);
      }
}

static void evolve()
{
   printf("evolve\n");
}

static void save()
{
   size_t    n;
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

static gboolean key_press(GtkWidget *w, GdkEventKey *event, gpointer data)
{
   //event->state & (GDK_SHIFT_MASK | GDK_CONTROL_MASK)
   if(event->keyval ==  GDK_KEY_Escape)
      gtk_main_quit();
   return FALSE;
}

static void destroy_pixbuf(guchar *pixels, gpointer image)
{
   printf("!WRN! destroy pixbuf %p %p\n", pixels, image);
}

static void select_all(gboolean bool)
{
   size_t n;
   for(n=0 ; n<IMG_N ; n++)
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtk_gen.images[n].check), bool);
}

static void all()
{
   select_all(TRUE);
}

static void clear()
{
   select_all(FALSE);
}

static gboolean image_clicked(GtkWidget *w, GdkEventButton *event, gpointer data)
{
   gboolean active;
   size_t   n = (size_t)data;

   active = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtk_gen.images[n].check));
   gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtk_gen.images[n].check), !active);
   return TRUE;
}

static void check_toggled(GtkToggleButton *b, gpointer data)
{
   size_t n = (size_t)data;

   gtk_gen.images[n].active = gtk_toggle_button_get_active(b);

   if(gtk_gen.images[n].active)
      gtk_gen.count++;
   else
      gtk_gen.count--;

   gtk_widget_set_sensitive(gtk_gen.gn, (gtk_gen.count != 0));
   gtk_widget_set_sensitive(gtk_gen.sv, (gtk_gen.count != 0));
   gtk_widget_set_sensitive(gtk_gen.eb, (gtk_gen.count == 3));
}

static void mode_renderer(GtkToggleButton *b, int mode, int value)
{
   size_t n;
   void (*handler)(image_t*, int);

   if(!gtk_toggle_button_get_active(b))
      return;

   if(mode)
   {
      gtk_gen.modifier = value;
      handler = image_set_modifier;
   }
   else
   {
      gtk_gen.renderer = value;
      handler = image_set_renderer;
   }

   for(n=0 ; n<IMG_N ; n++)
      if(gtk_gen.images[n].active)
      {
	 image_t *img = image_get(n);
	 handler(img, value);
	 image_render(img);
	 gtk_widget_queue_draw(gtk_gen.images[n].view);
      }
}

static void mode_toggled(GtkToggleButton *b, gpointer data)
{
   mode_renderer(b, 1, (int)data);
}

static void renderer_toggled(GtkToggleButton *b, gpointer data)
{
   mode_renderer(b, 0, (int)data);
}

int init_gui(int argc, char **argv)
{
   GtkWidget *bbox, *vbox, *hbox, *vbox2, *hbox2;
   GtkWidget *grid;
   GtkWidget *button;
   GtkWidget *ebox;
   GtkWidget *sep;
   GdkPixbuf *pix;
   size_t     i;

   gtk_init(&argc, &argv);
   gtk_gen.win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
   gtk_window_set_title(GTK_WINDOW(gtk_gen.win), "genpixel");
   gtk_window_set_resizable(GTK_WINDOW(gtk_gen.win), FALSE);
   g_signal_connect(gtk_gen.win, "destroy", G_CALLBACK(gtk_main_quit), NULL);
   g_signal_connect(gtk_gen.win, "key_press_event", G_CALLBACK(key_press), NULL);
   gtk_container_set_border_width(GTK_CONTAINER(gtk_gen.win), 10);

   vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
   gtk_container_add(GTK_CONTAINER(gtk_gen.win), vbox);
   gtk_box_set_spacing(GTK_BOX(vbox), 10);


   /* image(s) buffer(s) */
   grid = gtk_grid_new();
   gtk_container_add(GTK_CONTAINER(vbox), grid);
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


   /* controls */
   sep = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
   gtk_container_add(GTK_CONTAINER(vbox), sep);

   hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
   gtk_container_add(GTK_CONTAINER(vbox), hbox);
   gtk_box_set_spacing(GTK_BOX(hbox), 10);

   bbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
   gtk_container_add(GTK_CONTAINER(hbox), bbox);

   gtk_gen.gn = gtk_button_new_with_label("generate");
   g_signal_connect(gtk_gen.gn, "clicked", G_CALLBACK(generate), NULL);
   gtk_container_add(GTK_CONTAINER(bbox), gtk_gen.gn);

   gtk_gen.eb = gtk_button_new_with_label("evolve");
   g_signal_connect(gtk_gen.eb, "clicked", G_CALLBACK(evolve), NULL);
   gtk_container_add(GTK_CONTAINER(bbox), gtk_gen.eb);

   gtk_gen.sv = gtk_button_new_with_label("save");
   g_signal_connect(gtk_gen.sv, "clicked", G_CALLBACK(save), NULL);
   gtk_container_add(GTK_CONTAINER(bbox), gtk_gen.sv);


   /* image(s) chooser */
   sep = gtk_separator_new(GTK_ORIENTATION_VERTICAL);
   gtk_container_add(GTK_CONTAINER(hbox), sep);

   vbox2 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
   gtk_container_add(GTK_CONTAINER(hbox), vbox2);

   grid = gtk_grid_new();
   gtk_container_add(GTK_CONTAINER(vbox2), grid);

   for(i=0 ; i<IMG_N ; i++)
   {
      gtk_gen.images[i].check = gtk_check_button_new();
      gtk_grid_attach(GTK_GRID(grid), gtk_gen.images[i].check, 1+(i%3), i/3, 1, 1);
      g_signal_connect(gtk_gen.images[i].check, "toggled", G_CALLBACK(check_toggled), (gpointer)i);
   }

   hbox2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
   gtk_container_add(GTK_CONTAINER(vbox2), hbox2);
   gtk_box_set_homogeneous(GTK_BOX(hbox2), TRUE);

   button = gtk_button_new_with_label("all");
   g_signal_connect(button, "clicked", G_CALLBACK(all), NULL);
   gtk_container_add(GTK_CONTAINER(hbox2), button);

   button = gtk_button_new_with_label("clear");
   g_signal_connect(button, "clicked", G_CALLBACK(clear), NULL);
   gtk_container_add(GTK_CONTAINER(hbox2), button);


   /* modifiers */
   sep = gtk_separator_new(GTK_ORIENTATION_VERTICAL);
   gtk_container_add(GTK_CONTAINER(hbox), sep);

   bbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
   gtk_container_add(GTK_CONTAINER(hbox), bbox);

   gtk_gen.modifier = IMG_M_256;

   button = gtk_radio_button_new_with_label(NULL, "mod256");
   gtk_container_add(GTK_CONTAINER(bbox), button);
   g_signal_connect(button, "toggled", G_CALLBACK(mode_toggled), (gpointer)IMG_M_256);

   button = gtk_radio_button_new_with_label(
      gtk_radio_button_get_group(GTK_RADIO_BUTTON(button)),"linear");
   gtk_container_add(GTK_CONTAINER(bbox), button);
   g_signal_connect(button, "toggled", G_CALLBACK(mode_toggled), (gpointer)IMG_M_LIN);


   /* renderers */
   sep = gtk_separator_new(GTK_ORIENTATION_VERTICAL);
   gtk_container_add(GTK_CONTAINER(hbox), sep);

   grid = gtk_grid_new();
   gtk_container_add(GTK_CONTAINER(hbox), grid);

   gtk_gen.renderer = IMG_R_RGB;

   button = gtk_radio_button_new_with_label(NULL,"RGB");
   gtk_grid_attach(GTK_GRID(grid), button, 0, 0, 1, 1);
   g_signal_connect(button, "toggled", G_CALLBACK(renderer_toggled), (gpointer)IMG_R_RGB);

   button = gtk_radio_button_new_with_label(
      gtk_radio_button_get_group(GTK_RADIO_BUTTON(button)),"single R");
   gtk_grid_attach(GTK_GRID(grid), button, 1, 0, 1, 1);
   g_signal_connect(button, "toggled", G_CALLBACK(renderer_toggled), (gpointer)IMG_R_SR);

   button = gtk_radio_button_new_with_label(
      gtk_radio_button_get_group(GTK_RADIO_BUTTON(button)),"single G");
   gtk_grid_attach(GTK_GRID(grid), button, 1, 1, 1, 1);
   g_signal_connect(button, "toggled", G_CALLBACK(renderer_toggled), (gpointer)IMG_R_SG);

   button = gtk_radio_button_new_with_label(
      gtk_radio_button_get_group(GTK_RADIO_BUTTON(button)),"single B");
   gtk_grid_attach(GTK_GRID(grid), button, 1, 2, 1, 1);
   g_signal_connect(button, "toggled", G_CALLBACK(renderer_toggled), (gpointer)IMG_R_SB);

   button = gtk_radio_button_new_with_label(
      gtk_radio_button_get_group(GTK_RADIO_BUTTON(button)),"combined R");
   gtk_grid_attach(GTK_GRID(grid), button, 2, 0, 1, 1);
   g_signal_connect(button, "toggled", G_CALLBACK(renderer_toggled), (gpointer)IMG_R_CR);

   button = gtk_radio_button_new_with_label(
      gtk_radio_button_get_group(GTK_RADIO_BUTTON(button)),"combined G");
   gtk_grid_attach(GTK_GRID(grid), button, 2, 1, 1, 1);
   g_signal_connect(button, "toggled", G_CALLBACK(renderer_toggled), (gpointer)IMG_R_CG);

   button = gtk_radio_button_new_with_label(
      gtk_radio_button_get_group(GTK_RADIO_BUTTON(button)),"combined B");
   gtk_grid_attach(GTK_GRID(grid), button, 2, 2, 1, 1);
   g_signal_connect(button, "toggled", G_CALLBACK(renderer_toggled), (gpointer)IMG_R_CB);

   select_all(TRUE);
   gtk_widget_show_all(gtk_gen.win);
   gtk_main();
   return 0;
}
