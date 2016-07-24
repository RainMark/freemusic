#include "client.h"
#include "list.h"

int server = 0;
int online = 0;
static char SERVER_IP[32] = "192.168.12.192";
struct sockaddr_in server_addr;
unsigned int addr_len = 0;
char currnet_uid[ID_LEN + 1];

int client_init(void)
{
	int server_sd;

	server_sd = socket ( AF_INET , SOCK_DGRAM , 0 );
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons ( DEFAULT_PORT );
	server_addr.sin_addr.s_addr = inet_addr ( SERVER_IP );
	addr_len = sizeof ( server_addr );

	return server_sd;
}

int login(const char *id, const char *passwd)
{

	struct message *msg;
	int len = 0;
	int ret, r = 0;

	msg = malloc(BUF_SIZE);
	if (!msg)
		exit(-1);

	msg->type = LOGIN_IN;
	strcpy(msg->data, id);
	strcpy(msg->data + ID_LEN, passwd);

	/* printf("server = %d\n", server); */
	len = TYPE_LEN + ID_LEN + strlen(passwd);
	/* ret = write(server, msg, len); */
	ret = sendto(server, msg, len, 0, (struct sockaddr *)&server_addr, addr_len);
	if (!ret) {
		r = -1;
		goto failed;
	}

	/* printf("Write %d byte!\n", ret); */

	/* ret = read(server, msg, BUF_SIZE); */
	ret = recvfrom(server, msg, BUF_SIZE, 0, (struct sockaddr *)&server_addr, &addr_len);
	msg->data[ret - TYPE_LEN] = '\0';

	/* printf("Receive %d byte!\n", ret); */
	/* printf("Type: %d Data:%s\n", msg->type, msg->data); */

	if (ret && LOGIN_SUCCESS == msg->type) {
		online = 1;
		r = 0;
		printf("%s\n", msg->data);
	} else {
		printf("%s\n", msg->data);
		r = 2;
		goto failed;
	}
failed:
	free(msg);
	return r;
}

int sync_online_list_all(void)
{
	struct list_st *lst;
	int ret = 0;
	int i;

	if (!online)
		return ret;

	lst = malloc(MAX_LIST * sizeof(struct list_st));
	ret = fetch_online_list(currnet_uid, lst);
	for (i = 0; i < ret; i++)
		add_online_list(&lst[i]);

	free(lst);
	return ret;
}

int clicked_login(GtkWidget *bt, gpointer data)
{
	const char *id = "0000000001";
	const char *passwd = "rain";

	/* printf("Clicked login!\n"); */
	if (online)
		return 1;

	if(!login(id, passwd)) {
		strcpy(currnet_uid, id);
		list_store_clear_all();
		return sync_online_list_all();
	} else {
		return 1;
	}
}

int fetch_online_list(const char *uid, struct list_st *list)
{
	struct message *m;
	int len = 0;
	int ret = 0;
	int i = 0;

	if (strlen(uid) != ID_LEN)
		return 0;

	m = malloc(BUF_SIZE);
	if (!m)
		exit(-1);

	m->type = GET_LISTS;
	strcpy(m->data, uid);
	/* ret = write(server, m, ID_LEN + TYPE_LEN); */
	ret = sendto(server, m, ID_LEN + TYPE_LEN, 0, (struct sockaddr *)&server_addr, addr_len);
	/* printf("list,write:%d bytes\n", ret); */

	while (1) {
		/* ret = read(server, m, BUF_SIZE); */
		ret = recvfrom(server, m, BUF_SIZE, 0, (struct sockaddr *)&server_addr, &addr_len);
		if (END == m->type)
			break;

		if (LISTS != m->type)
			continue;

		strncpy(list[i].id, m->data, ID_LEN);
		list[i].id[ID_LEN] = '\0';
		strncpy(list[i].owner_id, m->data + ID_LEN, ID_LEN);
		list[i].owner_id[ID_LEN] = '\0';
		len = ret - TYPE_LEN - 2 * ID_LEN;
		strncpy(list[i].name, m->data + 2 * ID_LEN, len);
		list[i].name[len] = '\0';
		printf("id:%s owne_id:%s name:%s\n", list[i].id, list[i].owner_id, list[i].name);

		i++;
		if (i >= MAX_LIST)
			break;
	}

	free(m);
	return i;
}

void add_online_list(struct list_st *lst)
{
	GtkTreeIter iter;

	/* printf("Adding\n"); */
	/* printf("id:%s owne_id:%s name:%s\n", lst->id, lst->owner_id, lst->name); */
	strcpy(lists[listssum].listname, lst->name);
	strcpy(lists[listssum].id, lst->id);
	strcpy(lists[listssum].owner_id, lst->owner_id);

	lists[listssum].store = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_STRING);
	lists[listssum].songsum = 0;

	gtk_list_store_append(lview_store, &iter);
	gtk_list_store_set(lview_store, &iter, 0, lists[listssum].listname, -1);
	listssum++;
}

int fetch_online_song(const char *lid, struct song_st *sst)
{
	struct message *m;
	int len = 0;
	int ret = 0;
	int i = 0;
	char buf[BUF_SIZE];

	printf("Fetching\n");
	printf("list id:%s\n", lid);
	if (strlen(lid) != ID_LEN)
		return 0;

	printf("mallocing\n");
	m = malloc(BUF_SIZE);
	if (!m)
		exit(-1);

	m->type = GET_SONGS;
	strcpy(m->data, lid);
	ret = sendto(server, m, ID_LEN + TYPE_LEN, 0, (struct sockaddr *)&server_addr, addr_len);
	printf("Write:%d bytes\n", ret);

	while (1) {
		/* ret = read(server, m, BUF_SIZE); */
		ret = recvfrom(server, m, BUF_SIZE, 0, (struct sockaddr *)&server_addr, &addr_len);
		if (END == m->type)
			break;

		if (SONGS != m->type)
			continue;

		strncpy(sst[i].id, m->data, ID_LEN);
		sst[i].id[ID_LEN] = '\0';
		len = ret - TYPE_LEN - ID_LEN;
		strncpy(buf, m->data + ID_LEN, len);
		buf[len] = '\0';
		sscanf(buf, "%[^\n]%s%s%s", sst[i].name, sst[i].style, sst[i].type, sst[i].singer_id);
		strcpy(sst[i].path, sst[i].id);
		/* printf("%s %s %s %s %s %s\n", sst[i].id, sst[i].name, sst[i].style, sst[i].type, sst[i].singer_id, sst[i].path); */

		i++;
		if (i >= MAX_SONG)
			break;
	}

	free(m);
	return i;

}

void add_online_song(int listid, struct song_st *sst)
{
	GtkTreeIter iter;

	gtk_list_store_append(lists[listid].store, &iter);
	gtk_list_store_set(lists[listid].store, &iter, 0, sst->name, -1);
	gtk_list_store_set(lists[listid].store, &iter, 1, sst->id, -1);


	lists[listid].songsum++;
}

int new_online_list(const char *name)
{
	struct message *m;
	char buf[BUF_SIZE];
	int ret;

	if (!online)
		return -1;

	m = malloc(BUF_SIZE);
	if (!m)
		exit(-1);

	if (strlen(name) > ALL_NAME_LEN)
		return -1;

	sprintf(buf, "%s\n%s\n", currnet_uid, name);
	m->type = ADD_LIST;
	strcpy(m->data, buf);

	sendto(server, m, BUF_SIZE, 0, (struct sockaddr *)&server_addr, addr_len);

	ret = recvfrom(server, m, BUF_SIZE, 0, (struct sockaddr *)&server_addr, &addr_len);
	strncpy(buf, m->data, ret - TYPE_LEN);
	buf[strlen(buf)] = '\0';
	if (ADD_LIST_SUCCESS == m->type)
		ret = 0;
	else
		ret = -1;

	printf("%s\n", buf);
	free(m);
	return ret;
}

int rm_online_list(const char *id)
{
	struct message *m;
	int ret;
	char buf[BUF_SIZE];

	m = malloc(BUF_SIZE);
	if (!m)
		exit(-1);

	m->type = DELETE_LIST;
	sprintf(m->data, "%s\n", id);
	sendto(server, m, ID_LEN + TYPE_LEN + 1, 0, (struct sockaddr *)&server_addr, addr_len);

	ret = recvfrom(server, m, BUF_SIZE, 0, (struct sockaddr *)&server_addr, &addr_len);
	strncpy(buf, m->data, ret - TYPE_LEN);
	buf[strlen(buf)] = '\0';

	if (DELETE_LIST_SUCCESS == m->type)
		ret = 0;
	else
		ret = -1;

	printf("%s\n", buf);
	free(m);
	return ret;
}

int sync_online_list(int id)
{
	int i;
	struct song_st *sst;
	int ret;

	sst = malloc(MAX_SONG * sizeof(struct song_st));
	if (!sst)
		exit(-1);

	ret = fetch_online_song(lists[id].id, sst);
	for (i = 0; i < ret; i++)
		add_online_song(id, sst + i);

	free(sst);
	return i;
}

int add_online_song_to_list(const char *lid, const char *sid)
{
	struct message *m;
	char buf[BUF_SIZE];
	int ret;

	m = malloc(BUF_SIZE);
	if (!m)
		exit(-1);
	sprintf(buf, "%s\n%s\n", lid, sid);
	m->type = ADD_SONG;
	strcpy(m->data, buf);
	sendto(server, m, strlen(buf) + TYPE_LEN, 0, (struct sockaddr *)&server_addr, addr_len);

	ret = recvfrom(server, m, BUF_SIZE, 0, (struct sockaddr *)&server_addr, &addr_len);
	strncpy(buf, m->data, ret - TYPE_LEN);
	buf[ret - TYPE_LEN] = '\0';

	if (ADD_SONG_SUCCESS == m->type)
		ret = 0;
	else
		ret = -1;

	printf("%s\n", buf);
	free(m);
	return ret;
}

int fetch_song(const char *id)
{
	FILE *f;
	char buf[BUF_SIZE];
	int ret;
	char *server_user = "rain";
	char *server_pass = "zby2010";
	char *server_dir = "~/flib";
	char *cache_dir = "/tmp";

	sprintf(buf, "%s/%s", cache_dir, id);
	if ((f = fopen(buf, "r")))
		goto file_rlease;

	sprintf(buf, "./get.sh %s %s %s %s %s %s", SERVER_IP, server_user, server_pass, server_dir, id, cache_dir);
	printf("%s\n", buf);
	system(buf);

	return 0;
file_rlease:
	fclose(f);
	return 0;
}


int search_online_song(const char *txt, struct song_st *sst)
{
	struct message *m;
	int i = 0;
	int ret = 0;
	char buf[BUF_SIZE];
	int len;

	m = malloc(BUF_SIZE);
	if (!m)
		exit(-1);
	m->type = SEARCH_SONG;
	strcpy(m->data, txt);

	ret = sendto(server, m, ID_LEN + ALL_NAME_LEN, 0, (struct sockaddr *)&server_addr, addr_len);

	while (1) {
		ret = recvfrom(server, m, BUF_SIZE, 0, (struct sockaddr *)&server_addr, &addr_len);
		if (END == m->type)
			break;

		if (SONGS != m->type)
			continue;

		strncpy(sst[i].id, m->data, ID_LEN);
		sst[i].id[ID_LEN] = '\0';
		len = ret - TYPE_LEN - ID_LEN;
		strncpy(buf, m->data + ID_LEN, len);
		buf[len] = '\0';
		sscanf(buf, "%[^\n]%s%s", sst[i].name, sst[i].style, sst[i].type);
		sscanf(buf + strlen(sst[i].name) + strlen(sst[i].style) + strlen(sst[i].type) + 3, "%[^\n]", sst[i].singer_name);
		strcpy(sst[i].path, sst[i].id);
		printf("%s %s %s %s %s %s\n", sst[i].id, sst[i].name, sst[i].style, sst[i].type, sst[i].singer_name, sst[i].path);

		i++;
		if (i >= MAX_SONG)
			break;
	}

	free(m);
	return i;
}
