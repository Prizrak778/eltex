run : main main2 main3
	./main
	./main2
	./main3

main3 : libfunc_str.so main3.o
	gcc main3.o -L. -ldl -o main3
main3.o : main_with_dll.c
	gcc -c main_with_dll.c -o main3.o
main2 : libfunc_str.so main2.o
	gcc main2.o -o main2 -L. -lfunc_str -Wl,-rpath,.
main2.o: main.c
	gcc -c main.c -o main2.o
libfunc_str.so: union_str2.o divide_str2.o
	gcc -shared -o libfunc_str.so union_str2.o divide_str2.o
union_str2.o : union_str.c 
	gcc -c -fPIC union_str.c -o union_str2.o 
divide_str2.o : divide_str.c
	gcc -c -fPIC divide_str.c -o divide_str2.o

main : main.c libfunc_str.a
	gcc main.c -L. -lfunc_str -Wall -o main
libfunc_str.a : union_str.o divide_str.o
	ar rc libfunc_str.a union_str.o divide_str.o
	ranlib libfunc_str.a
union_str.o :
	gcc -g -c -Wall union_str.c
divide_str.o : 
	gcc -g -c -Wall divide_str.c


clean : 
	rm main main2 main2.o union_str.o divide_str.o divide_str2.o union_str2.o libfunc_str.a libfunc_str.so
