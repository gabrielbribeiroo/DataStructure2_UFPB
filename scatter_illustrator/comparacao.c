#include <stdio.h>
#include "Ilustrador.h"

#define TAM_TABELA 13

int ValorDispersao(int chave){
    IlustraDestacaLinha(__LINE__+1, 1);
    return (chave % 10) % TAM_TABELA;
}

void InsereDesordenado(int valor, int ar[], int t){
    ar[t] = valor;
    IlustraAtualizaElementosArray(ar, t);
    IlustraDestacaLinha(__LINE__-2, 1);
}

void InsereOrdenado(int valor, int ar[], int t){
    int pos = 0;
    for(int i = 0; i < t; i++, pos++){
        IlustraDestacaElementosArray(ar, i, 1, AMARELO);
        IlustraDestacaLinha(__LINE__+1, 1);
        if (valor < ar[i]){
            IlustraDestacaElementosArray(ar, i, 1, LARANJA);
            IlustraDestacaLinha(__LINE__+1, 1);
            break;
        }
    }
    for(int i = t; i > pos; i--){
        ar[i] = ar[i-1];
        IlustraAtualizaElementosArray(ar, i);
        IlustraDestacaLinha(__LINE__-2, 1);
    }
    ar[pos] = valor;
    IlustraAtualizaElementosArray(ar, pos);
    IlustraDestacaLinha(__LINE__+1, 1);
}

int main(void){
    int tabelaDispersao[TAM_TABELA];
    int listaDesordenada[TAM_TABELA];
    int listaOrdenada[TAM_TABELA];
    int chaves[5] = {300, 201, 702, 124, 616};
    int i = -1, indice = -1, chave = -1;

    IniciaIlustrador("comparacao.c");
    for(i = 0; i < TAM_TABELA; i++){
        tabelaDispersao[i] = listaDesordenada[i] = listaOrdenada[i] = -1;
    }
    IlustraCriaArrayDeInteiros(tabelaDispersao, TAM_TABELA, NOME_VAR(tabelaDispersao));
    IlustraCriaArrayDeInteiros(listaDesordenada, TAM_TABELA, NOME_VAR(listaDesordenada));
    IlustraCriaArrayDeInteiros(listaOrdenada, TAM_TABELA, NOME_VAR(listaOrdenada));
    //IlustraCriaArrayDeInteiros(chaves, 5, NOME_VAR(chaves));
    IlustraCriaVariavelInteira(&i, NOME_VAR(i));
    IlustraCriaVariavelInteira(&indice, NOME_VAR(indice));
    IlustraCriaVariavelInteira(&chave, NOME_VAR(chave));
    IlustraDestacaLinha(__LINE__+2, 1);

    for(i = 0; i < 5; i++){
        IlustraAtualizaVariavelInteira(&i);
        IlustraDestacaLinha(__LINE__+1, 1);
        chave = chaves[i];
        IlustraAtualizaVariavelInteira(&chave);
        IlustraDestacaLinha(__LINE__+1, 1);
        indice = ValorDispersao(chaves[i]);
        IlustraAtualizaVariavelInteira(&indice);
        tabelaDispersao[indice] = chaves[i];
        IlustraAtualizaElementosArray(tabelaDispersao, indice);
        IlustraDestacaLinha(__LINE__-2, 1);
        IlustraDestacaLinha(__LINE__+1, 1);
        InsereDesordenado(chaves[i], listaDesordenada, i);
        IlustraDestacaLinha(__LINE__+1, 1);
        InsereOrdenado(chaves[i], listaOrdenada, i);
    }
    IlustraAtualizaVariavelInteira(&i);
    IlustraDestacaLinha(__LINE__+2, 1);

    return 0;
}