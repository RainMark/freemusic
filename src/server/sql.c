#include "sql.h"

MYSQL *connectp;
MYSQL_RES *resp;
MYSQL_ROW row;

static unsigned long list_cnt = 5;
static unsigned long user_cnt = 3;
static unsigned long song_cnt = 3;
static unsigned long singer_cnt = 3;

int sql_init(void)
{
	connectp = mysql_init(NULL);
	if (!connectp) {
		fprintf(stderr, "Failed to init a connection struct!\n");
		exit(-1);
	}

	if (!mysql_real_connect(connectp, HOST, USER, PASSWORD, DATABASE, 0, NULL, 0)) {
		fprintf(stderr, "Connect Failed!\n");
		if (mysql_errno(connectp))
			fprintf(stderr, "Failed!:%d:%s\n", mysql_errno(connectp), mysql_error(connectp));
		exit(-1);
	}

	/* 设置数据库默认字符集 */
        if (mysql_set_character_set(connectp, "utf8" )) {
		fprintf (stderr , "Failed:%s\n", mysql_error(connectp)) ;
        }

	return 0;
}

void sql_get_user_passwd(char user_id[10], char password[10])
{
	char cmd[512];
	int ret;

	sprintf(cmd, "select user_passwd from user where user_id=%s", user_id);
	ret = mysql_query(connectp, cmd);
	if (ret) {
		printf("Select error: %s\n", mysql_error(connectp));
	} else {
		resp = mysql_use_result(connectp);
		if (resp) {
			row = mysql_fetch_row(resp);
			/* printf("%s\n", row[0]); */
			strcpy(password, row[0]);
		}

		if (mysql_errno(connectp))
			fprintf(stderr, "Get data failed: %s\n", mysql_error(connectp));
		mysql_free_result(resp);
	}
}

void sql_send_list(const char *uid)
{
	char cmd[512];
	int ret;
	struct message *m;

	m = malloc(BUF_SIZE);
	if (!m)
		exit(-1);

	/* printf("enter sql_send_list()\n"); */
	sprintf(cmd, "select list_id,owner_id,list_name from list where owner_id=%s", uid);
	ret = mysql_query(connectp, cmd);
	if (ret) {
		printf("Select error: %s\n", mysql_error(connectp));
	} else {
		resp = mysql_use_result(connectp);
		if (resp) {
			while ((row = mysql_fetch_row(resp))) {
				m->type = LISTS;
				strncpy(m->data, row[0], ID_LEN);
				strncpy(m->data + ID_LEN, row[1], ID_LEN);
				strcpy(m->data + 2 * ID_LEN, row[2]);
				/* write(cd, m, TYPE_LEN + 2 * ID_LEN + NAME_LEN); */
				sendto(server, m, TYPE_LEN + 2 * ID_LEN + NAME_LEN, 0, (struct sockaddr *)&client_addr, client_addr_len);
				/* printf("%s %s %s\n", row[0], row[1], row[2]); */
			}
		}

		if (mysql_errno(connectp))
			fprintf(stderr, "Get data failed: %s\n", mysql_error(connectp));
		mysql_free_result(resp);
	}

	free(m);
}

void sql_new_list(const char *uid, const char *name)
{
	/* unsigned long lines = 0;; */
	char cmd[512];
	int ret;
	char buf[ID_LEN + 1];
	char lid[ID_LEN + 1];
	int i, len;


	/* sprintf(cmd, "select count(*) from list"); */
	/* ret = mysql_query(connectp, cmd); */
	/* if (ret) { */
	/* 	printf("Select error: %s\n", mysql_error(connectp)); */
	/* } else { */
	/* 	resp = mysql_use_result(connectp); */
	/* 	if (resp) { */
	/* 		row = mysql_fetch_row(resp); */
	/* 		sscanf(row[0], "%lu", &lines); */
	/* 	} */

	/* 	if (mysql_errno(connectp)) */
	/* 		fprintf(stderr, "Get data failed: %s\n", mysql_error(connectp)); */
	/* 	mysql_free_result(resp); */
	/* } */

	sprintf(buf, "%lu", ++list_cnt);
	len = strlen(buf);
	for (i = 0; i < ID_LEN - len; i++)
		lid[i] = '0';
	lid[i] = '\0';
	strcat(lid, buf);
	sprintf(cmd, "insert into list(list_id,list_name,owner_id) values ('%s','%s','%s')", lid, name, uid);
	printf("%s\n", cmd);
	ret = mysql_query(connectp, cmd);
	if (ret) {
		printf("Insert error: %s\n", mysql_error(connectp));
	} else {
		if (mysql_errno(connectp))
			fprintf(stderr, "Get data failed: %s\n", mysql_error(connectp));
	}
}

void sql_rm_list(const char *lid)
{
	char cmd[512];
	int ret;

	sprintf(cmd, "delete from list where list_id = '%s'", lid);
	ret = mysql_query(connectp, cmd);
	if (ret) {
		printf("Delete error: %s\n", mysql_error(connectp));
	} else {
		if (mysql_errno(connectp))
			fprintf(stderr, "Get data failed: %s\n", mysql_error(connectp));
	}
}

void sql_add_song_to_list(const char *lid, const char *sid)
{
	char cmd[512];
	int ret;

	sprintf(cmd, "insert into songs_of_lists(list_id,song_id) values ('%s','%s')", lid, sid);
	printf("%s\n", cmd);
	ret = mysql_query(connectp, cmd);
	if (ret) {
		printf("Insert error: %s\n", mysql_error(connectp));
	} else {
		if (mysql_errno(connectp))
			fprintf(stderr, "Get data failed: %s\n", mysql_error(connectp));
	}
}

void sql_send_song(const char *lid)
{
	char cmd[512];
	int ret;
	struct message *m;
	char buf[BUF_SIZE];

	m = malloc(BUF_SIZE);
	if (!m)
		exit(-1);

	/* printf("enter sql_send_list()\n"); */
	sprintf(cmd, "select s.song_id,s.song_name,s.song_style,s.song_ftype,s.song_singer from song s,list l,songs_of_lists sol where s.song_id = sol.song_id and l.list_id = sol.list_id and sol.list_id = %s", lid);
	ret = mysql_query(connectp, cmd);
	if (ret) {
		printf("Select error: %s\n", mysql_error(connectp));
	} else {
		resp = mysql_use_result(connectp);
		if (resp) {
			while ((row = mysql_fetch_row(resp))) {
				/* printf("%s %s %s %s %s\n", row[0], row[1], row[2], row[3], row[4]); */
				m->type = SONGS;
				strncpy(m->data, row[0], ID_LEN);
				sprintf(buf, "%s\n%s\n%s\n%s", row[1], row[2], row[3], row[4]);
				strcpy(m->data + ID_LEN, buf);
				sendto(server, m, TYPE_LEN + 2 * ID_LEN + NAME_LEN, 0, (struct sockaddr *)&client_addr, client_addr_len);
			}
		}

		if (mysql_errno(connectp))
			fprintf(stderr, "Get data failed: %s\n", mysql_error(connectp));
		mysql_free_result(resp);
	}

	free(m);
}


int is_grep_match(const char *target, const char *str)
{
	FILE *f;
	char buf[BUF_SIZE];

	sprintf(buf, "echo \"%s\" | grep \"%s\"", target, str);
	printf("%s\n", buf);
	f = popen(buf, "r");
	memset(buf, 0, BUF_SIZE);
	fgets(buf, BUF_SIZE, f);
	printf("%lu\n", strlen(buf));
	if (strlen(buf))
		return 1;
	else
		return 0;
}

void sql_search_song(const char *txt)
{
	char cmd[512];
	int ret;
	struct message *m;
	char buf[BUF_SIZE];

	m = malloc(BUF_SIZE);
	if (!m)
		exit(-1);

	sprintf(cmd, "select s.song_id,s.song_name,s.song_style,s.song_ftype,sg.singer_name from song s, singer sg where sg.singer_id = s.song_singer");
	ret = mysql_query(connectp, cmd);
	if (ret) {
		printf("Select error: %s\n", mysql_error(connectp));
	} else {
		resp = mysql_use_result(connectp);
		if (resp) {
			while ((row = mysql_fetch_row(resp))) {
				printf("%s %s %s %s %s\n", row[0], row[1], row[2], row[3], row[4]);
				if (!is_grep_match(row[1], txt))
					continue;

				m->type = SONGS;
				strncpy(m->data, row[0], ID_LEN);
				sprintf(buf, "%s\n%s\n%s\n%s", row[1], row[2], row[3], row[4]);
				strcpy(m->data + ID_LEN, buf);
				sendto(server, m, BUF_SIZE, 0, (struct sockaddr *)&client_addr, client_addr_len);
			}
		}

		if (mysql_errno(connectp))
			fprintf(stderr, "Get data failed: %s\n", mysql_error(connectp));
		mysql_free_result(resp);
	}

	free(m);
}
