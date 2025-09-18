#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

#define MAX_ELEMENTOS_EXIBIR 20
//#define TAMANHO_TABELA MAX_ELEMENTOS_EXIBIR
#define TAMANHO_TABELA 500000

typedef void tFuncaoArray(unsigned char ar[], int t); 
typedef void tFuncaoInicia(unsigned char ar[], int t); 

typedef struct {
        char nomeAlgoritmo[21];
        tFuncaoArray *funcao;
        int64_t tempoExecucao;
    } tTesteOrdenacao;

typedef struct {
        char descricao[21];
        tFuncaoInicia *funcao;
    } tFuncaoIniciaArray;

int64_t currentTimeMillis() {
    struct timeval time;
    gettimeofday(&time, NULL);
    int64_t s1 = (int64_t)(time.tv_sec) * 1000;
    int64_t s2 = (time.tv_usec / 1000);
    return s1 + s2;
}