#include <stdio.h>
#include "Ilustrador.h"

#define Q_CHAVES 6

int pegaIndiceChave(int chave, int *chaves, int *funcao){
    for(int i = 0; i < Q_CHAVES; i++){
        if (chave == chaves[i]){
            return funcao[i];
        }
    }
    return -1;
}

int main(){
    int chaves[]  = {10, 13, 27, 31, 45, 63, 50};      
    int funcao1[] = {0, 0, 1, 1, 3, 3, 1};             
    int funcao2[] = {2, 0, 4, 0, 2, 4, 2};             
    int i = 0, chaveInserir = -1, pos = 0, cDesalojamentos = -1;
    int tabela1[] = {-1, -1, -1, -1, -1, -1, -1};
    int tabela2[] = {-1, -1, -1, -1, -1, -1, -1};

    IniciaIlustrador("dispersaoCuco.c");
    IlustraCriaArrayDeInteiros(chaves, Q_CHAVES, NOME_VAR(chaves));
    IlustraCriaArrayDeInteiros(funcao1, Q_CHAVES, NOME_VAR(funcao1));
    IlustraCriaArrayDeInteiros(funcao2, Q_CHAVES, NOME_VAR(funcao2));
    IlustraCriaArrayDeInteiros(tabela1, Q_CHAVES, NOME_VAR(tabela1));
    IlustraCriaArrayDeInteiros(tabela2, Q_CHAVES, NOME_VAR(tabela2));
    IlustraCriaVariavelInteira(&i, NOME_VAR(i));
    IlustraCriaVariavelInteira(&cDesalojamentos, NOME_VAR(cDesalojamentos));
    IlustraCriaVariavelInteira(&chaveInserir, NOME_VAR(chaveInserir));
    IlustraCriaVariavelInteira(&pos, NOME_VAR(pos));

    for(i = 0; i < Q_CHAVES; i++){
        IlustraDestacaElementosArray(chaves, i, 1, VERDE);
        chaveInserir = chaves[i];
        cDesalojamentos = 0;

        IlustraAtualizaVariavelInteira(&chaveInserir);
        IlustraAtualizaVariavelInteira(&cDesalojamentos);
        IlustraDestacaLinha(__LINE__+1, 1);
        while(1){
            pos = pegaIndiceChave(chaveInserir, chaves, funcao1);// funcao1[i];
            IlustraAtualizaVariavelInteira(&pos);
            IlustraDestacaElementosArray(tabela1, -1, 1, LARANJA);
            IlustraDestacaLinha(__LINE__+2, 1);

            if (tabela1[pos] == -1){ //vazio
                tabela1[pos] = chaveInserir;
                IlustraAtualizaElementosArray(tabela1, pos);
                IlustraDestacaLinha(__LINE__+1, 1);
                break;
            }else{
                int chaveDesalojada = tabela1[pos];
                IlustraDestacaElementosArray(tabela1, pos, 1, VERMELHO);
                IlustraDestacaLinha(__LINE__+1, 1);
                tabela1[pos] = chaveInserir;
                IlustraAtualizaElementosArray(tabela1, pos);
                cDesalojamentos++;
                chaveInserir = chaveDesalojada;
                IlustraAtualizaVariavelInteira(&cDesalojamentos);
                IlustraAtualizaVariavelInteira(&chaveInserir);
                IlustraDestacaLinha(__LINE__-3, 1);
            }

            pos = pegaIndiceChave(chaveInserir, chaves, funcao2);// funcao2[chaveDesalojada];
            IlustraAtualizaVariavelInteira(&pos);
            IlustraDestacaElementosArray(tabela2, -1, 1, LARANJA);
            IlustraDestacaLinha(__LINE__+2, 1);

            if (tabela2[pos] == -1){ //vazio
                tabela2[pos] = chaveInserir;
                IlustraAtualizaElementosArray(tabela2, pos);
                IlustraDestacaLinha(__LINE__+1, 1);
                break;
            }else{
                int chaveDesalojada = tabela2[pos];
                IlustraDestacaElementosArray(tabela2, pos, 1, VERMELHO);
                IlustraDestacaLinha(__LINE__+1, 1);
                tabela2[pos] = chaveInserir;
                IlustraAtualizaElementosArray(tabela2, pos);
                cDesalojamentos++;
                chaveInserir = chaveDesalojada;
                IlustraAtualizaVariavelInteira(&cDesalojamentos);
                IlustraAtualizaVariavelInteira(&chaveInserir);
                IlustraDestacaLinha(__LINE__-3, 1);
            }
        }

    }

    IlustraDestacaLinha(__LINE__+1, 1);
    return 0;

}