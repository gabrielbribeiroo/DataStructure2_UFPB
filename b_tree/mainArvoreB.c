#include <stdio.h>
#include <stdlib.h>
#include "arvoreB.h"

int main(void){
    int posicaoRaiz, retorno;

    posicaoRaiz = CarregaPosicaoRaiz("posicaoRaiz.bin");
    if (posicaoRaiz == POSICAO_NULA){
        printf("Arvore ainda nao iniciada.\n");
    }else{
        printf("A raiz da arvore estah na posicao %d.\n", posicaoRaiz);
    }

    AbrirArquivoArvore("minhaArvoreB.bin");

    tChaveIndice umaChaveIndice = {.chave = 10, .indice = POSICAO_NULA};
    retorno = InsereB(&umaChaveIndice);
    if (retorno){
        puts("Chave inserida com sucesso.");
        SalvaPosicaoRaiz("posicaoRaiz.bin");
    }else{
        puts("Erro ao inserir nova chave.");
    }

    FechaArquivoArvore();

    return 0;
}