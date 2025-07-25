#include <stdio.h>
#include <stdlib.h>
#include "ArvoveBinariaDeBusca.h"

void IniciaArvoreBB(tArvoreBB *arvore){
   *arvore = NULL;
}

tNoArvoreBB *BuscaArvoreBB(tArvoreBB arvore, int chave){
    tNoArvoreBB *noAtual = arvore;

    while(noAtual){
        if (chave == noAtual->chave){
            return noAtual;
        }

        if (chave < noAtual->chave){
            noAtual = noAtual->esquerda;
        }else{
            noAtual = noAtual->direita;
        }
    }

    return NULL;
}

static tNoArvoreBB *NovoNo(int chave){
    tNoArvoreBB *novo = malloc(sizeof(tNoArvoreBB));
    if (!novo)
        return NULL;
    
    novo->chave = chave;
    novo->esquerda = novo->direita = NULL;

    return novo;
}

int InsereArvoreBB(tArvoreBB *arvore, int chave){
    if (!*arvore){
        *arvore = NovoNo(chave);
        return !*arvore;
    }

    tNoArvoreBB *noAtual = *arvore;
    tNoArvoreBB *paiNoAtual = NULL;

    while(noAtual != NULL){
        if (chave == noAtual->chave){
            return 1; //duplicada
        }

        paiNoAtual = noAtual;
        if (chave < noAtual->chave){
            noAtual = noAtual->esquerda;
        }else{
            noAtual = noAtual->direita;
        }
    }

    if ( !(noAtual = NovoNo(chave)) ){
        return 1;
    }

    if (noAtual->chave < paiNoAtual->chave){
        paiNoAtual->esquerda = noAtual;
    }else{
        paiNoAtual->direita = noAtual;
    }

    return 0;
}

int RemoveArvoreBB(tArvoreBB *arvore, int chave){
    tNoArvoreBB *noRemover = *arvore, *noSubstituto = NULL;
    tNoArvoreBB *paiNoRemover = NULL, *paiSubstituto = NULL;

    while(noRemover){
        if (chave == noRemover->chave)
            break;

        paiNoRemover = noRemover;

        if (chave < noRemover->chave)
            noRemover = noRemover->esquerda;
        else
            noRemover = noRemover->direita;
    }

    if (!noRemover)
        return 1; //nao encontrada

    //agora encontrar substituto de noRemover
    noSubstituto = paiSubstituto = NULL;

    if (!noRemover->esquerda){
        //nao tem filho, ou apenas na direita, q sera o substituto
        noSubstituto = noRemover->direita;
    }else if (!noRemover->direita){
        //soh tem filho a esquerda, que sera o substituto
        noSubstituto = noRemover->esquerda;
    }else{
        //dois filhos, noSubstituto deve ser o sucessor direto
        paiSubstituto = noRemover;
        noSubstituto = noRemover->direita; //para iniciar busca do sucessor

        while(noSubstituto->esquerda){ //enquanto o substituto atual possuir filho esquerdo, enquanto nao for o sucessor
            paiSubstituto = noSubstituto;
            noSubstituto = noSubstituto->esquerda;
        }

        if (paiSubstituto == noRemover){
            //noSubstituto nao tem filho a esquerda, o sucessor eh o filho a direita do noRemover, nao chegou a procurar
            /* nao precisa fazer nada aqui, pois ele apenas vai subir um nivel.
            alem disso, ele nao possui filhos a esquerda (senao nao teria entrado no if),
            sendo assim, ele permanece com os mesmos filhos a direita e vai receber os
            filhos da esquerda do no a ser removido (fora do if/else). 
            Perceba que ele será "removido" como um nó de um único filho, ou seja, os filhos deles irao para o pai, que
            é o nó a ser removido. A diferença é que nessa "remoção" o conteúdo dele vai para quem recebe seu filho, ou seja
            o nó a ser removido.
            */
            ;
        }else{
            //sendo folha ou tendo filho a direita, esse filho (ou NULL) sera o filho a esquerda do pai do sucessor
            //toma lugar do sucessor o filho ou NULL se for folha
            /*
            Perceba que é semelhante a "remoção" de um nó com 1 filho ou sem nenhum, aqui o interesse eh "remove-lo"
            dessa posição e substituílo pelo filho em seu pai.
            */
            paiSubstituto->esquerda = noSubstituto->direita;

            //a direita do no substituto vai ser o que tinha a direita do no a remover
            //antes tinha o proprio filho que foi pro pai dele acima
            /*
            Como o substituto foi "removido" de sua posicao, ele precisa receber seus novos filhos, que sao os mesmos
            dos nós q será removido. Aqui é feito apenas o direito, pois o da esquerda é feito para todos os casos
            a seguir.
            */
            noSubstituto->direita = noRemover->direita;
        }

        //tendo caminhado ou nao para encontrar o sucessor, o no esquerdo sempre sera igual a esquerda a remover, mesmo NULL
        noSubstituto->esquerda = noRemover->esquerda;    
    }

    /* o pai do no q sera removido precisa ser pai agora do nó substituto, na mesma posicao (esq/dir) do q foi removido*/
    if (!paiNoRemover){
        //remocao da raiz
        *arvore = noSubstituto;
    }else if (noRemover == paiNoRemover->esquerda){
        paiNoRemover->esquerda = noSubstituto;
    }else{
        paiNoRemover->direita = noSubstituto;
    }

    free(noRemover);
    return 0;
}

void ImprimeInfixa(tArvoreBB arvore){
    if (!arvore)
        return;
    
    ImprimeInfixa(arvore->esquerda);
    printf("%d ", arvore->chave);
    ImprimeInfixa(arvore->direita);
}

void ImprimePrefixa(tArvoreBB arvore){
    if (!arvore)
        return;
    
    printf("%d ", arvore->chave);
    ImprimePrefixa(arvore->esquerda);
    ImprimePrefixa(arvore->direita);
}

static tNoArvoreBB *RotacaoDireitaArvoreBB(tNoArvoreBB *raiz){
    tNoArvoreBB *pivo = raiz->esquerda;
    if (!pivo) 
        return NULL;
    
    raiz->esquerda = pivo->direita;
    pivo->direita = raiz;
    return pivo;
}

static tNoArvoreBB *RotacaoEsquerdaArvoreBB(tNoArvoreBB *raiz){
    tNoArvoreBB *pivo = raiz->direita;
    if (!pivo) 
        return NULL;
    
    raiz->direita = pivo->esquerda;
    pivo->esquerda = raiz;
    return pivo;
}