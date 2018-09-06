#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define port_serv 50000
#define ip_serv "192.168.0.104"
#define MAX_TIME_T 20
#define MAX_SIZE_STR 100


//Client v1
struct DATA_send
{
	int client_v;
};

int main()
{
	int socket_tcp, socket_udp;
	int broadcastPermission;
	char wait_mess[]={"Жду сообщения\0"};
	int recvStringLen;
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
	inet_aton(ip_serv, &st_addr_tcp.sin_addr);
	st_addr_tcp.sin_family = AF_INET;
	st_addr_tcp.sin_port = htons(port_serv);
	char *recvString;
	while(1)
	{
		if((recvStringLen = recvfrom(socket_udp, recvString, MAX_SIZE_STR, 0, NULL, 0 ))<0)
		{
			printf("Клиент v1: ошибка при получении udp пакета");
		}
		if(!strcmp(recvString, wait_mess))
		{
			if((socket_tcp = socket(AF_INET, SOCK_STREAM, 0)) == -1)
			{
				printf("Клиент v1: не смог создать сокет\n");
				sleep(5);
				exit(1);
			}
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
			close(socket_tcp);
			printf("Клиент v1: сообщение отправлено\n");
			int time_T = rand()%MAX_TIME_T;
			sleep(time_T);
		}
	}
	close(socket_tcp);
	free(data_send);
	return 0;
}


