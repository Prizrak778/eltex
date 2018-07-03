#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void divide_str(char *str1, int pos_divide, char *str_return)
{
	char str_start[pos_divide];
	char str_end[strlen(str1)-pos_divide];
	strncpy(str_start, str1, pos_divide);
	strcpy(str_end, str1+pos_divide);
	str1 = str_start;
	strcpy(str_return, str_end);
}
