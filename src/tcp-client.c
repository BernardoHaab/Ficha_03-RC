#include <arpa/inet.h>
#include <bits/types/struct_iovec.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "debug.h"

#define BUFFER_SIZE 1024
#define EXIT_MESSAGE "Até logo!"
#define loop for (;;)

void usage(const char *const programName)
{
	fprintf(stderr, "Usage: %s <ip address> <port>\n", programName);
	exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
	struct sockaddr_in serverIPAddress = {0};
	int socketFD;
	char buffer[BUFFER_SIZE] = { [0] = '\0' , [BUFFER_SIZE - 1] = '\0' };

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

	loop {
		ssize_t receivedBytes = read(socketFD, buffer, BUFFER_SIZE);
		if (receivedBytes > 0) {
			printf("%s", buffer);

			if (strncmp(
						buffer,
						EXIT_MESSAGE "\n",
						sizeof(EXIT_MESSAGE "\n")
				   ) == 0) {
				break;
			}
		}

		if (receivedBytes < 0) {
			perror("Receiving failed");
			exit(EXIT_FAILURE);
		} else if (receivedBytes == 0) {
			break;
		}


		printf("> ");
		if (scanf(" %[^\n]%*c", buffer) == EOF) {
			break;
		}

		const size_t bufferLength = strnlen(buffer, BUFFER_SIZE);

		write(socketFD, buffer, bufferLength + 1);
	}

	debugMessage(stdout, INFO, "Closing connection with server ");
	printSocketIP(stdout, true, serverIPAddress);

	close(socketFD);

	return 0;
}
