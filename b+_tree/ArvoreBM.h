/****–
 *
 * Arquivo ArvoreBM.h
 *
 * Autor: Ulysses de Oliveira
 *
 * Data de Criação: 07/08/2008
 * Última modificação: 25/05/2010
 *
 * Descrição: Iterface das funções que implementam operações
 *            de busca e inserção usando árvores B+
 *
 ****/

#ifndef _ArvoreBM_H_ /* Previne inclusão múltipla deste arquivo */
#define _ArvoreBM_H_

/*********************** Includes *************************/

#include <stdio.h>     /* Tipo FILE                     */
#include "Registros.h" /* Interface do módulo Registros */

/************ Definições de Tipos e Constantes ************/

   /***                                                 ***/
   /**                                                   **/
   /** O tipo tChave é definido no cabeçalho Registros.h **/
   /**                                                   **/
   /***                                                 ***/

   /* Posição nula (inválida) em arquivo */
#define POSICAO_NULA     -1

   /* Constantes usadas para informar se   */
   /* um nó é interno, folha ou está vazio */
typedef enum {FOLHA, INTERNO, NO_VAZIO} tTipoDoNo;

   /* Tipo do conteúdo armazenado em cada folha da árvore */
typedef struct  {
           tChave chave;  /* Chave do registro              */
           int    indice; /* Posição do registro no arquivo */
        } tChaveIndice;

      /***                              ***/
      /* Dimensionamento de nós da árvore */
      /***                              ***/

#define TB   4096 /* Tamanho do bloco lido/escrito  */
#define TC   sizeof(tChave) /* Tamanho de uma chave */
#define TCI  sizeof(tChaveIndice)  /* Tamanho de um    */
                                   /* par chave/índice */
#define TI   sizeof(int) /* Tamanho de um filho e do inteiro */
                         /* que representa o grau do nó      */
#define TT   sizeof(tTipoDoNo) /* Tamanho de uma constante */
                               /* de enumeração            */

   /* Correção devido a preenchimento (tentativa e erro) */
#define CORRECAO 5

   /* Cálculo do grau da árvore */
#define G   ((TB - CORRECAO - TT - TI + TC)/(TC + TI))

   /* Metade do grau da árvore */
#define TG   G%2 ? G/2 + 1 : G/2

   /* Número máximo de pares chave/índice em cada folha */
#define F   ((TB - CORRECAO - TT - 2*TI)/TCI)

   /* Metade do máximo de pares chave/índice numa folha */
#define TF   F%2 ? F/2 + 1 : F/2

      /***                                     ***/
      /* Fim do dimensionamento de nós da árvore */
      /***                                     ***/

   /* Tipo usado para armazenamento de nós internos */
typedef struct {
           int    nFilhos;       /* Número de filhos do nó */
           tChave chaves[G - 1]; /* Array de chaves        */
           int    filhos[G];     /* Array de filhos do nó  */
        } tNoInterno;

   /* Tipo usado para armazenamento de folhas */
typedef struct {
           int          nChaves;    /* Número de chaves do nó */
           tChaveIndice chaves[F]; /* Array de pares */
                                   /* chave/índice   */
           int          proximaFolha; /* Índice da próxima folha */
        } tNoFolha;

   /* Tipo usado para armazenamento de   */
   /* nós (internos/folhas) da árvore B+ */
typedef struct {
           tTipoDoNo     tipoDoNo; /* Tipo do nó */
           union {
              tNoInterno noInterno; /* Usado se o nó é interno */
              tNoFolha   noFolha;   /* Usado se o nó é folha   */
           } conteudo;
        } tNoBM, *tArvoreBM;

/************************* Alusões ************************/

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
