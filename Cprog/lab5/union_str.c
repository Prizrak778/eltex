#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void union_str(char *str1, char *str2, char str_return[])
{
	char str_temp[50];
	printf("%s %s\n", str1, str2);
	strcpy(str_temp, str1);
	printf("%s\n", str_temp);
	strcat(str_temp, str2);
	printf("%s\n", str_temp);
	strcpy(str_return, str_temp);
	printf("111\n");
	printf("%s\n", str_return);
}
