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

void IniciaArrayAleatorio(unsigned char ar[], int t){
    static int primeiro = 1;
    int i;
    
    if (primeiro){
        srand(time(NULL));
        primeiro = 0;
    }
    
    for(i = 0; i < t; i++){
        ar[i] = rand() % 256;
    }
}

void IniciaArrayOrdenado(unsigned char ar[], int t){
    int i;
    
    for(i = 0; i < t; i++){
        ar[i] = i*256 / t;
    }
}

void IniciaArrayInversamenteOrdenado(unsigned char ar[], int t){
    int i;
    
    for(i = 0; i < t; i++){
        ar[i] = (t-i)*256 / t;
    }
}