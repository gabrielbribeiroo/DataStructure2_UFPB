#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "listaComSaltos.h"
#include "tipoFilme.h"

typedef enum {CARA, COROA} tCaraCoroa;

struct rotListaSaltos {
        tNoListaComSalto *cabeca;
        int nivelAtual;
        int nivelMax;
    };

static int qOperacoes = 0;

static tCaraCoroa CaraOuCoroa(void) {
    static int primeiraChamada = 1;

    if (primeiraChamada){
        srand(time(NULL));
        primeiraChamada = 0;
    }

    return rand() % 2 ? CARA : COROA;
}

void IniciaListaComSaltos(tListaComSaltos **lista, int maxRegs){
    int i, nivelMaximo;

    *lista = malloc(sizeof(tListaComSaltos));
    if (!(*lista)){
        puts("Erro ao alocar a lista.");
        exit(1);
    }

    nivelMaximo = (*lista)->nivelMax = (int) floor(log2(maxRegs)) + 1;

    (*lista)->cabeca = malloc(sizeof(tNoListaComSalto));
    if (!(*lista)->cabeca){
        puts("Erro ao alocar cabeca da lista.");
        exit(1);
    }
    (*lista)->cabeca->proximo = malloc((nivelMaximo + 1) * sizeof(tNoListaComSalto *)); //array com proximos
    if (!(*lista)->cabeca->proximo){
        puts("Erro ao alocar array de proximos da cabeca lista.");
        exit(1);
    }
    for (i = 0; i <= (*lista)->nivelMax; i++)  //lista circular
        (*lista)->cabeca->proximo[i] = (*lista)->cabeca;

    (*lista)->nivelAtual = 0; 
}

extern void DestroiListaComSaltos(tListaComSaltos *lista){
    tNoListaComSalto *noAtual = lista->cabeca->proximo[0]; 

    while (noAtual != lista->cabeca){
        tNoListaComSalto *proxNo = noAtual->proximo[0];
        free(noAtual->proximo);
        free(noAtual);
        noAtual = proxNo;
    }
    free(noAtual); //libera a cabeca
}

int ComprimentoListaComSaltos(tListaComSaltos *lista){
    int c = 0;
    tNoListaComSalto *noAtual = lista->cabeca->proximo[0]; 

    while (noAtual != lista->cabeca){
        c++;
        noAtual = noAtual->proximo[0];
    }

    return c;
}

int EstaVaziaListaComSaltos(tListaComSaltos *lista){
    return lista->cabeca->proximo[0] == lista->cabeca;
}

tFilme BuscaListaComSaltos(tListaComSaltos *lista, int chave){
    int nivelAtual;
    tNoListaComSalto *noAtual = lista->cabeca; 

    qOperacoes = 0;
    for (nivelAtual = lista->nivelAtual; nivelAtual >= 0; nivelAtual--){
        while (noAtual->proximo[nivelAtual] != lista->cabeca &&
                noAtual->proximo[nivelAtual]->conteudo.id < chave){
            qOperacoes++;
            noAtual = noAtual->proximo[nivelAtual];
        }
    }

    noAtual = noAtual->proximo[0];
    qOperacoes++;

    if (noAtual != lista->cabeca && noAtual->conteudo.id == chave){
        return noAtual->conteudo; 
    }else{
        tFilme filmeNulo = {.id = -1, .titulo = NULL};
        return filmeNulo;
    }
}

tNoListaComSalto* InsereListaComSaltos(tListaComSaltos *lista, tFilme filme){
    int nivelAtual, novoNivel; 
    tNoListaComSalto **anteriores, *novoNo, *noAtual;                   
    
    anteriores = calloc(lista->nivelMax + 1, sizeof(tNoListaComSalto *));
    if (!anteriores){
        puts("Nao foi possivel alocar array auxiliar de anteriores.");
        exit(1);
    }

    //procura posicao
    qOperacoes = 0;
    noAtual = lista->cabeca; 
    for (nivelAtual = lista->nivelAtual; nivelAtual >= 0; nivelAtual--){
        while (noAtual->proximo[nivelAtual] != lista->cabeca && 
               noAtual->proximo[nivelAtual]->conteudo.id < filme.id){
            qOperacoes++;
            noAtual = noAtual->proximo[nivelAtual];
        }
        anteriores[nivelAtual] = noAtual;
    }
    if (noAtual->proximo[0] != lista->cabeca && 
        noAtual->proximo[0]->conteudo.id == filme.id){
        //ja existe
        free(anteriores); 
        //ImprimeFilme(&filme);
        return NULL;     
    }

    //definir nivel e criar o no
    for (novoNivel = 0; CaraOuCoroa() == CARA && novoNivel < lista->nivelMax; novoNivel++){
        ; //faz nada, so calcula o nivel
    }
    novoNo = malloc(sizeof(tNoListaComSalto));
    if (!novoNo){
        puts("Erro ao alocar novo no.");
        exit(1);

    }
    novoNo->proximo = malloc((novoNivel + 1) * sizeof(tNoListaComSalto *)); //cria array de proximos, de acordo com o nivel calculado
    if (!novoNo->proximo){
        puts("Erro ao alocar array de proximos do novo no.");
        exit(1);
    }
    novoNo->conteudo = filme; 
   
    if (novoNivel > lista->nivelAtual)    {
        for (nivelAtual = lista->nivelAtual + 1; nivelAtual <= novoNivel; nivelAtual++){
            anteriores[nivelAtual] = lista->cabeca; //como vai aumentar o nivel da lista, vai fazer a cabeca apontar para esse primeiro no de novo nivel
        }
        lista->nivelAtual = novoNivel;
    }

    //ajeita os ponteiros anteriores e do novo no
    for (nivelAtual = 0; nivelAtual <= novoNivel; nivelAtual++){
        novoNo->proximo[nivelAtual] = anteriores[nivelAtual]->proximo[nivelAtual]; //novo no aponta pra onde os anteriores apontam
        anteriores[nivelAtual]->proximo[nivelAtual] = novoNo; //anteriores apontam para o novo No
    }

    free(anteriores); 
    return novoNo; 
}

int RemoveListaComSaltos(tListaComSaltos *lista, int chave){
    int nivelAtual;                        
    tNoListaComSalto **anteriores, *noAtual;                   
                                  
    anteriores = calloc(lista->nivelMax + 1, sizeof(tNoListaComSalto *));
    if (!anteriores){
        puts("Nao foi possivel alocar array auxiliar de anteriores.");
        exit(1);
    }

    qOperacoes = 0;
    noAtual = lista->cabeca; 
    for (nivelAtual = lista->nivelAtual; nivelAtual >= 0; nivelAtual--){
        while (noAtual->proximo[nivelAtual] != lista->cabeca &&
               noAtual->proximo[nivelAtual]->conteudo.id < chave){
            qOperacoes++;
            noAtual = noAtual->proximo[nivelAtual];
        }
        anteriores[nivelAtual] = noAtual;
    }

    if (noAtual->proximo[0] == lista->cabeca || noAtual->proximo[0]->conteudo.id != chave){
        return 0; 
    }
    noAtual = noAtual->proximo[0];

    //atualiza os anteriores que apontam para o no (apenas quem aponta para o no removido)
    for (nivelAtual = 0; nivelAtual <= lista->nivelAtual && anteriores[nivelAtual]->proximo[nivelAtual] == noAtual; nivelAtual++){
        anteriores[nivelAtual]->proximo[nivelAtual] = noAtual->proximo[nivelAtual]; 
    }

    free(noAtual->proximo);
    free(noAtual);
    free(anteriores);
    
    //verificar se o nivel da lista diminuiu com a remocao 
    while (lista->nivelAtual > 0 && lista->cabeca->proximo[lista->nivelAtual] == lista->cabeca){
        lista->nivelAtual--;
    }

    return 1;
}

void ImprimeEstado(tListaComSaltos *lista){
    tNoListaComSalto *noAtual;
    int n, nAnterior = 0;
    
    for (int nivel = 0; nivel <= lista->nivelAtual; nivel++){
        noAtual = lista->cabeca->proximo[nivel];
        n = 0;
        while (noAtual != lista->cabeca){
            n++;
            noAtual = noAtual->proximo[nivel];
        }
        printf("Nivel %2d: %7d chaves (%3.2lf%% do nivel inferior)\n", nivel, n,  100.0*n/nAnterior);
        nAnterior = n;
    }
}

int QuantOperacoesSaltos(){
    return qOperacoes;
}