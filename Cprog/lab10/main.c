#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
//Вариант 7
void *thread_func(void *arg)
{
	int loc_id = *(int *)arg;
	printf("Thread %d запущен\n", loc_id);
	sleep(1);
	pthread_exit(NULL);
}
int main()
{
	int col_command;
	int result;
	printf("Введите количество команд\n");
	scanf("%d", &col_command);
	pthread_t pthread_command[col_command];
	int id_pthread[col_command];
	int id;
	for(int i=0;i<col_command;i++)
	{
		id=i;
		result=pthread_create(&pthread_command[i],NULL, thread_func, &id);
		if(result != 0)
		{
			perror("Создание треда не удалось\n");
			return EXIT_FAILURE;
		}
	}
	for(int i=0;i<col_command; i++)
	{
		id=i;
		result=pthread_join(pthread_command[i], NULL);
		if(result != 0)
		{
			perror("Вход в тред не удалось\n");
			return EXIT_FAILURE;
		}
	}
	printf("Done\n");
	return EXIT_SUCCESS;
}
