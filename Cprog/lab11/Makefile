run : server client
	./server<test.txt &
	xterm ./client &
	xterm ./client &
	xterm ./client &
	xterm ./client &
server : server.o
	gcc server.o -Wall -o server -lpthread
server.o : server.c
	gcc -g -c server.c
client: client.o
	gcc client.o -Wall -o client -lpthread
client.o : client.c
	gcc -g -c client.c
clean : server server.o 
	rm server server.o
	rm client client.o
