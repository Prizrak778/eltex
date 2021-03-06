#include <errno.h>
#include <pthread.h>
#include <sys/stat.h>
#include "cmessage.pb-c.h"
#include "Header.h"

#define MAX_COL 50
#define MAX_TIME_K 20
#define MAX_TIME_L 20

//server
struct ThreadArgs
{
	int clntSock;
};

struct DATA_ip
{
	char ip_addr_udp[16];
	char ip_addr_udp_broad[16];
};

struct 
{
	pthread_mutex_t mutex;
	char str[MAX_COL][MAX_SIZE_STR];
	int time_work[MAX_COL];
	int len_str[MAX_COL];
	int col_mess;
}
queue = {
	PTHREAD_MUTEX_INITIALIZER
};

typedef struct DATA_tcp data;
typedef struct DATA_udp DATA_send_udp;

int UDP_socket_int_server()
{
	int broadcastPermission = 1;
	int socket_udp;
	if((socket_udp = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
	{
		printf("Сервер: udp сокет не работает\n");
		exit(1);
	}
	if(setsockopt(socket_udp, SOL_SOCKET, SO_BROADCAST, (void *) &broadcastPermission, sizeof(broadcastPermission)) < 0)
	{
		printf("Сервер: для сокета udp не получилось задать парамметры для бродкаста\n");
		exit(1);
	}
	if(setsockopt(socket_udp, SOL_SOCKET, SO_REUSEPORT, (void *) &broadcastPermission, sizeof(broadcastPermission)) < 0)
	{
		printf("Сервер: для сокета udp не получилось задать парамметры для бродкаста\n");
		exit(1);
	}
	return socket_udp;
}

void *UDP_SEND_client_v1(void *arg)
{
	struct DATA_ip *ip_udp = (struct DATA_ip *)arg;
	int socket_udp = UDP_socket_int_server();
	printf("Сервер: udp сокет готов %d\n", socket_udp);
	struct sockaddr_in st_addr_udp;
	st_addr_udp.sin_family = AF_INET;
	inet_aton(ip_udp->ip_addr_udp_broad, &st_addr_udp.sin_addr);
	st_addr_udp.sin_port = htons(port_serv + 1);
	printf("Сервер: готов отправлять udp оповещения для клиентов v1\n");
	while(1)
	{
		pthread_mutex_lock(&queue.mutex);
		if(queue.col_mess < MAX_COL)
		{
			DATA_send_udp *send_udp;
			send_udp = (DATA_send_udp *)malloc(sizeof(DATA_send_udp));
			send_udp->mess = 1;
			strcpy(send_udp->ip_addr_udp, ip_udp->ip_addr_udp);
			int size_send = sizeof(DATA_send_udp);
			if(sendto(socket_udp, send_udp, size_send, 0, (struct sockaddr *)&st_addr_udp, sizeof(st_addr_udp)) != size_send)
			{
				printf("Сервер: ошибка при отправке udp оповещения для 1 типа клиентов\n");
			}
			else
			{
				printf("Сервер: отправил udp сообщения клиентам v1\n");
			}
			free(send_udp);
		}
		pthread_mutex_unlock(&queue.mutex);
		sleep(rand() % MAX_TIME_K);
	}
}

void *UDP_SEND_client_v2(void *arg)
{
	struct DATA_ip *ip_udp = (struct DATA_ip *)arg;
	int socket_udp = UDP_socket_int_server();
	printf("Сервер: udp сокет готов %d\n", socket_udp);
	struct sockaddr_in st_addr_udp;
	st_addr_udp.sin_family = AF_INET;
	inet_aton(ip_udp->ip_addr_udp_broad, &st_addr_udp.sin_addr);
	st_addr_udp.sin_port = htons(port_serv + 1);
	printf("Сервер: готов отправлять udp оповещения для клиентов v2\n");
	while(1)
	{
		pthread_mutex_lock(&queue.mutex);
		if(queue.col_mess > 0)
		{
			DATA_send_udp *send_udp;
			send_udp = (DATA_send_udp *)malloc(sizeof(DATA_send_udp));
			send_udp->mess = 2;
			strcpy(send_udp->ip_addr_udp, ip_udp->ip_addr_udp);
			int size_send = sizeof(DATA_send_udp);
			if(sendto(socket_udp, send_udp, size_send, 0, (struct sockaddr *)&st_addr_udp, sizeof(st_addr_udp)) != size_send)
			{
				printf("Сервер: ошибка при отправке udp оповещения для 2 типа клиентов\n");
			}
			else
			{
				printf("Сервер: отправил udp сообщения клиентам v2\n");
			}
			free(send_udp);
		}
		pthread_mutex_unlock(&queue.mutex);
		sleep(rand() % MAX_TIME_L);
	}
}

void *Threadclient1(void *arg)
{
	CMessage *msg;
	uint8_t buf[MAX_MSG_SIZE];
	int clntSock;
	pthread_detach(pthread_self());
	clntSock = ((struct ThreadArgs *) arg) -> clntSock;
	printf("Сервер: клиент версии 1 обнаружен, номер сокета = %d\n", clntSock);
	free(arg);
	data *data_recv;
	data_recv = (data *)malloc(sizeof(data));
	int msg_len;
	if((msg_len = recv(clntSock, (void *)buf, MAX_MSG_SIZE, MSG_DONTROUTE)) > 0)
	{
		printf("Севрер: принял сообщение от клиента v1 len = %d\n", msg_len);
		pthread_mutex_lock(&queue.mutex);
		if(queue.col_mess < MAX_COL)
		{
						
			//size_t msg_len = read_buffer (MAX_MSG_SIZE, buf);
			msg = cmessage__unpack(NULL, msg_len, buf);
			if(msg == NULL)
			{
				printf("Сервер: ошибка при распаковывании сообщения\n");
				return NULL;
			}
			queue.time_work[queue.col_mess] = msg->time[0];
			queue.len_str[queue.col_mess] = msg->len[0];
			//из-за того что msg->str не char strcpy() не работает коректно
			for(int i = 0; i < msg->len[0]; i++)
			{
				queue.str[queue.col_mess][i] = msg->str[i];
			}
			printf("Сервер: принял сообщение строка %s время %d длина %d\n", queue.str[queue.col_mess], queue.time_work[queue.col_mess], queue.len_str[queue.col_mess]);

			queue.col_mess++;
			cmessage__free_unpacked(msg, NULL);
			printf("Сервер: в очередь добавлено сообщение кол = %d\n", queue.col_mess);
		}
		else
		{
			printf("Сервер: в очереди нет места\n");
		}
		pthread_mutex_unlock(&queue.mutex);
	}
	else
	{
		printf("Сервер: ошибка получения сообщения от клиента v1\n");
	}
	free(data_recv);
	return NULL;
}

void del_mess()
{
	int i;
	for(i = 0; i < queue.col_mess - 1; i++)
	{
		queue.time_work[i] = queue.time_work[i + 1];
		queue.len_str[i] = queue.len_str[i + 1];
		strcpy(queue.str[i], queue.str[i + 1]);
	}
	queue.col_mess--;
}

void *Threadclient2(void *arg)
{
	int clntSock;
	CMessage msg = CMESSAGE__INIT;
	void *buf;
	int len_buf;
	pthread_detach(pthread_self());
	clntSock = ((struct ThreadArgs *) arg) -> clntSock;
	printf("Сервер: клиент версии 2 обнаружен, номер сокета = %d\n", clntSock);
	free(arg);
	data *now_send;
	pthread_mutex_lock(&queue.mutex);
	now_send = (data *)malloc(sizeof(data));
	if(queue.col_mess > 0)
	{
		msg.n_len = 1;
		msg.n_time = 1;
		msg.n_str = strlen(queue.str[0]) + 1;
		msg.len = malloc(sizeof(int) * msg.n_len);
		msg.time = malloc(sizeof(int) * msg.n_time);
		msg.str = malloc(sizeof(int) * msg.n_str);
		msg.len[0] = queue.len_str[0];
		msg.time[0] = queue.time_work[0];
		for(int i = 0; i < msg.n_str; i++)
		{
			msg.str[i] = queue.str[0][i];
		}
		len_buf = cmessage__get_packed_size(&msg);
		buf = malloc(len_buf);
		cmessage__pack(&msg, buf);
		del_mess();
	}
	else
	{
		now_send->time_work = -1;
		printf("Сервер: в очереди нет сообщений\n");
	}
	pthread_mutex_unlock(&queue.mutex);
	if(now_send != NULL)
	{
		if(send(clntSock, buf, len_buf, MSG_WAITALL) == -1)
		{
			printf("Сервер: не смог отправить сообщение\n");
		}
		free(now_send);
	}
	return NULL;
}

int input(int argc, char* argv[], struct DATA_ip *ip_udp)
{
	if(argc == 2)
	{
		strcpy(ip_udp->ip_addr_udp, argv[1]);
	}
	else if(argc == 1)
	{
		printf("Сервер: введите ip адресс для сервера\n");
		scanf("%s", ip_udp->ip_addr_udp);
	}
	else
	{
		printf("Сервер: cлишком много аргументов\n");
		exit(1);
	}
	struct in_addr in_addr_test;
	if(inet_aton(ip_udp->ip_addr_udp, &in_addr_test) == 0 )
	{
		printf("Сервер: IP адресс некоректен\n");
		exit(1);
	}
	int i;
	for(i = strlen(ip_udp->ip_addr_udp); i > 0 && ip_udp->ip_addr_udp[i] != '.'; i--);
	strcpy(ip_udp->ip_addr_udp_broad, ip_udp->ip_addr_udp);
	ip_udp->ip_addr_udp_broad[i + 1]='2';
	ip_udp->ip_addr_udp_broad[i + 2]='5';
	ip_udp->ip_addr_udp_broad[i + 3]='5';
	ip_udp->ip_addr_udp_broad[i + 4]='\0';
	printf("%s\n", ip_udp->ip_addr_udp_broad);
	return 0;
}

int main(int argc, char* argv[])
{
	int servSock;
	int clntSock;
	struct DATA_ip ip_udp;
	struct sockaddr_in st_addr;
	struct ThreadArgs *thread_client;
	//thread_udp = i
	struct DATA_client_v* data_recv = (struct DATA_client_v *) malloc(sizeof(struct DATA_client_v));
	//ServPort = atoi(argv[1]);
	st_addr.sin_family = AF_INET;
	st_addr.sin_addr.s_addr = INADDR_ANY;
	st_addr.sin_port = htons(port_serv);
	pthread_t threadID;
	input(argc, argv, &ip_udp);
	if(pthread_create(&threadID, NULL, UDP_SEND_client_v1, &ip_udp) == -1)
	{
		printf("Сервер: ошибка при создание udp треда для клиентов v1\n");
	}
	if(pthread_create(&threadID, NULL, UDP_SEND_client_v2, &ip_udp) == -1)
	{
		printf("Сервер: ошибка при создание udp треда для клиентов v2\n");
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
	printf("Сервер: сокет прослушивается\n");
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
			if(recv(clntSock, data_recv, sizeof(struct DATA_client_v), MSG_DONTROUTE))
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
