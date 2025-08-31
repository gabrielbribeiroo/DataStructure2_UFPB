/****�
 *
 * Arquivo ArvoreBM.h
 *
 * Autor: Ulysses de Oliveira
 *
 * Data de Cria��o: 07/08/2008
 * �ltima modifica��o: 25/05/2010
 *
 * Descri��o: Iterface das fun��es que implementam opera��es
 *            de busca e inser��o usando �rvores B+
 *
 ****/

#ifndef _ArvoreBM_H_ /* Previne inclus�o m�ltipla deste arquivo */
#define _ArvoreBM_H_

/*********************** Includes *************************/

#include <stdio.h>     /* Tipo FILE                     */
#include "Registros.h" /* Interface do m�dulo Registros */

/************ Defini��es de Tipos e Constantes ************/

   /***                                                 ***/
   /**                                                   **/
   /** O tipo tChave � definido no cabe�alho Registros.h **/
   /**                                                   **/
   /***                                                 ***/

   /* Posi��o nula (inv�lida) em arquivo */
#define POSICAO_NULA     -1

   /* Constantes usadas para informar se   */
   /* um n� � interno, folha ou est� vazio */
typedef enum {FOLHA, INTERNO, NO_VAZIO} tTipoDoNo;

   /* Tipo do conte�do armazenado em cada folha da �rvore */
typedef struct  {
           tChave chave;  /* Chave do registro              */
           int    indice; /* Posi��o do registro no arquivo */
        } tChaveIndice;

      /***                              ***/
      /* Dimensionamento de n�s da �rvore */
      /***                              ***/

#define TB   4096 /* Tamanho do bloco lido/escrito  */
#define TC   sizeof(tChave) /* Tamanho de uma chave */
#define TCI  sizeof(tChaveIndice)  /* Tamanho de um    */
                                   /* par chave/�ndice */
#define TI   sizeof(int) /* Tamanho de um filho e do inteiro */
                         /* que representa o grau do n�      */
#define TT   sizeof(tTipoDoNo) /* Tamanho de uma constante */
                               /* de enumera��o            */

   /* Corre��o devido a preenchimento (tentativa e erro) */
#define CORRECAO 5

   /* C�lculo do grau da �rvore */
#define G   ((TB - CORRECAO - TT - TI + TC)/(TC + TI))

   /* Metade do grau da �rvore */
#define TG   G%2 ? G/2 + 1 : G/2

   /* N�mero m�ximo de pares chave/�ndice em cada folha */
#define F   ((TB - CORRECAO - TT - 2*TI)/TCI)

   /* Metade do m�ximo de pares chave/�ndice numa folha */
#define TF   F%2 ? F/2 + 1 : F/2

      /***                                     ***/
      /* Fim do dimensionamento de n�s da �rvore */
      /***                                     ***/

   /* Tipo usado para armazenamento de n�s internos */
typedef struct {
           int    nFilhos;       /* N�mero de filhos do n� */
           tChave chaves[G - 1]; /* Array de chaves        */
           int    filhos[G];     /* Array de filhos do n�  */
        } tNoInterno;

   /* Tipo usado para armazenamento de folhas */
typedef struct {
           int          nChaves;    /* N�mero de chaves do n� */
           tChaveIndice chaves[F]; /* Array de pares */
                                   /* chave/�ndice   */
           int          proximaFolha; /* �ndice da pr�xima folha */
        } tNoFolha;

   /* Tipo usado para armazenamento de   */
   /* n�s (internos/folhas) da �rvore B+ */
typedef struct {
           tTipoDoNo     tipoDoNo; /* Tipo do n� */
           union {
              tNoInterno noInterno; /* Usado se o n� � interno */
              tNoFolha   noFolha;   /* Usado se o n� � folha   */
           } conteudo;
        } tNoBM, *tArvoreBM;

/************************* Alus�es ************************/

extern void LeNoBM(FILE *stream, int pos, tNoBM *no);
extern int LeRaiz(const char *nomeArqRaiz);
extern void EscreveRaiz(const char *nomeArqRaiz, int raiz);
extern int BuscaBM( FILE *stream, const tNoBM* raiz,
                    tChave chave );
extern int InsereBM( tNoBM* raiz, tChaveIndice *chaveEIndice,
                     FILE *streamArvore );
extern int RemoveBM( FILE *stream, tNoBM *raiz, tChave chave );

extern int MenorChaveBM(FILE *stream);
extern int MaiorChaveBM(FILE *stream);
extern void EscreveChavesOrdenadas( const tNoBM *arvore,
                                    FILE *streamArv,
                                    FILE *streamChaves );
extern int BuscaIntervaloBM( tChave chave1, tChave chave2,
                             const tNoBM* arvore, FILE *streamArv,
                             FILE *streamChaves );
extern int ProfundidadeBM(FILE *stream);
extern int NumeroDeChavesBM(FILE *streamArv);
extern int NumeroDeFolhasBM(FILE *stream);
extern int NumeroDeNosInternosBM( FILE *stream );
extern int NumeroDeNosVaziosBM(FILE *stream);
extern int NumeroDeNosBM(FILE *stream);

#endif
