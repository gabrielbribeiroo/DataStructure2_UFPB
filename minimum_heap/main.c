#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "HeapBinario.h"

int main(){
    int i;
    tHeap heap;

    srand(time(NULL));
    IniciaHeap(&heap);

    printf("Inserir os 5 primeiros itens...\n");
    for(i = 0; i < 5; i++){
        int novoItem = rand() % 100;
        InsereNoHeap(&heap, novoItem);
    }
    printf("Heap atual: ");
    ImprimeHeap(&heap);

    printf("\nInserir mais 5 interativamente...\n");
    for(i = 0; i < 5; i++){
        int novoItem = rand() % 100;
        printf("(pressione enter para inserir \'%d\')", novoItem);
        scanf("%*c");
        InsereNoHeap(&heap, novoItem);
        printf("Heap atual: ");
        ImprimeHeap(&heap);
    }

    printf("\nRemover 3 elementos interativamente...\n");
    for(i = 0; i < 3; i++){
        printf("(pressione enter para remover do heap)");
        scanf("%*c");
        int removido = RemoveDoHeap(&heap);
        printf("Elemento removido: %d\n", removido);
        printf("Heap atual: ");
        ImprimeHeap(&heap);
    }

    return 0;
}