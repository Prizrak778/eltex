#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/shm.h>

#define MAX_LEN 1024
//Вариант 9
void control_sum()
{
	int t;
}

int input(char *dir_name)
{
	char dir_name_this[MAX_LEN];
	char commad_line[]={"ls -p "};
	printf("Введите название папки в которой будет считаться контрольная сумма\n");
	scanf("%s", dir_name_this);
	strcpy(dir_name, dir_name_this);
	strcat(commad_line, dir_name_this);
	strcat(commad_line, "|grep -v / >.temp.txt");
	if(system(commad_line)!=0)
	{
		return -1;
	}
	system("cat .temp.txt| wc -l> .col.txt")
	FILE *file=fopen(".col.txt", "r");
	int col_file;
	fscanf(file, "%d", &col_file);
	fclose(file);
	return col_file; 
}
int main()
{
	char dir_name[MAX_LEN];
	pid_t pid_parent = getpid();
	int col_file = input(dir_name);
	if(col_file==-1)
	{
		return 1;
	}
	pid_t pid_file[col_file];
	int *shm, shmid;
	if(col_file == 0)
	{
		printf("В этом каталоге нет файлов\n");
		return 0;
	}
	if((shmid = shmget(IPC_PRIVATE, size, IPC_CREAT|0666))<0)
	{
		perrir("shmget");
		exit(1);
	}
	if((shm = shmat(shmid, NULL, 0))== (int *) -1)
	{
		perror("shmat");
		exit(1);
	}
	for(int i = 0; i < col_file&&pid_parent; i++)
	{
		pid_file[i]=fork();
		srand(getpid());
		if(pid_file[i]==-1)
		{
			prerror("Fork is down, is not work\n");
			printf("Process %d is not work", i);
			for (int j = 0; j < i; j++)
			{
				kill(pid_file[j], SIGKILL);
			}
			return 1;
		}
	}
	control_sum();
	system("rm .temp.txt .col.txt");
	return 0;
}
