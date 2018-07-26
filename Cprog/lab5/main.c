#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern void union_str();
extern void divide_str();
#define MAX_LEN 50
#define str2_origin "1234"
#define str1_origin "qwerty"
#define DIVIDE_NUM 5

int main()
{
	int get_num = DIVIDE_NUM;
	char str1[MAX_LEN];
	char str2[MAX_LEN];
	printf("Начальные данные:\n str1=%s, str2=%s, позиция для разделения 5\n", str1_origin, str2_origin);
	union_str(str2_origin, str1_origin, str2);
	printf("union_func: str2=%s\n", str2);
	divide_str(str2, get_num, str1);
	printf("divide_func: str1 = %s, str2 = %s\n", str1, str2);
	return 0;
}

