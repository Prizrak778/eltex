#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern void union_str();
extern void divide_str();

int main()
{
	int get_num = 5;
	char str1[50];
	char str2[50];
	printf("Начальные данные:\n str1=1234, str2=qwerty, позиция для разделения 5\n");
	union_str("1234", "qwerty", str2);
	printf("union_func: str2=%s\n", str2);
	divide_str(str2, get_num, str1);
	printf("divide_func: str1 = %s, str2 = %s\n", str1, str2);
	return 0;
}

