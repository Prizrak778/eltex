#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/ipc.h>

#define MAX_LEN 1024
union semun {
	int val;
	struct semid_ds *buf;
	unsigned short *array;
	struct seminfo *__buf;
};

//Вариант 9
void control_sum(pid_t pid_distr_file, int *shm, int col_file, pid_t* pid_file, char* dir_name, int sem_id)
{
	struct sembuf lock_res={0, -1, 0};
	struct sembuf rel_res={0, 1, 0};
	if(pid_distr_file==0)
	{
		int *s;
		s = shm;
		//Для определения процессам какой файл им брать
		printf("Файлы раздаю\n");
		if((semop(sem_id, &lock_res, 1)) == -1)
		{
			printf("Lock failed\n");
			exit(1);
		}
		for(int i = 0; i < col_file; i++)
		{
			s[i] = pid_file[i];
		}
		if((semop(sem_id, &rel_res, 1))== -1)
		{
			printf("Unlock failed\n");
			exit(1);
		}
		if(shmdt(s)<0)
		{
			printf("Ошибка отключения\n");
			exit(1);
		}
		printf("Файлы розданы\n");
	}
	else
	{
		FILE *file;
		int file_sum = 0;
		char symbol;
		if((file = fopen(".temp.txt", "r"))==NULL)
		{
			printf("Не удалость открыть .temp.txt\n");
			exit(1);
		}
		char file_name[MAX_LEN];
		sleep(1);
		int *s;
		s = shm;
		int flag = 1;
		if((semop(sem_id, &lock_res, 1))== -1)
		{
			printf("Lock failed\n");
			exit(1);
		}
		for(int i=0; i<col_file&&flag; i++)
		{
			int pid_now = getpid();
			if(pid_now==s[i])
			{
				flag=0;
			}
			fscanf(file,"%s", file_name);
		}
		if((semop(sem_id, &rel_res, 1))== -1)
		{
			printf("Unlock failed\n");
			exit(1);
		}
		if(shmdt(s)<0)
		{
			printf("Ошибка отключения\n");
			exit(1);
		}
		fclose(file);
		strcat(dir_name, "/");
		strcat(dir_name, file_name);
		printf("Процесс %d начинает обработку файла %s\n",getpid(), dir_name);
		if((file=fopen(dir_name, "r"))==NULL)
		{
			printf("Не получилось открыть файл %s\n", dir_name);
			exit(1);
		}
		
		while(!feof(file))
		{
			fscanf(file, "%c", &symbol);
			file_sum+=(int)symbol;
		}
		printf("Процесс %d закончил обработку файла.\nКонтрольная сумма файла %s равна = %d\n", getpid(), dir_name, file_sum);
		fclose(file);
	}
}

int input(char *dir_name)
{
	char dir_name_this[MAX_LEN];
	char commad_line[MAX_LEN]={'l','s',' ','-','p',' ', '-', 'a', ' '};
	printf("Введите имя папки в которой хотите провести подсчёт\n");
	scanf("%s", dir_name_this);
	strcpy(dir_name, dir_name_this);
	strcat(commad_line, dir_name_this);
	strcat(commad_line, "|grep -v / >.temp.txt");
	if(system(commad_line)!=0)
	{
		return -1;
	}
	system("cat .temp.txt| wc -l> .col.txt");
	FILE *file;
	if((file=fopen(".col.txt", "r"))==NULL)
	{
		printf("Не удалось открыть файл .col.txt\n");
		exit(1);
	}
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
	key_t key;
	pid_t pid_parent = getpid();
	pid_t pid_distr_file;
	int col_file = input(dir_name);
	pid_t pid_file[col_file];
	int *shm, shmid;
	int sem_id;
	union semun tmp;

	if((key= ftok(".", 'S'))<0)
	{
		printf("Невозможно получить ключь\n");
		exit(1);
	}
	if(col_file==-1)
	{
		return 1;
	}
	if((sem_id = semget(key, 1, IPC_EXCL|IPC_CREAT|0666))<0)
	{
		perror("semget");
		exit(1);
	}
	tmp.val=1;
	semctl(sem_id, 0, SETVAL, tmp);
	if(col_file == 0)
	{
		printf("В этом каталоге нет файлов\n");
		return 0;
	}
	if((shmid = shmget(key, col_file*sizeof(int), IPC_CREAT|0666))<0)
	{
		perror("shmget");
		exit(1);
	}
	if((shm = shmat(shmid, NULL, 0))== (int *) -1)
	{
		perror("shmat");
		exit(1);
	}
	
	for(int i = 0; i < col_file&&getpid()==pid_parent; i++)
	{
		pid_file[i]=fork();
		srand(getpid());
		if(pid_file[i]==-1)
		{
			perror("Fork is down, is not work\n");
			printf("Process %d is not work", i);
			for (int j = 0; j < i; j++)
			{
				kill(pid_file[j], SIGKILL);
			}
			return 1;
		}
	}
	if(getpid()==pid_parent)
	{
		pid_distr_file=fork();
		if(pid_distr_file == -1)
		{
			perror("Fork is down, is not work\n");
			exit(1);
		}
	}
	if(getpid()!=pid_parent)
	{
		control_sum(pid_distr_file, shm, col_file, pid_file, dir_name, sem_id);
		exit(getpid());
	}
	else
	{
		for(int i = 0; i<col_file; i++)
		{
			status_pid=waitpid(pid_file[i], &stat, 0);
			if(pid_file[i] == status_pid)
			{
				printf("Процесс потомок %d завершил свою работу, result = %d\n", i, WEXITSTATUS(stat));
			}
		}
		status_pid=waitpid(pid_distr_file, &stat, 0);
		if(pid_distr_file == status_pid)
		{
			printf("Процесс потомок раздачик файлов завершил работу, result = %d\n", WEXITSTATUS(stat));
		}
	}
	if(semctl(sem_id, 0, IPC_RMID)<0)
	{
		printf("Невозможно удалить семафор\n");
		exit(1);
	}
	if(shmctl(shmid, IPC_RMID, 0)<0)
	{
		printf("Невозможно удалить область\n");
		exit(1);
	}
	system("rm .temp.txt .col.txt");
	return 0;
}
