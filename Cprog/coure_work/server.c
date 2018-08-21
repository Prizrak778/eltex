#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>
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
}
queue = {
	PTHREADMUTEX_INITIALER
};

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


int main(int argc, char* argv[])
{
	int* client_v;
	int servSock;
	int clntSock;
	struct sockaddr_in st_addr;
	struct ThreadArgs *thread_client, *thread_udp;
	unsigned short ServPort;
	//thread_udp = i
	struct DATA_recv* data_recv = (struct DATA_recv *) malloc(sizeof(struct DATA_recv));
	//ServPort = atoi(argv[1]);
	st_addr.sin_family = AF_INET;
	st_addr.sin_addr.s_addr = INADDR_ANY;
	st_addr.sin_port = htons(port_server);
	pthread_t threadID;
	/*if(pthread_create(&threadID, NULL, Thread_udp, (void *)) == -1)
	{
		printf("Сервер: ошибка при создание udp треда\n");
	}*/
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
		if((clntSock = accept(servSock, NULL, NULL)) == -1)
		{
			printf("Сервер: aксепт фэйл\n");
		}
		else
		{
			if((threadclient = (struct ThreadArgs *) malloc(sizeof(struct ThreadArgs))) == NULL)
			{
				printf("Сервер: malloc is fail\n");
			}
			threadclient -> clntSock = clntSock;
			if(recv(clntSock, data_recv, sizeof(struct DATA_recv), MSG_DONTROUTE))
			{
				printf("recv_data = %d\n", data_recv->client_v);
				if(data_recv->client_v == 1)
				{
					if(pthread_create(&threadID, NULL, Threadclient1, (void *) threadclient) != 0)
					{
						printf("Сервер: ошибка при создание треда\n");
					}
				}
				else if(data_recv->client_v == 2)
				{
					if(pthread_create(&threadID, NULL, Threadclient2, (void *) threadclient) != 0)
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
