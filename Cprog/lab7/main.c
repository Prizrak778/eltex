#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>

int fd1[2];

void work(int gold_mine, int col_get_mine, pid_t pid_mine)
{
	if(0==pid_mine)
	{
		printf("Шахта найдена\n");
		close(fd1[1]);
		char col_gold[1024];
		printf("В шахте золота: %d\n", gold_mine);
		while(gold_mine>0)
		{
			printf("В шахте нет рабочих\n");
			read(fd1[0], col_gold, sizeof(col_gold));
			printf("Col_gold = %d\n", atoi(col_gold));
			gold_mine-=atoi(col_gold);
			printf("Золото осталось: %d\n", gold_mine);
		}
		printf("Шахта рухнула\n");
		printf("GET OUT IN THE CHOPPER!!!\n");
		exit(pid_mine);
	}
	else
	{
		close(fd1[0]);
		while(1)
		{
			write(fd1[1], &col_get_mine, sizeof(col_get_mine));
			printf("Process: %d, отнёс золото\n", getpid());
			int time = rand()%15;
			printf("Process: %d, вернётся через %d\n", getpid(), time);
			sleep(time);
		}
	}
}

int main()
{
	pid_t pid_parent = getpid();
	pid_t pid_mine;
	int status_pid;
	int stat;
	int col_work;
	int gold_mine;
	int col_get_mine;
	pipe(fd1);
	printf("Введите количество балбесов собирающие золото в шахте\n");
	scanf("%d", &col_work);
	printf("Введите сколько они золота буду брать\n");
	scanf("%d", &col_get_mine);
	pid_t pid_work[col_work];
	printf("Введите количество золота в шахте\n");
	scanf("%d", &gold_mine);
	pid_mine = fork();
	if(pid_mine == -1)
	{
		perror("Fork is down, process is not work\n");
		printf("Process mine is not work");
		exit(1);
	}
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
			kill(pid_mine, SIGKILL);
			exit(1);
		}
	}
	if(getpid()!=pid_parent)
	{
		work(gold_mine, col_get_mine, pid_mine);
	}
	else
	{
		status_pid = waitpid(pid_mine, &stat, 0);
		if(pid_mine == status_pid)
		{
			printf("Процесс потомок %d вышел, result = %d\n", pid_mine, WEXITSTATUS(stat));
		}
		for(int i = 0; i < col_work; i++)
		{
			kill(pid_work[i], SIGKILL);
			printf("Process: %d вышел\n", pid_work[i]);
		}
	}
	printf("Ready\n");
	return 0;
}
