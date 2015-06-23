#include "list.h"
#include <string.h>
#include <stdlib.h>
#include "main.h"


/////////////////////////////////////////////////////////////////////////////////////
int add_a_list(const gchar *listname)
{
	GtkTreeIter iter;

	strcpy(lists[listssum].listname , listname);
	lists[listssum].songsum = 0;
	lists[listssum].store = gtk_list_store_new(2 , G_TYPE_STRING , G_TYPE_STRING);

	gtk_list_store_append(lview_store , &iter);
	gtk_list_store_set(lview_store , &iter , 0 , lists[listssum].listname , -1);
	
	listssum++;

	return 1;
}

int get_info_of_sel_list(GtkTreeIter *iter , gchar **sel_listname , gint *sel_listid)
{
	GtkTreeSelection * selec   = gtk_tree_view_get_selection(GTK_TREE_VIEW(lview));

	GtkTreeModel * model;
	if(!gtk_tree_selection_get_selected(selec , &model , iter))
		return 0;

	gtk_tree_model_get(model , iter , 0 , sel_listname , -1);

	*sel_listid = find_list_id_by_name(*sel_listname);

	return 1;
}

int remove_list_from_lists(gint id)
{
	int i;
	if(id <= 0)
		return 0;

	for(i = id ; i < listssum ; i++) {

		lists[i] = lists[i + 1];
	}

	listssum--;
	
	return 1;
}

int find_list_id_by_name(const gchar * listname)
{
	int i;
	for(i = 0;i < listssum;i++) {
	
		if(0 == strcmp(lists[i].listname , listname))
			return i;
	}
	return -1;
}

int find_list_id_by_store(GtkListStore * liststore)
{
	int i;
	for(i = 0 ; i < listssum ; i++) {
	
		if(liststore == lists[i].store)
			return i;
	}
	return -1;
}
///////////////////////////////////////////////////////////////////////////////////////////
int add_a_song_to_a_list(gint listid , const gchar * songname , const gchar *songpath)
{	
	GtkTreeIter iter;
 
	gtk_list_store_append(lists[listid].store , &iter);
	gtk_list_store_set(lists[listid].store , &iter , 0 , songname , 1 , songpath , -1);

	lists[listid].songsum++;

	return 1;
}

int get_info_of_sel_song(GtkTreeIter * iter , gchar ** name , gchar ** path)
{
	GtkTreeSelection * sview_sel;
	GtkTreeModel * model;

	sview_sel = gtk_tree_view_get_selection( GTK_TREE_VIEW(sview) );
	if(!gtk_tree_selection_get_selected(GTK_TREE_SELECTION(sview_sel) , &model , iter))
		return 0;

	gtk_tree_model_get(model , iter , 0 , name , 1 , path , -1);

	return 1;
}

//////////////////////////////////////////////////////////////////////////////////////////
int clicked_add_a_list(GtkWidget * bt , gpointer data)
{
	GtkWidget * dialog;
	gint result;
	GtkWidget * entry;
	GtkWidget * label;
	
	dialog = gtk_dialog_new_with_buttons("新增歌曲列表" , 
				GTK_WINDOW(window) , 
				GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT , 
				GTK_STOCK_OK , 
				GTK_RESPONSE_ACCEPT , 
				GTK_STOCK_CANCEL ,
				GTK_RESPONSE_REJECT , 
				NULL);
	entry  = gtk_entry_new();
	label  = gtk_label_new("请输入你想增加的列表的名称:");

	gtk_box_pack_start(GTK_BOX( GTK_DIALOG(dialog)->vbox ) , 
				label , FALSE , FALSE , 20);
	gtk_box_pack_start(GTK_BOX( GTK_DIALOG(dialog)->vbox ) , 
				entry , FALSE , FALSE , 20);

	gtk_widget_set_size_request(dialog , 250 , 200);
	gtk_widget_show_all(dialog);

	result = gtk_dialog_run( GTK_DIALOG(dialog) );

	if(GTK_RESPONSE_ACCEPT == result) {
	
		const gchar * listname;

		listname = gtk_entry_get_text( GTK_ENTRY(entry) );
		if( -1 == find_list_id_by_name(listname) )
			add_a_list(listname);
	}

	gtk_widget_destroy(dialog);
	return 1;
}

int clicked_del_a_list(GtkWidget * bt , gpointer data)
{
	gint sel_listid;
	gchar * sel_listname;
	GtkTreeIter iter;

	if(!get_info_of_sel_list(&iter , &sel_listname , &sel_listid) || 0 == sel_listid)
		return 0;
	gtk_list_store_clear(lists[sel_listid].store);
	gtk_list_store_remove(lview_store , &iter);
	remove_list_from_lists(sel_listid);
	gtk_tree_view_set_model(GTK_TREE_VIEW(sview) , GTK_TREE_MODEL(lists[0].store));

	return 1;
}
////////////////////////////////////////////////////////////////////////
int clicked_add_song_to_list(GtkWidget * bt , gpointer data)
{
	GtkTreeIter iter;    //no use
	GtkWidget * combo;
	GtkWidget * dialog;
	gint result;
	gint i;

	gchar * sel_songname;
	gchar * sel_songpath;

	if(!get_info_of_sel_song(&iter , &sel_songname , &sel_songpath))
		return 0;

	combo = gtk_combo_box_text_new();
//	gtk_widget_set_size_request(combo , 300 , 200);
	
	for(i = 0 ; i < listssum ; i++) {
	
		gtk_combo_box_text_insert_text(GTK_COMBO_BOX_TEXT(combo) , 
				0 , 
				lists[i].listname);
	}

	dialog = gtk_dialog_new_with_buttons("添加歌曲到列表" , 
				GTK_WINDOW(window) , 
				GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT , 
				GTK_STOCK_OK , 
				GTK_RESPONSE_ACCEPT , 
				GTK_STOCK_CANCEL ,
				GTK_RESPONSE_REJECT , 
				NULL);

	gtk_box_pack_start(GTK_BOX( GTK_DIALOG(dialog)->vbox ) , 
				combo , FALSE , FALSE , 20);
	
	gtk_widget_set_size_request(dialog , 250 , 100);

	gtk_widget_show_all(dialog);

	result = gtk_dialog_run( GTK_DIALOG(dialog) );

	if(GTK_RESPONSE_ACCEPT == result) {
	
		gchar * sel_listname = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(combo));

		if(NULL != sel_listname) {
			
			gint listid = find_list_id_by_name(sel_listname);
			add_a_song_to_a_list(listid , sel_songname , sel_songpath);
		}
	}

	g_free(sel_songname);
	g_free(sel_songpath);
	release_combo_text(GTK_COMBO_BOX_TEXT(combo));
	gtk_widget_destroy(dialog);


	gtk_tree_view_set_model(GTK_TREE_VIEW(sview) , GTK_TREE_MODEL(lists[0].store));
	return 1;
}

int clicked_remove_song_from_list(GtkWidget * bt , gpointer data)
{
	GtkTreeIter iter;
	gchar * name;         //no use
	gchar * path;		  //no use	
	GtkTreeModel * model;
	gint listid;

	if(!get_info_of_sel_song(&iter , &name , &path))
		return 0;

	model = gtk_tree_view_get_model(GTK_TREE_VIEW(sview));
	
	if((listid = find_list_id_by_store(GTK_LIST_STORE(model)) <= 0))
		return 0;
	
	gtk_list_store_remove(GTK_LIST_STORE(model) , &iter);
	lists[listid].songsum--;
	return 1;
}

int clicked_remove_all_of_list(GtkWidget * bt , gpointer data)
{
	GtkTreeModel * model;
	
	model = gtk_tree_view_get_model(GTK_TREE_VIEW(sview));
	
	if(lists[0].store == GTK_LIST_STORE(model))
		return 0;
	
	gtk_list_store_clear(GTK_LIST_STORE(model));
	
	return 1;
}

///////////////////////////////////////////////////////////////////////////////
int release_combo_text(GtkComboBoxText * cb)
{
	gint i;
	for(i = 0 ; i < listssum ; i++)
		gtk_combo_box_text_remove(cb , 0);
	return 1;
}



///////////////////////////////////////////////////////////////////////////////
 

int on_change_list(GtkWidget * selec , gpointer data)
{
	GtkTreeIter iter;
	GtkTreeModel * model;
	gchar * listname;
	gint listid;

	if(!gtk_tree_selection_get_selected(GTK_TREE_SELECTION(selec) , &model , &iter))
		return 0;

	gtk_tree_model_get(model , &iter , 0 , &listname , -1);

	listid = find_list_id_by_name(listname);

	gtk_tree_view_set_model(GTK_TREE_VIEW(sview) , GTK_TREE_MODEL(lists[listid].store));

	return 1;
}

////////////////////////////////////////////////////////////////////////////////////
int write_list_to_cfg(void)
{
	FILE * list_file;
	gint i;

	if(NULL == (list_file = fopen("/opt/free-music/etc/lists.ml" , "w")))
		return 0;

	for(i = 0 ; i < listssum ; i++) {

		fprintf(list_file , "%d %d %s\n" , i , (int)strlen(lists[i].listname), lists[i].listname);
		write_songs_of_list(i);
	}

	fclose(list_file);

	return 1;
}

int write_songs_of_list(int id)
{
	gchar idstr[512];
	FILE * song_file;
	GtkTreeIter iter;
	GtkTreePath * treepath;
	gint i;

	sprintf(idstr , "/opt/free-music/etc/lists.d/%d.ml" , id);
	if(NULL == (song_file = fopen(idstr , "w")))
		return 0;
	
	if(gtk_tree_model_get_iter_first(GTK_TREE_MODEL(lists[id].store) , &iter))
		treepath = gtk_tree_model_get_path(GTK_TREE_MODEL(lists[id].store) , &iter);

	for(i = 0 ; i < lists[id].songsum ; i++) {
	
		gchar * name;
		gchar * path;
		gtk_tree_model_get(GTK_TREE_MODEL(lists[id].store) , &iter , 0 , &name , 1 , &path , -1);
		fprintf(song_file , "%d %s\n" ,(int)strlen(name) , name);
		fprintf(song_file , "%d %s\n" ,(int)strlen(path) , path);
		gtk_tree_path_next(treepath);
		gtk_tree_model_get_iter(GTK_TREE_MODEL(lists[id].store) , &iter , treepath);
	}

	fclose(song_file);
	
	return 1;
}



int read_list_from_cfg(void)
{
	FILE * list_file;
	gint id;
	gint len;
	gchar listname[512];

	if(NULL == (list_file = fopen("/opt/free-music/etc/lists.ml" , "r"))) {
	
		get_songs_by_scan_dir();
		return 0;
	}
	while(EOF != fscanf(list_file , "%d %d " ,&id , &len)) {
		
		fgets(listname , sizeof listname , list_file);
		listname[len] = '\0';
		add_a_list(listname);
		read_songs_to_list(id);
	}

	fclose(list_file);
	
	return 1;
}

int read_songs_to_list(gint id)
{
	FILE * song_file;
	gint len;
	gchar listpath[512];
	gchar songname[512];
	gchar songpath[512];

	sprintf(listpath , "/opt/free-music/etc/lists.d/%d.ml" , id);

	if(NULL == (song_file = fopen(listpath , "r")))
		return 0;
	while(EOF != fscanf(song_file , "%d " , &len)) {
	
		fgets(songname , sizeof songname , song_file);
		songname[len] = '\0';
		fscanf(song_file , "%d " , &len);
		fgets(songpath , sizeof songpath , song_file);
		songpath[len] = '\0';
		add_a_song_to_a_list(id , songname , songpath);
	}

	fclose(song_file);

	return 0;
}



