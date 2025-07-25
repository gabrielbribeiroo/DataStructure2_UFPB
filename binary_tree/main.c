#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ArvoveBinariaDeBusca.h"

int main(){
    tArvoreBB arvore;

    srand(time(NULL));
    IniciaArvoreBB(&arvore);
    
    printf("Insercoes: \n");
    for(int i = 0; i < 13; i++){
        int chave = rand() % 20;
        if (!InsereArvoreBB(&arvore, chave)){
            printf("\tInserida %d\n", chave);
        }else{
            printf("\tErro ao inserir %d (duplicada?)\n", chave);
        }
    }

    printf("infixa: ");
    ImprimeInfixa(arvore);
    puts("");

    printf("prefixa: ");
    ImprimePrefixa(arvore);
    puts("");

    printf("Buscas: \n");
    for(int i = 0; i < 10; i++){
        int chave = rand() % 20;
        tNoArvoreBB *no;
        if ( !(no = BuscaArvoreBB(arvore, chave)) ){
            printf("\tChave %d NAO encontrada\n", chave);
        }else{
            printf("\tChave %d encontrada: %d\n", chave, no->chave);
        }
    }

    
    printf("Remocoes: \n");
    for(int i = 0; i < 5; i++){
        int chave = rand() % 20;
        if (!RemoveArvoreBB(&arvore, chave)){
            printf("\tRemovida %d\n", chave);
        }else{
            printf("\tErro ao remover %d (existe?)\n", chave);
        }
    }

    printf("infixa: ");
    ImprimeInfixa(arvore);
    puts("");

    printf("prefixa: ");
    ImprimePrefixa(arvore);
    puts("");
    

    return 0;
}