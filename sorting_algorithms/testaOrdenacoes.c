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

void Troca(unsigned char *a, unsigned char *b){
    unsigned char aux = *a;
    *a = *b;
    *b = aux;
}

void BubbleSort2(unsigned char ar[], int t){
    int trocou;
    int i, nElem = t - 1;

    trocou = 1;
    while(trocou){
        trocou = 0;
        for(i = 0; i < nElem; i++){
            if (ar[i] > ar[i+1]){
                int aux = ar[i];
                ar[i] = ar[i+1];
                ar[i+1] = aux;
                trocou = 1;
            }
        }
        nElem--;
    }
}

void SelectionSort2(unsigned char ar[], int t){
    int iMenor, i, j;

    for(i = 0; i < t-1; i++){
        iMenor = i;
        for(j = i + 1; j < t; j++){
            if (ar[j] < ar[iMenor]){
                iMenor = j;
            }
        }

        if (iMenor != i){
            int aux = ar[i];
            ar[i] = ar[iMenor];
            ar[iMenor] = aux;
        }
    }
}

void InsertionSort2(unsigned char ar[], int t){
    int i, j, aux;

    for(i = 1; i < t; i++){
        aux = ar[i];
        for(j = i-1; j >= 0 && ar[j] > aux; j-- ){
            ar[j+1] = ar[j];
        }
        ar[j+1] = aux;
    }
}

void BubbleSort(unsigned char ar[], int t){
    int i, trocou;

    trocou = 1;
    while(trocou){
        trocou = 0;
        for(i = 0; i < t-1; i++){
            if (ar[i] > ar[i+1]){
                Troca(&ar[i], &ar[i+1]);
                trocou = 1;
            }
        }
        t--;
    }

}

void SelectionSort(unsigned char ar[], int t){
    int i, j, iMenor;

    for(i = 0; i < t-1; i++){
        iMenor = i;
        for(j = i+1; j < t; j++){
            if (ar[j] < ar[iMenor]){
                iMenor = j;
            }
        }
        if (iMenor != i){
            Troca(&ar[iMenor], &ar[i]);
        }
    }

}

void InsertionSort(unsigned char ar[], int t){
    int aux, i, j;

    for(i = 1; i < t; i++){
        aux = ar[i];

        for(j = i-1; j >=0 && ar[j] > aux; j--){
            ar[j+1] = ar[j];
        }

        ar[j+1] = aux;
    }
}

void QuickSort(unsigned char ar[], int t){
    if (t <= 1){
        return;
    }

    int iPivo = 0;
    int esq = 0;
    int dir = t-1;
    
    while(esq < dir){
        while (esq < t-1 && ar[esq] <= ar[iPivo]){
            esq++;
        }
        while(ar[dir] > ar[iPivo]){
            dir--;
        }
        if (esq < dir){
            Troca(&ar[esq], &ar[dir]);
        }
    }
    Troca(&ar[iPivo], &ar[dir]);

    QuickSort(ar, dir);
    QuickSort(&ar[dir+1], t-dir-1);
}

void MergeSortAux(unsigned char ar[], unsigned char aux[], int t){
    if (t <= 1){
        return;
    }

    int meio = t/2;
    MergeSortAux(ar, aux, meio);
    MergeSortAux(&ar[meio], &aux[meio], t-meio);

    int i = 0;
    int j = meio;
    int k = 0;

    while(i < meio && j < t){
        if (ar[i] <= ar[j]){
            aux[k++] = ar[i++];
        }else{
            aux[k++] = ar[j++];
        }
    }

    while(i < meio){
        aux[k++] = ar[i++];
    }
    while(j < t){
        aux[k++] = ar[j++];
    }

    for(int i = 0; i < t; i++){
        ar[i] = aux[i];
    }
}

void MergeSort(unsigned char ar[], int t){
    unsigned char aux[t];

    MergeSortAux(ar, aux, t);
}

void OrdenaHeap(unsigned char ar[], int raiz, int t){
    int fEsq, fDir, maior;

    maior = raiz;
    while(1){
        fEsq = 2*raiz + 1;
        fDir = fEsq + 1;

        if (fEsq >= t){
            break;
        }

        maior = (ar[maior] >= ar[fEsq]) ? maior : fEsq;
        if (fDir < t){
            maior = (ar[maior] >= ar[fDir]) ? maior : fDir;
        }

        if (maior != raiz){
            Troca(&ar[raiz], &ar[maior]);
            raiz = maior;
        }else{
            break;
        }
    }  
}

void HeapSort(unsigned char ar[], int t){
    int i;

    for(i = (t-1-1)/2; i >= 0; i--){
        OrdenaHeap(ar, i, t);
    }

    for (i = t-1; i > 0; i--){
        Troca(&ar[0], &ar[i]);
        OrdenaHeap(ar, 0, i);
    }
}

void CountingSort(unsigned char ar[], int t){
    int maior, i;

    //procurar maior valor
    maior = ar[0];
    for(i = 1; i < t; i++){
        if (ar[i] > maior){
            maior = ar[i];
        }
    }

    //aloca contagem
    int tContagem = maior+1;
    int contagem[tContagem];
    memset(contagem, 0, sizeof(int)*(tContagem));

    //conta ocorrencias
    for(i = 0; i < t; i++){
        contagem[ar[i]]++;
    }
    //ajusta contagem
    for(i = 1; i < tContagem; i++){
        contagem[i] += contagem[i-1];
    }

    //aloca ordenacao e preenche
    unsigned char ordenado[t];
    for(i = t-1; i >= 0; i--){
        ordenado[contagem[ar[i]]-1] = ar[i];
        contagem[ar[i]]--;
    }

    //copia para a tabela orig
    for(i = 0; i < t; i++){
        ar[i] = ordenado[i];
    }
}