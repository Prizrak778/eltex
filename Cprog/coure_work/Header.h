#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#define port_serv 50000
#define MAX_TIME_T 20
#define MAX_SIZE_STR 100
#define MAX_MSG_SIZE 4096

struct DATA_client_v
{
	int client_v;
};

struct DATA_udp
{
	int mess;
	char ip_addr_udp[16];
};

struct DATA_tcp
{
	int time_work;
	char str[MAX_SIZE_STR];
	int len_str;
};

int init_socket_udp(int flag, int type)
{
	int broadcastPermission = 1;
	int socket_udp;
	struct sockaddr_in st_addr_udp;
	st_addr_udp.sin_family = AF_INET;
	st_addr_udp.sin_addr.s_addr = htonl(INADDR_ANY);
	st_addr_udp.sin_port = htons(port_serv + 1);
	if((socket_udp = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
	{
		printf("Клиент v%d: сокет не получен для udp\n", type);
	}
	if(setsockopt(socket_udp, SOL_SOCKET, flag, (void *)&broadcastPermission, sizeof(broadcastPermission)) < 0)
	{
		printf("Клиент v%d: сокет не смог установить параммет для сокета\n", type);
	}
	if(bind(socket_udp, (struct sockaddr *)&st_addr_udp, sizeof(st_addr_udp)) < 0)
	{
		printf("Клиент v%d: сокет не забиндился для udp\n", type);
	}
	return socket_udp;
}

