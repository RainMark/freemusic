#ifndef _list

#define _list

#include <gtk/gtk.h>
#include <common.h>
#include "client.h"

struct _songlist {
	GtkListStore *store;
	gchar listname[ALL_NAME_LEN];
	gchar id[ID_LEN + 1];
	gchar owner_id[ID_LEN + 1];
	int songsum;
	int is_fetch;
};

typedef struct _songlist Songlist;

extern GtkWidget *window;
extern GtkWidget *lview , *sview;
extern GtkWidget *Mode1 , *Mode2 , *Mode3;
extern GtkListStore *lview_store;
extern GtkTreeSelection *lview_selec;
extern GtkWidget *statusbar;

extern Songlist lists[];
extern gint listssum;

int find_list_id_by_name(const gchar *);
int find_list_id_by_store(GtkListStore *);
int get_info_from_sview(GtkTreeIter *, gchar **, gchar **);
int get_info_from_lview(GtkTreeIter *, gchar **, gint *);
void list_store_clear(int id);
void list_store_clear_all(void);

int clicked_add_list(GtkWidget *, gpointer );
int clicked_rm_list(GtkWidget *, gpointer );
int clicked_add_song_to_list(GtkWidget *, gpointer );
int clicked_rm_song_from_list(GtkWidget *, gpointer );

int release_combo_text(GtkComboBoxText *);

int on_change_list(GtkWidget *, gpointer );

#endif


