all: server client udp_server

CC=gcc
CFLAGS=-Wall -Wextra -g -Iinclude

server: ex2/server.c
	$(CC) $(CFLAGS) -o $@ $^

client: ex2/client.c src/debug.c
	$(CC) $(CFLAGS) -o $@ $^

udp_server: ex3/udp_server.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f server client udp_server
