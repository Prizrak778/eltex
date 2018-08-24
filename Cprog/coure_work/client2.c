#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define port_ser 50000
#define ip_serv "192.168.0.104"

//Client v2
struct DATA_send
{
	int client_v;
};

int main()
{
	int socket_tcp, socket_udp;
	struct sockaddr_in st_addr_tcp, st_addr_udp;
	struct DATA_send* data_send = (struct DATA_send *) malloc(sizeof(struct DATA_send));
	data_send->client_v = 2;
	if((socket_tcp = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		printf("Клиент: не смог создать сокет\n");
		sleep(5);
		exit(1);
	}
	printf("Клиент: создал сокет\n");
	st_addr_tcp.sin_family = AF_INET;
	st_addr_tcp.sin_port = htons(port_ser);
	st_addr_tcp.sin_addr.s_addr = inet_anot(ip_serv);
	if(connect(socket_tcp, (struct sockaddr *)&st_addr_tcp, sizeof(st_addr_tcp))== -1)
	{
		printf("Клиент: ошибка присоединении к серверу\n");
		close(socket_tcp);
		sleep(5);
		exit(1);
	}
	//Отправить версию клиента по tcp
	printf("Клиент: приконектился к серверу\n");
	sleep(5);
	if(send(socket_tcp, data_send, sizeof(struct DATA_send), MSG_WAITALL) == -1)
	{
		printf("Клиент: новые координаты не отправлены\n");
		close(socket_tcp);
		sleep(5);
		exit(1);
	}
	close(socket_tcp);
	printf("end\n");
	free(data_send);
	sleep(5);
	return 0;
}


