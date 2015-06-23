#ifndef _list

#define _list

#include <gtk/gtk.h>


typedef struct _songlist {

	GtkListStore *store;
	int songsum;
	gchar listname[256];

}Songlist;


extern GtkWidget *window;
extern GtkWidget *lview , *sview;  //list view and song view
extern GtkListStore *lview_store;
extern Songlist lists[50];
extern gint listssum;
extern GtkTreeSelection * lview_selec;
extern GtkWidget * statusbar;
extern GtkWidget *Mode1 , *Mode2 , *Mode3;

int add_a_list(const gchar *listname);
int add_a_song_to_a_list(gint listid , const gchar * songname , const gchar *songpath);
int find_list_id_by_name(const gchar * listname);
int find_list_id_by_store(GtkListStore * liststore);
int clicked_add_a_list(GtkWidget * bt , gpointer data);

int clicked_add_song_to_list(GtkWidget * bt , gpointer data);
int get_info_of_sel_song(GtkTreeIter * iter , gchar ** name , gchar ** path);
int release_combo_text(GtkComboBoxText * cb);
int clicked_remove_song_from_list(GtkWidget * bt , gpointer data);
int clicked_remove_all_of_list(GtkWidget * bt , gpointer data);

int get_info_of_sel_list(GtkTreeIter *iter , gchar **sel_listname , gint *sel_listid);
int clicked_del_a_list(GtkWidget * bt , gpointer data);
int remove_list_from_lists(gint id);


int on_change_list(GtkWidget * selec , gpointer data);


int write_list_to_cfg(void);
int write_songs_of_list(int id);

int read_list_from_cfg(void);
int read_songs_to_list(gint id);

#endif


