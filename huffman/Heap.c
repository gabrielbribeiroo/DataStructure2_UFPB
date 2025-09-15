/****
 *
 * Arquivo: Heap.c
 *
 * Autor: Ulysses de Oliveira
 *
 * Data de Cria��o: 06/10/2017
 * �ltima modifica��o: 06/10/2017
 *
 * Descri��o: Implementa��o de heap
 *
 ****/

/*********************** Includes *************************/

#include <string.h>   /* memmove()              */
#include <stdlib.h>   /* Aloca��o din�mica      */
#include "Heap.h"     /* Interface deste m�dulo */
#include "ASSEGURA.h" /* Macro ASSEGURA()       */

/***************** Constantes Simb�licas ******************/

#define TAMANHO_INICIAL 10 /* Capacidade inicial do heap */

/****************** Defini��es de Macros ******************/

#define FILHO_E(x) (2*(x) + 1)
#define FILHO_D(x) (2*(x) + 2)
#define PAI(x) (((x) - 1)/2)

/********************* Fun��es Locais *********************/

/****
 *
 * TrocaGenerica(): Troca os valores de duas vari�veis do mesmo tamanho
 *
 * Par�metros:
 *      var1, var2 (entrada e sa�da) - endere�os das vari�veis que ser�o trocadas
 *      tam (entrada) - tamanho das vari�veis que ser�o trocadas
 *
 * Retorno: 0, se n�o ocorrer erro; 1, em caso contr�rio
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
 * Par�metros:
 *      *heap (sa�da) - heap que ser� redimensionado
 *
 * Retorno: O primeiro par�metro
 *
 ****/
static tHeapHuff *RedimensionaHeapHuff(tHeapHuff *heap)
{
   heap->capacidade *= 2; /* Tenta duplicar a capacidade do heap */

      /* Aqui n�o h� problema em atribuir o retorno de realloc() ao ponteiro */
      /* passado como primeiro par�metro, pois se n�o realloc() fracassar,   */
      /* o programa ser� abortado                                            */
   heap->itens = realloc( heap->itens, heap->capacidade*sizeof(tNoHeapHuff) );
   ASSEGURA(heap->itens, "Impossivel redimensionar heap");

   return heap;
}

/****
 *
 * OrdenaHeapHuff(): Restaura a propriedade de ordena��o de um
 *               heap de m�nimo ap�s uma opera��o de remo��o
 *
 * Par�metros:
 *      heap (entrada e sa�da) - ponteiro para o heap
 *      indice (entrada) - �ndice do elemento do heap a partir
 *                         do qual ser� efetuada a restaura��o
 *      Compara (entrada) - ponteiro para uma fun��o que compara
 *                          dois elementos de array armazenados no heap
 *
 * Retorno: Nada
 *
 ****/
static void OrdenaHeapHuff(tHeapHuff *heap, int indice, tFComparaHuff Compara)
{
   int iEsq = FILHO_E(indice), /* �ndice do filho esquerdo */
       iDir = FILHO_D(indice), /* �ndice do filho direito  */
       iMenor = indice; /* Sup�e que o menor elemento � o pai */

      /* Compara o filho esquerdo com seu pai */
   if ( iEsq < heap->nItens &&
        Compara( heap->itens[iEsq], heap->itens[indice] ) < 0 )
      iMenor = iEsq; /* O filho esquerdo � menor do que o pai */

      /* Compara o filho direito com o menor entre seu pai e seu irm�o */
   if ( iDir < heap->nItens &&
        Compara( heap->itens[iDir], heap->itens[iMenor] ) < 0 )
      iMenor = iDir; /* O filho direito � o menor de todos */

      /* Se o n� n�o for menor do que seus filhos, */
      /* troca-o de posi��o com o menor deles      */
   if (iMenor != indice) {
         /* O pai e o menor filho trocam de posi��o */
      TrocaGenerica( &heap->itens[indice], &heap->itens[iMenor],
                     sizeof(tNoHeapHuff) );

         /* Reordena o heap a partir do filho que se tornou pai */
      OrdenaHeapHuff(heap, iMenor, Compara);
   }
}

/****
 *
 * HeapCheioHuff(): Verifica se um heap est� repleto
 *
 * Par�metros:
 *      heap (entrada) - ponteiro para o heap
 *
 * Retorno: 1, se o heap estiver repleto; 0, em caso contr�rio
 *
 ****/
static int HeapCheioHuff(const tHeapHuff *heap)
{
   return heap->nItens == heap->capacidade;
}

/********************* Fun��es Globais ********************/

/****
 *
 * IniciaHeapHuff(): Inicia um heap
 *
 * Par�metros:
 *      *heap (sa�da) - heap que ser� iniciado
 *
 * Retorno: O primeiro par�metro
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
 * DestroiHeapHuff(): Libera o espa�o ocupado por um heap
 *
 * Par�metros: heap (entrada/sa�da) - o heap
 *
 * Retorno: Nada
 *
 ****/
void DestroiHeapHuff(tHeapHuff *heap)
{
      /* Libera o espa�o ocupado pelos n�s do heap */
   free(heap->itens);
   heap->nItens = 0;
}

/****
 *
 * InsereEmHeapHuff(): Insere um novo item num heap
 *
 * Par�metros:
 *     heap (entrada/sa�da) - heap no qual ser� feita inser��o
 *     item (entrada) - o item a ser inserido
 *     Compara (entrada) - fun��o de compara��o
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
      /* e incrementa o n�mero de elementos no heap   */
   heap->itens[heap->nItens] = item;
   heap->nItens++;

      /* Reordena o heap se for necess�rio */
   for ( i = heap->nItens - 1; i &&
         Compara( heap->itens[PAI(i)], heap->itens[i] ) > 0; ) {
      TrocaGenerica(&heap->itens[i], &heap->itens[PAI(i)], sizeof(tNoHeapHuff));
      i = PAI(i);
   }
}

/****
 *
 * RemoveMinHeapHuff(): Remove o menor elemento de um heap bin�rio ascendente
 *
 * Par�metros:
 *     heap (entrada/sa�da) - heap onde ser� feita remo��o
 *     Compara (entrada/sa�da) - fun��o de compara��o
 *
 * Retorno: O menor elemento do heap
 *
 ****/
tNoHeapHuff RemoveMinHeapHuff(tHeapHuff *heap, tFComparaHuff Compara)
{
   tNoHeapHuff menorElemento;

   ASSEGURA( !HeapVazioHuff(heap), "Tentativa de remover elemento de heap vazio" );

      /* O menor elemento est� sempre na raiz de um heap de m�nimo */
   menorElemento = heap->itens[0];

      /* Coloca o �ltimo elemento do heap em sua raiz e reordena o heap */
   heap->itens[0] = heap->itens[--heap->nItens];
   OrdenaHeapHuff(heap, 0, Compara);

   return menorElemento;
}

/****
 *
 * ObtemMinimoHeapHuff(): Obt�m o menor elemento de um heap de m�nimo
 *
 * Par�metros:
 *      heap (entrada) - ponteiro para o heap
 *
 * Retorno: O menor elemento do heap recebido como par�metro
 *
 ****/
tNoHeapHuff ObtemMinimoHeapHuff(const tHeapHuff *heap)
{
      /* O menor elemento de um heap de m�nimo est� na raiz */
   return heap->itens[0];
}

/****
 *
 * HeapVazioHuff(): Verifica se um heap est� vazio
 *
 * Par�metros:
 *      heap (entrada) - ponteiro para o heap
 *
 * Retorno: 1, se o heap estiver vazio; 0, em caso contr�rio
 *
 ****/
int HeapVazioHuff(const tHeapHuff *heap)
{
   return !heap->nItens;
}

/****
 *
 * TamanhoHeapHuff(): Retorna o n�mero de itens de um heap
 *
 * Par�metros:
 *      heap (entrada) - ponteiro para o heap
 *
 * Retorno: O n�mero de itens do heap
 *
 ****/
int TamanhoHeapHuff(const tHeapHuff *heap)
{
   return heap->nItens;
}
