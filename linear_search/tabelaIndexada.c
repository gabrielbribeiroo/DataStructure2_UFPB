#include <stdio.h>
#include <stdlib.h>
#include "tabelaIndexada.h"
#include "tipoFilme.h"

struct rotTabIdx {
    tDados *elementos;   /* Ponteiro para o array que contem os elementos */
    int    nElementos;   /* Numero de elementos */
    int    tamanhoArray;
};

static int qOperacoes = 0;

tTabelaIdx * CriaTabelaIdx(void){
    tTabelaIdx *tabela;

        /* Aloca espaco para a estrutura que armazena a tabela */
    tabela = malloc(sizeof(struct rotTabIdx));
    if (!tabela){
        puts("Erro ao alocar tabela.");
        exit(1);
    }

    tabela->elementos = calloc(1, sizeof(tDados)); //aloca elementos
    if (!tabela->elementos){
        puts("Erro ao alocar elementos.");
        exit(1);
    }

    tabela->tamanhoArray = 1;
    tabela->nElementos = 0;

    return tabela;
}

void DestroiTabelaIdx(tTabelaIdx * tabela){
    for(int i = 0; i < tabela->nElementos; i++){
        LiberaFilme(&tabela->elementos[i]);
    }
    free(tabela->elementos);
    free(tabela);
}

int ComprimentoIdx(tTabelaIdx * tabela){
    return tabela->nElementos; 
}

tDados ObtemElementoIdx(tTabelaIdx * tabela, int indice){
    if (indice < 0 || indice >= tabela->nElementos){
        puts("Indice inexistente para obtencao.");
        exit(1);
    }

    return tabela->elementos[indice];
}

tDados RemoveElementoIdx(tTabelaIdx * tabela, int indice){
    tDados itemRemovido;

    if (indice < 0 || indice >= tabela->nElementos){
        puts("Indice inexistente para remocao.");
        exit(1);
    }

    itemRemovido = tabela->elementos[indice];
    for (int i = indice; i < tabela->nElementos - 1; i++)
        tabela->elementos[i] = tabela->elementos[i + 1];

    tabela->nElementos--; 

    return itemRemovido;
}

int EstaVaziaIdx(tTabelaIdx * tabela){
    return tabela->nElementos == 0;
}

void AcrescentaElementoIdx(tTabelaIdx * tabela, const tDados *elemento){
    tDados *novoArray; 

    
    /*
    for(int i = 0; i < tabela->nElementos; i++){
        if (tabela->elementos[i].id == elemento->id){
            return; //ja existe
        }
    }
    */
    

    if (tabela->nElementos >= tabela->tamanhoArray) { //array completo
        tabela->tamanhoArray *= 2;

        novoArray = realloc(tabela->elementos,
                            tabela->tamanhoArray*sizeof(tDados));

        if (!novoArray){
            puts("Erro ao realocar tabela.");
            exit(1);
        }

        tabela->elementos = novoArray;
    }

    tabela->elementos[tabela->nElementos] = *elemento;

    ++tabela->nElementos;
}

int BuscaSequencial(tTabelaIdx *tab, int chave){
    qOperacoes = 0;
    for(int i = 0; i < tab->nElementos; i++){
        qOperacoes++;
        if (chave == tab->elementos[i].id){
            return i;
        }
    }
    return -1;
}

int BuscaDedilhada(tTabelaIdx *tab, int chave){
    static int ultimoIndice = 0;

    if (chave < tab->elementos[ultimoIndice].id){
        ultimoIndice = 0;
    }

    qOperacoes = 0;
    for(int i = ultimoIndice; i < tab->nElementos; i++){
        qOperacoes++;
        if (chave == tab->elementos[i].id){
            ultimoIndice = i;
            return i;
        }
        if (chave < tab->elementos[i].id){
            ultimoIndice = i;
            break;
        }
    }
    
    return -1;
}

int BuscaTransposicao(tTabelaIdx *tab, int chave){
    qOperacoes = 0;
    for(int i = 0; i < tab->nElementos; i++){
        qOperacoes++;
        if (chave == tab->elementos[i].id){
            if (i > 0){ //transpor
                qOperacoes++;
                tDados aux = tab->elementos[i];
                tab->elementos[i] = tab->elementos[i-1];
                tab->elementos[i-1] = aux;
                i = i - 1;
            }
            return i;
        }
    }
    return -1;
}

int BuscaMovimentacaoInicio(tTabelaIdx *tab, int chave){
    qOperacoes = 0;
    for(int i = 0; i < tab->nElementos; i++){
        qOperacoes++;
        if (chave == tab->elementos[i].id){
            if (i > 0){ //levar pro inicio
                tDados aux = tab->elementos[i];
                for(int j = i; j > 0; j--){
                    qOperacoes++;
                    tab->elementos[j] = tab->elementos[j-1];
                }
                qOperacoes++;
                tab->elementos[0] = aux;
                i = 0;
            }
            return i;
        }
    }
    return -1;
}

int BuscaBinaria(tTabelaIdx *tab, int chave){
    int inf = 0, sup = tab->nElementos-1, meio;

    qOperacoes = 0;
    while(inf <= sup){
        qOperacoes++;
        //meio = (inf + sup) / 2;
        meio = inf + (sup - inf)/2;
        //printf("meio = %d [%d]\n", meio, tab->elementos[meio].id);
        if (tab->elementos[meio].id == chave){
            return meio;
        }else{
            if (chave > tab->elementos[meio].id){
                inf = meio + 1;
            }else{
                sup = meio - 1;
            }
        }
    }

    return -1;
}


int BuscaInterpolacao(tTabelaIdx *tab, int chave){
    int inf = 0, sup = tab->nElementos-1, meio;

    qOperacoes = 0;
    while(inf <= sup){
        qOperacoes++;
        meio = inf + (sup - inf) * (1.0*(chave - tab->elementos[inf].id) / (tab->elementos[sup].id - tab->elementos[inf].id));
        //printf("meio inter = %d [%d]\n", meio, tab->elementos[meio].id);
        if (tab->elementos[meio].id == chave){
            return meio;
        }else{
            if (chave > tab->elementos[meio].id){
                inf = meio + 1;
            }else{
                sup = meio - 1;
            }
        }
    }

    return -1;
}

int QuantOperacoes(){
    return qOperacoes;
}

static int Compara(const void* a, const void* b) {
   return (((tDados*)a)->id - ((tDados*)b)->id);
}

void Ordena(tTabelaIdx *tab){
    qsort(tab->elementos, tab->nElementos, sizeof(tab->elementos[0]), Compara);
}