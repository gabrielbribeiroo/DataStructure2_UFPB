#include <stdio.h>
#include <stdlib.h>
#include "pilhaEncadeada.h"

static tNo * NovoNo(tElemento valor){
    tNo *temp;
    
    temp = malloc(sizeof(tNo));
    if (temp == NULL){
        puts("Erro: nao foi possivel alocar novo no.");
        exit(2);
    }

    temp->valor = valor;
    temp->prox = NULL;

    return temp;
}

void IniciaPilha(tNo **pilha){
    *pilha = NULL;
}

tElemento ConsultaTopo(tNo *pilha){
    if (pilha != NULL){
        return pilha->valor;
    }else{
        puts("Erro ao consultar topo: pilha vazia.");
        exit(1);
    }
}

void Push(tNo **pilha,tElemento valor){
    tNo *novo;

    novo = NovoNo(valor);
    novo->prox = *pilha;
    *pilha = novo;
}

tElemento Pop(tNo **pilha){
    tNo *temp;
    tElemento valor;

    temp = *pilha;
    valor = temp->valor;
    *pilha = temp->prox;
 
    free(temp);
    return valor;
}

void EsvaziaPilha(tNo **pilha){
    while(*pilha){
        Pop(pilha);
    }

}