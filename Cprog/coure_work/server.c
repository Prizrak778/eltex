#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>

#define port_server 50000
#define MAX_COL 50

//server
struct ThreadArgs
{
	int clntSock;
};

struct DATA_recv
{
	int client_v;
};

struct 
{
	char *str[MAX_COL];
	int col_mess;
}
queue = {
	PTHREAD_MUTEX_INITIALIZER
};

void *UDP_SEND(void *arg)
{
	struct sockaddr_in st_addr_udp;
	int socket_udp;
	st_addr_udp.sin_family = AF_INET;
	st_addr_udp.sin_addr.s_addr = inet_aton(arg);
	st_addr_udp.sin_port = htons(port_server + 1);
	if((socket_udp = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
	{
		printf("Сервер: udp сокет не работает\n");
		exit(1);
	}
}

void *Threadclient1(void *arg)
{
	int clntSock;
	//pthread_detach(pthread_self());
	clntSock = ((struct ThreadArgs *) arg) -> clntSock;
	printf("Сервер: клиент версии 1 обнаружен, номер сокета = %d\n", clntSock);
	free(arg);
	return NULL;
}

void *Threadclient2(void *arg)
{
	int clntSock;
	//pthread_detach(pthread_self());
	clntSock = ((struct ThreadArgs *) arg) -> clntSock;
	printf("Сервер: клиент версии 1 обнаружен, номер сокета = %d\n", clntSock);
	free(arg);
	return NULL;
}

int input(int argc, char argv[], char ip_addr_udp[])
{
	if(argc == 2)
	{
		strcpy(ip_addr_udp, argv[1]);
	}
	else if(argc > 3)
	{
		printf("Сервер: cлишком много аргументов\n");
		exit(1);
	}
	else
	{
		printf("Сервер: введите широковещательный адресс для udp\n");
		scanf("%s", ip_addr_udp);
	}
	if(inet_aton(ip_addr_udp) == 0 )
	{
		printf("Сервер: широковещательный IP адресс некоректен\n");
		exit(1);
	}
}

int main(int argc, char* argv[])
{
	int servSock;
	int clntSock;
	char ip_addr_udp[16];
	struct sockaddr_in st_addr;
	struct sockaddr_in st_addr_ac;
	struct ThreadArgs *thread_client, *thread_udp;
	unsigned short ServPort;
	//thread_udp = i
	struct DATA_recv* data_recv = (struct DATA_recv *) malloc(sizeof(struct DATA_recv));
	//ServPort = atoi(argv[1]);
	st_addr.sin_family = AF_INET;
	st_addr.sin_addr.s_addr = INADDR_ANY;
	st_addr.sin_port = htons(port_server);
	pthread_t threadID;
	input(argc, argv, &ip_addr_udp);
	if(pthread_create(&threadID, NULL, UDP_SEND, (void *)ip_addr_udp) == -1)
	{
		printf("Сервер: ошибка при создание udp треда\n");
	}
	if(( servSock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		printf("Сервер: ошибка при создании сокета\n");
		close(servSock);
		exit(1);
	}
	printf("Сервер: создал сокет\n");
	if((bind(servSock, (struct sockaddr *)&st_addr, sizeof(st_addr))) == -1)
	{
		printf("Сервер: ошибка в bind\n");
		close(servSock);
		exit(1);
	}
	printf("Сервер: bind прошёл успешно\n");
	if(listen(servSock, 10) == -1)
	{
		printf("Сервер: ошибка в попытке прослушать порт\n");
		close(servSock);
		exit(1);
	}
	printf("Сервер: сокет прослушывается\n");
	while(1)
	{
		int size_struct = sizeof(st_addr);
		if((clntSock = accept(servSock, (struct sockaddr *)&st_addr, (socklen_t*)&size_struct)) == -1)
		//if((clntSock = accept(servSock, NULL, NULL)) == -1)
		{
			printf("Сервер: aксепт фэйл\n");
		}
		else
		{
			printf("Сервер: полученно соединение по адресу %s\n", inet_ntoa(st_addr.sin_addr));
			if((thread_client = (struct ThreadArgs *) malloc(sizeof(struct ThreadArgs))) == NULL)
			{
				printf("Сервер: malloc is fail\n");
			}
			thread_client -> clntSock = clntSock;
			if(recv(clntSock, data_recv, sizeof(struct DATA_recv), MSG_DONTROUTE))
			{
				if(data_recv->client_v == 1)
				{
					if(pthread_create(&threadID, NULL, Threadclient1, (void *) thread_client) != 0)
					{
						printf("Сервер: ошибка при создание треда\n");
					}
				}
				else if(data_recv->client_v == 2)
				{
					if(pthread_create(&threadID, NULL, Threadclient2, (void *) thread_client) != 0)
					{
						printf("Сервер: ошибка при создание треда\n");
					}
				}
				else
				{
					printf("Сервер: клиент не распознан\n");
				}
			}
			else
			{
				printf("Сервер: ошибка при принятии сообщения\n");
			}
		}
	}
}
