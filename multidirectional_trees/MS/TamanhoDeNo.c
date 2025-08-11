#include <stdio.h>

typedef int tChave; /* Tipo da chave */

   /* Tipo do conte�do armazenado em cada folha da �rvore */
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
#define G ((TB + TCI - TI)/(TCI + TI) - 1)

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

int main(void)
{
   int ti = sizeof(int),   /* Largura do tipo inteiro (chave) */
       tc = 2*sizeof(int), /* Tamanho de um par chave/�ndice  */
       tn, /* Tamanho de um n� sem preenchimento */
       tp; /* Tamanho de um n� com preenchimento */

   printf("\n>>> Tamanho de bloco: %d", TB);
   printf("\n>>> Grau da arvore: %d\n", G);

   printf("\n>>> Tamanho do tipo inteiro: %d", ti);
   printf("\n>>> Tamanho de um par chave/indice: %d", tc);

   tn = ti + (G - 1)*tc + G*ti;
   printf("\n\n>>> Tamanho de no' sem preenchimento: %d", tn);

   tp = sizeof(tNoMultiMS);

   printf("\n>>> Tamanho de no' COM preenchimento: %d\n", tp);

   printf("\n>>> Espaco de bloco nao utilizado: %d bytes\n\n",
          TB - tp);

   return 0;
}

/******************* Exemplo de Execu��o ******************

>>> Tamanho de bloco: 4096
>>> Grau da arvore: 340

>>> Tamanho do tipo inteiro: 4
>>> Tamanho de um par chave/indice: 8

>>> Tamanho de no' sem preenchimento: 4076
>>> Tamanho de no' COM preenchimento: 4076

>>> Espaco de bloco nao utilizado: 20 bytes

**********************************************************/

