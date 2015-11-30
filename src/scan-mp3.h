#ifndef _SCAN_MP3_

#define _SCAN_MP3_

#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct _music {

	char name[256];
	char path[256];
	int id;

}Music;

extern int music_number;
extern Music music_lib[1000];

int scan_dir(char *dir);
int is_mp3(char *file_name);


#endif
