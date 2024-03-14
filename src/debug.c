#include <arpa/inet.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "debug.h"

#define IP_FORMAT \
	"{\n" \
	"    Address: %s\n" \
	"    Port: %d\n" \
	"}\n"
#define IP_FORMAT_BRIEF "%s:%d\n"

void printSocketIP(
		FILE *const file,
		const bool brief,
		const struct sockaddr_in address
		)
{
	char ipAddressString[INET_ADDRSTRLEN] = { [0] = '\0' };
	fprintf(file, brief ? IP_FORMAT_BRIEF : IP_FORMAT,
			inet_ntop(
				AF_INET,
				&address.sin_addr,
				ipAddressString,
				INET_ADDRSTRLEN
				),
			ntohs(address.sin_port)
			);
}

void debugMessage(
		FILE *const file,
		const DebugMessageType type,
		const char *const format,
		...
		) {
	char typeString[DEBUG_MESSAGE_TYPE_STRING_MAX_LENGTH] = { [0] = '\0' };
	char colorANSI[ANSI_LENGTH] = { [0] = '\0' };

	va_list args;
	va_start(args, format);

	switch (type) {
#define WRAPPER(type, color)                                                   \
		case (type): {                                                 \
            strncpy(typeString, #type, DEBUG_MESSAGE_TYPE_STRING_MAX_LENGTH);  \
            strncpy(colorANSI, color, ANSI_LENGTH);                            \
            break;                                                             \
			     }
        DEBUG_MESSAGE_TYPE_ENUM
#undef WRAPPER
    }

	fprintf(file, "%s[%s]\e[0m: ", colorANSI, typeString);
	vfprintf(file, format, args);

	va_end(args);
}

void error(const char *const message) {
	debugMessage(stderr, ERROR, "%s", message);
	perror(message);
	exit(EXIT_FAILURE);
}
