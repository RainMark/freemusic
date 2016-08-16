#ifndef __CLIENT_H
#define __CLIENT_H

#include <common.h>
#include <list.h>

extern int server;
extern int online;

#define MAX_LIST (10)
#define MAX_SONG (10)

struct list_st {
	char id[ID_LEN + 1];
	char name[BUF_SIZE];
	char owner_id[ID_LEN + 1];
};

struct song_st {
	char id[ID_LEN + 1];
	char name[BUF_SIZE];
	char path[BUF_SIZE];
	char type[FILE_TYPE_LEN];
	char style[STYLE_LEN];
	char singer_id[ID_LEN + 1];
	char singer_name[ALL_NAME_LEN];
};

int client_init(void);
int clicked_login(GtkWidget *bt, gpointer data);

int new_online_list(const char *name);
int rm_online_list(const char *id);
int fetch_online_list(const char *uid, struct list_st *list);
void add_online_list(struct list_st *lst);
int sync_online_list_all(void);
int sync_online_list(int id);

int fetch_online_song(const char *lid, struct song_st *sst);
void add_online_song(int listid, struct song_st *sst);

int add_online_song_to_list(const char *lid, const char *sid);

int fetch_song(const char *id);

int search_online_song(const char *txt, struct song_st *sst);

#endif /* __CLIENT_H */
