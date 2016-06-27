#ifndef __COMMON_H
#define __COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <unistd.h>
#include <sys/fcntl.h>
#include <signal.h>
#include <sys/wait.h>


#define DEFAULT_PORT		(6666)

/* Type of socket packed. */
#define LOGIN_IN		(1)
#define LOGIN_SUCCESS		(2)
#define LOGIN_FAILED		(3)
#define NEW_USER		(4)
#define NEW_USER_SUCCESS	(5)
#define NEW_USER_FAILED		(6)
#define GET_LISTS		(7)
#define LISTS			(8)
#define GET_SONGS		(9)
#define SONGS			(10)
#define ADD_LIST		(11)
#define ADD_LIST_SUCCESS	(12)
#define ADD_LIST_FAILED	(13)
#define DELETE_LIST		(14)
#define DELETE_LIST_SUCCESS	(15)
#define DELETE_LIST_FAILED	(16)
#define ADD_SONG		(17)
#define ADD_SONG_SUCCESS	(18)
#define ADD_SONG_FAILED		(19)
#define DELETE_SONG		(20)
#define DELETE_SONG_SUCCESS	(21)
#define DELETE_SONG_FAILED	(22)
#define SEARCH_SONG		(23)
#define PLAY_SONG		(24)
#define END			(25)

/* Define length of messages. */
#define BUF_SIZE		(1024)
#define TYPE_LEN		(1)
#define ID_LEN			(10)
#define PASS_LEN		(128)
#define ALL_NAME_LEN		(128)
#define FILE_TYPE_LEN		(16)
#define STYLE_LEN		(16)

struct message {
	char type;
	char data[1];
} __attribute__((packed));

#endif /* __COMMON_H */
