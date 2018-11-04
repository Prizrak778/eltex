run : game_fild
	./game_fild<test.txt 
game_fild : main.o
	gcc main.o -Wall -o game_fild
main.o : main.c
	gcc -std=c99 -g -c main.c
clean : main main.o 
	rm game_fild main.o
