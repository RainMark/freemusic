#include "scan-mp3.h"


int music_number = 0;
Music music_lib[1000];


int scan_dir(char *dir)
{
	DIR *dp;
	struct dirent *entry;
	struct stat statbuf;

	if(NULL == (dp = opendir(dir))) {
		printf("Can not open %s dir!\n",dir);
		return 0;
	}

	chdir(dir);

	while(NULL != (entry = readdir(dp)) ) {
	
		lstat(entry->d_name , &statbuf);
		if(S_ISDIR(statbuf.st_mode) && '.' != entry->d_name[0]) {  //非隐藏目录才进入扫描
		
			scan_dir(entry->d_name);
		
		}else if(S_ISREG(statbuf.st_mode))	{
		
			if(is_mp3(entry->d_name)) {
			
				char buf[512];
				getcwd(buf , sizeof (buf));
				if(NULL != buf) {
				
					strcpy(music_lib[music_number].path , buf);
					strcpy(music_lib[music_number].name , entry->d_name);
					music_lib[music_number].id = music_number;
					music_number++;
				}
			}
		}
	}
	chdir("..");
	closedir(dp);
	return 1;
}
int is_mp3(char *file_name)
{
	char *p_end = file_name + strlen(file_name) - 1;
	if((p_end - file_name) < 3)
		return 0;
	if('3' == *p_end 
			&& 'p' == *(p_end - 1) 
			&& 'm' == *(p_end - 2) 
			&& '.' == *(p_end - 3))
		return 1;
	else
		return 0;
}

