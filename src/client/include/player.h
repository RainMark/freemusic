#ifndef _player
#define _player

#include <gtk/gtk.h>
#include <mpg123_api.h>

enum _playstate {
	PLAYING,
	PAUSE,
	STOP
};

enum _playmode{
	SINGLE,
	ORDER,
	RANDOM
};

extern enum _playmode playmode;

typedef struct _player {
	pid_t pid;
	int playlist;
	enum _playstate state;
}Player;

extern Player player;

typedef struct _readysong {
	gchar *name;
	gchar *path;
	GtkTreeIter   iter;
}ReadySong;

extern ReadySong readysong;
extern GtkListStore  *playedsong;

int init_player(void);
void finish_call(struct private_data pd);
int clicked_play(GtkWidget *bt , gpointer data);
int clicked_stop(GtkWidget *bt , gpointer data);
int clicked_next(GtkWidget * bt , gpointer data);
int clicked_previous(GtkWidget * bt , gpointer data);
int Double_clicked(GtkTreeView *view , GtkTreePath *path , GtkTreeViewColumn *col , gpointer data);
int clicked_change_play_seq(GtkWidget *bt, gpointer data);
void push_status_message(GtkStatusbar *stb, gchar *message);

#endif
