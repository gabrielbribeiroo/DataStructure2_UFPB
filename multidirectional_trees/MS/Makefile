CC      = gcc
OBJ     = ArvoreMulti2.o leitura.o main.o Registros.o
LINKOBJ = ArvoreMulti2.o leitura.o main.o Registros.o
BIN     = Secao06_3.exe
CFLAGS  = -std=c11 -Wall -g
LFLAGS  = $(CFLAGS) -lm

$(BIN): $(OBJ)
	$(CC) $(LINKOBJ) -o $(BIN) $(LFLAGS)

ArvoreMulti2.o: ArvoreMulti2.c ArvoreMulti2.h ASSEGURA.h
	$(CC) -c ArvoreMulti2.c -o ArvoreMulti2.o $(CFLAGS)

leitura.o: leitura.c leitura.h
	$(CC) -c leitura.c -o leitura.o $(CFLAGS)

main.o: main.c ArvoreMulti2.h Registros.h leitura.h ASSEGURA.h
	$(CC) -c main.c -o main.o $(CFLAGS)

Registros.o: Registros.c Registros.h ASSEGURA.h
	$(CC) -c Registros.c -o Registros.o $(CFLAGS)
