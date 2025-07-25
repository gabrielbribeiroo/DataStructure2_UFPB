#ifndef _ArvoreBinariaDeBusca_H_
#define _ArvoreBinariaDeBusca_H_

typedef struct rotNoABB {
        struct rotNoABB *esquerda;
        int chave;
        struct rotNoABB *direita;
    } tNoArvoreBB, *tArvoreBB;

extern void IniciaArvoreBB(tArvoreBB *arvore);
extern tNoArvoreBB *BuscaArvoreBB(tArvoreBB arvore, int chave);
extern int InsereArvoreBB(tArvoreBB *arvore, int chave);
extern int RemoveArvoreBB(tArvoreBB *arvore, int chave);
extern void ImprimeInfixa(tArvoreBB arvore);
extern void ImprimePrefixa(tArvoreBB arvore);

#endif