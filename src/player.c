#include "player.h"
#include <stdlib.h>
#include "list.h"
#include <time.h>

ReadySong readysong;
enum _playmode playmode;
Player player;
GtkListStore * playedsong;

int init_player(void)
{
	player.state = STOP;
	playmode = ORDER;
	playedsong = gtk_list_store_new(2 , G_TYPE_STRING , G_TYPE_STRING);
	player.loop = g_main_loop_new(NULL , FALSE);
	
	return 1;
}

int create_pipe(void)
{
	player.pipe = gst_pipeline_new("mp3-player");
	player.source = gst_element_factory_make("filesrc" , "file-source");
	player.decoder = gst_element_factory_make("mad" , "mad-decoder");
	player.sink = gst_element_factory_make("autoaudiosink" ,"auto-output");

	player.bus = gst_pipeline_get_bus(GST_PIPELINE(player.pipe));

	if(!player.pipe || !player.decoder || !player.sink) {
		printf("Can't create pipe!Need gsteamer-0.10");
		exit(1);
	}

	gst_bus_add_watch(player.bus , bus_call , player.loop);
	gst_object_unref(player.bus);
	gst_bin_add_many(GST_BIN(player.pipe) ,
			player.source , 
			player.decoder , 
			player.sink , 
			NULL);
	gst_element_link_many(player.source , player.decoder , player.sink);

	return 0;
}

int destroy_pipe(void)
{
	g_object_unref(player.pipe);
	return 0;
}

int play_readysong(void)
{
	gchar src[512];
	sprintf(src , "%s/%s", readysong.path , readysong.name);

	create_pipe();
	g_object_set(G_OBJECT(player.source) , "location" , src , NULL);
	gst_element_set_state(player.pipe , GST_STATE_PLAYING);

	player.state = PLAYING;
	push_status_message(readysong.name);
	
	g_main_loop_run(player.loop);
	
	return 0;
}

int clicked_play(GtkWidget *bt , gpointer data)
{
	GtkTreeIter iter;
	int listid;
	gchar *listname;

	if(STOP == player.state) {
	
		get_info_of_sel_song(&readysong.iter , &readysong.name , &readysong.path);
		get_info_of_sel_list(&iter , &listname , &player.playlist);
		play_readysong();
		return 1;
	
	} else if (PAUSE == player.state)
		 gst_element_set_state(player.pipe , GST_STATE_PLAYING);

	player.state = PLAYING;
	return 1;
}

int clicked_pause(GtkWidget *bt , gpointer data)
{
	if(PLAYING == player.state) {
		gst_element_set_state(player.pipe , GST_STATE_PAUSED);
		player.state = PAUSE;
	}
	return 1;
}

int clicked_stop(GtkWidget *bt , gpointer data)
{	
	GtkTreeIter iter;
	if(STOP == player.state)
		return 0;
	g_main_loop_quit(player.loop);
	gst_element_set_state(player.pipe , GST_STATE_NULL);
	destroy_pipe();
	
	player.state = STOP;
	gtk_list_store_prepend(playedsong , &iter);
	gtk_list_store_set(playedsong , &iter , 0 , readysong.name , 1 , readysong.path , -1);

	return 1;
}

int clicked_next(GtkWidget * bt , gpointer data)
{
	clicked_stop(NULL , NULL);
	set_readysong();
	play_readysong();
	return 0;
}

int clicked_previous(GtkWidget * bt , gpointer data)
{
	GtkTreeIter iter;
	if(!gtk_tree_model_get_iter_first(GTK_TREE_MODEL(playedsong) , &iter))
		return 0;
	gtk_tree_model_get(GTK_TREE_MODEL(playedsong) , &iter , 0 , &readysong.name , 1 , &readysong.path , -1);
	gtk_list_store_remove(playedsong , &iter);
	
	if(STOP != player.state) {
		g_main_loop_quit(player.loop);
		gst_element_set_state(player.pipe , GST_STATE_NULL);
		destroy_pipe();
		
		player.state = STOP;
	}
	play_readysong();
	return 1;
}


int push_status_message(gchar * name)
{
	gchar msg[512];
	sprintf(msg , "正在播放: %s" , name);
	gtk_statusbar_push(GTK_STATUSBAR(statusbar) ,                                  //将需要显示的内容push到窗口
			gtk_statusbar_get_context_id(GTK_STATUSBAR(statusbar) , msg) , 
			msg);
	return 0;
}


int Double_clicked(GtkTreeView *view , GtkTreePath *path , GtkTreeViewColumn *col , gpointer data)
{ 
		clicked_stop(NULL , NULL);
		clicked_play(NULL , NULL);
		return 0;
}


int bus_call(GstBus *bus , GstMessage *msg , gpointer date)
{
	switch(GST_MESSAGE_TYPE(msg))
	{
		case GST_MESSAGE_EOS:
			{
				clicked_next(NULL , NULL);
				break;
			}
		case GST_MESSAGE_ERROR:
			{
				printf("Play Error!\n");
				clicked_stop(NULL , NULL);
				break;
			}
		default:
			break;
	}
	return 1;
}


int set_readysong(void)
{
	switch(playmode)
	{
		case ORDER:
			{
				if(gtk_tree_model_iter_next( GTK_TREE_MODEL(lists[player.playlist].store) , &readysong.iter) )
						break;
				else
					gtk_tree_model_get_iter_first(GTK_TREE_MODEL(lists[player.playlist].store) , &readysong.iter);
				break;
			}
		case RANDOM:
			{
				int i;
				int rad;
				srand(time(NULL));
				rad = rand() % 10;
				for(i = 0;i < rad ; i++) {
					
					if(gtk_tree_model_iter_next( GTK_TREE_MODEL(lists[player.playlist].store) , &readysong.iter) )
						continue;
					else
						gtk_tree_model_get_iter_first(GTK_TREE_MODEL(lists[player.playlist].store) , &readysong.iter);
				}
				break;
			}
		case SINGLE:
			{
				break;
			}
		default:
			break;
	}
	gtk_tree_model_get(GTK_TREE_MODEL(lists[player.playlist].store) , &readysong.iter , 
			0 , &readysong.name , 
			1 , &readysong.path , 
			-1);

	return 1;
}

int clicked_radio_buttons(GtkWidget * bt , gpointer data)
{
	if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(Mode1))) {
	
		playmode = ORDER;
		return 1;
	} else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(Mode2))) {
	
		playmode = SINGLE;
		return 1;
	} else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(Mode3))) {
	
		playmode = RANDOM;
		return 1;
	}
	return 0;
}







