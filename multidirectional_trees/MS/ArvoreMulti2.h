/****
 *
 * Arquivo ArvoreMulti2.h
 *
 * Autor: Ulysses de Oliveira
 *
 * Data de Criação: 25/04/2004
 * Última modificação: 21/07/2008
 *
 * Descrição: Interface das funções que implementam operações
 *            usando árvores de busca multidirecional em arquivo
 *
 ****/

#ifndef _ArvoreMulti2_H_  /* Previne inclusão múltipla */
#define _ArvoreMulti2_H_  /* deste arquivo             */

/*********************** Includes *************************/

#include <stdio.h>     /* Tipo FILE */
#include "Registros.h" /* Interface do módulo Registros */

/************ Definições de Tipos e Constantes ************/

   /* Posição nula (inválida) em arquivo */
#define POSICAO_NULA -1

   /* Tipo do par chave/índice armazenado nos nós */
typedef struct  {
          tChave chave;  /* Chave do registro              */
          int    indice; /* Posição do registro no arquivo */
} tChaveIndice;

      /***                               ***/
      /* Dimensionamento do grau da árvore */
      /***                               ***/

   /* Tamanho do bloco lido/escrito  */
#define TB   4096

   /* Tamanho de um par chave/índice */
#define TCI   sizeof(tChaveIndice)

   /* Tamanho de um filho e do inteiro */
   /* que representa o grau do nó      */
#define TI   sizeof(int)

   /* Cálculo do grau da árvore */
//#define G ((TB + TCI - TI)/(TCI + TI) - 1)
#define G 6

      /***                                      ***/
      /* Fim do dimensionamento do grau da árvore */
      /***                                      ***/

   /* Tipo de um nó da árvore de busca      */
   /* multidirecional armazenada em arquivo */
typedef struct {
           int          nFilhos; /* O número de filhos do nó */
           tChaveIndice chaves[G - 1]; /* array contendo   */
                                       /* chaves e índices */
                                          /* de registros     */
           int          filhos[G]; /* array contendo as   */
                                      /* posições dos filhos */
                                      /* no arquivo que      */
                                      /* contém a árvore     */
        } tNoMultiMS, *tArvoreMultiMS;

/************************* Alusões ************************/

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

