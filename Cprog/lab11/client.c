#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#define MAX_LEN 1024
#define port_ser 2525
#define ip_serv "192.168.0.118"

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
	printf("size = %d\n", sizeof(data));
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
		if(recv(socket_client, data_scout, sizeof(data), 0) == -1)
		{
			printf("Клиент: данные для новых координат не приняты\n");
			close(socket_client);
			sleep(5);
			exit(1);
		}
		printf("Клиент: принял сообщение\n");
		printf("x=%d, y=%d, deltaX=%d, deltaY=%d, signX=%d, signY=%d, error=%d, flag_end=%d\n", data_scout->x, data_scout->y, data_scout->deltaX, data_scout->deltaY, data_scout->signX, data_scout->signY, data_scout->error, data_scout->flag_end);
		next_step(data_scout);
		printf("Новые координаты для скаута\n");
		printf("x=%d y=%d\n\n", data_scout->x, data_scout->y);
		flag_end = data_scout->flag_end;
		if(!flag_end)
		{
			if(send(socket_client, data_scout, sizeof(data), MSG_WAITALL) == -1)
			{
				printf("Клиент: новые координаты не отправлены\n");
				close(socket_client);
				sleep(5);
				exit(1);
			}
		}
	}
	close(socket_client);
	printf("end\n");
	sleep(5);
	return 0;
}
