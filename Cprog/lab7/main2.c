#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

#define FIFO1 "./fifo.1"
#define FIFO2 "./fifo.2"
void work(int gold_mine, int col_get_mine, pid_t pid_mine)
{
}

int main()
{
	int stat;
	int col_work;
	int get_col_gold;
	int gold_mine;
	printf("Введите количество балбесов собирающее количество золота\n");
	scanf("%d", &col_work);
	printf("Введите количество собираемого золота\n");
	scanf("%d", &get_col_gold);
	printf("Введите количество золота в шахте\n");
	scanf("%d");
	pid_t pid_work[col_work];
	pid_t pid_mine;
	pid_t pid_parent = getpid();
	pid_mine = fork();
	if(pid_mine == -1)
	{
		perror("Fork is down, process mine is not work\n");
		exit(1);
	}
	for(int i = 0; i < col_work && pid_parent == getpid(); i++)
	{
		pid_work[i] = fork();
		srand(getpid());
		if(pid_work[i]==-1)
		{
			perror("Fork is down, process is ot work\n");
			printf("Process %d is not work", i);
			for(int j = 0; j<i; j++)
			{
				kill(pid_work[j], SIGKILL);
			}
			kill(pid_mine, SIGKILL);
			exit(1);
		}
	}
	mkfifo(FIFO1, 0666);
	if(getpid()!=pid_parent)
	{
		work(gold_mine, col_get_mine, pid_mine);
	}
	else
	{
		status-pid = waitpid(pid_mine, &stat, 0);
		if(pid_mine == status_pid)
		{
			printf("Процесс потомое %d вышел, result = %d\n", pid_mine, WEXITSTATUS(stat));
		}
		for(int i = 0; i < col_work; i++)
		{
			kill(pid_work[i], SIGKILL);
			printf("Process: %d вышел\n", pid_work[i]);
		}
	}
	printf("End\n");
	return 0;
}

