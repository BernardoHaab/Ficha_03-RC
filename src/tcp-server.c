#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <string.h>
#include <getopt.h>

#include "debug.h"
#include "command.h"
#include "utils.h"

#define SERVER_IP_DEFAULT "0.0.0.0"
#define SERVER_PORT_DEFAULT 9014
#define BUFFER_SIZE 1024
#define DOMAIN_FILEPATH "domains.txt"
#define LISTEN_N_CONNECTIONS 5
#define MOTD "Bem-vindo ao servidor de nomes do DEI. Indique o nome de dominio"
#define EXIT_MESSAGE "Até logo!"
#define loop for(;;)

FILE *domainFile = NULL;

void processClient(
		const int clientSocketFD,
		const struct sockaddr_in clientIPAddress
		)
{
	char receivedBuffer[BUFFER_SIZE] = {
		[0] = '\0',
		[BUFFER_SIZE - 1] = '\0'
	};


	int nread = 0;
	do {
		char responseBuffer[BUFFER_SIZE] = "";
		nread = read(clientSocketFD, receivedBuffer, BUFFER_SIZE);
		receivedBuffer[nread] = '\0';

		if (nread > 0) {
			char ipAddressString[INET_ADDRSTRLEN] = { [0] = '\0' };
			printf("[" RED "%s" RESET ":" CYAN "%-6hu" RESET
					"] Received " BLUE "%4d" RESET
					" bytes: %s",
					inet_ntop(
						AF_INET,
						&clientIPAddress.sin_addr,
						ipAddressString,
						INET_ADDRSTRLEN
						),
					clientIPAddress.sin_port,
					nread,
					receivedBuffer);
			if (countNChar(receivedBuffer, '\n', BUFFER_SIZE) == 0)
				printf("\n");

			Command command = processCommand(
					receivedBuffer,
					BUFFER_SIZE,
					responseBuffer
					);

			fflush(stdout);

			if (command == EXIT) {
				debugMessage(
						stderr,
						INFO,
						"Closing connection with: "
					    );
				printSocketIP(stdout, true, clientIPAddress);
				write(
						clientSocketFD,
						EXIT_MESSAGE "\n",
						sizeof(EXIT_MESSAGE "\n")
				     );
				break;
			}
		} else if (nread == 0) {
			debugMessage(
					stderr,
					INFO,
					"Client Closed the connection: "
				    );
			printSocketIP(stdout, true, clientIPAddress);
		} else {
			debugMessage(stderr, ERROR, "Reading from client\n");
		}

		const size_t responseBufferLength = strnlen(responseBuffer, BUFFER_SIZE);
		write(clientSocketFD, responseBuffer, responseBufferLength + 1);
	} while (nread > 0);

	close(clientSocketFD);
}

void usage(const char *const programName) {
    printf("Usage: %s [options]\n", programName);
    printf("Options:\n");
    printf("  -a, --address <ip address>   Set the server IP address\n");
    printf("  -p, --port <port>            Set the server port number\n");
    printf("  -f, --file <filepath>        Set the domain file text\n");
    printf("  -h, --help                   Print this usage message\n");

    exit(EXIT_FAILURE);
}

void sendMOTD(const int socketFD, const char *const motd)
{
	write(socketFD, motd, strnlen(motd, BUFFER_SIZE));
}

int main(int argc, char **argv)
{
	int c;
	char *ipAddress = SERVER_IP_DEFAULT;
	int port        = SERVER_PORT_DEFAULT;
	char *filePath  = NULL;

	static struct option long_options[] = {
		{"address", required_argument, 0, 'a'},
		{"port",    required_argument, 0, 'p'},
		{"file",    required_argument, 0, 'f'},
		{"help",    no_argument,       0, 'h'},
		{0,         0,                 0, 0}
	};

	loop {
		int option_index = 0;
		c = getopt_long(
				argc,
				argv,
				"a:p:f:h",
				long_options,
				&option_index
			       );
		if (c == -1) break;

		switch (c) {
			case 'a':
				ipAddress = optarg;
				break;
			case 'p':
				port = atoi(optarg);
				break;
			case 'f':
				filePath = optarg;
				break;
			case 'h':
			case ':':
			case '?':
			default:
				usage(argv[0]);
				break;
		}
	}

	int serverSocketFD, clientSocketFD;
	struct sockaddr_in serverIPAddress = {0};
	struct sockaddr_in clientIPAddress = {0};

	serverIPAddress.sin_family      = AF_INET;
	serverIPAddress.sin_addr.s_addr = inet_addr(ipAddress);
	serverIPAddress.sin_port        = htons(port);

	serverSocketFD = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocketFD < 0) {
		error("Error opening socket\n");
	}

	debugMessage(stdout, OK, "TCP Socket successfully opened!\n");

	if (bind(
				serverSocketFD,
				(struct sockaddr *) &serverIPAddress,
				sizeof(serverIPAddress)
		) < 0) {
		error("Error binding socket\n");
	}

	char ipAddressString[INET_ADDRSTRLEN] = {0};
	debugMessage(stdout, OK, "Successfully binded to "
			RED "%s" RESET ":" GREEN "%d" RESET "\n",
			inet_ntop(
				AF_INET,
				&serverIPAddress.sin_addr,
				ipAddressString,
				INET_ADDRSTRLEN
				),
			ntohs(serverIPAddress.sin_port));
	debugMessage(stdout, INFO, "Listening for connections...\n");

	if (listen(serverSocketFD, LISTEN_N_CONNECTIONS) < 0) {
		error("Error listening for packets\n");
	}

	debugMessage(stdout, OK, "Successfully started listening "
			"for client connections...\n");

	const long clientIPAddressSize = sizeof(clientIPAddress);

	domainFile = fopen(filePath, "r");
	if (domainFile == NULL) {
		error("Error opening file...\n");
	}

	loop {
		// NOTE: Clean finished child process to avoid zombies
		while (waitpid(-1, NULL, WNOHANG) > 0);

		if ((clientSocketFD = accept(
			serverSocketFD,
			(struct sockaddr *) &clientIPAddress,
			(socklen_t *) &clientIPAddressSize
			)
		    ) <= 0) {
			continue;
		}

		debugMessage(stdout, OK, "New Connection with client: ");
		printSocketIP(stdout, true, clientIPAddress);

		if (fork() == 0) {
			close(serverSocketFD);
			sendMOTD(clientSocketFD, MOTD "\n");
			processClient(clientSocketFD, clientIPAddress);
			exit(0);
		}

		close(clientSocketFD);
	}

	return 0;
}
