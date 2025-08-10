/****
 *
 * Arquivo ArvoreMulti2.h
 *
 * Autor: Ulysses de Oliveira
 *
 * Data de Cria��o: 25/04/2004
 * �ltima modifica��o: 21/07/2008
 *
 * Descri��o: Interface das fun��es que implementam opera��es
 *            usando �rvores de busca multidirecional em arquivo
 *
 ****/

#ifndef _ArvoreMulti2_H_  /* Previne inclus�o m�ltipla */
#define _ArvoreMulti2_H_  /* deste arquivo             */

/*********************** Includes *************************/

#include <stdio.h>     /* Tipo FILE */
#include "Registros.h" /* Interface do m�dulo Registros */

/************ Defini��es de Tipos e Constantes ************/

   /* Posi��o nula (inv�lida) em arquivo */
#define POSICAO_NULA -1

   /* Tipo do par chave/�ndice armazenado nos n�s */
typedef struct  {
          tChave chave;  /* Chave do registro              */
          int    indice; /* Posi��o do registro no arquivo */
} tChaveIndice;

      /***                               ***/
      /* Dimensionamento do grau da �rvore */
      /***                               ***/

   /* Tamanho do bloco lido/escrito  */
#define TB   4096

   /* Tamanho de um par chave/�ndice */
#define TCI   sizeof(tChaveIndice)

   /* Tamanho de um filho e do inteiro */
   /* que representa o grau do n�      */
#define TI   sizeof(int)

   /* C�lculo do grau da �rvore */
//#define G ((TB + TCI - TI)/(TCI + TI) - 1)
#define G 6

      /***                                      ***/
      /* Fim do dimensionamento do grau da �rvore */
      /***                                      ***/

   /* Tipo de um n� da �rvore de busca      */
   /* multidirecional armazenada em arquivo */
typedef struct {
           int          nFilhos; /* O n�mero de filhos do n� */
           tChaveIndice chaves[G - 1]; /* array contendo   */
                                       /* chaves e �ndices */
                                          /* de registros     */
           int          filhos[G]; /* array contendo as   */
                                      /* posi��es dos filhos */
                                      /* no arquivo que      */
                                      /* cont�m a �rvore     */
        } tNoMultiMS, *tArvoreMultiMS;

/************************* Alus�es ************************/

extern int BuscaMultiMS(FILE *arvore, tChave chave);
extern int InsereMultiMS( int *posicaoRaiz, tChaveIndice *chaveEIndice,
                          FILE *streamArvore );
extern int RemoveChaveMultiMS(FILE **streamArvore, tChave chave);
extern void CaminhamentoInfixoMultiMS( FILE *streamArvore, int posNo,
                                       FILE *streamChaves );
extern tChaveIndice MenorChaveMultiMS(FILE *streamArvore, int raiz);
extern tChaveIndice MaiorChaveMultiMS(FILE *streamArvore, int raiz);
extern int NumeroDeChavesMultiMS(FILE *stream);
extern int NumeroDeNosMultiMS(FILE *stream, int posNo);
extern int AlturaMultiMS(FILE *stream, long posNo);

#endif

