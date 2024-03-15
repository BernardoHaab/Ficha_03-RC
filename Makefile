all: server client udp_server

CC=gcc
CFLAGS=-Wall -Wextra -g -I$(PWD)/include

SRC_DIR = src

server: $(SRC_DIR)/server.c $(SRC_DIR)/debug.c $(SRC_DIR)/command.c
	$(CC) $(CFLAGS) -o $@ $^

client: $(SRC_DIR)/client.c $(SRC_DIR)/debug.c
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
