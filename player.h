#ifndef _player
#define _player

#include <gst/gst.h>
#include <gtk/gtk.h>
#include <stdio.h>

enum _playstate {

	PLAYING,
	PAUSE,
	STOP
};

enum _playmode{

	SINGLE ,
	ORDER , 
	RANDOM
};

extern enum _playmode playmode;

typedef struct _player {

	GstElement  * pipe;
	GstElement  * source;
	GstElement  * decoder;
	GstElement  * sink;
	GstBus      * bus;
	GMainLoop   * loop;
	int playlist;
	enum _playstate state;         //0<==>notplay    1<==>play

}Player;

extern Player player;

typedef struct _readysong {

	gchar * name;
	gchar * path;
	GtkTreeIter   iter;

}ReadySong;

extern ReadySong readysong;


extern GtkListStore  * playedsong;


int init_player(void);

int create_pipe(void);
int destroy_pipe(void);

int play_readysong(void);
int clicked_play(GtkWidget *bt , gpointer data);
int clicked_pause(GtkWidget *bt , gpointer data);
int clicked_stop(GtkWidget *bt , gpointer data);
int bus_call(GstBus *bus , GstMessage *msg , gpointer date);
int set_readysong(void);
int clicked_next(GtkWidget * bt , gpointer data);
int clicked_previous(GtkWidget * bt , gpointer data);
int Double_clicked(GtkTreeView *view , GtkTreePath *path , GtkTreeViewColumn *col , gpointer data);
int clicked_radio_buttons(GtkWidget * bt , gpointer data);
int push_status_message(gchar * name);

#endif
