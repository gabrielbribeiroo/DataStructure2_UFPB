#include <stdio.h>

typedef int tChave; /* Tipo da chave */

   /* Tipo do conteúdo armazenado em cada folha da árvore */
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
#define G ((TB + TCI - TI)/(TCI + TI) - 1)

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

int main(void)
{
   int ti = sizeof(int),   /* Largura do tipo inteiro (chave) */
       tc = 2*sizeof(int), /* Tamanho de um par chave/índice  */
       tn, /* Tamanho de um nó sem preenchimento */
       tp; /* Tamanho de um nó com preenchimento */

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

/******************* Exemplo de Execução ******************

>>> Tamanho de bloco: 4096
>>> Grau da arvore: 340

>>> Tamanho do tipo inteiro: 4
>>> Tamanho de um par chave/indice: 8

>>> Tamanho de no' sem preenchimento: 4076
>>> Tamanho de no' COM preenchimento: 4076

>>> Espaco de bloco nao utilizado: 20 bytes

**********************************************************/

