# Compilador utilizado
COMP = gcc

# Linker utilizado
LINKER = gcc

# Opções de compilação
OPCOES_COMP = -c -std=c99 -Wall -g

# Opções de ligação
OPCOES_LINK = -lm

# Arquivos-objeto
OBJETOS = Heap.o Arquivos.o Bits.o Huffman.o main.o

# Nome do arquivo executável
EXEC = Secao09_5_1.exe

$(EXEC): $(OBJETOS)
	$(LINKER) $(OPCOES_LINK) $(OBJETOS) -o $(EXEC)

Heap.o: Heap.c
	$(COMP) $(OPCOES_COMP) Heap.c -o Heap.o

Arquivos.o: Arquivos.c
	$(COMP) $(OPCOES_COMP) Arquivos.c -o Arquivos.o

Bits.o: Bits.c
	$(COMP) $(OPCOES_COMP) Bits.c -o Bits.o

Huffman.o: Huffman.c
	$(COMP) $(OPCOES_COMP) Huffman.c -o Huffman.o

main.o: main.c
	$(COMP) $(OPCOES_COMP) main.c -o main.o
