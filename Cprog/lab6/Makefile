run : main
	./main<test.txt 
main : main.o
	gcc main.o -Wall -o main
main.o : main.c
	gcc -std=c99 -g -c main.c
clean : main main.o 
	rm main main.o
