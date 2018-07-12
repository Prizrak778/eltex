#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/msg.h>

struct msg_buf
{
	long mtype;
	int x;
	int y;
	int value;
};

void game_fild(pid_t *pid_gamers, struct msg_buf msg_text, int col_gamers, int size_fild, pid_t pid_fild, int msgqid)
{
	if(pid_fild == 0)
	{
		int fild_games[size_fild][size_fild];
		system("clear");
		for(int i = 0; i < size_fild; i++)
		{
			for(int j = 0; j < size_fild; j++)
			{
				fild_games[i][j]=rand()%3-1;
				printf("%2d ", fild_games[i][j]);
			}
			printf("\n");
		}
		for(int i = 0; i < col_gamers; i++)
		{
			printf("\033[1;3%dm *\033[0m - life = %d\n", i, 1);
		}
		while(col_gamers>1)
		{
			
		}
	}
	else
	{
		int x = rand()%size_fild;
		int y = rand()%size_fild;
		int life = 1;
		sleep(2);
	}
}

int main()
{
	pid_t pid_parent = getpid();
	int status_pid;
	int stat;
	int col_gamers;
	int size_fild;
	struct msg_buf msg_text;
	int msgqid;
	msgqid = msgget(IPC_PRIVATE, IPC_CREAT | IPC_EXCL | 0666);
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
		game_fild(pid_gamers, msg_text, col_gamers, size_fild, pid_fild, msgqid);
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
	msgctl(msgqid, IPC_RMID, NULL);
	return 0;
}
