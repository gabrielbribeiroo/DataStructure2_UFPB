/****
 *
 * Arquivo Tipos.h
 *
 * Autor: Ulysses de Oliveira
 *
 * Data de Cria��o: 06/10/2017
 * �ltima modifica��o: 06/10/2017
 *
 * Descri��o: Tipos utilizados pelo programa de codifica��o de
 *            Huffman
 *
 ****/

#ifndef _Tipos_H_ /* Previne inclus�o m�ltipla deste arquivo */
#define _Tipos_H_

/*********************** Includes *************************/

#include <limits.h>

/***************** Constantes Simb�licas ******************/

   /* N�mero m�ximo de bits num c�digo associado a um byte */
#define MAX_BITS  (UCHAR_MAX + 1)

   /* N�mero m�ximo de bytes num c�digo associado a um byte */
#define MAX_BYTES MAX_BITS/CHAR_BIT

/****************** Defini��es de Tipos *******************/

typedef unsigned char tByte; /* Tipo de um byte */

   /* Tipo do campo de informa��o dos n�s da �rvore de codifica��o */
typedef struct {
          tByte  byte;
          double frequencia;
        } tByteFreq;

   /* Tipo de cada n� e tipo de ponteiro para n� da �rvore de codifica��o */
typedef struct rotNoArvoreHuff {
          tByteFreq                byteFreq;
          struct rotNoArvoreHuff *filhoEsquerda;
          struct rotNoArvoreHuff *filhoDireita;
        } tNoArvoreHuff, *tArvoreHuff;

   /* Tipo de n� de um heap que armazena ponteiros para */
   /* os n�s da �rvore de codifica��o temporariamente   */
typedef tNoArvoreHuff *tNoHeapHuff;

   /* Tipo de heap que armazena as informa��es */
   /* sobre os n�s da �rvore de codifica��o    */
typedef struct {
           tNoHeapHuff *itens;      /* Array de elementos */
           int          capacidade; /* Quantos elementos o heap pode conter */
           int          nItens;     /* N�mero de elementos do heap */
        } tHeapHuff;

   /* Tipo de ponteiro para fun��o de compara��o */
   /* que compara elementos do heap              */
typedef int (*tFComparaHuff) (const void *, const void *);

   /* Tipo de n� de uma lista de c�digos can�nicos */
typedef struct {
           tByte valor;          /* Valor do byte */
           tByte nBits;          /* N�mero de bits na codifica��o */
           tByte cod[MAX_BYTES]; /* Codifica��o do byte */
        } tNoListaCanHuff;

   /* Tipo de estrutura que armazena os dados de um cabe�alho */
typedef struct {
           int              tamLista; /* N�mero de elementos da lista */
                                      /* de c�digos can�nicos         */
           tNoListaCanHuff *lista; /* A lista de c�digos can�nicos */
        } tCabecalhoHuff;

#endif
