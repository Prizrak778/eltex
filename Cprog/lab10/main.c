#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <wait.h>
#include <signal.h>
#include <time.h>
#include <math.h>

//Вариант 10

struct {
	pthread_mutex_t mutex;
	int x;
	int y;
	pthread_t pthread_scout;
	int step;
	int flag_end;
} shared = {
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

void input(int *targets)
{
	printf("Введите размер карты(квадратная)\n");
	scanf("%d", &size_fild);
	printf("Введите количество разведчиков\n");
	scanf("%d", targets);
}

void init_map(int map[size_fild][size_fild], int scouts)
{
	int targets=rand()%(size_fild*size_fild);
	printf("%d\n", size_fild);
	for (int i = 0; i < size_fild; i++)
	{
		for(int j = 0; j < size_fild; j++)
		{
			map[i][j]=0;
		}
	}
	int x;
    int y;
	for (int k = 0; k < targets; k++)
	{
		x = rand()%size_fild;
		y = rand()%size_fild;
		map[x][y]++;
	}
	//system("clear");
	sleep(1);
	printf("Карта без разведчиков\n");
	for (int i = 0; i < size_fild; i++)
	{
		for(int j = 0; j < size_fild; j++)
		{
			printf("%d ", map[i][j]);
		}
		printf("\n");
	}
	for(int i = 0; i<scouts; i++)
	{
		printf("Разведчик%d -\n", i+1);
	}
}

void *thread_func_scout(void *arg)
{
	srand(abs(pthread_self()));
	coord_loc *start_loc =(coord_loc*)arg;
	coord_loc end_loc;
	coord_loc now_loc;
	if(rand()%2)
	{
		end_loc.x=rand()%size_fild;
		end_loc.y=(rand()%2)*size_fild;
	}
	else
	{
		end_loc.y=rand()%size_fild;
		end_loc.x=(rand()%2)*size_fild;
	}
	now_loc.x=start_loc->x;
	now_loc.y=start_loc->y;
	int len_way = abs(start_loc->x-end_loc.x)+abs(start_loc->y-end_loc.y);
	int len_x = end_loc.x-start_loc->x;
	int len_y = end_loc.y-start_loc->y;
	int flag = 1;
	for(int i=0; i<len_way; i++)
	{
		flag = 1;
		while(flag)
		{
			pthread_mutex_lock(&shared.mutex);
			if(shared.pthread_scout==0)
			{
				shared.pthread_scout=abs(pthread_self());
				shared.x = now_loc.x;
				shared.y = now_loc.y;
				shared.step = i;
				shared.flag_end = 0;
				if(i==len_way-1)
				{
					shared.flag_end = 1;
				}
				flag=0;
			}
			pthread_mutex_unlock(&shared.mutex);
		}
		if(now_loc.x==end_loc.x)
		{
			now_loc.y+=(len_y/abs(len_y));
		}
		else
		{
			now_loc.x+=(len_x/abs(len_x));
		}
	}
	//printf("x=%d, y=%d, pid=%d\n", start_loc->x, start_loc->y, abs(pthread_self()));
	//printf("end_x=%d, end_y=%d, pid=%d\n", end_loc.x, end_loc.y, abs(pthread_self()));
	return pthread_self();
}

void out_map(int map[size_fild][size_fild], int status_scout[][4], int pthread_scout, int scouts)
{
	int num_scout=0;
	for(int i=0; i < scouts; i++)
	{
		if(status_scout[i][0]==pthread_scout)
		{
			num_scout=i;
			break;
		}
	}
	for(int i = 0; i < size_fild; i++)
	{
		for(int j = 0; j < size_fild; j++)
		{
			if(status_scout[num_scout][1]==i&&status_scout[num_scout][2]==j)
			{
				status_scout[num_scout][3]+=map[i][j];
				printf("* ");
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
		printf("Игрок%d x=%d, y=%d, col_target=%d\n", i, status_scout[i][1], status_scout[i][2], status_scout[i][3]);
	}
}

void *thread_func_map(void *arg)
{
	srand(time(NULL));
	data_map *data_init_map = (data_map *)arg;
	int size_fild = data_init_map->size_fild;
	int scouts = data_init_map->scouts;
	int map[size_fild][size_fild];
	int status_scout[scouts][4];
	for(int i = 0; i < scouts; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			status_scout[i][j]=0;
		}
	}
	init_map(map, scouts);
	printf("Карта готова\n");
	int flag_end=scouts;
	while(flag_end)
	{
		pthread_mutex_lock(&shared.mutex);
		if(shared.pthread_scout!=0)
		{
			int flag_new_pth=1;
			for(int i=0;i<scouts;i++)
			{
				if(status_scout[i][0]==shared.pthread_scout)
				{
					status_scout[i][1]=shared.x;
					status_scout[i][2]=shared.y;
					flag_new_pth=0;
				}
			}
			for(int i=0;i<scouts&&flag_new_pth;i++)
			{
				if(status_scout[i][0]==0)
				{
					status_scout[i][0]=shared.pthread_scout;
					status_scout[i][1]=shared.x;
					status_scout[i][2]=shared.y;
					flag_new_pth=0;
				}
			}
			flag_end-=shared.flag_end;
			shared.pthread_scout=0;
			out_map(map, status_scout, shared.pthread_scout, scouts);
			sleep(1);
		}
		pthread_mutex_unlock(&shared.mutex);
	}
	return arg;
}

int main()
{
	//system("clear");
	int scouts;
	int result;
	input(&scouts);
	pthread_t pthread_scout[scouts];
	pthread_t pthread_map;
	coord_loc start_loc;
	start_loc.x = rand()%size_fild;
	start_loc.y = rand()%size_fild;
	data_map data_init_map;
	data_init_map.size_fild = size_fild;
	data_init_map.scouts = scouts;
	void *status[scouts];
	void *status_map;
	int id_pthread[scouts];
	result=pthread_create(&pthread_map, NULL, thread_func_map, &data_init_map);
	if(result != 0)
	{
		perror("Создание треда карты не удалось\n");
		return EXIT_FAILURE;
	}
	for(int i=0;i<size_fild;i++)
	{
		result=pthread_create(&pthread_scout[i], NULL, thread_func_scout, &start_loc);
		if(result != 0)
		{
			perror("Создание треда не удалось\n");
			return EXIT_FAILURE;
		}
	}
	for(int i=0;i<size_fild; i++)
	{
		result=pthread_join(pthread_scout[i], &status[i]);
		if(result != 0)
		{
			perror("Вход в тред не удалось\n");
			return EXIT_FAILURE;
		}
		else
		{
			//printf("В тред было передано %d\n",*((int *)status[i]));
		}
	}
	result=pthread_join(pthread_map, &status_map);
	if(result != 0)
	{
		perror("Вход в тред не удалось\n");
		return EXIT_FAILURE;
	}
	else
	{
		printf("В тред карты было передано %d\n",*((int *)status_map));
	}
	printf("Done\n");
	return EXIT_SUCCESS;
}
