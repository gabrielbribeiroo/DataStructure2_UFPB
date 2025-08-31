/****
 *
 * T�tulo: Tamanho de n�s de �rvores B+ com e sem preenchimento
 *
 * Autor: Ulysses de Oliveira
 *
 * Data de Cria��o: 16/09/2017
 * �ltima modifica��o: 16/09/2017
 *
 * Descri��o: Calcula o tamanho de uma estrutura do tipo tNoBM
 *            com e sem preenchimento
 *
 * Entrada: Nada
 *
 * Sa�da: Tamanho de uma estrutura do tipo tNoBM com e sem preenchimento
 *
 ****/

#include <stdio.h>

#define MAX(a,b) (((a) > (b)) ? (a) : (b))

typedef long tChave; /* Tipo da chave */

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
typedef struct noInterno {
           int    nFilhos;       /* N�mero de filhos do n� */
           tChave chaves[G - 1]; /* Array de chaves        */
           int    filhos[G];     /* Array de filhos do n�  */
        } tNoInterno;

   /* Tipo usado para armazenamento de folhas */
typedef struct noFolha {
           int          nChaves;    /* N�mero de chaves do n� */
           tChaveIndice chaves[F]; /* Array de pares */
                                   /* chave/�ndice   */
           int          proximaFolha; /* �ndice da pr�xima folha */
        } tNoFolha;

   /* Tipo usado para armazenamento de   */
   /* n�s (internos/folhas) da �rvore B+ */
typedef struct no {
           tTipoDoNo     tipoDoNo; /* Tipo do n� */
           union {
              tNoInterno noInterno; /* Usado se o n� � interno */
              tNoFolha   noFolha;   /* Usado se o n� � folha   */
           } conteudo;
        } tNoBM, *tArvoreBM;

/****
 *
 * TamanhoNoSemPreenchimento(): Calcula o tamanho de um n� sem
 *                              o preenchimento de estrutura
 *                              efetuado pelo compilador
 *
 * Par�metros: Nenhum
 *
 * Retorno: O tamanho de um n� sem preenchimento
 *
 * Observa��o: O prop�sito desta fun��o � apenas depura��o e
 *             melhor entendimento do programa
 *
 ****/
static int TamanhoNoSemPreenchimento(void)
{
   int tni, /* Tamanho de um n� interno sem preenchimento */
       tnf, /* Tamanho de uma folha sem preenchimento     */
       ti,  /* largura do tipo int                        */
       tc,  /* Tamanho de uma chave                       */
       tci, /* Tamanho de um par chave/�ndice             */
       tt,  /* Tamanho de uma constante de enumera��o     */
       t;   /* Tamanho de um n� sem preenchimento         */

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

/******************* Exemplo de Execu��o ******************

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
