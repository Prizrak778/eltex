#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <netdb.h>
#include <pthread.h>
#include <sys/typed.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#define MAX_LEN 1024
#define port_ser 2524
#define ip_serv INADDR_LOOPBACK

//Вариант 10

struct SHARED
{
	pthread_mutex_t mutex;
	int x;
	int y;
	int end_x;
	int end_y;
	pthread_t pthread_scout;
	int step;
	int flag_end;
}

struct SHARED2
{
	pthread_mutex_t mutex;
	int *x;
	int *y;
	int size_mass;
}

int size_fild;

struct DATA
{
	int x;
	int y;
};
typedef struct DATA coord_loc;

struct DATA_MAP
{
	int size_fild;
	int scouts;
};
typedef struct DATA_MAP data_map;

void next_step(coord_loc *now_loc, int deltaX, int deltaY, int signX, int signY, int *error)
{
	int error2 = *error * 2;
	if(error2 > -deltaY)
	{
		*error -= deltaY;
		now_loc->+=singX;
	}
	else
	{
		*error += deltaX;
		now_loc->y += signY;
	}
}

void end_loc_init(coord_loc *end_loc, int size_fild)
{
	int flag = 1;
	while(flag)
	{
		if(rand() % 2)
		{
			end_loc->x = rand() % size_fild;
			end_loc->y = (rand() % 2) * (size_fild - 1);
		}
		else
		{
			end_loc->y = rand() % size_fild;
			end_loc->x = (rand() % 2) * (size_fild - 1);
		}
	}
}

int main()
{
	srand(abs(getpid()));
	int sokect_client;
	struct sockaddr_in st_addr;
	if((socket1 = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		printf("Клиент: не смог создать сокет\n");
		exit(1);
	}
	printf("Клиент: создал сокет\n");
	st_addr.sin_family = AF_INET;
	st_addr.sin_port = htons(port_ser);
	st_addr.sin_addr.s_addr = htonl(ip_serv);
	return 0;
}
