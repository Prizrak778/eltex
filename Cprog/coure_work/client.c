#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#define port_serv 50000
#define MAX_TIME_T 20
#define MAX_SIZE_STR 100


//Client v1
struct DATA_send
{
	int client_v;
};

struct DATA_recv_udp
{
	int mess;
	char ip_addr_udp[16];
};

struct DATA_send_tcp
{
	int time_work;
	char str[MAX_SIZE_STR];
	int len_str;
};
typedef struct DATA_send_tcp data_send_tcp;

void random_string(data_send_tcp *data_string)
{
	data_string->time_work = rand()%MAX_TIME_T;
	data_string->len_str = rand()%MAX_SIZE_STR;
	for(int i = 0; i < data_string->len_str-1; i++)
	{
		int case_ascii = rand()%3;
		char char_ascii;
		if(case_ascii == 0)
		{
			char_ascii = 48 + rand()%10;
		}
		else if(case_ascii == 1)
		{
			char_ascii = 65 + rand()%25;
		}
		else if(case_ascii == 2)
		{
			char_ascii = 97 + rand()%25;
		}
		data_string->str[i] = char_ascii;
	}
	data_string->str[data_string->len_str] = '\0';
	printf("Клиент v1: сгенерировал сообещине %s\n", data_string->str);
	printf("Клиент v1: длина строки %d\n", data_string->len_str);
	printf("Клиент v1: время обработки %d\n", data_string->time_work);
}

int main()
{
	srand(getpid());
	int socket_tcp, socket_udp;
	int broadcastPermission = 1;
	int recvLen;
	struct sockaddr_in st_addr_tcp, st_addr_udp;
	struct DATA_send *data_send = (struct DATA_send *)malloc(sizeof(struct DATA_send));
	data_send->client_v = 1;
	st_addr_udp.sin_family = AF_INET;
	st_addr_udp.sin_addr.s_addr = htonl(INADDR_ANY);
	st_addr_udp.sin_port = htons(port_serv + 1);
	
	if(( socket_udp = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
	{
		printf("Клиент v1: сокет не получен для udp\n");
	}
	if(setsockopt(socket_udp, SOL_SOCKET, SO_REUSEPORT, (void *)&broadcastPermission, sizeof(broadcastPermission)) < 0)
	{
		printf("Клиент v1: сокет не смог установить параметры для сокета\n");
	}
	if(bind(socket_udp, (struct sockaddr *) &st_addr_udp, sizeof(st_addr_udp)) < 0)
	{
		printf("Клиент v1: сокет не забиндился для udp\n");
	}
	printf("Клиент v1: создал сокет\n");
	st_addr_tcp.sin_family = AF_INET;
	st_addr_tcp.sin_port = htons(port_serv);
	struct DATA_recv_udp *ip_udp;
	ip_udp = (struct DATA_recv_udp *)malloc(sizeof(struct DATA_recv_udp));
	int size_send = sizeof(struct DATA_recv_udp);
	while(1)
	{
		printf("Клиент v1: ждёт udp сообщения\n");
		if((recvLen = recvfrom(socket_udp, ip_udp, size_send, 0, NULL, 0 ))<0)
		{
			printf("Клиент v1: ошибка при получении udp пакета\n");
		}
		if(ip_udp->mess == 1)
		{
			if((socket_tcp = socket(AF_INET, SOCK_STREAM, 0)) == -1)
			{
				printf("Клиент v1: не смог создать сокет\n");
				sleep(5);
				exit(1);
			}
			inet_aton(ip_udp->ip_addr_udp, &st_addr_tcp.sin_addr);
			if(connect(socket_tcp, (struct sockaddr *)&st_addr_tcp, sizeof(st_addr_tcp))== -1)
			{
				printf("Клиент v1: ошибка присоединении к серверу\n");
				close(socket_tcp);
				sleep(5);
				exit(1);
			}
			//Отправить версию клиента по tpc
			printf("Клиент v1: приконектился к серверу\n");
			if(send(socket_tcp, data_send, sizeof(struct DATA_send), MSG_WAITALL) == -1)
			{
				printf("Клиент v1: данные по клиенту не отправлены\n");
				close(socket_tcp);
				sleep(5);
				exit(1);
			}
			data_send_tcp *data_string;
			data_string = (data_send_tcp *)malloc(sizeof(data_send_tcp));
			random_string(data_string);
			if(send(socket_tcp, data_string, sizeof(data_send_tcp), MSG_WAITALL) == -1)
			{
				printf("Клиент v1: сообщение серверу не отправлен\n");
				close(socket_tcp);
				sleep(5);
				exit(1);
			}
			close(socket_tcp);
			printf("Клиент v1: сообщение отправлено\n");
			int time_T = data_string->time_work;
			sleep(time_T);
		}
	}
	close(socket_tcp);
	free(data_send);
	return 0;
}


