#ifndef __SERVER_H
#define __SERVER_H
#include "../include/common.h"

extern int server;
extern struct sockaddr_in client_addr;
extern unsigned int client_addr_len;

void do_communication(int);
void user_login(struct message *, int);
void send_one_message_to_client(const char *, int);
void get_list(struct message *, int);
void get_song(struct message *, int);
void new_list(struct message *, int);
void rm_list(struct message *, int);
void add_song_to_list(struct message *, int);
void search_song(struct message *, int);


#endif /* __SERVER_H */
