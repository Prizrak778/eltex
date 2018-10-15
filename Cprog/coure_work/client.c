#include "Header.h"
#include "cmessage.pb-c.h"

typedef struct DATA_tcp data_send_tcp;

void random_string(CMessage *msg, void **buf, int *time_T, int *len_buf)
{
	msg->n_len = 1;
	msg->n_time = 1;
	msg->n_str = rand()%MAX_SIZE_STR;
	msg->len = malloc(sizeof(int) * msg->n_len);
	msg->time = malloc(sizeof(int) * msg->n_time);
	msg->str = malloc(sizeof(int) * msg->n_str);
	msg->len[0] = msg->n_str;
	msg->time[0] = rand()%MAX_TIME_T;
	*time_T = msg->time[0];
	printf("Клиент v1: сгенерировал сообщение ");
	for(int i = 0; i < msg->n_str - 1; i++)
	{
		int case_ascii = rand()%3;
		char char_ascii;
		if(case_ascii == 0)
		{
			char_ascii = 48 + rand()%10;
		}
		else if(case_ascii == 1)
		{
			char_ascii = 65 + rand()%25;
		}
		else if(case_ascii == 2)
		{
			char_ascii = 97 + rand()%25;
		}
		msg->str[i] = char_ascii;
		printf("%c", msg->str[i]);
	}
	printf("\n");
	msg->str[msg->n_str - 1] = '\0';
	printf("Клиент v1: длина строки %d\n", (int)msg->len[0]);
	printf("Клиент v1: время обработки %d\n", (int )msg->time[0]);
	*len_buf = cmessage__get_packed_size(msg);
	*buf = malloc(*len_buf);
	cmessage__pack(msg, *buf);
}

int main()
{
	CMessage msg = CMESSAGE__INIT;
	void *buf;
	int len_buf;
	srand(getpid());
	int socket_tcp, socket_udp;
	int recvLen;
	struct sockaddr_in st_addr_tcp;
	struct DATA_client_v *data_send = (struct DATA_client_v *)malloc(sizeof(struct DATA_client_v));
	data_send->client_v = 1;
	//socket_udp = init_socket_udp();
	socket_udp = init_socket_udp(SO_REUSEPORT, 1);
	printf("Клиент v1: создал сокет\n");
	st_addr_tcp.sin_family = AF_INET;
	st_addr_tcp.sin_port = htons(port_serv);
	struct DATA_recv_udp *ip_udp;
	ip_udp = (struct DATA_recv_udp *)malloc(sizeof(struct DATA_recv_udp));
	int size_send = sizeof(struct DATA_recv_udp);
	while(1)
	{
		printf("Клиент v1: ждёт udp сообщения\n");
		if((recvLen = recvfrom(socket_udp, ip_udp, size_send, 0, NULL, 0 ))<0)
		{
			printf("Клиент v1: ошибка при получении udp пакета\n");
		}
		if(ip_udp->mess == 1)
		{
			if((socket_tcp = socket(AF_INET, SOCK_STREAM, 0)) == -1)
			{
				printf("Клиент v1: не смог создать сокет\n");
				sleep(5);
				exit(1);
			}
			inet_aton(ip_udp->ip_addr_udp, &st_addr_tcp.sin_addr);
			if(connect(socket_tcp, (struct sockaddr *)&st_addr_tcp, sizeof(st_addr_tcp))== -1)
			{
				printf("Клиент v1: ошибка присоединении к серверу\n");
				close(socket_tcp);
				sleep(5);
				exit(1);
			}
			//Отправить версию клиента по tpc
			printf("Клиент v1: приконектился к серверу\n");
			if(send(socket_tcp, data_send, sizeof(struct DATA_client_v), MSG_WAITALL) == -1)
			{
				printf("Клиент v1: данные по клиенту не отправлены\n");
				close(socket_tcp);
				sleep(5);
				exit(1);
			}
			int time_T;
			random_string(&msg, &buf, &time_T, &len_buf);
			printf("Клиент v1: размер от %d\n", len_buf);
			if(send(socket_tcp, buf, len_buf, MSG_WAITALL) != len_buf)
			{
				printf("Клиент v1: сообщение серверу не отправлен\n");
				close(socket_tcp);
				sleep(5);
				exit(1);
			}
			free(msg.len);
			free(msg.time);
			free(msg.str);
			free(buf);
			close(socket_tcp);
			printf("Клиент v1: сообщение отправлено\n");
			sleep(time_T);
		}
	}
	close(socket_tcp);
	free(data_send);
	return 0;
}


