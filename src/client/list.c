#include <string.h>
#include <stdlib.h>

#include "list.h"
#include "main.h"

int get_info_from_lview(GtkTreeIter *iter, gchar **name, gint *id)
{
	GtkTreeModel *model;
	GtkTreeSelection *select;

	select = gtk_tree_view_get_selection(GTK_TREE_VIEW(lview));
	if(!gtk_tree_selection_get_selected(select, &model, iter))
		return -1;

	gtk_tree_model_get(model, iter, 0, name, -1);
	*id = find_list_id_by_name(*name);

	return 0;
}

int get_info_from_sview(GtkTreeIter *iter , gchar **name , gchar **id)
{
	GtkTreeSelection *select;
	GtkTreeModel *model;

	select = gtk_tree_view_get_selection(GTK_TREE_VIEW(sview));
	if(!gtk_tree_selection_get_selected(select, &model, iter))
		return -1;

	gtk_tree_model_get(model, iter, 0, name, 1, id, -1);

	return 0;
}

int find_list_id_by_name(const gchar *listname)
{
	int i;
	for(i = 0; i < listssum; i++) {
		if(0 == strcmp(lists[i].listname, listname))
			return i;
	}
	return -1;
}

int find_list_id_by_store(GtkListStore *liststore)
{
	int i;
	for(i = 0 ; i < listssum; i++) {
		if(liststore == lists[i].store)
			return i;
	}
	return -1;
}

void list_store_clear(int id)
{
	gtk_list_store_clear(lists[id].store);
	lists[id].songsum = 0;
}

void list_store_clear_all(void)
{
	int i;

	for (i = 0; i < listssum; i++)
		list_store_clear(i);

	gtk_list_store_clear(lview_store);
	memset(lists, 0, 50 * sizeof(Songlist));
	listssum = 0;
}

int clicked_new_list(GtkWidget * bt , gpointer data)
{
	GtkWidget *dialog;
	GtkWidget *dialog_area;
	GtkWidget *vbox;
	gint result;
	GtkWidget *entry;
	GtkWidget *label;
	const gchar *str;

	dialog = gtk_dialog_new_with_buttons("New list" , GTK_WINDOW(window) ,
				GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT ,
				"_OK", GTK_RESPONSE_OK,
				"_Cancel", GTK_RESPONSE_CANCEL,
				NULL);

	dialog_area = gtk_dialog_get_content_area (GTK_DIALOG (dialog));

	entry  = gtk_entry_new();
	label  = gtk_label_new("List name");
	vbox = gtk_box_new( GTK_ORIENTATION_VERTICAL , 0 );
	gtk_box_pack_start(GTK_BOX( vbox ) , label , TRUE ,TRUE , 10);
	gtk_box_pack_start(GTK_BOX( vbox ) , entry , TRUE, TRUE, 10);

	gtk_widget_set_size_request(dialog , 250 , 150);
	gtk_box_pack_start (GTK_BOX(dialog_area), vbox, FALSE, FALSE, 0);
	gtk_container_set_border_width(GTK_CONTAINER(vbox), 15);
	gtk_widget_show_all(dialog);

	result = gtk_dialog_run( GTK_DIALOG(dialog) );

	if(GTK_RESPONSE_OK != result)
		goto do_release;

	str = gtk_entry_get_text( GTK_ENTRY(entry) );
	if (!str || !str[0])
		goto do_release;

	if(find_list_id_by_name(str) >= 0)
		goto do_release;

	if(!new_online_list(str)) {
		list_store_clear_all();
		sync_online_list_all();
	}

do_release:
	gtk_widget_destroy(dialog);
	return 1;
}

int clicked_rm_list(GtkWidget * bt , gpointer data)
{
	gint lid;
	gchar *name;
	GtkTreeIter iter;
	int ret;

	if(get_info_from_lview(&iter, &name, &lid))
		return 0;

	if (!rm_online_list(lists[lid].id)) {
		list_store_clear_all();
		sync_online_list_all();
	}
	return 1;
}

int clicked_add_song_to_list(GtkWidget * bt , gpointer data)
{
	GtkTreeIter iter;
	GtkWidget *combo;
	GtkWidget *dialog;
	GtkWidget *dialog_area;
	GtkWidget *vbox;
	gchar *sname;
	gchar *sid;
	gchar *lname;
	gint lid;
	gint result;
	gint i;

	if(get_info_from_sview(&iter , &sname , &sid))
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
	add_online_song_to_list(lists[lid].id, sid);
	list_store_clear(lid);
	sync_online_list(lid);
	gtk_tree_view_set_model(GTK_TREE_VIEW(sview), GTK_TREE_MODEL(lists[lid].store));

do_free:
	g_free(sname);
	g_free(sid);
	release_combo_text(GTK_COMBO_BOX_TEXT(combo));
	gtk_widget_destroy(dialog);
	return 0;
}

int clicked_rm_song_from_list(GtkWidget * bt , gpointer data)
{
	GtkTreeIter iter;
	gchar *name;
	gchar *sid;
	GtkTreeModel *model;
	gint listid;

	if(!get_info_from_sview(&iter , &name , &sid))
		return 0;

	model = gtk_tree_view_get_model(GTK_TREE_VIEW(sview));
	if((listid = find_list_id_by_store(GTK_LIST_STORE(model)) < 0))
		return 0;
	gtk_list_store_remove(GTK_LIST_STORE(model), &iter);
	lists[listid].songsum--;
	return 1;
}

int release_combo_text(GtkComboBoxText * cb)
{
	gint i;
	for(i = 0 ; i < listssum ; i++)
		gtk_combo_box_text_remove(cb , 0);
	return 0;
}

int on_change_list(GtkWidget * select , gpointer data)
{
	GtkTreeIter iter;
	GtkTreeModel * model;
	gchar * listname;
	gint listid;
	struct song_st sst[MAX_SONG];
	int i, ret;

	if(!gtk_tree_selection_get_selected(GTK_TREE_SELECTION(select) , &model , &iter))
		return 1;

	gtk_tree_model_get(model , &iter , 0 , &listname , -1);

	listid = find_list_id_by_name(listname);
	if (listid < 0)
		exit(-1);

	if(!lists[listid].is_fetch) {
		sync_online_list(listid);
		lists[listid].is_fetch = 1;
	}

	gtk_tree_view_set_model(GTK_TREE_VIEW(sview) , GTK_TREE_MODEL(lists[listid].store));

	return 0;
}
