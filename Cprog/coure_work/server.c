#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
#define MAX_TIME_K 20
#define MAX_TIME_L 20
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
	pthread_mutex_t mutex;
	char *str[MAX_COL];
	int time_work[MAX_COL];
	int len_str[MAX_COL];
	int col_mess;
}
queue = {
	PTHREAD_MUTEX_INITIALIZER
};

struct DATA
{
	int time_work;
	char *str;
	int len_str;
};
typedef struct DATA data;

void *UDP_SEND(void *arg)
{
	struct sockaddr_in st_addr_udp;
	int socket_udp;
	st_addr_udp.sin_family = AF_INET;
	inet_aton(arg, &st_addr_udp.sin_addr);
	st_addr_udp.sin_port = htons(port_server + 1);
	int broadcastPermission;
	if((socket_udp = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
	{
		printf("Сервер: udp сокет не работает\n");
		exit(1);
	}
	if(setsockopt(socket_udp, SOL_SOCKET, SO_BROADCAST, (void *)&broadcastPermission, sizeof(broadcastPermission)) < 0);
	{
		printf("Сервер: для сокета udp не получилосб задать парамметры ля бродкаста\n");
	}
	int time_next_L = 0;
	int time_next_K = 0;
	while(1)
	{
		int time_now;
		time(&time_now);
		if(time_now > time_next_L || time_now > time_next_K)
		{
			pthread_mutex_lock(&queue.mutex);
			if(queue.col_mess < MAX_COL && time_now > time_next_L)
			{
				time_next_L = time_now + rand() % MAX_TIME_L;
				char wait_mess[] = {"Жду сообщения\0"};
				int size_wait_mess = strlen(wait_mess) + 1;
				if(sendto(socket_udp, wait_mess, size_wait_mess, 0, (struct sockaddr *)&st_addr_udp, sizeof(st_addr_udp)) != size_wait_mess)
				{
					printf("Сервер: ошибка при отправке udp оповещения для 1 типа клиентов\n");
				}
			}
			if(queue.col_mess > 0 && time_now > time_next_K)
			{
				time_next_K = time_now + rand() % MAX_TIME_K;
				char mess_present[] = {"Есть сообщения\0"};
				int size_pres_mess = strlen(mess_present) + 1;
				if(sendto(socket_udp, mess_present, size_pres_mess, 0, (struct sockaddr *)&st_addr_udp, sizeof(st_addr_udp)) != size_pres_mess)
				{
					printf("Сервер: ошибка при отправке udp оповещения ддля 2 типа клиентов\n");
				}
			}
			pthread_mutex_unlock(&queue.mutex);
		}
	}
}

void *Threadclient1(void *arg)
{
	int clntSock;
	pthread_detach(pthread_self());
	clntSock = ((struct ThreadArgs *) arg) -> clntSock;
	printf("Сервер: клиент версии 1 обнаружен, номер сокета = %d\n", clntSock);
	free(arg);
	while(1)
	{
		data *data_recv;
		data_recv = (data *)malloc(sizeof(data));
		if(recv(clntSock, data_recv, sizeof(data), MSG_DONTROUTE))
		{
			pthread_mutex_lock(&queue.mutex);
			if(queue.col_mess < MAX_COL)
			{
				queue.str[queue.col_mess] = (char *) malloc(sizeof(char) * data_recv->len_str);
				strcpy(queue.str[queue.col_mess], data_recv->str);
				queue.time_work[queue.col_mess] = data_recv->time_work;
				queue.len_str[queue.col_mess] = data_recv->len_str;
				queue.col_mess++;
			}
			else
			{
				printf("Сервер: в очереди нет места\n");
			}
			pthread_mutex_unlock(&queue.mutex);
		}
		free(data_recv);
	}
	return NULL;
}

void del_mess()
{
	for(int i = 0; i < queue.col_mess - 1; i++)
	{
		free(queue.str[i]);
		queue.time_work[i] = queue.time_work[i + 1];
		queue.len_str[i] = queue.len_str[i + 1];
		queue.str[i] = queue.str[i + 1];
	}
}

void *Threadclient2(void *arg)
{
	int clntSock;
	pthread_detach(pthread_self());
	clntSock = ((struct ThreadArgs *) arg) -> clntSock;
	printf("Сервер: клиент версии 1 обнаружен, номер сокета = %d\n", clntSock);
	free(arg);
	while(1)
	{
		data *now_send;
		pthread_mutex_lock(&queue.mutex);
		if(queue.col_mess > 0)
		{
			now_send = (data *)malloc(sizeof(data));
			now_send->str = (char *) malloc(sizeof(char) * queue.len_str[0]);
			strcpy(now_send->str, queue.str[0]);
			now_send->time_work = queue.time_work[0];
			now_send->len_str = queue.len_str[0];
			del_mess();
		}
		else
		{
			printf("Сервер: в очереди нет сообщений\n");
		}
		pthread_mutex_unlock(&queue.mutex);
		if(now_send != NULL)
		{
			if(send(clntSock, now_send, sizeof(data), MSG_WAITALL) == -1)
			{
				printf("Сервер: не смог отправить сообщение\n");
			}
			free(now_send);
		}
	}
	return NULL;
}

int input(int argc, char* argv[], char ip_addr_udp[])
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
	struct in_addr in_addr_test;
	if(inet_aton(ip_addr_udp, &in_addr_test) == 0 )
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
