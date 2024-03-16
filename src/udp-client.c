#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "debug.h"

#define SERVER_IP_DEFAULT "0.0.0.0"
#define SERVER_PORT_DEFAULT 9876
#define BUFFER_SIZE 512
#define NUMBER_MAX_LENGTH 5

void erro(char *s)
{
  perror(s);
  exit(1);
}

void usage(const char *const programName)
{
	fprintf(
			stderr,
			"Usage: %s <ip address> <port> <number>\n",
			programName
	       );
	exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
  struct sockaddr_in si_res, si_outra;

  int socketFD, receivedBytes;
  socklen_t slen = sizeof(si_outra);
  char buffer[BUFFER_SIZE];

  if (argc != 4) {
	  usage(argv[0]);
  }

  // Cria um socket para envio e recepção de pacotes UDP
  if ((socketFD = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
	  error("Erro na criação do socket");
  }

  debugMessage(stdout, OK, "UDP Socket successfully created!\n");

  // Preenchimento da socket address structure
  si_outra.sin_family = AF_INET;
  si_outra.sin_addr.s_addr = inet_addr(argv[1]);
  si_outra.sin_port = htons(atoi(argv[2]));

  char number[NUMBER_MAX_LENGTH];

  strncpy(number, argv[3], NUMBER_MAX_LENGTH);
  printf("Numero: %s\n", number);

  if (sendto(socketFD, number, strlen(number), 0, (struct sockaddr *)&si_outra, slen) < 0) {
    error("Erro no sendto");
  }

  // Espera recepção de mensagem (a chamada é bloqueante)
  if ((receivedBytes = recvfrom(socketFD, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&si_res, (socklen_t *)&slen)) == -1)
  {
    error("Erro no recvfrom");
  }
  // Para ignorar o restante conteúdo (anterior do buffer)
  buffer[receivedBytes] = '\0';

  printf("Resposta:\n%s", buffer);

  // Fecha socket e termina programa
  close(socketFD);
  return 0;
}
