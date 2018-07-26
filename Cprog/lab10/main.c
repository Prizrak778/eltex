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


void *thread_func(void *arg)
{
	shared.minus_col_people=0;
	shared.col_command=0;
	int col_people_command=rand()%RAND_FOR_INIT_COL;
	int flag=0;
	printf("col_command = %d\n", (*(int *)arg));
	sleep(1);
	while(col_people_command>0&&(!flag))
	{
		pthread_mutex_lock(&shared.mutex);
		col_people_command-=shared.minus_col_people;
		shared.minus_col_people=rand()%RAND_FOR_MINUS_COL;
		if((*(int *)arg)==shared.col_command+1)
		{
			flag=1;
		}
		pthread_mutex_unlock(&shared.mutex);
		col_people_command+=rand()%RAND_FOR_PLUS_COL;
		printf("Количество человек в комманде %d, комманда %d\n", col_people_command, pthread_self());
		sleep(1);
	}
	pthread_mutex_lock(&shared.mutex);
	shared.col_command++;
	pthread_mutex_unlock(&shared.mutex);
	return arg;
}

int main()
{
	int col_command;
	int result;
	printf("Введите количество команд\n");
	scanf("%d", &col_command);
	pthread_t pthread_command[col_command];
	void *status[col_command];
	int id_pthread[col_command];
	int id;
	for(int i=0;i<col_command;i++)
	{
		result=pthread_create(&pthread_command[i],NULL, thread_func, &col_command);
		if(result != 0)
		{
			perror("Создание треда не удалось\n");
			return EXIT_FAILURE;
		}
	}
	for(int i=0;i<col_command; i++)
	{
		id=i;
		result=pthread_join(pthread_command[i], &status[i]);
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
	printf("Done\n");
	return EXIT_SUCCESS;
}
