all: server client udp_server

CC=gcc
CFLAGS=-Wall -Wextra -g

server: ex2/server.c
	$(CC) $(CFLAGS) -o server ex2/server.c

client: ex2/client.c
	$(CC) $(CFLAGS) -o client ex2/client.c

udp_server: ex3/udp_server.c
	$(CC) $(CFLAGS) -o udp_server ex3/udp_server.c

clean:
	rm -f server client udp_server
