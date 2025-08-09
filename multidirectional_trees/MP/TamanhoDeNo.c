#include <stdio.h>

typedef int tChave; /* Tipo da chave */

   /* Tipo do conte�do armazenado em cada folha da �rvore */
typedef struct  {
          tChave chave;  /* Chave do registro              */
          int    indice; /* Posi��o do registro no arquivo */
} tChaveIndice;

#define G  5

   /* Tipo de um n� da �rvore de busca      */
   /* multidirecional armazenada em arquivo */
typedef struct no {
           int          nFilhos; /* O n�mero de filhos do n� */
           tChaveIndice chaves[G - 1]; /* array contendo   */
                                       /* chaves e �ndices */
                                       /* de registros     */
           struct no   *filhos[G]; /* array contendo as   */
                                   /* posi��es dos filhos */
                                   /* no arquivo que      */
                                   /* cont�m a �rvore     */
        } tNoMulti, *tArvoreMulti;

int main(void)
{
   int ti = sizeof(int),   /* Largura do tipo inteiro (chave) */
       tc = 2*sizeof(int), /* Tamanho de um par chave/�ndice  */
       tf = sizeof(struct no*), /* Tamanho de um ponteiro struct no* */
       tn, /* Tamanho de um n� sem preenchimento */
       tp; /* Tamanho de um n� com preenchimento */

   printf("\n>>> Grau da arvore: %d\n", G);

   printf("\n>>> Tamanho do tipo inteiro: %d", ti);
   printf("\n>>> Tamanho de um par chave/indice: %d", tc);
   printf("\n>>> Tamanho de um ponteiro: %d", tf);

   tn = ti + (G - 1)*tc + G*tf;
   printf("\n\n>>> Tamanho de no' sem preenchimento: %d", tn);

   tp = sizeof(tNoMulti);

   printf("\n>>> Tamanho de no' COM preenchimento: %d\n", tp);

   return 0;
}

/****************** Resultado do Programa *****************

Resultado no Windows:
====================

>>> Grau da arvore: 5

>>> Tamanho do tipo inteiro: 4
>>> Tamanho de um par chave/indice: 8
>>> Tamanho de um ponteiro: 4

>>> Tamanho de no' sem preenchimento: 56
>>> Tamanho de no' COM preenchimento: 56

Resultado no Linux/MacOS:
========================

>>> Grau da arvore: 5

>>> Tamanho do tipo inteiro: 4
>>> Tamanho de um par chave/indice: 8
>>> Tamanho de um ponteiro: 8

>>> Tamanho de no' sem preenchimento: 76
>>> Tamanho de no' COM preenchimento: 80

**********************************************************/
