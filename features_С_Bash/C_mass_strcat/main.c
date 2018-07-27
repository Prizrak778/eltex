#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_LEN 50


int main()
{
	char str1[MAX_LEN];
	char str2[]="qwertyuiop";
	strcpy(str1, "1234567890");
	printf("=======1======\n");
	printf("str2=%s, len_str2=%d, str1=%s, len_str1=%d\n", str2, strlen(str2) ,str1 ,strlen(str1));
	strcat(str2, str1);
	printf("str2=%s, len_str2=%d, str1=%s, len_str1=%d\n", str2, strlen(str2) ,str1 ,strlen(str1));
	printf("=======2======\n");
	char str3[MAX_LEN];
	char str4[]={'q','w','e','r','t','y','u','i','o','p'};
	strcpy(str3, "1234567890");
	printf("str4=%s, len_str4=%d, str3=%s, len_str3=%d\n", str4, strlen(str4) ,str3 ,strlen(str3));
	strcat(str4, str3);
	printf("str4=%s, len_str4=%d, str3=%s, len_str3=%d\n", str4, strlen(str4) ,str3 ,strlen(str3));
	printf("=======3=======\n");
	char str5[MAX_LEN];
	char str6[]={113, 119, 101, 114, 116, 121, 117, 105, 111, 112};
	strcpy(str5, "1234567890");
	printf("str6=%s, len_str6=%d, str5=%s, len_str5=%d\n", str6, strlen(str6) ,str5 ,strlen(str5));
	strcat(str6, str5);
	printf("str6=%s, len_str6=%d, str5=%s, len_str5=%d\n", str6, strlen(str6) ,str5 ,strlen(str5));
	printf("=======4=======\n");
	const char str7[MAX_LEN];
	const char str8[]={113, 119, 101, 114, 116, 121, 117, 105, 111, 112};
	strcpy(str7, "1234567890");
	printf("str8=%s, len_str8=%d, str7=%s, len_str7=%d\n", str8, strlen(str8) ,str7 ,strlen(str7));
	strcat(str8, str7);
	printf("str8=%s, len_str8=%d, str7=%s, len_str7=%d\n", str8, strlen(str8) ,str7 ,strlen(str7));


	return 0;
}
