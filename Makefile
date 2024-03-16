all: tcp-server tcp-client udp-server udp-client

CC=gcc
CFLAGS=-Wall -Wextra -g -I$(PWD)/include

SRC_DIR = src

tcp-server: $(SRC_DIR)/tcp-server.c $(SRC_DIR)/debug.c $(SRC_DIR)/command.c
	$(CC) $(CFLAGS) -o $@ $^

tcp-client: $(SRC_DIR)/tcp-client.c $(SRC_DIR)/debug.c
	$(CC) $(CFLAGS) -o $@ $^

udp-server: $(SRC_DIR)/udp-server.c $(SRC_DIR)/debug.c
	$(CC) $(CFLAGS) -o $@ $^

udp-client: $(SRC_DIR)/udp-client.c
	$(CC) $(CFLAGS) -o $@ $^

archive: ficha03-pl8-BernardoHaab-LuísGóis.zip
.PHONY: ficha03-pl8-BernardoHaab-LuísGóis.zip
ficha03-pl8-BernardoHaab-LuísGóis.zip:
	git archive --output=$@ HEAD

clean:
	rm -f tcp-server tcp-client udp-server udp-client

.PHONY: .clangd
.clangd:
	rm --force $@

	@echo Diagnostics: | tee --append $@
	@echo '  UnusedIncludes: Strict' | tee --append $@
	@echo '  MissingIncludes: Strict' | tee --append $@
	@echo CompileFlags: | tee --append $@
	@echo '  Add:' | tee --append $@

	@for flag in $(CFLAGS) ; do echo "    - $$flag" | tee --append $@ ; done
