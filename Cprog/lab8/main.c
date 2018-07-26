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
	pid_t pid_gamers;
};

void game_fild(pid_t *pid_gamers, struct msg_buf msg_text, int col_gamers, int size_fild, pid_t pid_fild, int msgqid, int max_step)
{
	if(pid_fild == 0)
	{
		int fild_games[size_fild][size_fild];
		int status_gamers[col_gamers][4];
		system("clear");
		printf("=======================================\n");
		printf("Ход=0\n");
		printf("Поле без игроков\n");
		for(int i = 0; i < size_fild; i++)
		{
			for(int j = 0; j < size_fild; j++)
			{
				fild_games[i][j]=rand()%3-1;
				printf("%2d ", fild_games[i][j]);
			}
			printf("\n");
		}
		int lenght = sizeof(struct msg_buf)-sizeof(long);
		int type=1;
		for(int i = 0; i < col_gamers; i++)
		{
			if(msgrcv(msgqid, &msg_text, lenght, type, 0)==-1)
			{
				printf("При чтении данных произошла ошибка\n");
			}
			status_gamers[i][0]=msg_text.x;
			status_gamers[i][1]=msg_text.y;
			status_gamers[i][2]=1;
			status_gamers[i][3]=msg_text.pid_gamers;
			for(int j = 0; j < col_gamers; j++)
			{
				if(status_gamers[i][0]==status_gamers[j][0]&&status_gamers[i][1]==status_gamers[j][1]&&status_gamers[i][2]>0&&status_gamers[j][2]>0&&i!=j)
				{
					int life_temp = status_gamers[i][2];
					status_gamers[i][2]-=status_gamers[j][2];
					status_gamers[j][2]-=life_temp;
				}
			}
			status_gamers[i][2]+=fild_games[msg_text.x][msg_text.y];
			fild_games[msg_text.x][msg_text.y]=0;
		}
		sleep(1);
		system("clear");
		printf("=======================================\n");
		printf("Ход=0\n");
		printf("Игроки расположило по полю\n");
		for(int i = 0; i < size_fild; i++)
		{
			for(int j = 0; j < size_fild; j++)
			{
				int flag_k = -1;
				for(int k = 0; k < col_gamers; k++)
				{
					if(status_gamers[k][0]==i&&status_gamers[k][1]==j&&status_gamers[k][2]>0)
					{
						flag_k=k;
					}
				}
				if(flag_k!=-1)
				{
					printf("\033[1;3%dm * \033[0m", flag_k+1);
				}
				else
				{
					printf("%2d ", fild_games[i][j]);
				}
			}
			printf("\n");
		}
		int flag_end=0;
		for(int i = 0; i < col_gamers; i++)
		{
			if(status_gamers[i][2]>0)
			{
				printf("\033[1;3%dm%d *\033[0m - life = %d coord x=%d y=%d\n", i+1, status_gamers[i][3], status_gamers[i][2], status_gamers[i][0], status_gamers[i][1]);
			}
			else
			{
				flag_end++;
				printf("\033[1;3%dm%d *\033[0m - убит\n", i+1, status_gamers[i][3]);
			}
		}
		if(flag_end==col_gamers-1)
		{
			exit(getpid());
		}
		int flag=1;
		while(flag&&type<max_step)
		{
			type++;
			for(int i = 0; i < col_gamers; i++)
			{
				msgrcv(msgqid, &msg_text, lenght, type, 0);
				for(int j=0; j<col_gamers; j++)
				{
					if(status_gamers[j][3]==msg_text.pid_gamers)
					{
						status_gamers[j][0]=msg_text.x;
						status_gamers[j][1]=msg_text.y;
					}
				}
				if(status_gamers[i][2]>0)
				{
					for(int j = 0; j < col_gamers; j++)
					{
						if(status_gamers[i][0]==status_gamers[j][0]&&status_gamers[i][1]==status_gamers[j][1]&&status_gamers[i][2]>0&&status_gamers[j][2]>0&&i!=j)
						{
							int life_temp = status_gamers[i][2];
							status_gamers[i][2]-=status_gamers[j][2];
							status_gamers[j][2]-=life_temp;
						}
					}
					status_gamers[i][2]+=fild_games[status_gamers[i][0]][status_gamers[i][1]];
					fild_games[status_gamers[i][0]][status_gamers[i][1]]=0;
					sleep(1);
					system("clear");
					printf("=======================================\n");
					printf("Ход=%d\n", type-1);
					printf("Ход игрока %d\n", status_gamers[i][3]);
					for(int i = 0; i < size_fild; i++)
					{
						for(int j = 0; j < size_fild; j++)
						{
							int flag_k = -1;
							for(int k = 0; k < col_gamers; k++)
							{
								if(status_gamers[k][0]==i&&status_gamers[k][1]==j&&status_gamers[k][2]>0)
								{
									flag_k=k;
								}
							}
							if(flag_k!=-1)
							{
								printf("\033[1;3%dm * \033[0m", flag_k+1);
							}
							else
							{
								printf("%2d ", fild_games[i][j]);
							}
						}
						printf("\n");
					}
					flag_end=0;
					for(int i = 0; i < col_gamers; i++)
					{
						if(status_gamers[i][2]>0)
						{
							printf("\033[1;3%dm%d *\033[0m - life = %d coord x=%d y=%d\n", i+1, status_gamers[i][3], status_gamers[i][2], status_gamers[i][0], status_gamers[i][1]);
						}
						else
						{
							flag_end++;
							printf("\033[1;3%dm%d *\033[0m - убит\n", i+1, status_gamers[i][3]);
						}
					}
					if(flag_end==col_gamers-1)
					{
						exit(getpid());
					}
				}
			}
		}
	}
	else
	{
		int type = 1;
		int x = rand()%size_fild;
		int y = rand()%size_fild;
		int life = 1;
		struct msqid_ds info;
		msg_text.mtype=type;
		msg_text.x = x;
		msg_text.y = y;
		msg_text.pid_gamers=getpid();
		int lenght=sizeof(struct msg_buf) - sizeof(long);
		if(msgsnd(msgqid, &msg_text, lenght, 0)==-1)
		{
			printf("Ошибка при передаче даннных у процесса %d\n", getpid());
		}
		msgctl(msgqid, IPC_STAT, &info);
		while(type<max_step)
		{
			type++;
			int flag = 1;
			int x_temp;
			int y_temp;
			while(flag)
			{
				x_temp = msg_text.x+rand()%3-1;
				y_temp = msg_text.y+rand()%3-1;
				if(x_temp > 0 && x_temp < size_fild && y_temp > 0 && y_temp < size_fild)
				{
					flag = 0;
				}
				if(msg_text.x==x_temp&&msg_text.y==y_temp)
				{
					flag = 1;
				}
			}
			msg_text.mtype=type;
			msg_text.x = x_temp;
			msg_text.y = y_temp;
			msg_text.pid_gamers=getpid();
			lenght=sizeof(struct msg_buf) - sizeof(long);
			if(msgsnd(msgqid, &msg_text, lenght, 0)==-1)
			{
				printf("Ошибка при передаче даннных у процесса %d\n", getpid());
			}
			msgctl(msgqid, IPC_STAT, &info);
		}
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
	int max_step;
	key_t keymsg=ftok(".", 'a');
	
	if((msgqid=(msgget(keymsg, IPC_CREAT|IPC_EXCL|0666))) <0)
	{
		printf("Очередь с таким ключём не может быть создана\n");
		return 1;
	}
	printf("Введите количество балбесов игры\n");
	scanf("%d", &col_gamers);
	printf("Введите размерность поля(поле квадратное)\n");
	scanf("%d", &size_fild);
	printf("Введите максимальное количество ходов, которое смогут сделать игроки\n");
	scanf("%d", &max_step);
	
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
	//printf("%d\n", getpid());
	if(getpid()!=pid_parent)
	{
		game_fild(pid_gamers, msg_text, col_gamers, size_fild, pid_fild, msgqid, max_step);
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
