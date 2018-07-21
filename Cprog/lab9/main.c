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
void control_sum(pid_t pid_distr_file, int *shm, int col_file, pid_t pid_file[])
{
	if(pid_dir_name==0)
	{
		int *s;
		s = shm;
		for(int i = 0; i < col_file; i++)
		{
			s[i] = pid_file[i];
		}
	}
	else
	{
		FILE *file = fopen(".temp.txt", "r");
	}
}

int input(char *dir_name)
{
	char dir_name_this[MAX_LEN];
	char commad_line[]={"ls -p "};
	printf("Введите имя папки в которой хотите провести подсчёт\n");
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
	int status_pid;
	int stat;
	char dir_name[MAX_LEN];
	pid_t pid_parent = getpid();
	int col_file = input(dir_name);
	if(col_file==-1)
	{
		return 1;
	}
	pid_t pid_file[col_file];
	pid_t pid_distr_file;
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
	pid_distr_file=fork();
	if(pid_distr_file == -1)
	{
		prerror("Fork is down, is not work\n");
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
	if(getpid()!=pid_parent)
	{
		control_sum(pid_distr_file, shm, col_file, pid_file);
		exit(getpid());
	}
	else
	{
		for(int i = 0; i<col_file; i++)
		{
			status_pid=waitpid(pid_file[i], &stat, 0);
			if(pid_file[i] == status_pid)
			{
				printf("Процесс потомок %d завершил свою работу, result =%d \n", i, WEXITSTATUS(stat));
			}
		}
		status_pid=waitpid(pid_distr_file, &stat, 0);
		if(pid_distr_file == status_pid)
		{
			printf("Процесс потомок раздачик файлов завершил работу, result = %d\n", WEXITSTATUS(stat));
		}
	}
	if(shmctl(shmid, IPC_RMID, 0)<0)
	{
		printf("Невозможно удалить область\n");
		exit(1);
	}
	system("rm .temp.txt .col.txt");
	return 0;
}
