#include <stdio.h>
#include <stdlib.h>
#include "HeapBinario.h"

static int IndicePai(int i){
    return (i-1)/2;
}

static int IndiceFilhoEsquerdo(int i){
    return 2*i + 1;
}

static int IndiceFilhoDireito(int i){
    return 2*i + 2;
}

void IniciaHeap(tHeap *heap){
    heap->nItens = 0;
}

void InsereNoHeap2(tHeap *heap, int novoItem){
    int iNovoNo, iPai;
    
    iNovoNo = heap->nItens;
    heap->itens[iNovoNo] = novoItem;
    heap->nItens++;

    while(iNovoNo != 0){
        iPai = IndicePai(iNovoNo);
        if (heap->itens[iNovoNo] < heap->itens[iPai]){
            int aux = heap->itens[iNovoNo];
            heap->itens[iNovoNo] = heap->itens[iPai];
            heap->itens[iPai] = aux;
            iNovoNo = iPai;
        }else{
            break;
        }
    }

}

int RemoveDoHeap2(tHeap *heap){
    int iNovaRaiz, iFE, iFD;
    int removido = heap->itens[0];

    heap->itens[0] = heap->itens[heap->nItens-1];
    heap->nItens--;

    iNovaRaiz = 0;
    while(iNovaRaiz < heap->nItens){
        int iMenor = iNovaRaiz;
        iFE = IndiceFilhoEsquerdo(iNovaRaiz);
        iFD = IndiceFilhoDireito(iNovaRaiz);

        if (iFE < heap->nItens && heap->itens[iFE] < heap->itens[iMenor]){
            iMenor = iFE;
        }
        if (iFD < heap->nItens &&heap->itens[iFD] < heap->itens[iMenor]){
            iMenor = iFD;
        }

        if (iMenor != iNovaRaiz){
            int aux = heap->itens[iNovaRaiz];
            heap->itens[iNovaRaiz] = heap->itens[iMenor];
            heap->itens[iMenor] = aux;
            iNovaRaiz = iMenor;
        }else{
            break;
        }
    }

    return removido;
}

void InsereNoHeap(tHeap *heap, int novoItem){
    int iPai;
    int iNovoItem = heap->nItens;

    heap->itens[iNovoItem] = novoItem;
    heap->nItens++;

    while(iNovoItem != 0){
        iPai = IndicePai(iNovoItem);
        if (heap->itens[iNovoItem] < heap->itens[iPai]){
            int aux = heap->itens[iNovoItem];
            heap->itens[iNovoItem] = heap->itens[iPai];
            heap->itens[iPai] = aux;
            iNovoItem = iPai;
        }else{
            break;
        }
    }
}

int RemoveDoHeap(tHeap *heap){
    int iNovaRaiz = 0;
    int removido = heap->itens[0];

    heap->itens[0] = heap->itens[heap->nItens-1];
    heap->nItens--;

    while(IndiceFilhoEsquerdo(iNovaRaiz) < heap->nItens){
        int iMenor = iNovaRaiz;
        int iFE = IndiceFilhoEsquerdo(iNovaRaiz);
        int iFD = IndiceFilhoDireito(iNovaRaiz);

        if (iFE < heap->nItens && heap->itens[iFE] < heap->itens[iMenor]){
            iMenor = iFE;
        }
        if (iFD < heap->nItens && heap->itens[iFD] < heap->itens[iMenor]){
            iMenor = iFD;
        }

        if (iMenor != iNovaRaiz){
            int aux = heap->itens[iNovaRaiz];
            heap->itens[iNovaRaiz] = heap->itens[iMenor];
            heap->itens[iMenor] = aux;
            iNovaRaiz = iMenor;
        }else{
            break;
        }

    }

    return removido;
}

void ImprimeHeap(const tHeap *heap){
    int i;
    
    for (i = 0; i < heap->nItens; i++){
        printf("%d%c", heap->itens[i], (i==heap->nItens-1) ? '\n' : ' ');
    }
}