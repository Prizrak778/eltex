#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mcheck.h>
#define MAX_LEN 1024

int input(char ***mass_char, int *col_str)
{
	char str[MAX_LEN];
	printf("Введите кол строк\n");
	scanf("%d", col_str);
	(*mass_char) = (char **) malloc((*col_str)*sizeof(char*));
	scanf("%c");
	for(int i = 0; i < *col_str; i++)
	{
		printf("Введите строку\n");
		int j=0;
		char c;
		scanf("%c", &c);
		while(c!='\n')
		{
			str[j] = c;
			j++;
			scanf("%c", &c);
		}
		str[j]='\0';
		printf("str = %s %d\n",str, strlen(str));
		(*mass_char)[i] = (char *)malloc(strlen(str)*sizeof(char));
		strcpy((*mass_char)[i], str);
		printf("mass_char = %s\n", (*mass_char)[i]);
	}
	return 0;
}
int Len_first_w(char *mass_char, int *max_len)
{
	char str[MAX_LEN];
	int col = strlen(mass_char);
	strcpy(str, mass_char);
	char *str_now = strtok(str, " .,?!");
	if(str_now!=NULL)
	{
		col = strlen(str_now);
		if(col>*max_len)
		{
			*max_len = col;
		}
		while(str_now!=NULL)
		{
			str_now = strtok(NULL, " .,?!");
			if(str_now!=NULL&&strlen(str_now)>*max_len)
			{
				*max_len = strlen(str_now);
			}
		}
	}
	else if(col>*max_len)
	{
		*max_len = col;
	}
	return col;
}
int sort_first_word(char ***mass_char, int *max_len, int *col_swap, int col_str)
{
	int k = 0;
	char *str;
	int col_sym1 = 0;
	int col_sym2 = 0;
	int max_len_now = 0;
	while(k==0)
	{
		k=1;
		for(int i=0;i<col_str-1;i++)
		{
			col_sym1 = Len_first_w((*mass_char)[i], &max_len_now);
			col_sym2 = Len_first_w((*mass_char)[i+1], &max_len_now);
			if(col_sym1<col_sym2)
			{
				str=(*mass_char)[i];
				(*mass_char)[i]=(*mass_char)[i+1];
				(*mass_char)[i+1] = str;
				k = 0;
				(*col_swap)++;
			}
		}
	}
	*max_len = max_len_now;
	return 0;
}
int main()
{
	mtrace();
	int col_str = 0, max_len = 0, col_swap = 0;
	char** mass_char = NULL;
	if(input(&mass_char, &col_str)==0)
	{
		sort_first_word(&mass_char, &max_len, &col_swap, col_str);
		for(int i = 0; i<col_str;i++)
		{
			printf("%s\n", mass_char[i]);
		}
		printf("Количество перестановок %d\n", col_swap);
		printf("Максимальная длина слова %d\n", max_len);
	}
	free(mass_char);
	return 0;
}
