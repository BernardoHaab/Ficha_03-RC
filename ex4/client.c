#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFLEN 512    // Tamanho do buffer
#define PORT_OUT 9876 // Porto para envio das mensagens

void erro(char *s)
{
  perror(s);
  exit(1);
}

int main(int argc, char *argv[])
{
  struct sockaddr_in si_res, si_outra;

  int s, recv_len;
  socklen_t slen = sizeof(si_outra);
  char buf[BUFLEN], buf_res[BUFLEN];

  if (argc != 2)
  {
    printf("client <number>");
    exit(-1);
  }

  printf("argv[1]: %s", argv[1]);

  // Cria um socket para envio e recepção de pacotes UDP
  if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
  {
    erro("Erro na criação do socket");
  }

  // Preenchimento da socket address structure
  si_outra.sin_family = AF_INET;
  si_outra.sin_port = htons(PORT_OUT);
  si_outra.sin_addr.s_addr = htonl(INADDR_ANY);

  // // Preenchimento da socket address structure
  // si_minha.sin_family = AF_INET;
  // si_minha.sin_port = htons(PORT_OUT);
  // si_minha.sin_addr.s_addr = htonl(INADDR_ANY);

  // // Associa o socket à informação de endereço
  // if (bind(s, (struct sockaddr *)&si_minha, sizeof(si_minha)) == -1)
  // {
  //   erro("Erro no bind");
  // }

  // // Espera recepção de mensagem (a chamada é bloqueante)
  // if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *)&si_outra, (socklen_t *)&slen)) == -1)
  // {
  //   erro("Erro no recvfrom");
  // }
  // // Para ignorar o restante conteúdo (anterior do buffer)
  // buf[recv_len] = '\0';

  // printf("Recebi uma mensagem do sistema com o endereço %s e o porto %d\n", inet_ntoa(si_outra.sin_addr), ntohs(si_outra.sin_port));
  // printf("Conteúdo da mensagem: %s\n", buf);

  char number[5];

  strncpy(number, argv[1], 5);
  printf("Numero: %s\n", number);

  if (sendto(s, number, strlen(number), 0, (struct sockaddr *)&si_outra, slen) < 0)
  {
    erro("Erro no sendto");
  }

  // Espera recepção de mensagem (a chamada é bloqueante)
  if ((recv_len = recvfrom(s, buf_res, BUFLEN, 0, (struct sockaddr *)&si_res, (socklen_t *)&slen)) == -1)
  {
    erro("Erro no recvfrom");
  }
  // Para ignorar o restante conteúdo (anterior do buffer)
  buf_res[recv_len] = '\0';

  printf("Res: %s", buf_res);

  // Fecha socket e termina programa
  close(s);
  return 0;
}
