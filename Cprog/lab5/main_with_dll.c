#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>


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
	char str1[50];
	char str2[50];
	int get_num=5;
	printf("Начальные данные:\n str1=1234, str2=qwerty, позийция для разделения 5\n");
	union_str("1234", "qwerty", str2);
	printf("union_func :str2=%s\n", str2);
	divide_str(str2, get_num, str1);
	printf("divide_func: str1 = %s, str2 = %s\n", str1, str2);
	dlclose(library_handler);
	return 0;
}
