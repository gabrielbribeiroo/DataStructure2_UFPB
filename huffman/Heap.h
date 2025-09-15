/****
 *
 * Arquivo: Heap.h
 *
 * Autor: Ulysses de Oliveira
 *
 * Data de Criação: 06/10/2017
 * Última modificação: 06/10/2017
 *
 * Descrição: Interface do módulo Heap
 *
 ****/

#ifndef _Heap_H_
#define _Heap_H_

/*********************** Includes *************************/

#include "Tipos.h"  /* Definições de tipos */

/******************* Alusões de Funções *******************/

extern tHeapHuff *IniciaHeapHuff(tHeapHuff *heap);
extern void DestroiHeapHuff(tHeapHuff *heap);
extern void InsereEmHeapHuff( tHeapHuff *heap, tNoHeapHuff item,
                              tFComparaHuff Compara );
extern tNoHeapHuff RemoveMinHeapHuff(tHeapHuff *heap, tFComparaHuff Compara);
extern tNoHeapHuff ObtemMinimoHeapHuff(const tHeapHuff *heap);
extern int HeapVazioHuff(const tHeapHuff *heap);
extern int TamanhoHeapHuff(const tHeapHuff *heap);

#endif
