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

//Вариант 10
#define RAND_FOR_INIT_COL 50
#define RAND_FOR_MINUS_COL 20
#define RAND_FOR_PLUS_COL 5

struct {
	pthread_mutex_t mutex;
	int minus_col_people;
	int col_command;
} shared = {
	PTHREAD_MUTEX_INITIALIZER
};
int size_fild;
void input(int *targets)
{
	printf("Введите размер карты(квадратная)\n");
	scanf("%d", &size_fild);
	printf("Введите количество разведчиков\n");
	scanf("%d", targets);
}

void init_map(int map[size_fild][size_fild])
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
	printf("1\n");
	int x;
    int y;
	for (int k = 0; k < targets; k++)
	{
		x = rand()%size_fild;
		y = rand()%size_fild;
		map[x][y]++;
	}
	//system("clear");
	printf("Карта без разведчиков\n");
	for (int i = 0; i < size_fild; i++)
	{
		for(int j = 0; j < size_fild; j++)
		{
			printf("%d ", map[i][j]);
		}
		printf("\n");
	}
	sleep(1);
}

void *thread_func_scaut(void *arg)
{
	return arg;
}

void *thread_func_map(void *arg)
{
	int size_fild=*((int *) arg);
	int map[size_fild][size_fild];
	init_map(map);
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
	void *status[scouts];
	void *status_map;
	int id_pthread[scouts];
	result=pthread_create(&pthread_map, NULL, thread_func_map, &size_fild);
	if(result != 0)
	{
		perror("Создание треда карты не удалось\n");
		return EXIT_FAILURE;
	}
	for(int i=0;i<size_fild;i++)
	{
		result=pthread_create(&pthread_scout[i], NULL, thread_func_scaut, &size_fild);
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
			printf("В тред было передано %d\n",*((int *)status[i]));
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
