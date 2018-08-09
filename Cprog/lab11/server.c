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

void input(int *scouts)
{
	printf("Введите размер карты(квадратная)\n");
	scanf("%d", &size_fild);
	printf("Введите количество разведчиков\n");
	scanf("%d", scouts);
}

void *thread_func_scout(void *arg)
{
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
	//system("clear");
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
			printf("\033[1;3%dmРазведчик%d\033[0m x=%2d, y=%2d, end_x=%2d, end_y=%2d, col_target=%2d\n", i, i, status_scouts[i][1], status_scouts[i][2], status_scouts[i][3], status_scouts[i][4], status_scouts[i][5]);
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
	pthread_mutex_lock(&shared2.mutex);
	free(shared2.x);
	free(shared2.y);
	pthread_mutex_unlock(&shared2.mutex);
	return arg;
}

int main()
{
	srand(getpid());
	system("clear");
	int scouts;
	int result;
	input(&scouts);
	pthread_t pthread_scout[scouts];
	pthread_t pthread_map;
	coord_loc start_loc;
	start_loc.x = 1 + rand()%(size_fild-2);
	start_loc.y = 1 + rand()%(size_fild-2);
	data_map data_init_map;
	data_init_map.size_fild = size_fild;
	data_init_map.scouts = scouts;
	void *status[scouts];
	void *status_map;
	int id_pthread[scouts];
	result = pthread_create(&pthread_map, NULL, thread_func_map, &data_init_map);
	if(result != 0)
	{
		perror("Создание треда карты не удалось\n");
		return EXIT_FAILURE;
	}
	for(int i = 0; i<scouts; i++)
	{
		result = pthread_create(&pthread_scout[i], NULL, thread_func_scout, &start_loc);
		if(result != 0)
		{
			perror("Создание треда не удалось\n");
			return EXIT_FAILURE;
		}
	}
	for(int i = 0; i < scouts; i++)
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
	printf("Done\n");
	return EXIT_SUCCESS;
}


