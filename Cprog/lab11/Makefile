run : server
	./server<test.txt 
server : server.o
	gcc server.o -Wall -o server -lpthread
server.o : server.c
	gcc -g -c server.c
clean : server server.o 
	rm server server.o
