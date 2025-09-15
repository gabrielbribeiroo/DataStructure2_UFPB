/****
 *
 * Arquivo Tipos.h
 *
 * Autor: Ulysses de Oliveira
 *
 * Data de Criação: 06/10/2017
 * Última modificação: 06/10/2017
 *
 * Descrição: Tipos utilizados pelo programa de codificação de
 *            Huffman
 *
 ****/

#ifndef _Tipos_H_ /* Previne inclusão múltipla deste arquivo */
#define _Tipos_H_

/*********************** Includes *************************/

#include <limits.h>

/***************** Constantes Simbólicas ******************/

   /* Número máximo de bits num código associado a um byte */
#define MAX_BITS  (UCHAR_MAX + 1)

   /* Número máximo de bytes num código associado a um byte */
#define MAX_BYTES MAX_BITS/CHAR_BIT

/****************** Definições de Tipos *******************/

typedef unsigned char tByte; /* Tipo de um byte */

   /* Tipo do campo de informação dos nós da árvore de codificação */
typedef struct {
          tByte  byte;
          double frequencia;
        } tByteFreq;

   /* Tipo de cada nó e tipo de ponteiro para nó da árvore de codificação */
typedef struct rotNoArvoreHuff {
          tByteFreq                byteFreq;
          struct rotNoArvoreHuff *filhoEsquerda;
          struct rotNoArvoreHuff *filhoDireita;
        } tNoArvoreHuff, *tArvoreHuff;

   /* Tipo de nó de um heap que armazena ponteiros para */
   /* os nós da árvore de codificação temporariamente   */
typedef tNoArvoreHuff *tNoHeapHuff;

   /* Tipo de heap que armazena as informações */
   /* sobre os nós da árvore de codificação    */
typedef struct {
           tNoHeapHuff *itens;      /* Array de elementos */
           int          capacidade; /* Quantos elementos o heap pode conter */
           int          nItens;     /* Número de elementos do heap */
        } tHeapHuff;

   /* Tipo de ponteiro para função de comparação */
   /* que compara elementos do heap              */
typedef int (*tFComparaHuff) (const void *, const void *);

   /* Tipo de nó de uma lista de códigos canônicos */
typedef struct {
           tByte valor;          /* Valor do byte */
           tByte nBits;          /* Número de bits na codificação */
           tByte cod[MAX_BYTES]; /* Codificação do byte */
        } tNoListaCanHuff;

   /* Tipo de estrutura que armazena os dados de um cabeçalho */
typedef struct {
           int              tamLista; /* Número de elementos da lista */
                                      /* de códigos canônicos         */
           tNoListaCanHuff *lista; /* A lista de códigos canônicos */
        } tCabecalhoHuff;

#endif
