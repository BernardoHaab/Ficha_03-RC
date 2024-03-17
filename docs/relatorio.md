# Relatório

---
title: Ficha 03
author: Bernardo Haab, Luís Góis
date: março 17, 2024
---

## Código fonte

Todos os ficheiros headers encontram-se dentro do diretório `include` e todos os
ficheiros código em c encontram-se dentro do diretório `src`.

No ficheiro README.md encontra-se os passos de como compilar e executar os
programas tanto cliente como servidor.

O Código para TCP encontra-se em `tcp-client.c` e `tcp-server.c`.
O Código para UDP encontra-se em `udp-client.c` e `udp-server.c`.

Os outros ficheiros C, são ou helper functions para printar bonito para o
terminal, ou funções utilitários.

O `command.h` e `command.c` contêm código para lidar com um comando em ligações
tcp para devolver ou o ip de um dominio, ou o dominio em função de um ip.

Embora que vá um bocado além do que é pedido no exercício, deixamos estar o código assim.

Se não quiser compilar nem rodar os programas, também temos uma demonstração do
servidor/cliente tanto tcp como udp, junto com a captura dos pacotes através do
wireshark, em formato gif dentro do diretório assets. Nomeadamente,
`assets/demo-tcp.gif` para o tcp e `assets/demo-udp.gif` para o udp, e os
ficheiros de captura de pacotes do wireshark
`assets/demo-tcp-packets-capture.pcapng` e
`assets/demo-udp-packets-capture.pcapng`.

## Exercício

> Descreve de que forma, com recurso ao `Wireshark`, validou o funcionamento das
> aplicações desenvolvidas nos Exercícios 2 e 4.

Com o Wireshark foi possível identificar a troca de mensagens entre cliente e
servidor, tanto utilizando UDP quanto TCP. Na questão 2, utilizando comunicação
TCP foi possível ver identificar mensagens enviadas por ambas as partes (cliente
e servidor), e o Acknowledge para confirmar o recebimento.

![TCP Packets Wireshark Capture Screenshot](demo-tcp-packets-capture-wireshark-screenshot.png)

![UDP Packets Wireshark Capture Screenshot](demo-udp-packets-capture-wireshark-screenshot.png)
