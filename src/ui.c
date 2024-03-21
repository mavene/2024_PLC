#include <stdio.h>
#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>


/* UI here, including preview's redo and undo feature */

/* SHIVANI */
/* Put in scanf for file paths and sanitize them here*/
char * retrieve_file() {
    /*Call GTK here to retrieve file */
    return 0;
}

static void
print_hello (GtkWidget *widget, gpointer data)
{
  g_print ("Previous\n");
}

static void activate (GtkApplication *app, gpointer userdata)
{
  GtkWidget *window;
  GtkWidget *button;
  GtkWidget *image;
  GtkWidget *box;

  window = gtk_application_window_new (GTK_APPLICATION (app));
  gtk_window_set_title (GTK_WINDOW (window), "Preview");
  gtk_window_set_default_size (GTK_WINDOW (window), 500, 500);

  box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
  gtk_container_add (GTK_CONTAINER (window), box);
  gtk_widget_set_valign (box, GTK_ALIGN_CENTER);
  gtk_widget_set_halign (box, GTK_ALIGN_CENTER);

  image = gtk_image_new_from_file("image.png"); 
  gtk_box_pack_start (GTK_BOX (box), image, FALSE, FALSE, 0);
  gtk_widget_set_halign (image, GTK_ALIGN_CENTER);
  gtk_widget_set_valign (image, GTK_ALIGN_CENTER);
  
  button = gtk_button_new_with_label ("Hello World");
  g_signal_connect (button, "clicked", G_CALLBACK (print_hello), NULL);
  gtk_box_pack_start (GTK_BOX (box), button, FALSE, FALSE, 0);
  gtk_widget_set_halign (button, GTK_ALIGN_CENTER);
  gtk_widget_set_valign (button, GTK_ALIGN_CENTER);
  gtk_widget_set_size_request (button, 100, 50);

  gtk_widget_show_all (window);
}

void open_gtk_window(const char *image_path) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), (gpointer)image_path);
    status = g_application_run(G_APPLICATION(app), 0, NULL);
    g_object_unref(app);
}


int main(int argc, char **argv) {
    open_gtk_window("image.png");
    return 0;
}