#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <player.h>
#include <list.h>
#include <main.h>

ReadySong readysong;
enum _playmode playmode;
Player player;
GtkListStore * playedsong;


static int ready_song(void);
static int play_ready_song(void);

int init_player(void)
{
	readysong.path = "233";
	player_init(finish_call);
	player.state = STOP;
	playmode = ORDER;
	return 1;
}

int play_ready_song(void)
{
	gchar buf[512];
	char *cache_dir = "/tmp";

	sprintf(buf, "%s/%s", cache_dir, readysong.path);
	player.pid = play(buf);
	player.state = PLAYING;
	sprintf(buf, "Playing: %s", readysong.name);
	push_status_message(GTK_STATUSBAR(statusbar), buf);
	return 0;
}

int clicked_play(GtkWidget *bt , gpointer data)
{
	GtkTreeIter iter;
	int listid;
	gchar *listname;
	gchar *id;
	gchar *name;

	get_info_from_sview(&iter, &name, &id);
	if ( PLAYING == player.state) {
		clicked_stop(NULL, NULL);
		return 0;
	}

	if (!strcmp(id, readysong.path))
	    clicked_stop(NULL, NULL);

	get_info_from_sview(&readysong.iter, &readysong.name, &readysong.path);
	get_info_from_lview(&iter, &listname, &player.playlist);
	if (!fetch_song(readysong.path))
		play_ready_song();

	return 0;
}

int clicked_stop(GtkWidget *bt , gpointer data)
{
	stop(player.pid);
	player.state = STOP;
	push_status_message(GTK_STATUSBAR(statusbar), "");
	return 1;
}

int clicked_next(GtkWidget * bt , gpointer data)
{
	/* if(STOP != player.state) */
	/* 	clicked_stop(NULL , NULL); */
	/* ready_song(); */
	/* play_ready_song(); */
	return 0;
}

int clicked_previous(GtkWidget * bt , gpointer data)
{
	/* GtkTreeIter iter; */

	/* if(!gtk_tree_model_get_iter_first(GTK_TREE_MODEL(playedsong) , &iter)) */
	/* 	return 0; */
	/* gtk_tree_model_get(GTK_TREE_MODEL(playedsong) , &iter , 0 , &readysong.name , 1 , &readysong.path , -1); */
	/* gtk_list_store_remove(playedsong , &iter); */

	/* if (STOP != player.state) */
	/* 	clicked_stop(NULL, NULL); */
	/* play_ready_song(); */

	return 1;
}


void push_status_message(GtkStatusbar *stb, gchar * msg)
{
	gtk_statusbar_push(GTK_STATUSBAR(stb), gtk_statusbar_get_context_id(GTK_STATUSBAR(stb), msg), msg);
}


int Double_clicked(GtkTreeView *view , GtkTreePath *path , GtkTreeViewColumn *col , gpointer data)
{
	if (STOP != player.state)
		clicked_stop(NULL, NULL);
	clicked_play(NULL, NULL);
	return 0;
}


void finish_call(struct private_data pd)
{
	clicked_next(NULL, NULL);
}


int ready_song(void)
{
	/* int i; */
	/* int rad; */

	/* switch(playmode) { */
	/* case ORDER: */
	/* 	if(gtk_tree_model_iter_next( GTK_TREE_MODEL(lists[player.playlist].store) , &readysong.iter) ) */
	/* 		break; */
	/* 	else */
	/* 		gtk_tree_model_get_iter_first(GTK_TREE_MODEL(lists[player.playlist].store) , &readysong.iter); */
	/* 	break; */
	/* case RANDOM: */
	/* 	srand(time(NULL)); */
	/* 	rad = rand() % 10; */
	/* 	for(i = 0;i < rad ; i++) { */
	/* 		if(gtk_tree_model_iter_next( GTK_TREE_MODEL(lists[player.playlist].store) , &readysong.iter) ) */
	/* 			continue; */
	/* 		else */
	/* 			gtk_tree_model_get_iter_first(GTK_TREE_MODEL(lists[player.playlist].store) , &readysong.iter); */
	/* 	} */
	/* 	break; */
	/* case SINGLE: */
	/* 	break; */
	/* default: */
	/* 	break; */
	/* } */

	/* gtk_tree_model_get(GTK_TREE_MODEL(lists[player.playlist].store), &readysong.iter, */
	/* 		0, &readysong.name, */
	/* 		1, &readysong.path, */
	/* 		-1); */
	return 1;
}

int clicked_change_play_seq(GtkWidget * bt , gpointer data)
{
	gchar buf[512];

	playmode = ++playmode % 3;
	strcpy(buf, "Current play sequence: ");

	switch (playmode) {
	case ORDER:
		strcat(buf, "order");
		break;
	case SINGLE:
		strcat(buf, "single");
		break;
	case RANDOM:
		strcat(buf, "random");
		break;
	default:
		break;
	}

	push_status_message(GTK_STATUSBAR(statusbar), buf);

	return 1;
}
