/****
 *
 * Título: Tamanho de nós de árvores B+ com e sem preenchimento
 *
 * Autor: Ulysses de Oliveira
 *
 * Data de Criação: 16/09/2017
 * Última modificação: 16/09/2017
 *
 * Descrição: Calcula o tamanho de uma estrutura do tipo tNoBM
 *            com e sem preenchimento
 *
 * Entrada: Nada
 *
 * Saída: Tamanho de uma estrutura do tipo tNoBM com e sem preenchimento
 *
 ****/

#include <stdio.h>

#define MAX(a,b) (((a) > (b)) ? (a) : (b))

typedef long tChave; /* Tipo da chave */

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
typedef struct noInterno {
           int    nFilhos;       /* Número de filhos do nó */
           tChave chaves[G - 1]; /* Array de chaves        */
           int    filhos[G];     /* Array de filhos do nó  */
        } tNoInterno;

   /* Tipo usado para armazenamento de folhas */
typedef struct noFolha {
           int          nChaves;    /* Número de chaves do nó */
           tChaveIndice chaves[F]; /* Array de pares */
                                   /* chave/índice   */
           int          proximaFolha; /* Índice da próxima folha */
        } tNoFolha;

   /* Tipo usado para armazenamento de   */
   /* nós (internos/folhas) da árvore B+ */
typedef struct no {
           tTipoDoNo     tipoDoNo; /* Tipo do nó */
           union {
              tNoInterno noInterno; /* Usado se o nó é interno */
              tNoFolha   noFolha;   /* Usado se o nó é folha   */
           } conteudo;
        } tNoBM, *tArvoreBM;

/****
 *
 * TamanhoNoSemPreenchimento(): Calcula o tamanho de um nó sem
 *                              o preenchimento de estrutura
 *                              efetuado pelo compilador
 *
 * Parâmetros: Nenhum
 *
 * Retorno: O tamanho de um nó sem preenchimento
 *
 * Observação: O propósito desta função é apenas depuração e
 *             melhor entendimento do programa
 *
 ****/
static int TamanhoNoSemPreenchimento(void)
{
   int tni, /* Tamanho de um nó interno sem preenchimento */
       tnf, /* Tamanho de uma folha sem preenchimento     */
       ti,  /* largura do tipo int                        */
       tc,  /* Tamanho de uma chave                       */
       tci, /* Tamanho de um par chave/índice             */
       tt,  /* Tamanho de uma constante de enumeração     */
       t;   /* Tamanho de um nó sem preenchimento         */

   ti = sizeof(int);
   tc = sizeof(tChave);
   tci = sizeof(tChave) + sizeof(int);
   tt = sizeof(tTipoDoNo);

   tni = ti + (G - 1)*tc + G*ti;

   tnf = ti + F*tci + ti;

   t = tt + MAX(tni, tnf);

   return t;
}

int main(void)
{
   int ts, tc;

   printf("\n\t*** Parametros da arvore ***");

   printf("\n\t\t>>> G  = %d", (int) G);
   printf("\n\t\t>>> TG = %d", (int) TG);
   printf("\n\t\t>>> F  = %d", (int) F);
   printf("\n\t\t>>> TF = %d\n", (int) TF);

   ts = TamanhoNoSemPreenchimento();
   printf("\n\t>>> Tamanho de no' SEM preenchimento: %d bytes", ts);

   tc = sizeof(tNoBM);
   printf("\n\t>>> Tamanho de no' COM preenchimento: %d bytes", tc);

   printf("\n\t>>> Tamanho do preenchimento: %d bytes\n", tc - ts);

   printf( "\n\t>>> Espaco de bloco nao utilizado: %d bytes\n\n",
           TB - tc );

   return 0;
}

/******************* Exemplo de Execução ******************

GCC/Windows:
===========

        *** Parametros da arvore ***
                >>> G  = 510
                >>> TG = 255
                >>> F  = 509
                >>> TF = 255

        >>> Tamanho de no' SEM preenchimento: 4084 bytes
        >>> Tamanho de no' COM preenchimento: 4084 bytes
        >>> Tamanho do preenchimento: 0 bytes

        >>> Espaco de bloco nao utilizado: 12 bytes

4.8.5 (SUSE Linux):
==================

Usando #define CORRECAO 0:
===============================

        *** Parametros da arvore ***
                >>> G  = 341
                >>> TG = 171
                >>> F  = 255
                >>> TF = 128

        >>> Tamanho de no' SEM preenchimento: 4092 bytes
        >>> Tamanho de no' COM preenchimento: 4104 bytes
        >>> Tamanho do preenchimento: 12 bytes

        >>> Espaco de bloco nao utilizado: -8 bytes

Usando #define CORRECAO 5:
===============================

        *** Parametros da arvore ***
                >>> G  = 340
                >>> TG = 170
                >>> F  = 254
                >>> TF = 127

        >>> Tamanho de no' SEM preenchimento: 4080 bytes
        >>> Tamanho de no' COM preenchimento: 4088 bytes
        >>> Tamanho do preenchimento: 8 bytes

        >>> Espaco de bloco nao utilizado: 8 bytes

**********************************************************/
