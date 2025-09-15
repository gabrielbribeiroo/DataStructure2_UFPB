/****
 *
 * Arquivo: Heap.c
 *
 * Autor: Ulysses de Oliveira
 *
 * Data de Criação: 06/10/2017
 * Última modificação: 06/10/2017
 *
 * Descrição: Implementação de heap
 *
 ****/

/*********************** Includes *************************/

#include <string.h>   /* memmove()              */
#include <stdlib.h>   /* Alocação dinâmica      */
#include "Heap.h"     /* Interface deste módulo */
#include "ASSEGURA.h" /* Macro ASSEGURA()       */

/***************** Constantes Simbólicas ******************/

#define TAMANHO_INICIAL 10 /* Capacidade inicial do heap */

/****************** Definições de Macros ******************/

#define FILHO_E(x) (2*(x) + 1)
#define FILHO_D(x) (2*(x) + 2)
#define PAI(x) (((x) - 1)/2)

/********************* Funções Locais *********************/

/****
 *
 * TrocaGenerica(): Troca os valores de duas variáveis do mesmo tamanho
 *
 * Parâmetros:
 *      var1, var2 (entrada e saída) - endereços das variáveis que serão trocadas
 *      tam (entrada) - tamanho das variáveis que serão trocadas
 *
 * Retorno: 0, se não ocorrer erro; 1, em caso contrário
 *
 ****/
static void TrocaGenerica(void *var1, void *var2, size_t tam)
{
   void *p;

   p = malloc(tam);
   ASSEGURA(p, "Impossivel trocar duas variaveis");

   memmove(p, var1, tam);
   memmove(var1, var2, tam);
   memmove(var2, p, tam);

   free(p);
}

/****
 *
 * RedimensionaHeapHuff(): Redimensiona um heap dobrando sua capacidade
 *
 * Parâmetros:
 *      *heap (saída) - heap que será redimensionado
 *
 * Retorno: O primeiro parâmetro
 *
 ****/
static tHeapHuff *RedimensionaHeapHuff(tHeapHuff *heap)
{
   heap->capacidade *= 2; /* Tenta duplicar a capacidade do heap */

      /* Aqui não há problema em atribuir o retorno de realloc() ao ponteiro */
      /* passado como primeiro parâmetro, pois se não realloc() fracassar,   */
      /* o programa será abortado                                            */
   heap->itens = realloc( heap->itens, heap->capacidade*sizeof(tNoHeapHuff) );
   ASSEGURA(heap->itens, "Impossivel redimensionar heap");

   return heap;
}

/****
 *
 * OrdenaHeapHuff(): Restaura a propriedade de ordenação de um
 *               heap de mínimo após uma operação de remoção
 *
 * Parâmetros:
 *      heap (entrada e saída) - ponteiro para o heap
 *      indice (entrada) - índice do elemento do heap a partir
 *                         do qual será efetuada a restauração
 *      Compara (entrada) - ponteiro para uma função que compara
 *                          dois elementos de array armazenados no heap
 *
 * Retorno: Nada
 *
 ****/
static void OrdenaHeapHuff(tHeapHuff *heap, int indice, tFComparaHuff Compara)
{
   int iEsq = FILHO_E(indice), /* Índice do filho esquerdo */
       iDir = FILHO_D(indice), /* Índice do filho direito  */
       iMenor = indice; /* Supõe que o menor elemento é o pai */

      /* Compara o filho esquerdo com seu pai */
   if ( iEsq < heap->nItens &&
        Compara( heap->itens[iEsq], heap->itens[indice] ) < 0 )
      iMenor = iEsq; /* O filho esquerdo é menor do que o pai */

      /* Compara o filho direito com o menor entre seu pai e seu irmão */
   if ( iDir < heap->nItens &&
        Compara( heap->itens[iDir], heap->itens[iMenor] ) < 0 )
      iMenor = iDir; /* O filho direito é o menor de todos */

      /* Se o nó não for menor do que seus filhos, */
      /* troca-o de posição com o menor deles      */
   if (iMenor != indice) {
         /* O pai e o menor filho trocam de posição */
      TrocaGenerica( &heap->itens[indice], &heap->itens[iMenor],
                     sizeof(tNoHeapHuff) );

         /* Reordena o heap a partir do filho que se tornou pai */
      OrdenaHeapHuff(heap, iMenor, Compara);
   }
}

/****
 *
 * HeapCheioHuff(): Verifica se um heap está repleto
 *
 * Parâmetros:
 *      heap (entrada) - ponteiro para o heap
 *
 * Retorno: 1, se o heap estiver repleto; 0, em caso contrário
 *
 ****/
static int HeapCheioHuff(const tHeapHuff *heap)
{
   return heap->nItens == heap->capacidade;
}

/********************* Funções Globais ********************/

/****
 *
 * IniciaHeapHuff(): Inicia um heap
 *
 * Parâmetros:
 *      *heap (saída) - heap que será iniciado
 *
 * Retorno: O primeiro parâmetro
 *
 ****/
tHeapHuff *IniciaHeapHuff(tHeapHuff *heap)
{
   heap->capacidade = TAMANHO_INICIAL;
   heap->nItens = 0;
   heap->itens = calloc(TAMANHO_INICIAL, sizeof(tNoHeapHuff));

   ASSEGURA(heap->itens, "Impossivel alocar heap");

   return heap;
}

/****
 *
 * DestroiHeapHuff(): Libera o espaço ocupado por um heap
 *
 * Parâmetros: heap (entrada/saída) - o heap
 *
 * Retorno: Nada
 *
 ****/
void DestroiHeapHuff(tHeapHuff *heap)
{
      /* Libera o espaço ocupado pelos nós do heap */
   free(heap->itens);
   heap->nItens = 0;
}

/****
 *
 * InsereEmHeapHuff(): Insere um novo item num heap
 *
 * Parâmetros:
 *     heap (entrada/saída) - heap no qual será feita inserção
 *     item (entrada) - o item a ser inserido
 *     Compara (entrada) - função de comparação
 *
 * Retorno: Nada
 *
 ****/
void InsereEmHeapHuff(tHeapHuff *heap, tNoHeapHuff item, tFComparaHuff Compara)
{
   int i;

      /* Se o array que suporta o heap estiver repleto, redimensiona-o */
   if(HeapCheioHuff(heap))
      RedimensionaHeapHuff(heap);

      /* Acrescenta o novo elemento ao final do array */
      /* e incrementa o número de elementos no heap   */
   heap->itens[heap->nItens] = item;
   heap->nItens++;

      /* Reordena o heap se for necessário */
   for ( i = heap->nItens - 1; i &&
         Compara( heap->itens[PAI(i)], heap->itens[i] ) > 0; ) {
      TrocaGenerica(&heap->itens[i], &heap->itens[PAI(i)], sizeof(tNoHeapHuff));
      i = PAI(i);
   }
}

/****
 *
 * RemoveMinHeapHuff(): Remove o menor elemento de um heap binário ascendente
 *
 * Parâmetros:
 *     heap (entrada/saída) - heap onde será feita remoção
 *     Compara (entrada/saída) - função de comparação
 *
 * Retorno: O menor elemento do heap
 *
 ****/
tNoHeapHuff RemoveMinHeapHuff(tHeapHuff *heap, tFComparaHuff Compara)
{
   tNoHeapHuff menorElemento;

   ASSEGURA( !HeapVazioHuff(heap), "Tentativa de remover elemento de heap vazio" );

      /* O menor elemento está sempre na raiz de um heap de mínimo */
   menorElemento = heap->itens[0];

      /* Coloca o último elemento do heap em sua raiz e reordena o heap */
   heap->itens[0] = heap->itens[--heap->nItens];
   OrdenaHeapHuff(heap, 0, Compara);

   return menorElemento;
}

/****
 *
 * ObtemMinimoHeapHuff(): Obtém o menor elemento de um heap de mínimo
 *
 * Parâmetros:
 *      heap (entrada) - ponteiro para o heap
 *
 * Retorno: O menor elemento do heap recebido como parâmetro
 *
 ****/
tNoHeapHuff ObtemMinimoHeapHuff(const tHeapHuff *heap)
{
      /* O menor elemento de um heap de mínimo está na raiz */
   return heap->itens[0];
}

/****
 *
 * HeapVazioHuff(): Verifica se um heap está vazio
 *
 * Parâmetros:
 *      heap (entrada) - ponteiro para o heap
 *
 * Retorno: 1, se o heap estiver vazio; 0, em caso contrário
 *
 ****/
int HeapVazioHuff(const tHeapHuff *heap)
{
   return !heap->nItens;
}

/****
 *
 * TamanhoHeapHuff(): Retorna o número de itens de um heap
 *
 * Parâmetros:
 *      heap (entrada) - ponteiro para o heap
 *
 * Retorno: O número de itens do heap
 *
 ****/
int TamanhoHeapHuff(const tHeapHuff *heap)
{
   return heap->nItens;
}
