run : main main2
	./main test 5
	./main2 test 3 
main : main.o
	gcc main.o -Wall -o main
main.o : main.c
	gcc -std=c99 -g -c main.c
main2 : main2.o
	gcc main2.o -Wall -o main2
main2.o : main2.c
	gcc -std=c99 -g -c main2.c
clean : main main.o 
	rm main main.o
	rm main2 main2.o
