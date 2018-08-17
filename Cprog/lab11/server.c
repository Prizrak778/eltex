#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<unistd.h>
#include<pthread.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<wait.h>
#include<time.h>
#include<math.h>
#define port_server 2525
//Вариант 10

struct
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
shared = {
	PTHREAD_MUTEX_INITIALIZER
};

struct
{
	pthread_mutex_t mutex;
	int *x;
	int *y;
	int size_mass;
}
shared2 = {
	PTHREAD_MUTEX_INITIALIZER
};

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

struct DATA_SCOUT
{
	int x;
	int y;
	int socket_scout;
};
typedef struct DATA_SCOUT data_scout;

struct DATA_SEND
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
typedef struct DATA_SEND data_send;

void input(int *scouts)
{
	printf("Введите размер карты(квадратная)\n");
	scanf("%d", &size_fild);
	printf("Введите количество разведчиков\n");
	scanf("%d", scouts);
}

void next_step(coord_loc *now_loc, int deltaX, int deltaY, int signX, int signY, int *error, int socket, int flag_end)
{
	data_send *now_send;
	now_send->x = now_loc->x;
	now_send->y = now_loc->y;
	now_send->deltaX = deltaX;
	now_send->deltaY = deltaY;
	now_send->signX = signX;
	now_send->signY = signY;
	now_send->error = *error;
	now_send->flag_end = flag_end;
	if(send(socket, now_send, sizeof(data_send), MSG_WAITALL) == -1)
	{
		printf("Сервер: не отправил сообщение\n");
	}
	if(recv(socket, now_send, sizeof(data_send), MSG_DONTROUTE))
	{
		now_loc->x = now_send->x;
		now_loc->y = now_send->y;
		deltaX = now_send->deltaX;
		deltaY = now_send->deltaY;
		signX = now_send->signX;
		signY = now_send->signY;
		*error = now_send->error;
	}
	else
	{
		printf("Сервер: ошибка при принятии сообщения\n");
	}
}

void end_loc_init(coord_loc *end_loc, int size_fild)
{
	int flag = 1;
	while(flag)
	{
		int case_border = rand()%4;
		switch(case_border)
		{
			case 0:
				end_loc->x = rand() % size_fild;
				end_loc->y = size_fild - 1;
				break;
			case 1:
				end_loc->y = rand() % size_fild;
				end_loc->x = size_fild - 1;
				break;
			case 2:
				end_loc->x = rand() % size_fild;
				end_loc->y = 0;
				break;
			case 3:
				end_loc->y = rand() % size_fild;
				end_loc->x = 0;
				break;
		}
		pthread_mutex_lock(&shared2.mutex);
		flag = 0;
		int *x_exp = malloc(sizeof(int) * shared2.size_mass + 1);
		int *y_exp = malloc(sizeof(int) * shared2.size_mass + 1);
		for(int i = 0; i < shared2.size_mass; i++)
		{
			x_exp[i] = shared2.x[i];
			y_exp[i] = shared2.y[i];
			if(x_exp[i] == end_loc->x && y_exp[i] == end_loc->y)
			{
				flag = 1;
			}
		}
		if(!flag)
		{
			x_exp[shared2.size_mass] = end_loc->x;
			y_exp[shared2.size_mass] = end_loc->y;
			free(shared2.x);
			free(shared2.y);
			shared2.x = malloc(sizeof(int) * shared2.size_mass + 1);
			shared2.y = malloc(sizeof(int) * shared2.size_mass + 1);
			for(int i = 0; i < shared2.size_mass + 1; i++)
			{
				shared2.x[i] = x_exp[i];
				shared2.y[i] = y_exp[i];
			}
			shared2.size_mass = shared2.size_mass + 1;
			flag = 0;
		}
		free(x_exp);
		free(y_exp);
		pthread_mutex_unlock(&shared2.mutex);
		sleep(1);
	}
}

void *thread_func_scout(void *arg)
{
	srand(abs(pthread_self()));
	data_scout *data_loc = (data_scout *)arg;
	coord_loc start_loc;
	coord_loc end_loc;
	coord_loc now_loc;
	end_loc_init(&end_loc, size_fild);
	start_loc.x = data_loc->x;
	start_loc.y = data_loc->y;
	now_loc.x = start_loc.x;
	now_loc.y = start_loc.y;
	int socket = data_loc->socket_scout;
	int len_way = abs(start_loc.x - end_loc.x) + abs(start_loc.y - end_loc.y);
	int flag = 1;
	int deltaX = abs(end_loc.x - start_loc.x);
	int deltaY = abs(end_loc.y - start_loc.y);
	int signX = start_loc.x < end_loc.x ? 1 : -1;
	int signY = start_loc.y < end_loc.y ? 1 : -1;
	int error = deltaX - deltaY;
	int flag_end = 0;
	for(int i = 0; i < len_way + 1; i++)
	{
		flag = 1;
		while(flag)
		{
			pthread_mutex_lock(&shared.mutex);
			if(shared.pthread_scout == 0)
			{
				shared.pthread_scout=abs(pthread_self());
				shared.x = now_loc.x;
				shared.y = now_loc.y;
				shared.end_x = end_loc.x;
				shared.end_y = end_loc.y;
				flag_end = 0;
				shared.flag_end = flag_end;
				if(i == len_way)
				{
					flag_end = 1;
					shared.flag_end = flag_end;
				}
				flag = 0;
			}
			pthread_mutex_unlock(&shared.mutex);
		}
		next_step(&now_loc, deltaX, deltaX, signX, signY, &error, socket, flag_end);
	}
	close(socket);
	return arg;
}

void init_map(int map[size_fild][size_fild], int scouts)
{
	int targets = rand()%(size_fild*size_fild);
	for(int i = 0; i < size_fild; i++)
	{
		for(int j = 0; j < size_fild; j++)
		{
			map[i][j] = 0;
		}
	}
	int x;
	int y;
	for(int k = 0; k < targets; k++)
	{
		x = rand()%size_fild;
		y = rand()%size_fild;
		map[x][y]++;
	}
	printf("Карта без разведчиков\n");
	for(int i = 0; i < size_fild; i++)
	{
		for(int j = 0; j < size_fild; j++)
		{
			printf("%d ", map[i][j]);
		}
		printf("\n");
	}
	for(int i = 0; i < scouts; i++)
	{
		printf("Разведчик%d -\n", i);
	}
	sleep(1);
}

void out_map(int map[size_fild][size_fild], int status_scouts[][6], int pthread_scout, int scouts)
{
	system("clear");
	int num_scout = 0;
	for(int i = 0; i < scouts; i++)
	{
		if(status_scouts[i][0]==pthread_scout)
		{
			num_scout = i;
			break;
		}
	}
	for(int i = 0; i < size_fild; i++)
	{
		for(int j = 0; j < size_fild; j++)
		{
			num_scout = -1;
			for(int k = 0; k < scouts; k++)
			{
				if(status_scouts[k][1]==i&&status_scouts[k][2]==j)
				{
					num_scout=k;
				}
			}
			if(num_scout!=-1)
			{
				status_scouts[num_scout][5]+=map[i][j];
				printf("\033[1;3%dm* \033[0m", num_scout);
				map[i][j] = 0;
			}
			else
			{
				printf("%d ", map[i][j]);
			}
		}
		printf("\n");
	}
	for(int i = 0; i < scouts; i++)
	{
		if(status_scouts[i][0]!=-1)
		{
			printf("\033[1;3%dmРазведчик%d\033[0m x=%2d, y=%2d, end_x=%2d, end_y=%2d, col_target=%2d\n", i, i, status_scouts[i][2], status_scouts[i][1], status_scouts[i][4], status_scouts[i][3], status_scouts[i][5]);
		}
		else
		{
			printf("Разведчик%d -\n", i);
		}
	}
}

void *thread_func_map(void *arg)
{
	srand(abs(pthread_self()));
	data_map *data_init_map = (data_map *)arg;
	int size_fild = data_init_map->size_fild;
	int scouts = data_init_map->scouts;
	int map[size_fild][size_fild];
	int status_scouts[scouts][6];
	for(int i = 0; i < scouts; i++)
	{
		for(int j = 0; j < 5; j++)
		{
			status_scouts[i][j]=-1;
		}
		status_scouts[i][5]=0;
	}
	init_map(map, scouts);
	printf("Карта готова\n");
	int flag_end = scouts;
	while(flag_end)
	{
		pthread_mutex_lock(&shared.mutex);
		if(shared.pthread_scout != 0)
		{
			int flag_new_pth = 1;
			for(int i = 0; i < scouts; i++)
			{
				if(status_scouts[i][0] == shared.pthread_scout)
				{
					status_scouts[i][1] = shared.x;
					status_scouts[i][2] = shared.y;
					status_scouts[i][3] = shared.end_x;
					status_scouts[i][4] = shared.end_y;
					flag_new_pth = 0;
				}
			}
			for(int i = 0; i < scouts&&flag_new_pth; i++)
			{
				if(status_scouts[i][0] == -1)
				{
					status_scouts[i][0] = shared.pthread_scout;
					status_scouts[i][1] = shared.x;
					status_scouts[i][2] = shared.y;
					status_scouts[i][3] = shared.end_x;
					status_scouts[i][4] = shared.end_y;
					flag_new_pth = 0;
				}
			}
			flag_end -= shared.flag_end;
			shared.pthread_scout = 0;
			out_map(map, status_scouts, shared.pthread_scout, scouts);
			sleep(1);
		}
		pthread_mutex_unlock(&shared.mutex);
	}
	sleep(1);
	pthread_mutex_lock(&shared2.mutex);
	free(shared2.x);
	free(shared2.y);
	pthread_mutex_unlock(&shared2.mutex);
	return arg;
}

int main()
{
	int socket_client, socket_ac;
	struct sockaddr_in st_addr;
	srand(getpid());
	system("clear");
	int scouts;
	int error_ac = 0;
	int result;
	input(&scouts);
	pthread_t pthread_scout[scouts];
	pthread_t pthread_map;
	data_scout start_loc[scouts];
	int start_x =  1 + rand() % (size_fild - 2), start_y = 1 + rand() % (size_fild - 2);
	for(int i = 0; i < scouts; i++)
	{
		start_loc[i].x = start_x;
		start_loc[i].y = start_y;
	}
	data_map data_init_map;
	void *status[scouts];
	void *status_map;
	int check;
	int id_pthread[scouts];
	
	if((socket_client = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
	{
		printf("Сервер: ошибка при создании сокета\n");
		close(socket_client);
		exit(1);
	}
	st_addr.sin_family = AF_INET;
	st_addr.sin_addr.s_addr=INADDR_ANY;
	st_addr.sin_port=htons(port_server);
	if((check = bind(socket_client, (struct sockaddr *)&st_addr, sizeof(st_addr))) == -1)
	{
		printf("Сервер: ошибка в bind\n");
		close(socket_client);
		exit(1);
	}
	if(listen(socket_client, 10) == -1)
	{
		printf("Ошибка в попытке прослушать порт");
		close(socket_client);
		exit(1);
	}
	for(int i = 0; i < scouts; i++)
	{
		if((socket_ac = accept(socket_client, NULL, NULL)) == -1)
		{
			printf("Сервер: аксепт фэйл\n");
			error_ac++;
			if(error_ac == scouts - 1)
			{
				printf("Сервер: все соединения провалены\n");
				close(socket_client);
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			start_loc[i].socket_scout = socket_ac;
			result = pthread_create(&pthread_scout[i], NULL, thread_func_scout, &start_loc[i]);
			if(result != 0)
			{
				perror("Создание треда не удалось\n");
				return EXIT_FAILURE;
			}
		}
	}
	result = pthread_create(&pthread_map, NULL, thread_func_map, &data_init_map);
	data_init_map.size_fild = size_fild;
	data_init_map.scouts = scouts - error_ac;
	if(result != 0)
	{
		perror("Создание треда карты не удалось\n");
		return EXIT_FAILURE;
	}
	for(int i = 0; i < scouts - error_ac; i++)
	{
		result = pthread_join(pthread_scout[i], &status[i]);
		if(result != 0)
		{
			perror("Вход в тред не удалось\n");
			return EXIT_FAILURE;
		}
	}
	result = pthread_join(pthread_map, &status_map);
	if(result != 0)
	{
		perror("Вход в тред не удалось\n");
		return EXIT_FAILURE;
	}
	close(socket_client);
	printf("Done\n");
	return EXIT_SUCCESS;
}


