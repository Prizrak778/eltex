#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main()
{
	printf("====firts====\n");
	int a='4326';
	printf("sizeof(int)=%d\n", sizeof(int));
	printf("int a=\'4326\'=%d\n", a);
	char *p=&a;
	for(int i = 0; i<sizeof(int); i++)
	{
		printf("&a =%c\n", *(p+i));
	}
	printf("====second====\n");
	int *str1=(int *)"qwerty";
	char *str2=(char *)str1;
	printf("str1=(int *)\"qwerty\"\n");
	printf("str1=%d\n", str1);
	printf("str2=%s\n", str2);
	return 0;
}
