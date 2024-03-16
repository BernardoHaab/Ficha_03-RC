#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/socket.h>

#define ANSI_LENGTH 10
#define RED    "\e[31m"
#define GREEN  "\e[32m"
#define BLUE   "\e[34m"
#define CYAN   "\e[36m"
#define YELLOW "\e[33m"
#define RESET  "\e[0m"


#define DEBUG_MESSAGE_TYPE_ENUM \
	WRAPPER(ERROR, RED) \
	WRAPPER(INFO, CYAN) \
	WRAPPER(WARNING, YELLOW) \
	WRAPPER(OK, GREEN)
#define DEBUG_MESSAGE_TYPE_STRING_MAX_LENGTH 10
typedef enum {
#define WRAPPER(type, color) type,
	DEBUG_MESSAGE_TYPE_ENUM
#undef WRAPPER
} DebugMessageType;

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
