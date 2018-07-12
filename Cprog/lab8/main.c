#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/msg.h>


int main()
{
	pid_t pid_parent = getpid();
	int status_pid;
	int stat;
	int col_gamers;
	int size_fild;
	printf("Введите количество балбесов игры\n");
	scanf("%d", &col_gamers);
	printf("Введите размерность поля(поле квадратное)\n");
	scanf("%d", &size_fild);
	pid_t pid_fild=fork();
	if(pid_fild == -1)
	{
		perror("Fork is down, is not work\n");
		printf("Process fild is not work");
		exit(1);
	}
	pid_t pid_gamers[col_gamers];
	for(int i = 0; i < col_gamers&&pid_parent==getpid(); i++)
	{
		pid_gamers[i] = fork();
		srand(getpid());
		if(pid_gamers[i]==-1)
		{
			perror("Fork is down, is not work\n");
			printf("Process %d is not work", i);
			for(int j = 0; j < i; j++)
			{
				kill(pid_gamers[j], SIGKILL);
			}
			exit(1);
		}
	}
	if(getpid()!=pid_parent)
	{
		exit(getpid());
	}
	else
	{
		for(int i = 0; i < col_gamers; i++)
		{
			status_pid=waitpid(pid_gamers[i], &stat, 0);
			if(pid_gamers[i] == status_pid)
			{
				printf("Процесс потомок %d покинул игру, result = %d\n", i, WEXITSTATUS(stat));
			}
		}
		status_pid=waitpid(pid_fild, &stat, 0);
		if(pid_fild == status_pid)
		{
			printf("Процесс потомок поле покинул игру, result = %d\n", WEXITSTATUS(stat));
		}
	}
	return 0;
}
