# Ficha 03 RC

## Compilação

Para compilar todos os executáveis:

```sh
make
```

### Compilar server

```sh
make server
```

### Compilar client

```sh
make client
```

## Rodar um servidor tcp

```sh
./server
```

Optionalmente, também é possível especificar o endereço e a porta de escuta do
servidor.
Para mais informações ver a help usage page:

```sh
./server --help

./server --address 127.0.0.1 --port 9090
```

## Rodar um cliente

```sh
./client <ip> <port>
```
