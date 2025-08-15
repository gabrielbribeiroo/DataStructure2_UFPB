#ifndef _PILHAENCADEADA_H_
#define _PILHAENCADEADA_H_

#include "arvoreB.h"

typedef struct {
           tNoB no;        /* O nó             */
           int  posArquivo; /* Seu endereço      */
           int  indiceNoPai;      /* Sua posição no pai */
        } tNoCaminhoB;

typedef tNoCaminhoB tElemento; /* Apenas por conveni�ncia */


typedef struct rNo {
        tElemento valor;
        struct rNo *prox;
    }tNo;

extern void IniciaPilha(tNo **pilha);
extern tElemento ConsultaTopo(tNo *pilha);
extern void Push(tNo **pilha, tElemento valor);
extern tElemento Pop(tNo **pilha);
extern void EsvaziaPilha(tNo **pilha);

#endif