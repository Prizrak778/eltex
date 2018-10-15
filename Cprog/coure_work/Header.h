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

struct DATA_client_v
{
	int client_v;
};

struct DATA_recv_udp
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
