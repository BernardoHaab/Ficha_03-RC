#ifndef COMMAND_H
#define COMMAND_H

#include <stdlib.h>

#define COMMAND_ENUM \
	WRAPPER(HELP, "help", "help", commandHelp) \
	WRAPPER(DOMAIN, "domain", "domain <domain>", commandDomain) \
	WRAPPER(IP, "ip", "ip <ip>", commandIp)
typedef enum {
#define WRAPPER(enum, text, usage, function) enum,
	COMMAND_ENUM
#undef WRAPPER
} Command;

void processCommand(const char *const buffer, const size_t bufferSize, char *response);

void commandHelp(const char *const argument, char *response);
void commandDomain(const char *const argument, char *response);
void commandIp(const char *const argument, char *response);

#endif // !COMMAND_H
