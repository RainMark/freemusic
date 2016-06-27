//By Rain 2016.2.12

#ifndef _API_H
#define _API_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <signal.h>
#include <sys/wait.h>

#ifndef MAX_NAME_SIZE

#define MAX_NAME_SIZE (512)

#endif  //MAX_NAME_SIZE 

struct private_data {
	char file_name[ MAX_NAME_SIZE ];
};

typedef void (*call_back_func_t)( struct private_data );

ssize_t find( char *, char *, char (*)[MAX_NAME_SIZE] );
void player_init( call_back_func_t func );
int play( const char *file );
int stop( pid_t pid );

#endif  //_API_H 
