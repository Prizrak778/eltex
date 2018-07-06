run : main main2
	./main2<test.txt
	./main<test.txt

main : main.o
	gcc main.o -Wall -o main
main.o : main.c
	gcc -g -c main.c

main2: main2.o
	gcc main2.o -Wall -o main2
main2.o: main2.c
	gcc -g -c main2.c -o main2.o

clean : main main.o 
	rm main main.o
