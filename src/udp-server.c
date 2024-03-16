#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFLEN 512   // Tamanho do buffer
#define PORT_IN 9876 // Porto para recepção das mensagens

void decimalToBinary(int num, int res[32]);
void decToHex(int decimal, int hexadecimal[8]);

void erro(char *s)
{
  perror(s);
  exit(1);
}

int main(void)
{
  struct sockaddr_in si_minha, si_outra;

  int s, recv_len;
  socklen_t slen = sizeof(si_outra);
  char buf[BUFLEN];

  // Cria um socket para recepção de pacotes UDP
  if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
  {
    erro("Erro na criação do socket");
  }

  // Preenchimento da socket address structure
  si_minha.sin_family = AF_INET;
  si_minha.sin_port = htons(PORT_IN);
  si_minha.sin_addr.s_addr = htonl(INADDR_ANY);

  // Associa o socket à informação de endereço
  if (bind(s, (struct sockaddr *)&si_minha, sizeof(si_minha)) == -1)
  {
    erro("Erro no bind");
  }

  // Espera recepção de mensagem (a chamada é bloqueante)
  if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *)&si_outra, (socklen_t *)&slen)) == -1)
  {
    erro("Erro no recvfrom");
  }
  // Para ignorar o restante conteúdo (anterior do buffer)
  buf[recv_len] = '\0';

  while (strncmp(buf, "SAIR", 5) != 0)
  {
    int decimal = atoi(buf);
    int binary[32] = {0};
    int hexadecimal[8] = {0};

    printf("Recebi uma mensagem do sistema com o endereço %s e o porto %d\n", inet_ntoa(si_outra.sin_addr), ntohs(si_outra.sin_port));
    printf("Decimal recebido: %d\n", decimal);

    decimalToBinary(decimal, binary);
    decToHex(decimal, hexadecimal);

    char binaryStr[33] = {0}, hexaStr[8] = {0};

    for (int i = 0; i < 32; i++)
    {
      binaryStr[i] = binary[31 - i] + '0';
    }

    for (int i = 7; i >= 0; i--)
    {
      char temp[2];
      sprintf(temp, "%c", hexadecimal[i]);
      strcat(hexaStr, temp);
    }

    char msg[1000] = {0};

    sprintf(msg, "Número em binário: %s \nNúmero em hexadecimal: %s\n", binaryStr, hexaStr);

    if (sendto(s, msg, strlen(msg), 0, (struct sockaddr *)&si_outra, slen) < 0)
    {
      erro("Erro no sendto");
    }

    // Espera recepção de mensagem (a chamada é bloqueante)
    if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *)&si_outra, (socklen_t *)&slen)) == -1)
    {
      erro("Erro no recvfrom");
    }
    // Para ignorar o restante conteúdo (anterior do buffer)
    buf[recv_len] = '\0';
  }

  // Fecha socket e termina programa
  close(s);
  return 0;
}

void decimalToBinary(int decimal, int binary[32])
{
  for (int i = 0; i < 32; ++i)
    binary[i] = 0;

  int i = 0;

  while (decimal > 0)
  {
    binary[i] = decimal & 1;
    decimal = decimal >> 1;
    i++;
  }
}

void decToHex(int decimal, int hexadecimal[8])
{
  if (decimal == 0)
  {
    printf("Hexadecimal: 0\n");
    return;
  }

  for (int i = 0; i < 8; ++i)
    hexadecimal[i] = 0;

  // char hexadecimal[100];
  int indx = 0;

  while (decimal > 0)
  {
    int remainder = decimal % 16;

    if (remainder < 10)
      hexadecimal[indx++] = remainder + '0';
    else
      hexadecimal[indx++] = remainder + 'A' - 10;

    decimal /= 16;
  }
}
