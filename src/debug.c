#include "debug.h"
#include <arpa/inet.h>

#define FORMAT \
	"{\n" \
	"    Address: %s\n" \
	"    Port: %d\n" \
	"}\n"

void printSocketIP(FILE *const file, struct sockaddr_in address)
{
	char ipAddressString[INET_ADDRSTRLEN] = { [0] = '\0' };
	fprintf(file, FORMAT,
			inet_ntop(
				AF_INET,
				&address.sin_addr,
				ipAddressString,
				INET_ADDRSTRLEN
				),
			ntohs(address.sin_port)
			);
}
