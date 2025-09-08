# Compilador utilizado
COMP = gcc

# Linker utilizado
LINKER = gcc

# Opções de compilação
OPCOES_COMP = -c -std=c99 -Wall -g

# Opções de ligação
OPCOES_LINK = -lm

# Arquivos-objeto
OBJETOS = Lista.o Leitura.o Registros.o TabelaDE.o main.o

# Nome do arquivo executável
EXEC = Secao07_3_2.exe

$(EXEC): $(OBJETOS)
	$(LINKER) $(OPCOES_LINK) $(OBJETOS) -o $(EXEC)

Lista.o: Lista.c Lista.h
	$(COMP) $(OPCOES_COMP) Lista.c -o Lista.o

Leitura.o: Leitura.c Leitura.h
	$(COMP) $(OPCOES_COMP) Leitura.c -o Leitura.o

Registros.o: Registros.c Registros.h
	$(COMP) $(OPCOES_COMP) Registros.c -o Registros.o

TabelaDE.o: TabelaDE.c TabelaDE.h
	$(COMP) $(OPCOES_COMP) TabelaDE.c -o TabelaDE.o

main.o: main.c Lista.h
	$(COMP) $(OPCOES_COMP) main.c -o main.o
