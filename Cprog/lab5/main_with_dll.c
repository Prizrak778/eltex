#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

#define MAX_LEN 50
#define DIVIDE_NUM 5
#define str1_origin "1234"
#define str2_origin "qwerty"


int main()
{
	void *library_handler;
	library_handler = dlopen("./libfunc_str.so", RTLD_LAZY);
	if(!library_handler)
	{
		printf("dlopen() error: %s\n", dlerror());
		exit(1);
	}
	void (*union_str)(char* str1, char* str2, char* str_return) = dlsym(library_handler, "union_str");
	void (*divide_str)(char* str1, int pos_divide, char* str_return) = dlsym(library_handler, "divide_str");
	if(union_str==NULL||divide_str==NULL)
	{
		printf("dlsym() error : %s\n", dlerror());
		exit(1);
	}
	char str1[MAX_LEN];
	char str2[MAX_LEN];
	int get_num=DIVIDE_NUM;
	printf("Начальные данные:\n str1=%s, str2=%s, позийция для разделения %d\n", str1_origin, str2_origin, DIVIDE_NUM);
	union_str(str1_origin, str2_origin, str2);
	printf("union_func :str2=%s\n", str2);
	divide_str(str2, get_num, str1);
	printf("divide_func: str1 = %s, str2 = %s\n", str1, str2);
	dlclose(library_handler);
	return 0;
}
