#ifndef _HEAPBINARIO_H_
#define _HEAPBINARIO_H_

#define MAX_ITENS 1000

typedef struct {
        int itens[MAX_ITENS];
        int nItens;
    } tHeap;

void IniciaHeap(tHeap *heap);
void InsereNoHeap(tHeap *heap, int novoItem);
int  RemoveDoHeap(tHeap *heap);

void ImprimeHeap(const tHeap *heap);

#endif