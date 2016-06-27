#ifndef _music_main
#define _music_main
#include <gtk/gtk.h>

int init_a_tree_view(GtkWidget * view , const gchar * title);
int init_music_lib(void);
int get_songs_by_scan_dir(void);
void get_object_from_builder( void );
void CloseApp (GtkWidget *widget , gpointer data);
gboolean DeleteEvent(GtkWidget *widget , gpointer data);

extern GtkWidget * statusbar;

#endif
