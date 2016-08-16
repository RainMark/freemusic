#ifndef __SQL_H
#define __SQL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>
#include <common.h>
#include <server.h>


#define HOST ("localhost")
#define USER ("root")
#define PASSWORD ("root")
#define DATABASE ("free_music")

int sql_init(void);
void sql_get_user_passwd(char user_id[10], char password[10]);
void sql_send_list(const char *);
void sql_send_song(const char *);
void sql_new_list(const char *, const char *);
void sql_rm_list(const char *);
void sql_add_song_to_list(const char *, const char *);
void sql_search_song(const char *);
int is_grep_match(const char *target, const char *str);

#endif /* __SQL_H */
