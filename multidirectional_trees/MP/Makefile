CC      = gcc
OBJ     = ArvoreMulti.o leitura.o main.o Registros.o
LINKOBJ = ArvoreMulti.o leitura.o main.o Registros.o
BIN     = Secao06_1.exe
CFLAGS  = -std=c99 -Wall -g
LFLAGS  = $(CFLAGS) -lm

$(BIN): $(OBJ)
	$(CC) $(LINKOBJ) -o $(BIN) $(LFLAGS)

ArvoreMulti.o: ArvoreMulti.c ArvoreMulti.h ASSEGURA.h
	$(CC) -c ArvoreMulti.c -o ArvoreMulti.o $(CFLAGS)

leitura.o: leitura.c leitura.h
	$(CC) -c leitura.c -o leitura.o $(CFLAGS)

main.o: main.c ArvoreMulti.h Registros.h leitura.h ASSEGURA.h
	$(CC) -c main.c -o main.o $(CFLAGS)

Registros.o: Registros.c Registros.h ASSEGURA.h
	$(CC) -c Registros.c -o Registros.o $(CFLAGS)

