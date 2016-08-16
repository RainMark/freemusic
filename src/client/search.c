#include <list.h>
#include <main.h>
#include <player.h>
#include <client.h>
#include <search.h>
#include <common.h>

GtkWidget *search_entry;
GtkWidget *search_view;
GtkWidget *search_select;
GtkListStore *search_store;

void search_stack_init(void)
{
	GtkCellRenderer *renderer;

	renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_insert_column_with_attributes(
		GTK_TREE_VIEW(search_view),
		0,
		"Name",
		renderer,
		"text",
		0,
		NULL);
	/* renderer = gtk_cell_renderer_text_new(); */
	/* gtk_tree_view_insert_column_with_attributes( */
	/* 	GTK_TREE_VIEW(search_view), */
	/* 	1, */
	/* 	"Singer", */
	/* 	renderer, */
	/* 	"text", */
	/* 	0, */
	/* 	NULL); */
	/* renderer = gtk_cell_renderer_text_new(); */
	/* gtk_tree_view_insert_column_with_attributes( */
	/* 	GTK_TREE_VIEW(search_view), */
	/* 	2, */
	/* 	"Style", */
	/* 	renderer, */
	/* 	"text", */
	/* 	0, */
	/* 	NULL); */

	search_store = gtk_list_store_new(4, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);
}

int clicked_search(GtkWidget *bt, gpointer data)
{
	const char *context;
	struct song_st *sst;
	int r = 0;

	sst = malloc(MAX_SONG * sizeof(struct song_st));
	context = gtk_entry_get_text(GTK_ENTRY(search_entry));
	if ((r = search_online_song(context, sst)) < 0 )
		goto do_free;

	search_song_clear();
	search_song_show(sst, r);

do_free:
	free(sst);
	return r;
}

void search_song_clear(void)
{
	gtk_list_store_clear(search_store);
	gtk_tree_view_set_model(GTK_TREE_VIEW(search_view), GTK_TREE_MODEL(search_store));
}

void search_song_show(struct song_st *sst, int n)
{
	int i;
	GtkTreeIter iter;

	for (i = 0; i < n; i++) {
		gtk_list_store_append(search_store, &iter);
		gtk_list_store_set(search_store, &iter, 0, sst[i].name, 1, sst[i].singer_name, 2, sst[i].style, 3, sst[i].id, -1);
	}
	gtk_tree_view_set_model(GTK_TREE_VIEW(search_view), GTK_TREE_MODEL(search_store));
}
