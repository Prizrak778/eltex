run : server client client2
	./server 192.168.43.244 &
	xterm ./client &
	xterm ./client2 &

server : server.o
	gcc server.o -Wall -o server -lpthread
server.o : server.c
	gcc -g -c server.c

client: client.o
	gcc client.o -Wall -o client -lpthread
client.o : client.c
	gcc -g -c client.c

client2: client2.o
	gcc client2.o -Wall -o client2 -lpthread
client2.o: client2.c
	gcc -g -c client2.c

clean : server server.o client client.o client2 client2
	rm server server.o
	rm client client.o
	rm client2 client2.o
