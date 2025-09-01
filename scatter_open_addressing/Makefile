# Compilador utilizado
COMP = gcc

# Linker utilizado
LINKER = gcc

# Opções de compilação
OPCOES_COMP = -c -std=c99 -Wall -g

# Opções de ligação
OPCOES_LINK = -lm

# Arquivos-objeto
OBJETOS = Leitura.o Registros.o TabelaDEA.o main.o

# Nome do arquivo executável
EXEC = Secao07_04_05.exe

$(EXEC): $(OBJETOS)
	$(LINKER) $(OPCOES_LINK) $(OBJETOS) -o $(EXEC)

Leitura.o: Leitura.c Leitura.h
	$(COMP) $(OPCOES_COMP) Leitura.c -o Leitura.o

Registros.o: Registros.c Registros.h Macros.h Tipos.h
	$(COMP) $(OPCOES_COMP) Registros.c -o Registros.o

TabelaDEA.o: TabelaDEA.c TabelaDEA.h Macros.h Tipos.h
	$(COMP) $(OPCOES_COMP) TabelaDEA.c -o TabelaDEA.o

main.o: main.c Registros.h Macros.h Tipos.h
	$(COMP) $(OPCOES_COMP) main.c -o main.o
