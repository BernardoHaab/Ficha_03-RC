all: server client udp_server

CC=gcc
CFLAGS=-Wall -Wextra -g -I$(PWD)/include

server: ex2/server.c src/debug.c
	$(CC) $(CFLAGS) -o $@ $^

client: ex2/client.c src/debug.c
	$(CC) $(CFLAGS) -o $@ $^

udp_server: ex3/udp_server.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f server client udp_server

.PHONY: .clangd
.clangd:
	rm --force $@

	@echo Diagnostics: | tee --append $@
	@echo '  UnusedIncludes: Strict' | tee --append $@
	@echo '  MissingIncludes: Strict' | tee --append $@
	@echo CompileFlags: | tee --append $@
	@echo '  Add:' | tee --append $@

	@for flag in $(CFLAGS) ; do echo "    - $$flag" | tee --append $@ ; done
