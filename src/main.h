#ifndef _music_main

#define _music_main

int init_a_tree_view(GtkWidget * view , const gchar * title);
int init_music_lib(void);
int get_songs_by_scan_dir(void);

void CloseApp (GtkWidget *widget , gpointer data);
gboolean DeleteEvent(GtkWidget *widget , gpointer data);
void HelpDialog (GtkWidget *widget , gpointer data);
GtkWidget * create_button_with_image_none(const gchar *filename);
GtkWidget * create_button_with_image_left(const gchar *filename , const gchar *label);
GtkWidget * create_button_with_image_over(const gchar *filename , const gchar *label);
GtkWidget * create_pixbuf(const gchar *filename);

#endif
