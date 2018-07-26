#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>
#include <signal.h>
void work(int gold_mine, int col_get_mine)
{
	while(gold_mine > 0)
	{
		gold_mine-=col_get_mine;
		printf("Process %d: Осталось золота %d\n", getpid(), gold_mine);
		int sleep_time= rand()%10;
		printf("Process %d: Время сна %d\n", getpid(), sleep_time);
		sleep(sleep_time);
	}
	printf("Process %d: Золото в шахте кончилось\n", getpid());
	exit(getpid());
}

int main()
{
	pid_t pid_parent = getpid();
	int status_pid;
	int stat;
	int col_work;
	int gold_mine;
	int col_get_mine;
	printf("Введите количество шахтёров собирающие золото в шахте\n");
	scanf("%d", &col_work);
	printf("Введите сколько они золота буду брать\n");
	scanf("%d", &col_get_mine);
	pid_t pid_work[col_work];
	printf("Введите количество золота в шахте\n");
	scanf("%d", &gold_mine);
	for(int i = 0; i<col_work&& pid_parent==getpid() ; i++)
	{
		pid_work[i] = fork();
		srand(getpid());
		if(pid_work[i] == -1)
		{
			perror("Fork is down, process is not work\n");
			printf("Process %d is not work", i);
			for(int j = 0; j<i; j++)
			{
				kill(pid_work[j], SIGKILL);
			}
			exit(1);
		}
	}
	if(getpid()!=pid_parent)
	{
		work(gold_mine, col_get_mine);
	}
	else
	{
		for(int i = 0; i < col_work; i++)
		{
			status_pid = waitpid(pid_work[i], &stat, 0);
			if(pid_work[i] == status_pid)
			{
				printf("Процесс потомок %d свалил на вертолёте, result = %d\n", i, WEXITSTATUS(stat));
			}
		}
	}
	return 0;
}
