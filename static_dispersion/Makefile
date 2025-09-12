# Makefile do programa de dispersão estática em memória secundária

CC      = gcc
OBJ     = DispersaoEstatica.o Leitura.o main.o Registros.o
LINKOBJ = DispersaoEstatica.o Leitura.o main.o Registros.o
BIN     = Secao08_1_3.exe
CFLAGS  = -std=c99 -Wall -g
LFLAGS  = $(CFLAGS) -lm

$(BIN): $(OBJ)
	$(CC) $(LINKOBJ) -o $(BIN) $(LFLAGS)

DispersaoEstatica.o: DispersaoEstatica.c DispersaoEstatica.h ASSEGURA.h
	$(CC) -c DispersaoEstatica.c -o DispersaoEstatica.o $(CFLAGS)

Leitura.o: Leitura.c Leitura.h
	$(CC) -c Leitura.c -o Leitura.o $(CFLAGS)

Registros.o: Registros.c Registros.h ASSEGURA.h
	$(CC) -c Registros.c -o Registros.o $(CFLAGS)

main.o: main.c DispersaoEstatica.h Registros.h Leitura.h ASSEGURA.h
	$(CC) -c main.c -o main.o $(CFLAGS)
