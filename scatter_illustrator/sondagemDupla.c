#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "Ilustrador.h"

#define TAM_TABELA 29
#define MOD_SONDAGEM2 23
#define N_CHAVES 21

int ValorDispersao2(int chave){
    return 1 + chave%MOD_SONDAGEM2;
}

int ValorDispersao(int chave, int i){
    IlustraCriaVariavelInteira(&chave, NOME_VAR(chave));
    IlustraCriaVariavelInteira(&i, NOME_VAR(i));
    IlustraDestacaLinha(__LINE__+1, 1);
    int vDisp = ((chave%TAM_TABELA) + i*ValorDispersao2(chave)) % TAM_TABELA;
    IlustraCriaVariavelInteira(&vDisp, NOME_VAR(vDisp));
    IlustraDestacaLinha(__LINE__+4, 1);
    IlustraRemoveMemoriaPorEndereco(&vDisp);
    IlustraRemoveMemoriaPorEndereco(&i);
    IlustraRemoveMemoriaPorEndereco(&chave);
    return vDisp;
}

int main(void){
    int tabelaDispersao[TAM_TABELA];
    int chaves[N_CHAVES], qChaves = 0;
    int i = -1, indice = -1, chave = -1, tentativa = -1;

    for(i = 0; i < TAM_TABELA; i++){
        tabelaDispersao[i] = -1;
    }

    srand(time(NULL));
    while(qChaves < N_CHAVES){
        int achou = 0;
        int chave = rand() % 50;
        for(i = 0; i < qChaves; i++){
            if (chaves[i] == chave){
                achou = 1;
                break;
            }
        }
        if (!achou){
            chaves[qChaves++] = chave;
        }
    }
    IniciaIlustrador("sondagemDupla.c");
    IlustraCriaArrayDeInteiros(tabelaDispersao, TAM_TABELA, NOME_VAR(tabelaDispersao));
    IlustraCriaVariavelInteira(&i, NOME_VAR(i));
    IlustraCriaVariavelInteira(&indice, NOME_VAR(indice));
    IlustraCriaVariavelInteira(&chave, NOME_VAR(chave));
    IlustraCriaVariavelInteira(&tentativa, NOME_VAR(tentativa));
    IlustraDestacaLinha(__LINE__+2, 1);

    //inserções
    IlustraSaida("Inserindo...");
    for(i = 0; i < N_CHAVES; i++){
        IlustraAtualizaVariavelInteira(&i);
        IlustraDestacaLinha(__LINE__+1, 1);
        chave = chaves[i];
        IlustraAtualizaVariavelInteira(&chave);
        for(tentativa = 0; tentativa < TAM_TABELA; tentativa++){
            IlustraAtualizaVariavelInteira(&tentativa);
            IlustraDestacaLinha(__LINE__+6, 1);
            IlustraEscopoElemento(&tentativa, 0);
            IlustraEscopoElemento(&chave, 0);
            IlustraEscopoElemento(&indice, 0);
            IlustraEscopoElemento(&i, 0);
            IlustraEscopoElemento(&tabelaDispersao, 0);
            indice = ValorDispersao(chave, tentativa);
            IlustraEscopoElemento(&tentativa, 1);
            IlustraEscopoElemento(&chave, 1);
            IlustraEscopoElemento(&indice, 1);
            IlustraEscopoElemento(&i, 1);
            IlustraEscopoElemento(&tabelaDispersao, 1);
            IlustraAtualizaVariavelInteira(&indice);
            IlustraDestacaElementosArray(tabelaDispersao, indice, 1, AMARELO);
            IlustraDestacaLinha(__LINE__+1, 1);
            if (tabelaDispersao[indice] < 0) { //espaco vazio?
                IlustraDestacaElementosArray(tabelaDispersao, indice, 1, VERDE);
                IlustraDestacaLinha(__LINE__+1, 1);
                tabelaDispersao[indice] = chave;
                IlustraAtualizaElementosArray(tabelaDispersao, indice);
                IlustraDestacaLinha(__LINE__+1, 1);
                break;
            }
            else {IlustraDestacaElementosArray(tabelaDispersao, indice, 1, LARANJA); IlustraDestacaLinha(__LINE__+1, 1);}
        }
    }
    IlustraAtualizaVariavelInteira(&i);

    //remoções
    IlustraSaida("Removendo...");
    for(i = 0; i < N_CHAVES/3; i++){
        IlustraAtualizaVariavelInteira(&i);
        IlustraDestacaLinha(__LINE__+1, 1);
        indice = rand() % N_CHAVES;
        chave = chaves[indice];
        IlustraAtualizaVariavelInteira(&indice);
        IlustraDestacaLinha(__LINE__-2, 1);
        IlustraAtualizaVariavelInteira(&chave);
        int achou = 0;
        for(tentativa = 0; tentativa < TAM_TABELA; tentativa++){
            IlustraAtualizaVariavelInteira(&tentativa);
            IlustraDestacaLinha(__LINE__+6, 1);
            IlustraEscopoElemento(&tentativa, 0);
            IlustraEscopoElemento(&chave, 0);
            IlustraEscopoElemento(&indice, 0);
            IlustraEscopoElemento(&i, 0);
            IlustraEscopoElemento(&tabelaDispersao, 0);
            indice = ValorDispersao(chave, tentativa);
            IlustraEscopoElemento(&tentativa, 1);
            IlustraEscopoElemento(&chave, 1);
            IlustraEscopoElemento(&indice, 1);
            IlustraEscopoElemento(&i, 1);
            IlustraEscopoElemento(&tabelaDispersao, 1);
            IlustraAtualizaVariavelInteira(&indice);
            IlustraDestacaElementosArray(tabelaDispersao, indice, 1, AMARELO);
            IlustraDestacaLinha(__LINE__+1, 1);
            if (tabelaDispersao[indice] == chave) { //encontrou
                achou = 1;
                IlustraDestacaElementosArray(tabelaDispersao, indice, 1, VERDE);
                IlustraDestacaLinha(__LINE__+1, 1);
                tabelaDispersao[indice] = -2; //marcar como removido
                IlustraAtualizaElementosArray(tabelaDispersao, indice);
                IlustraDestacaLinha(__LINE__+1, 1);
                break;
            }else if (tabelaDispersao[indice] == -1){
                //parar sondagem
                IlustraDestacaElementosArray(tabelaDispersao, indice, 1, VERMELHO);
                IlustraDestacaLinha(__LINE__+1, 1);
                break;
            }
            else {IlustraDestacaElementosArray(tabelaDispersao, indice, 1, LARANJA); IlustraDestacaLinha(__LINE__+1, 1);}
        }
        if (!achou){
            IlustraAlerta("Chave nao encontrada.");
        }
    }

    //buscas
    IlustraSaida("Buscando...");
    for(i = 0; i < N_CHAVES/2; i++){
        IlustraAtualizaVariavelInteira(&i);
        IlustraDestacaLinha(__LINE__+1, 1);
        indice = rand() % N_CHAVES;
        chave = chaves[indice];
        IlustraAtualizaVariavelInteira(&indice);
        IlustraDestacaLinha(__LINE__-2, 1);
        IlustraAtualizaVariavelInteira(&chave);
        int achou = 0;
        for(tentativa = 0; tentativa < TAM_TABELA; tentativa++){
            IlustraAtualizaVariavelInteira(&tentativa);
            IlustraDestacaLinha(__LINE__+6, 1);
            IlustraEscopoElemento(&tentativa, 0);
            IlustraEscopoElemento(&chave, 0);
            IlustraEscopoElemento(&indice, 0);
            IlustraEscopoElemento(&i, 0);
            IlustraEscopoElemento(&tabelaDispersao, 0);
            indice = ValorDispersao(chave, tentativa);
            IlustraEscopoElemento(&tentativa, 1);
            IlustraEscopoElemento(&chave, 1);
            IlustraEscopoElemento(&indice, 1);
            IlustraEscopoElemento(&i, 1);
            IlustraEscopoElemento(&tabelaDispersao, 1);
            IlustraAtualizaVariavelInteira(&indice);
            IlustraDestacaElementosArray(tabelaDispersao, indice, 1, AMARELO);
            IlustraDestacaLinha(__LINE__+1, 1);
            if (tabelaDispersao[indice] == chave) { //encontrou
                achou = 1;
                IlustraDestacaElementosArray(tabelaDispersao, indice, 1, VERDE);
                IlustraDestacaLinha(__LINE__+1, 1);
                break;
            }else if (tabelaDispersao[indice] == -1){
                //parar sondagem
                IlustraDestacaElementosArray(tabelaDispersao, indice, 1, VERMELHO);
                IlustraDestacaLinha(__LINE__+1, 1);
                break;
            }
            else {IlustraDestacaElementosArray(tabelaDispersao, indice, 1, LARANJA); IlustraDestacaLinha(__LINE__+1, 1);}
        }
        if (!achou){
            IlustraAlerta("Chave nao encontrada.");
        }
    }

    IlustraDestacaLinha(__LINE__+8, 1);
    IlustraRemoveMemoriaPorEndereco(&tentativa);
    IlustraRemoveMemoriaPorEndereco(&chave);
    IlustraRemoveMemoriaPorEndereco(&indice);
    IlustraRemoveMemoriaPorEndereco(&i);
    IlustraRemoveMemoriaPorEndereco(&tabelaDispersao);
    IlustraDestacaLinha(__LINE__+3, 1);

    return 0;
}