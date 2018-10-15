#include "Header.h"
#include "cmessage.pb-c.h"

//Client v2

typedef struct DATA_tcp data_recv;

void output_recv(CMessage *msg)
{
	printf("Клиент v2: получил сообещние от сервера\n");
	printf("Клиент v2: время обработки сообщения %d\n", msg->time[0]);
	printf("Клиент v2: длина строки %d\n", msg->len[0]);
	printf("Клиент v2: строка ");
	for(int i = 0; i < msg->len[0] - 1; i++)
	{
		printf("%c", msg->str[i]);
	}
	printf("\n");
}

int init_socket_udp()
{
	int broadcastPermission = 1;
	int socket_udp;
	struct sockaddr_in st_addr_udp;
	st_addr_udp.sin_family = AF_INET;
	st_addr_udp.sin_addr.s_addr = htonl(INADDR_ANY);
	st_addr_udp.sin_port = htons(port_serv + 1);
	if((socket_udp = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
	{
		printf("Клиент v2: сокет не получен для udp\n");
	}
	if(setsockopt(socket_udp, SOL_SOCKET, SO_REUSEPORT, (void *)&broadcastPermission, sizeof(broadcastPermission)) < 0)
	{
		printf("Клиент v2: сокет не смог установить параметры для сокета\n");
	}
	if(bind(socket_udp, (struct sockaddr *)&st_addr_udp, sizeof(st_addr_udp)) < 0)
	{
		printf("Клиент v2: сокет не забиндился для udp\n");
	}
	return socket_udp;
} 

int main()
{
	CMessage *msg;
	uint8_t buf[MAX_MSG_SIZE];
	int msg_len;
	int socket_tcp, socket_udp;
	int recv_Len;
	struct sockaddr_in st_addr_tcp;
	struct DATA_client_v* data_send = (struct DATA_client_v *) malloc(sizeof(struct DATA_client_v));
	data_send->client_v = 2;
	socket_udp = init_socket_udp();
	printf("Клиент v2: создал сокет\n");
	st_addr_tcp.sin_family = AF_INET;
	st_addr_tcp.sin_port = htons(port_serv);
	struct DATA_recv_udp *ip_udp;
	ip_udp = (struct DATA_recv_udp *)malloc(sizeof(struct DATA_recv_udp));
	int size_send = sizeof(struct DATA_recv_udp);
	while(1)
	{
		printf("Клиент v2: ждёт udp сообщения\n");
		if((recv_Len = recvfrom(socket_udp, ip_udp, size_send, 0, NULL, 0)) < 0)
		{
			printf("Клиент v1: ошибка при получении udp пакета");
		}
		if(ip_udp->mess == 2)
		{
			if((socket_tcp = socket(AF_INET, SOCK_STREAM, 0)) == -1)
			{
				printf("Клиент v2: не смог создать сокет\n");
				sleep(5);
				exit(1);
			}
			inet_aton(ip_udp->ip_addr_udp, &st_addr_tcp.sin_addr);
			if(connect(socket_tcp, (struct sockaddr *)&st_addr_tcp, sizeof(st_addr_tcp))== -1)
			{
				printf("Клиент v2: ошибка присоединении к серверу\n");
				close(socket_tcp);
				sleep(5);
				exit(1);
			}
			//Отправить версию клиента по tcp
			printf("Клиент v2: приконектился к серверу\n");
			if(send(socket_tcp, data_send, sizeof(struct DATA_client_v), MSG_WAITALL) == -1)
			{
				printf("Клиент v2: данные по клиенту не отправлены\n");
				close(socket_tcp);
				sleep(5);
			}
			if((msg_len = recv(socket_tcp, (void *)buf, MAX_MSG_SIZE, MSG_DONTROUTE))!= -1)
			{
				msg = cmessage__unpack(NULL, msg_len, buf);
				if(msg == NULL)
				{
					printf("Клиент v2: ошибка при распаковывании сообщения\n");
				}
				else if(msg->time[0] == -1)
				{
					printf("Клиент v2: у сервера не было сообщений\n");
				}
				else
				{
					output_recv(msg);
				}
			}
			else
			{
				printf("Клиент v2: ошибка получения сообщения от сервера\n");
			}
			close(socket_tcp);
			int time_T = msg->time[0];
			sleep(time_T);
		}
	}
	close(socket_tcp);
	free(data_send);
	return 0;
}


