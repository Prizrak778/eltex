#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define MAX_LEN 1024

void check(FILE *file, char file_name[], int col_puts)
{
	char input_str[MAX_LEN];
	strcat(file_name, ".t2");
	FILE *file_out = fopen(file_name, "w");
	int str_len;
	while(!feof(file))
	{
		fscanf(file, "%[^\n]s", input_str);
		fscanf(file, "\n");
		for(int i = 0; i<strlen(input_str); i++)
		{
			if(input_str[i]==' ')
			{
				str_len = strlen(input_str);
				for(int j = str_len+1+col_puts; j>i+col_puts; j--)
				{
					input_str[j]=input_str[j-col_puts];
				}
				for(int j = i+1; j < col_puts+i+1; j++)
				{
					input_str[j]='.';
				}
			}
		}
		fprintf(file_out, "%s\n", input_str);
	}
}

FILE *input(int argc, char **argv, int *col_puts)
{
	FILE *file;
	if(argc!=3)
	{
		printf("Количество аргументов не равно 3\n");
		exit(1);
	}
	char file_name[MAX_LEN];
	strcpy(file_name, argv[1]);
	strcat(file_name, ".txt");
	if((file=fopen(file_name, "r"))==NULL)
	{
		printf("FUCK YOU!\n");
		exit(1);
	}
	*col_puts=atoi(argv[2]);
	return file;
}

int main(int argc, char **argv)
{
	int col_puts;
	FILE *file = input(argc, argv, &col_puts);
	check(file, argv[1], col_puts);
	fclose(file);
	return 0;
}
