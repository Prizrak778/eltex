#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LEN 50

struct sportman
{
	char surname[MAX_LEN];
	char name_sport[MAX_LEN];
	int col_medal;
};

int compare(const struct sportman *x1, const struct sportman *x2)
{
	if(x1->col_medal>0)
	{
		if(x2->col_medal>0)
		{
			return strcmp(x1->surname, x2->surname);
		}
		else
		{
			return -1;
		}
	}
	else
	{
		return 1;
	}
}

void output(struct sportman *spisok, int col_sportman)
{
	printf("Отсортированный список \n");
	for(int i = 0; i<col_sportman; i++)
	{
		printf("Фамилия :%s\n", spisok[i].surname);
	}
}

void input(struct sportman **spisok, int *col_sportman)
{
	printf("Введите кол спортсменов\n");
	scanf("%d", col_sportman);
	//struct sportman *spisok = malloc(sizeof(struct sportman)**col_sportman);
	*spisok = malloc(sizeof(struct sportman)*(*col_sportman));
	printf("Введите фамилию спортсмена, его вид спорта и кол медалей\n");
	for(int i = 0; i<*col_sportman; i++)
	{
		printf("Для %d спортсмена\n", i+1);
		printf("Фамилия:\n");
		scanf("%s", (*spisok)[i].surname);
		printf("Вид спорта:\n");
		scanf("%s", (*spisok)[i].name_sport);
		printf("Кол медалей:\n");
		scanf("%d", &(*spisok)[i].col_medal);
	}
}

int main()
{
	struct sportman *spisok;
	int col_sportman = 0;
	input(&spisok, &col_sportman);
	qsort(spisok, col_sportman, sizeof(struct sportman), compare);
	output(spisok, col_sportman);
	return 0;
}
