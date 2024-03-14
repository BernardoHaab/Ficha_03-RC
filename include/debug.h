#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/socket.h>

#define DEBUG_MESSAGE_TYPE_ENUM \
	WRAPPER(ERROR, "\033[1;31m") \
	WRAPPER(INFO, "\033[1;36m") \
	WRAPPER(WARNING, "\033[1;33m") \
	WRAPPER(OK, "\033[1;32m")
#define DEBUG_MESSAGE_TYPE_STRING_MAX_LENGTH 10
typedef enum {
#define WRAPPER(type, color) type,
	DEBUG_MESSAGE_TYPE_ENUM
#undef WRAPPER
} DebugMessageType;
#define ANSI_LENGTH 10


void printSocketIP(
		FILE *const file,
		const bool brief,
		const struct sockaddr_in address
		);
void debugMessage(
		FILE *const file,
		const DebugMessageType type,
		const char *const format,
		...
		);
void error(const char *const message);
