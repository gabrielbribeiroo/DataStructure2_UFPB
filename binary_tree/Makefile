# Compilador utilizado
COMP = gcc

# Linker utilizado
LINKER = gcc

# Op��es de compila��o
OPCOES_COMP = -c -std=c99 -Wall -g

# Op��es de liga��o
OPCOES_LINK = -lm

# Arquivos-objeto
OBJETOS = ArvoveBinariaDeBusca.o main.o

# Nome do arquivo execut�vel
EXEC = ArvoreBB.exe

$(EXEC): $(OBJETOS)
	$(LINKER) $(OPCOES_LINK) $(OBJETOS) -o $(EXEC)

ArvoveBinariaDeBusca.o: ArvoveBinariaDeBusca.c ArvoveBinariaDeBusca.h
	$(COMP) $(OPCOES_COMP) ArvoveBinariaDeBusca.c -o ArvoveBinariaDeBusca.o


main.o: main.c ArvoveBinariaDeBusca.h
	$(COMP) $(OPCOES_COMP) main.c -o main.o
