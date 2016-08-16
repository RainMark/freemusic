/*By Rain 2016.2.12*/

#include <mpg123_api.h>

static int include_key( char *target , char *key )
{
	int i,k;
	int find_flag;

	for ( i = 0; '\0' != target[i]; i++ ) {
		find_flag = 1;
		for ( k = 0; '\0' != key[k]; k++ ) {
			if ( target[i+k] != key[k] ) {
				find_flag = 0;
				break;
			}
		}

		if ( find_flag )
			break;
	}

	return find_flag;
}

ssize_t find( char *dir , char *key , char (*result)[MAX_NAME_SIZE] )
{
	DIR *dirp;
	char current_dir[ MAX_NAME_SIZE ];
	struct dirent *entry;
	struct stat stat_buf;
	static ssize_t cnt = 0;

	if (NULL == (dirp = opendir(dir))) {
		printf("Can't open %s!\n",dir);
		return -1;
	}

	chdir(dir);
	while (NULL != (entry = readdir(dirp)) ) {
		lstat( entry->d_name , &stat_buf );
		/*printf ( "%s\n", entry->d_name );*/
		getcwd( current_dir , sizeof current_dir );
		if ( S_ISDIR(stat_buf.st_mode) && '.' != entry->d_name[0] ) /*非隐藏目录才进入扫描*/
			find( entry->d_name , key , result );
		else if ( S_ISREG(stat_buf.st_mode) ) {
			if ( include_key( entry->d_name , key )) {
				strcpy( result[cnt], current_dir );
				strcat( result[cnt] , "/" );
				strcat( result[cnt++], entry->d_name );
				/*printf("%s/%s\n", current_dir , entry->d_name );*/
			}
		}
	}

	chdir("..");
	closedir(dirp);
	return cnt;
}


/*********************************************************
 *		Player
 *********************************************************/
static char *mpg123 = "/usr/bin/mpg123";
static call_back_func_t sig_call_back;
static struct private_data data;
static pid_t mpg123_pid;

static void sig_kill_mpg123_handler( int sig )
{
	kill( mpg123_pid , SIGKILL );
	exit( 0 );
}

static void sig_play_over_handler( int sig )
{
	sig_call_back( data );
}

static int playing( const char *file )
{
	char command[MAX_NAME_SIZE];
	int status;

	sprintf( command , "exec %s \"%s\" > /dev/null 2>&1" , mpg123 , file );
	mpg123_pid = fork();
	if ( 0 == mpg123_pid )
		execl("/bin/sh" , "sh" , "-c" , command , (char *)0);
	wait( &status );
	return 0;
}


void player_init( call_back_func_t func )
{
	sig_call_back = func;
	signal( SIGUSR2 , sig_play_over_handler );
	signal( SIGCHLD , SIG_IGN );
	signal( SIGUSR1 , sig_kill_mpg123_handler );
}

pid_t play( const char *file )
{
	pid_t pid;

       	/*在还未fork() 之前设置数据,避免在之后进程间传输数据*/
	strcpy( data.file_name , file );
	pid = fork();
	if ( pid > 0 )
		return pid;
	if ( !playing( file ) )
		kill( getppid() , SIGUSR2 );
	exit(0);
}

int stop( pid_t pid )
{
	return kill( pid , SIGUSR1 );
}
