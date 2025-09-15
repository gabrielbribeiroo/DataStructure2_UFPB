/****
 *
 * Arquivo: Heap.h
 *
 * Autor: Ulysses de Oliveira
 *
 * Data de Cria��o: 06/10/2017
 * �ltima modifica��o: 06/10/2017
 *
 * Descri��o: Interface do m�dulo Heap
 *
 ****/

#ifndef _Heap_H_
#define _Heap_H_

/*********************** Includes *************************/

#include "Tipos.h"  /* Defini��es de tipos */

/******************* Alus�es de Fun��es *******************/

extern tHeapHuff *IniciaHeapHuff(tHeapHuff *heap);
extern void DestroiHeapHuff(tHeapHuff *heap);
extern void InsereEmHeapHuff( tHeapHuff *heap, tNoHeapHuff item,
                              tFComparaHuff Compara );
extern tNoHeapHuff RemoveMinHeapHuff(tHeapHuff *heap, tFComparaHuff Compara);
extern tNoHeapHuff ObtemMinimoHeapHuff(const tHeapHuff *heap);
extern int HeapVazioHuff(const tHeapHuff *heap);
extern int TamanhoHeapHuff(const tHeapHuff *heap);

#endif
