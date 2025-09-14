# Makefile do programa de busca com árvore B+

CC      = gcc
OBJ     = DispersaoExt.o Leitura.o main.o Registros.o
LINKOBJ = DispersaoExt.o Leitura.o main.o Registros.o
BIN     = Secao08_2_6_ALT.exe
CFLAGS  = -std=c99 -Wall -g
LFLAGS  = $(CFLAGS) -lm

$(BIN): $(OBJ)
	$(CC) $(LINKOBJ) -o $(BIN) $(LFLAGS)

DispersaoExt.o: DispersaoExt.c DispersaoExt.h ASSEGURA.h
	$(CC) -c DispersaoExt.c -o DispersaoExt.o $(CFLAGS)

Leitura.o: Leitura.c Leitura.h
	$(CC) -c Leitura.c -o Leitura.o $(CFLAGS)

Registros.o: Registros.c Registros.h ASSEGURA.h
	$(CC) -c Registros.c -o Registros.o $(CFLAGS)

main.o: main.c DispersaoExt.h Registros.h Leitura.h ASSEGURA.h
	$(CC) -c main.c -o main.o $(CFLAGS)
