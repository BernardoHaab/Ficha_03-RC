#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

#include "debug.h"

void usage(const char *const programName)
{
	fprintf(stderr, "Usage: %s <ip address> <port>\n", programName);
	exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
	struct sockaddr_in serverIPAddress = {0};
	int socketFD;

	if (argc < 3) {
		usage(argv[0]);
		exit(EXIT_FAILURE);
	}

	serverIPAddress.sin_family = AF_INET;
	serverIPAddress.sin_addr.s_addr = inet_addr(argv[1]);
	serverIPAddress.sin_port = htons(atoi(argv[2]));

	debugMessage(stdout, INFO, "Connecting to server: ");
	printSocketIP(stdout, true, serverIPAddress);

	if ((socketFD = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		error("Error creating socket\n");
	}

	if (connect(
				socketFD,
				(struct sockaddr *) &serverIPAddress,
				sizeof(serverIPAddress)
		   ) < 0) {
		error("Error Connecting to server\n");
	}

	sleep(1);

	close(socketFD);

	return 0;
}
