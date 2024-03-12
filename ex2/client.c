/***********************************************************************
 * CLIENTE liga ao servidor (definido em argv[1]) no porto especificado
 * (em argv[2]), escrevendo a palavra predefinida (em argv[3]).
 * USO: >cliente <enderecoServidor> <porto> <Palavra>
 ***********************************************************************/
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>

#define SERVER_PORT 9002
#define BUFF_SIZE 1024

void sendIp(int fd);
void erro(char *msg);

int main(int argc, char *argv[])
{
  char endServer[100];
  int fd;
  struct sockaddr_in addr;
  struct hostent *hostPtr;
  if (argc != 4)
  {
    printf("cliente <host> <port> <string>\n");
    exit(-1);
  }
  strcpy(endServer, argv[1]);
  if ((hostPtr = gethostbyname(endServer)) == 0)
    erro("Nao consegui obter endereço");
  bzero((void *)&addr, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons((short)atoi(argv[2]));

  if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    erro("socket");
  if (connect(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    erro("Connect");

  // write(fd, argv[3], 1 + strlen(argv[3]));

  sendIp(fd);

  close(fd);
  exit(0);
}

void sendIp(int fd)
{
  char res[BUFF_SIZE];

  read(fd, res, BUFF_SIZE - 1);
  printf("%s", res);
  bzero((void *)&res, sizeof(res));

  char domain[100];

  while (strcmp(domain, "SAIR") != 0)
  {
    scanf("%s", domain);

    write(fd, domain, sizeof(domain));

    read(fd, res, BUFF_SIZE - 1);
    printf("%s\n", res);
    bzero((void *)&res, sizeof(res));
  }
}

void erro(char *msg)
{
  printf("Erro: %s\n", msg);
  exit(-1);
}