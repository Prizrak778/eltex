#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_LEN 50


int main()
{
	char str1[MAX_LEN];
	char str2[]="qwertyuiop";
	strcpy(str1, "1234567890");
	printf("str2=%s, len_str2=%d, str1=%s, len_str1=%d\n", str2, strlen(str2) ,str1 ,strlen(str1));
	strcat(str2, str1);
	printf("str2=%s, len_str2=%d, str1=%s, len_str1=%d\n", str2, strlen(str2) ,str1 ,strlen(str1));
	return 0;
}
