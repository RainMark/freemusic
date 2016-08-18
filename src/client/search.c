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

int get_info_from_search_view(GtkTreeIter *iter , gchar **name , gchar **id)
{
	GtkTreeSelection *select;
	GtkTreeModel *model;

	select = gtk_tree_view_get_selection(GTK_TREE_VIEW(search_view));
	if(!gtk_tree_selection_get_selected(select, &model, iter))
		return -1;

	gtk_tree_model_get(model, iter, 0, name, 3, id, -1);
	printf("Search: %s %s\n", *name, *id);

	return 0;
}

int clicked_add_search_to_list(GtkWidget *bt, gpointer data)
{
	GtkTreeIter iter;
	GtkWidget *combo;
	GtkWidget *dialog;
	GtkWidget *dialog_area;
	GtkWidget *vbox;
	gchar *sname, *sid, *lname;
	gint lid, result, i;

	if(get_info_from_search_view(&iter , &sname , &sid))
		return 0;

	combo = gtk_combo_box_text_new();
	for(i = 0 ; i < listssum ; i++)
		gtk_combo_box_text_insert_text(GTK_COMBO_BOX_TEXT(combo), 0, lists[i].listname);
	gtk_combo_box_set_active(GTK_COMBO_BOX(combo), 0);

	dialog = gtk_dialog_new_with_buttons("Add to", GTK_WINDOW(window),
				GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
				"_OK", GTK_RESPONSE_OK,
				"_Cancel", GTK_RESPONSE_CANCEL,
				NULL);
	dialog_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

	vbox = gtk_box_new( GTK_ORIENTATION_VERTICAL, 0 );
	gtk_box_pack_start(GTK_BOX( vbox ), combo, FALSE, FALSE, 20);
	gtk_box_pack_start(GTK_BOX(dialog_area), vbox, TRUE, TRUE, 0);
	gtk_container_set_border_width(GTK_CONTAINER(vbox), 15);
	gtk_widget_set_size_request(dialog , 250 , 100);
	gtk_widget_show_all(dialog);

	result = gtk_dialog_run( GTK_DIALOG(dialog) );

	if(GTK_RESPONSE_OK != result)
		goto do_free;


	lname = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(combo));

	if(!lname || !strlen(lname))
		goto do_free;

	lid = find_list_id_by_name(lname);
	if (lid < 0)
		goto do_free;

	printf("%s %s\n", lists[lid].id, sid);
	// add_online_song_to_list(lists[lid].id, sid);
	// list_store_clear(lid);
	// sync_online_list(lid);
	// gtk_tree_view_set_model(GTK_TREE_VIEW(sview), GTK_TREE_MODEL(lists[lid].store));

do_free:
	g_free(sname);
	g_free(sid);
	release_combo_text(GTK_COMBO_BOX_TEXT(combo));
	gtk_widget_destroy(dialog);
	return 0;
}