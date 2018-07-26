#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 1024
#define FILE_TEMPLATE ".t1"
#define FILE_FIND ".txt"


void check(FILE *file, char file_name[], int col_number)
{
	char input_str[MAX_LEN];
	char *str_without_num;
	char number_str[]="1234567890";
	strcat(file_name, FILE_TEMPLATE);
	FILE *file_out = fopen(file_name, "w");
	int col_f_number=0;
	while(!feof(file))
	{
		col_f_number=0;
		fscanf(file, "%[^\n]s", input_str);
		fscanf(file, "\n");
		str_without_num=strpbrk(input_str, number_str);
		if(str_without_num!=NULL)
		{
			col_f_number++;
		}
		while((str_without_num=strpbrk(str_without_num+1, number_str))!=NULL)
		{
			col_f_number++;
		}
		if(col_number==col_f_number)
		{
			fprintf(file_out, "%s\n", input_str);
		}
	}
	fclose(file_out);
}

FILE *input(int argc, char **argv, int *col_number)
{
	FILE *file;
	if(argc != 3)
	{
		printf("Количество аргументов не равно 3\n");
		exit(1);
	}
	char file_name[MAX_LEN];
	strcpy(file_name, argv[1]);
	strcat(file_name, FILE_FIND);
	if((file=fopen(file_name, "r"))==NULL)
	{
		printf("Неудалось открыть файл\n");
		exit(1);
	}
	*col_number=atoi(argv[2]);
	return file;
}

int main(int argc, char ** argv)
{
	int col_number;
	FILE *file=input(argc, argv, &col_number);
	check(file, argv[1], col_number);	
	fclose(file);
	return 0;
}
