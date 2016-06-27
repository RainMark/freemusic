#include "../include/common.h"
#include "sql.h"
#include "server.h"

int server;
struct sockaddr_in client_addr;
unsigned int client_addr_len;

void kill_server(int sig)
{
	close(server);
	exit(-1);
}

int main (int argc, char *argv[])
{
	struct sockaddr_in server_addr;
	int server_len;

	server = socket ( AF_INET , SOCK_DGRAM , 0 );
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons ( DEFAULT_PORT );
	server_addr.sin_addr.s_addr = htonl ( INADDR_ANY );
	server_len = sizeof ( server_addr );

	sql_init();
	signal ( SIGCHLD , SIG_IGN );
	signal ( SIGINT, kill_server);

	if ( -1 == bind ( server , (struct sockaddr * )&server_addr , server_len ) ) {
		perror ("bind()\n");
		exit(-1);
	}

	while ( 1 ) {

		client_addr_len = sizeof ( client_addr );
		do_communication(server);
	}

	return 0;
}

void do_communication(int sd)
{
	int ret = 0;
	struct message *msg;

	msg = malloc(BUF_SIZE);
	if (!msg)
		exit(-1);

	while (1) {
		/* memset(msg, 0, sizeof (struct message)); */
		/* ret = read(cd, msg, BUF_SIZE); */
		ret = recvfrom(server, msg, BUF_SIZE, 0, (struct sockaddr *)&client_addr, &client_addr_len);
		/* printf("Type:%d Len:%d\n", msg->type, ret); */
		if (!msg->type)
			goto no_data;

		switch (msg->type) {
		case NEW_USER:
			break;
		case LOGIN_IN:
			user_login(msg, ret);
			break;
		case GET_LISTS:
			get_list(msg, ret);
			break;
		case GET_SONGS:
			get_song(msg, ret);
			break;
		case ADD_LIST:
			new_list(msg, ret);
			break;
		case DELETE_LIST:
			rm_list(msg, ret);
			break;
		case ADD_SONG:
			add_song_to_list(msg, ret);
			break;
		case SEARCH_SONG:
			search_song(msg, ret);
			break;
		default:
			break;
		}
	no_data:
		sleep(1);
	}
}

void user_login(struct message *msg, int m_len)
{
	char pass[128];
	char passwd[128];
	char id[ID_LEN + 1];
	int len;

	/* printf("Login\n"); */
	strncpy(id, msg->data, ID_LEN);
	id[ID_LEN] = '\0';
	len =m_len - TYPE_LEN - ID_LEN;
	strncpy(passwd, msg->data + ID_LEN, len);
	passwd[len] = '\0';

	sql_get_user_passwd(id, pass);
	/* printf("user_login:%s\n", pass); */
	/* printf("user_login:%s\n", passwd); */
	if (!strlen(pass)) {
		send_one_message_to_client("invalid user id!", LOGIN_FAILED);
		return;
	}

	if (!strcmp(passwd, pass))
		send_one_message_to_client("Login successful!", LOGIN_SUCCESS);
	else
		send_one_message_to_client("Password failed!", LOGIN_FAILED);

}

void get_list(struct message *msg, int m_len)
{
	char uid[ID_LEN + 1];
	struct message *m;

	m = malloc(BUF_SIZE);
	if (!m)
		exit(-1);

	strncpy(uid, msg->data, ID_LEN);
	uid[ID_LEN] = '\0';
	/* printf("get_list():%s\n", uid); */
	sql_send_list(uid);

	m->type = END;
	/* write(client, m, TYPE_LEN + ID_LEN); */
	sendto(server, m, TYPE_LEN, 0, (struct sockaddr *)&client_addr, client_addr_len);

	free(m);
}

void get_song(struct message *msg, int m_len)
{
	char lid[ID_LEN + 1];
	struct message *m;

	m = malloc(BUF_SIZE);
	if (!m)
		exit(-1);

	strncpy(lid, msg->data, ID_LEN);
	sql_send_song(lid);

	m->type = END;
	sendto(server, m, TYPE_LEN, 0, (struct sockaddr *)&client_addr, client_addr_len);

	free(m);
}

void new_list(struct message *msg, int m_len)
{
	char uid[ID_LEN + 1];
	char lname[ALL_NAME_LEN];
	char buf[BUF_SIZE];

	strncpy(buf, msg->data, m_len - TYPE_LEN);
	strncpy(uid, buf, ID_LEN);
	uid[ID_LEN] = '\0';
	sscanf(buf + ID_LEN + 1, "%[^\n]", lname);
	printf("%s %s\n", uid, lname);
	sql_new_list(uid, lname);
	send_one_message_to_client("New Successful!", ADD_LIST_SUCCESS);
}

void rm_list(struct message *msg, int m_len)
{
	char lid[ID_LEN + 1];

	strncpy(lid, msg->data, ID_LEN);
	lid[ID_LEN] = '\0';
	sql_rm_list(lid);
	send_one_message_to_client("Remove Successful!", DELETE_LIST_SUCCESS);
}

void add_song_to_list(struct message *msg, int m_len)
{
	char lid[ID_LEN + 1];
	char sid[ID_LEN + 1];
	char buf[BUF_SIZE];

	strncpy(buf, msg->data, m_len - TYPE_LEN);
	sscanf(buf, "%s%s", lid, sid);
	printf("%s", buf);
	sql_add_song_to_list(lid, sid);
	send_one_message_to_client("Add to Successful!", ADD_SONG_SUCCESS);
}




void search_song(struct message *msg, int m_len)
{
	/* Why use ALL_NAME_LEN macro will cause segment fault ?*/
	char txt[BUF_SIZE];

	strncpy(txt, msg->data, m_len - TYPE_LEN);
	txt[m_len - TYPE_LEN] = '\0';
	sql_search_song(txt);
	send_one_message_to_client("Search Successful!", END);
}




void send_one_message_to_client(const char *str, int type)
{
	struct message *m;

	m = malloc(BUF_SIZE);
	if (!m)
		exit(-1);

	m->type = type;
	strcpy(m->data, str);
	sendto(server, m, strlen(str) + TYPE_LEN, 0, (struct sockaddr *)&client_addr, client_addr_len);

	free(m);
}
