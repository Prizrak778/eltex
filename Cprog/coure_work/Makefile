run : server client client2
	xterm ./client &
	xterm ./client &
	xterm ./client &
	xterm ./client2 &
	xterm ./client2 &
	xterm ./client2 &
	xterm ./server &

server : server.c
	protoc-c --c_out=. cmessage.proto
	gcc cmessage.pb-c.c server.c -std=c99 -Wall -o server -lprotobuf-c -lpthread

client: client.c
	protoc-c --c_out=. cmessage.proto
	gcc cmessage.pb-c.c client.c -std=c99 -Wall -o client -lprotobuf-c -lpthread

client2: client2.c
	protoc-c --c_out=. cmessage.proto
	gcc cmessage.pb-c.c  client2.c -std=c99 -Wall -o client2 -lprotobuf-c -lpthread

clean : server server.o client client.o client2 client2
	rm server server.o
	rm client client.o
	rm client2 client2.o
