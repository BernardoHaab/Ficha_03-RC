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
#include <stdarg.h>
#include <getopt.h>

#define SERVER_IP_DEFAULT "0.0.0.0"
#define SERVER_PORT_DEFAULT 9014
#define BUFFER_SIZE 1024
#define DOMAIN_FILEPATH "domains.txt"
#define LISTEN_N_CONNECTIONS 5
#define loop for(;;)

void error(const char *message) {
	perror(message);
	exit(EXIT_FAILURE);
}

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
	fprintf(file, "\n");

	va_end(args);
}

void processClient(const int clientSocketFD)
{
	char buffer[BUFFER_SIZE + 1] = {
		[0] = '\0',
		[BUFFER_SIZE] = '\0'
	};

	int nread = 0;
	do {
		nread = read(clientSocketFD, buffer, BUFFER_SIZE);
		buffer[nread] = '\0';

		if (nread > 0) {
			printf("Received %d bytes: %s\n", nread, buffer);
			fflush(stdout);
		} else if (nread == 0) {
			debugMessage(
					stderr,
					INFO,
					"Client Closed the connection"
				    );
		} else {
			debugMessage(stderr, ERROR, "Reading from client");
		}
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

int main(int argc, char **argv)
{
	int c;
	char *ipAddress = SERVER_IP_DEFAULT;
	int port = SERVER_PORT_DEFAULT;
	char *filePath = NULL;

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
				usage(argv[0]);
				return 0;
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

	serverIPAddress.sin_family = AF_INET;
	serverIPAddress.sin_addr.s_addr = inet_addr(ipAddress);
	serverIPAddress.sin_port = htons(port);

	serverSocketFD = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocketFD < 0) {
		error("Error opening socket\n");
	}

	debugMessage(stdout, OK, "TCP Socket successfully opened!");

	if (bind(
				serverSocketFD,
				(struct sockaddr *) &serverIPAddress,
				sizeof(serverIPAddress)
		) < 0) {
		error("Error binding socket\n");
	}

	char ipAddressString[INET_ADDRSTRLEN] = {0};
	debugMessage(stdout, OK, "Successfully binded to "
			"\e[1;31m%s\e[0m:\e[1;32m%d\e[0m",
			inet_ntop(
				AF_INET,
				&serverIPAddress.sin_addr,
				ipAddressString,
				INET_ADDRSTRLEN
				),
			ntohs(serverIPAddress.sin_port));
	debugMessage(stdout, INFO, "Listening for connections...");

	if (listen(serverSocketFD, LISTEN_N_CONNECTIONS) < 0) {
		error("Error listening for packets\n");
	}

	debugMessage(stdout, OK, "Successfully started listening "
			"for client connections...");

	printf("\n");

	const long clientIPAddressSize = sizeof(clientIPAddress);

	while (true) {
		// NOTE: Clean finished child process to avoid zombies
		while (waitpid(-1, NULL, WNOHANG) > 0);

		clientSocketFD = accept(
				serverSocketFD,
				(struct sockaddr *) &clientIPAddress,
				(socklen_t *) &clientIPAddressSize
				);

		if (clientSocketFD <= 0) {
			continue;
		}

		if (fork() == 0) {
			close(serverSocketFD);
			processClient(clientSocketFD);
			exit(0);
		}

		close(clientSocketFD);
	}

	return 0;
}
