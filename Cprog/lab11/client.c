#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <netdb.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#define MAX_LEN 1024
#define port_ser 2524
#define ip_serv "192.168.0.119"

//Вариант 10

struct DATA
{
	int x;
	int y;
	int deltaX;
	int deltaY;
	int signX;
	int signY;
	int error;
	int flag_end;
};
typedef struct DATA data;

void next_step(data *data_scout)
{
	int error2 = data_scout->error * 2;
	if(error2 > -data_scout->deltaY)
	{
		data_scout->error -= data_scout->deltaY;
		data_scout->x += data_scout->signX;
	}
	else
	{
		data_scout->error += data_scout->deltaX;
		data_scout->y += data_scout->signY;
	}
	printf("================\n");
	printf("Новые координаты для скаута\n");
	printf("x=%d y=%d\n\n", data_scout->x, data_scout->y);
}

int main()
{
	data *data_scout;
	srand(abs(getpid()));
	int socket_client;
	struct sockaddr_in st_addr;
	if((socket_client = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		printf("Клиент: не смог создать сокет\n");
		exit(1);
	}
	printf("Клиент: создал сокет\n");
	st_addr.sin_family = AF_INET;
	st_addr.sin_port = htons(port_ser);
	st_addr.sin_addr.s_addr = inet_addr(ip_serv);
	if(connect(socket_client, (struct sockaddr *)&st_addr, sizeof(st_addr))== -1)
	{
		printf("Клиент: ошибка присоединении к серверу\n");
		close(socket_client);
		sleep(5);
		exit(1);
	}
	printf("Клиент: приконектился к серверу\n");
	
	int flag_end = 0;
	while(!flag_end)
	{
		if(recv(socket_client, data_scout, sizeof(data_scout), 0) == -1)
		{
			printf("Клиент: данные для новых координат не приняты\n");
			close(socket_client);
			sleep(5);
			exit(1);
		}
		next_step(data_scout);
		flag_end = data_scout->flag_end;
		if(send(socket_client, data_scout, sizeof(data_scout), 0) == -1)
		{
			printf("Клиент: новые координаты не отправлены\n");
			close(socket_client);
			sleep(5);
			exit(1);
		}
	}
	sleep(5);
	close(socket_client);
	return 0;
}
