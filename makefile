all: server client

server: server.c
	gcc -o server server.c

client: client.c
	gcc -pthread -o client client.c
