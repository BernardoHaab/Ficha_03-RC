#include <arpa/inet.h>
#include <getopt.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "debug.h"
#include "utils.h"

#define SERVER_IP_DEFAULT "0.0.0.0"
#define SERVER_PORT_DEFAULT 9876
#define BUFFER_SIZE 512   // Tamanho do buffer
#define loop for(;;)

void erro(char *s)
{
  perror(s);
  exit(1);
}

void usage(const char *const programName) {
    printf("Usage: %s [options]\n", programName);
    printf("Options:\n");
    printf("  -a, --address <ip address>   Set the server IP address\n");
    printf("  -p, --port <port>            Set the server port number\n");
    printf("  -h, --help                   Print this usage message\n");

    exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
	int c;
	char *ipAddress = SERVER_IP_DEFAULT;
	int port        = SERVER_PORT_DEFAULT;

	static struct option long_options[] = {
		{"address", required_argument, 0, 'a'},
		{"port",    required_argument, 0, 'p'},
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
			case 'h':
			case ':':
			case '?':
			default:
				usage(argv[0]);
				break;
		}
	}

  struct sockaddr_in si_minha, si_outra;

  int socketFD, receivedBytes;
  socklen_t slen = sizeof(si_outra);
  char buffer[BUFFER_SIZE];

  // Cria um socket para recepção de pacotes UDP
  if ((socketFD = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
    error("Erro na criação do socket");
  }

  debugMessage(stdout, OK, "UDP Socket successfully opened!\n");

  // Preenchimento da socket address structure
  si_minha.sin_family      = AF_INET;
  si_minha.sin_port        = htons(port);
  si_minha.sin_addr.s_addr = inet_addr(ipAddress);

  // Associa o socket à informação de endereço
  if (bind(socketFD, (struct sockaddr *)&si_minha, sizeof(si_minha)) == -1)
  {
	  error("Error binding socket\n");
  }

  char ipAddressString[INET_ADDRSTRLEN] = {0};
  debugMessage(stdout, OK, "Successfully binded to "
  		RED "%s" RESET ":" GREEN "%d" RESET "\n",
  		inet_ntop(
  			AF_INET,
  			&si_minha.sin_addr,
  			ipAddressString,
  			INET_ADDRSTRLEN
  			),
  		ntohs(si_minha.sin_port));
  debugMessage(stdout, INFO, "Listening for connections...\n");

  do {
	  // Espera recepção de mensagem (a chamada é bloqueante)
	  if ((receivedBytes = recvfrom(
					  socketFD,
					  buffer,
					  BUFFER_SIZE,
					  0,
					  (struct sockaddr *) &si_outra,
					  (socklen_t *) &slen
				       )) == -1) {
		  error("Erro no recvfrom");
	  }

	  // Para ignorar o restante conteúdo (anterior do buffer)
	  buffer[receivedBytes] = '\0';

	  int decimal = atoi(buffer);

	  printf("[" RED "%s " RESET ":" CYAN "%-6hu" RESET
			  "] Received " BLUE "%4d" RESET
			  " bytes: %s",
			  inet_ntop(
				  AF_INET,
				  &si_outra.sin_addr,
				  ipAddressString,
				  INET_ADDRSTRLEN
				  ),
			  si_outra.sin_port,
			  receivedBytes,
			  buffer);
	  if (countNChar(buffer, '\n', BUFFER_SIZE) == 0) printf("\n");

	  char msg[BUFFER_SIZE] = { [0] = '\0', [BUFFER_SIZE - 1] = '\0' };
	  sprintf(
			  msg,
			  "Número em binário: 0b%b\nNúmero em hexadecimal: 0x%X\n",
			  decimal,
			  decimal
		 );

	  if (sendto(
				  socketFD,
				  msg,
				  strlen(msg),
				  0,
				  (struct sockaddr *) &si_outra,
				  slen
		    ) < 0) {
		  error("Erro no sendto");
	  }
  } while (strncmp(buffer, "SAIR", 5) != 0);

  // Fecha socket e termina programa
  close(socketFD);
  return 0;
}
