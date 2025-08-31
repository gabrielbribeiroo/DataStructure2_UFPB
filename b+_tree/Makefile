# Makefile do programa de busca com árvore B+

CC      = gcc
OBJ     = ArvoreBM.o Leitura.o main.o Registros.o Pilha.o
LINKOBJ = ArvoreBM.o Leitura.o main.o Registros.o Pilha.o
BIN     = Secao06_5.exe
CFLAGS  = -std=c99 -Wall -g
LFLAGS  = $(CFLAGS) -lm

$(BIN): $(OBJ)
	$(CC) $(LINKOBJ) -o $(BIN) $(LFLAGS)

ArvoreBM.o: ArvoreBM.c ArvoreBM.h ASSEGURA.h
	$(CC) -c ArvoreBM.c -o ArvoreBM.o $(CFLAGS)

Leitura.o: Leitura.c Leitura.h
	$(CC) -c Leitura.c -o Leitura.o $(CFLAGS)

main.o: main.c ArvoreBM.h Registros.h Leitura.h ASSEGURA.h
	$(CC) -c main.c -o main.o $(CFLAGS)

Registros.o: Registros.c Registros.h ASSEGURA.h
	$(CC) -c Registros.c -o Registros.o $(CFLAGS)

Pilha.o: Pilha.c Pilha.h ASSEGURA.h
	$(CC) -c Pilha.c -o Pilha.o $(CFLAGS)
