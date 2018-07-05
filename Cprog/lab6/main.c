#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>

int main()
{
	pid_t pid;
	pid = fork();
	if(-1 == pid)
	{
		perror("fork is break");
		exit(1);
	}
	else if(0 == pid)
	{
		printf("Child: Это процесс-потомок!\n");
		printf("Child: PID -- %d\n", getpid());
		printf("Child: PID родительского процесса -- %d\n", getppid());
		exit(0);
	}
	else
	{
		printf("Parent: Это процесс-родитель!\n");
		printf("Parent: PID -- %d\n", getpid());
		printf("Parenf: PID потомка %d\n", pid);
		if(wait(0)==-1)
		{
			perror("wait() error");
		}
		else if(WIFEXITED(0))
		{
			printf("Parent: Код возврата потомка: %d\n", WEXITSTATUS(0));
		}
		else
		{
			perror("Parent: GET OUT IN THE CHOPPER!\n");
		}
	}
	return 0;
}
