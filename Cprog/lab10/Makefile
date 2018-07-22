run : main
	./main<test.txt 
main : main.o
	gcc main.o -Wall -o main -lpthread
main.o : main.c
	gcc -g -c main.c
clean : main main.o 
	rm main main.o
