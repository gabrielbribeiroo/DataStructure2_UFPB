/****
 *
 * Arquivo: Huffman.c
 *
 * Autor: Ulysses de Oliveira
 *
 * Data de Cria��o: 06/10/2017
 * �ltima modifica��o: 06/10/2017
 *
 * Descri��o: Implementa��o da codifica��o de Huffman
 *
 ****/

/*********************** Includes *************************/

#include <string.h>   /* memmove()                */
#include <stdlib.h>   /* Aloca��o din�mica        */
#include <limits.h>   /* Limites de tipos inteiros */
#include <ctype.h>    /* Classifica��o de caracteres */
#include "Tipos.h"    /* Tipos usados pelo programa  */
#include "Heap.h"     /* Interface do m�dulo Heap  */
#include "Huffman.h"  /* Interface deste m�dulo    */
#include "Arquivos.h" /* Processamento de arquivos */
#include "Bits.h"     /* Processamento de bits     */
#include "ASSEGURA.h" /* Macro ASSEGURA()          */

/****************** Diretiva de Depura��o *****************/


#define DEPURACAO 0

/********************* Fun��es Locais *********************/

/****
 *
 * ComparaNosHeapHuff(): Compara dois elementos do heap
 *
 * Par�metros:
 *      e1, e2 (entrada) - os elementos
 *
 * Retorno:
 *     0, se e1->byteFreq.frequencia == e2->byteFreq.frequencia
 *     < 0, se e1->byteFreq.frequencia < e2->byteFreq.frequencia
 *     > 0, se e1->byteFreq.frequencia > e2->byteFreq.frequencia
 *
 ****/
static int ComparaNosHeapHuff(const void *e1, const void *e2)
{
   tNoHeapHuff elemento1 = (tNoHeapHuff)e1, elemento2 = (tNoHeapHuff)e2;

   ASSEGURA(e1 && e2, "Elemento nulo recebido");

   return elemento1->byteFreq.frequencia - elemento2->byteFreq.frequencia;
}

/****
 *
 * ExibeCodigosHuff(): Exibe o c�digo associado a cada byte de uma
 *                 lista de c�digos can�nicos
 *
 * Par�metros:
 *     listaCanonica (entrada) - a lista can�nica de c�digos
 *     tamLista (entrada) - tamanho da lista
 *
 * Retorno: Nada
 *
 * Observa��o: Esta fun��o � usada em teste e depura��o
 *
 ****/
static void ExibeCodigosHuff(tNoListaCanHuff *listaCanonica, int tamLista)
{
   int   i,
         tamanho = 0;

      /* Exibe cabe�alho da apresenta��o na tela */
   printf("Byte  Tamanho  Codificacao\n");
   printf("----- -------- ----------------\n");

      /* Apresenta os c�digos na tela  */
   for(i = 0; i < tamLista; i++) {
         /* Verifica a validade do c�digo */
      ASSEGURA(listaCanonica[i].nBits > 0, "Tamanho de codigo deveria ser positivo");

         /* Exibe informa��es sobre o valor do byte corrente */
//      printf( "0x%02X  %02d       ", listaCanonica[i].valor,
      printf("%c  %02d       ", listaCanonica[i].valor, listaCanonica[i].nBits);

         /* Escreve os bits do c�digo */
      for(tamanho = 0; tamanho < listaCanonica[i].nBits; tamanho++)
         if (ConsultaBitEmArray(listaCanonica[i].cod, tamanho))
            putchar('1');
         else
            putchar('0');

      putchar('\n');
   }
}

/****
 *
 * ConstroiNoArvoreHuff(): Constr�i um n� de uma �rvore bin�ria
 *
 * Par�metros: Nenhum
 *
 * Retorno: Endere�o do n� criado
 *
 ****/
static tNoArvoreHuff *ConstroiNoArvoreHuff(void)
{
   tNoArvoreHuff *pNo;

   pNo = malloc(sizeof(tNoArvoreHuff)); /* Tenta alocar o novo n� */

      /* Se n�o houve aloca��o, aborta o programa */
   ASSEGURA(pNo, "Nao foi possivel alocar no' de arvore");

      /* Este n� ainda n�o tem filhos */
   pNo->filhoEsquerda = NULL;
   pNo->filhoDireita = NULL;

   return pNo;
}

/****
 *
 * ReconstroiArvoreHuff(): Reconstr�i uma �rvore de codifica��o a
 *                     partir de uma lista de c�digos
 *
 * Par�metros:
 *       lista (entrada) - a lista de c�digos
 *       tamLista (entrada) - tamanho da lista
 *
 * Retorno: Endere�o da raiz da �rvore
 *
 ****/
static tArvoreHuff ReconstroiArvoreHuff(tNoListaCanHuff lista[], int tamLista)
{
   tNoArvoreHuff *raiz, /* Raiz da �rvore a ser criada */
                 *p, /* Ponteiro usado para descer a �rvore    */
                 *q; /* Segue o ponteiro p um n�vel acima dele */
   int            i, j;

   raiz = ConstroiNoArvoreHuff(); /* Cria a raiz da �rvore */

      /* Para cada valor de byte da lista, desce-se a �rvore criando ou */
      /* visitando n�s de acordo com o c�digo associado ao byte         */
   for (i = 0; i < tamLista; ++i) {
         /* A descida come�a sempre pela raiz */
      q = NULL;
      p = raiz;

         /* Acessa cada bit do c�digo associado ao byte corrente e desce a */
         /* �rvore de acordo com seu valor: se o bit for 0, desce-se pela  */
         /* esquerda; se o bit for 1, desce-se pela direita                */
      for (j = 0; j < lista[i].nBits; ++j) {
         q = p; /* Guarda o endere�o do n� corrente antes de descer */

         if (ConsultaBitEmArray(lista[i].cod, j)) {
            p = p->filhoDireita; /* O bit � 1 e a descida � pela direita */

               /* Se o n� n�o existir, cria-se um n� que ser� filho direito de q */
            if (!p) {
               p = ConstroiNoArvoreHuff();
               q->filhoDireita = p; /* Inclui novo n� na �rvore */
            }
         } else {
            p = p->filhoEsquerda; /* O bit � 0 e a descida � pela esquerda */

               /* Se o n� n�o existir, cria-se um */
               /* n� que ser� filho esquerdo de q */
            if (!p) {
               p = ConstroiNoArvoreHuff();
               q->filhoEsquerda = p;/* Inclui novo n� na �rvore */
            }
         }
      }

         /* Neste ponto, p deve estar apontando para uma folha */
      ASSEGURA( !p->filhoEsquerda && !p->filhoDireita, "Deveria ser folha!" );

      p->byteFreq.byte = lista[i].valor; /* Armazena o valor do byte na folha */
   }

   return raiz;
}

/****
 *
 * ExibeHeapHuff(): Exibe o conte�do do heap
 *
 * Par�metros:
 *     heap (entrada) - o heap
 *
 * Retorno: Nada
 *
 * Observa��o: Usada apenas em depura��o
 *
 ****/
static void ExibeHeapHuff(const tHeapHuff *heap)
{
   int i;
   double freqTotal = 0.0;

   for (i = 0; i < heap->nItens; ++i) {
      printf( "\n [%02d]\tbyte = 0x%02X\tfrequencia = %.2f", i,
              heap->itens[i]->byteFreq.byte,
              heap->itens[i]->byteFreq.frequencia );
      freqTotal += heap->itens[i]->byteFreq.frequencia;
   }

   printf( "\n\n\t>>> Frequencia total: %.2f\n\n", freqTotal);
}

/****
 *
 * CriaHeapHuff(): Cria um heap contendo n�s de uma �rvore que armazenam
 *                 valores de bytes e suas respectivas frequ�ncias
 *
 * Par�metros:
 *     stream (entrada) - stream associado ao arquivo no qual os bytes ser�o lidos
 *     nBytes (sa�da) - n�mero total de bytes lidos no arquivo
 *
 * Retorno: Endere�o do heap criado
 *
 ****/
static tHeapHuff *CriaHeapHuff(FILE *stream, int *nBytes)
{
   tHeapHuff     *pHeap; /* Ponteiro para o heap que ser� criado */
   tNoArvoreHuff *ptrNovoNo; /* Apontar� para o novo n� alocado */
   int           *byteFreq, /* Apontar� para um array contendo os */
                            /* n�meros de ocorr�ncias dos bytes   */
                  umByte, /* Armazena um byte */
                  tamArquivo = 0, /* N�mero de bytes do arquivo */
                  i;

      /* Aloca um array para conter as ocorr�ncias dos bytes */
   byteFreq = calloc(UCHAR_MAX + 1, sizeof(int));
   ASSEGURA(byteFreq, "Array de ocorrencias nao foi alocado");

   rewind(stream); /* Move o apontador de posi��o para o in�cio do arquivo */

      /* Obt�m o n�mero de ocorr�ncias de cada byte */
   while (1) {
      umByte = fgetc(stream); /* L� um byte */

         /* Verifica se ocorreu erro */
      ASSEGURA(!ferror(stream), "Erro de leitura de arquivo");

      if (feof(stream))
         break; /* N�o h� mais bytes a serem lidos */

         /* Verifica se o programa pode lidar a frequ�ncia de ocorr�ncia */
         /* desse byte. A aus�ncia desse teste causar� overflow.         */
      ASSEGURA(byteFreq[umByte] < INT_MAX,
               "Este programa nao suporta um arquivo tao grande");

         /* Incrementa o n�mero de ocorr�ncias do �ltimo byte lido */
      ++byteFreq[umByte];

         /* Verifica se o programa pode lidar com o tamanho do */
         /* arquivo. A aus�ncia desse teste causar� overflow.  */
      ASSEGURA(tamArquivo < INT_MAX,
               "Este programa nao suporta um arquivo tao grande");

      ++tamArquivo; /* Mais um byte foi lido */
   }

      /* Aloca a vari�vel que armazenar� o heap */
   ASSEGURA(pHeap = malloc(sizeof(*pHeap)), "Heap nao alocado");

   IniciaHeapHuff(pHeap); /* Inicia o heap */

      /* Constr�i o heap */
   for (i = 0; i < UCHAR_MAX; ++i) {
         /* Cria um n� para cada valor de byte encontrado no arquivo */
      if (byteFreq[i]) {
         ptrNovoNo = ConstroiNoArvoreHuff(); /* Tenta alocar um novo n� */

            /* Armazena o byte corrente e sua frequ�ncia no novo n� */
         ptrNovoNo->byteFreq.byte = i;
         ptrNovoNo->byteFreq.frequencia = (double) 100*byteFreq[i]/tamArquivo;

            /* Insere o novo n� no heap */
         InsereEmHeapHuff(pHeap, ptrNovoNo, ComparaNosHeapHuff);
      }
   }

   free(byteFreq); /* O array de ocorr�ncias n�o � mais necess�rio */

   *nBytes = tamArquivo; /* Atualiza o segundo par�metro antes de retornar */

   return pHeap; /* Retorna o endere�o do heap */
}

/****
 *
 * DestroiArvoreHuff(): Destr�i uma �rvore bin�ria; i.e., libera o
 *                  espa�o ocupado por cada n� da �rvore
 *
 * Par�metros:
 *      arvore (entrada e sa�da) - ponteiro para a raiz da �rvore
 *                                 bin�ria a ser destru�da
 *
 * Retorno: Nada
 *
 ****/
static void DestroiArvoreHuff(tArvoreHuff arvore)
{
   if (arvore) {
      DestroiArvoreHuff(arvore->filhoEsquerda); /* Destr�i sub�rvore esquerda */

      DestroiArvoreHuff(arvore->filhoDireita); /* Destr�i sub�rvore direita */

      free(arvore); /* Libera a raiz */
   }
}

/****
 *
 * ObtemTamanhosHuff(): Obt�m os tamanhos dos c�digos associados aos
 *                  bytes armazenados nas folhas de uma �rvore de
 *                  codifica��o de Huffman
 *
 * Par�metros:
 *      raiz (entrada) - ponteiro para a raiz da �rvore que cont�m a codifica��o
 *      tabela[] (sa�da) - tabela de consulta que conter� os
 *                         tamanhos dos c�digos can�nicos
 *      n (entrada) - n�vel da �rvore na qual a visita��o dos n�s ser� iniciada
 *
 * Retorno: Nada
 *
 * Observa��o: Na primeira chamada desta fun��o, o valor do
 *             �ltimo par�metro deve ser 0
 *
 ****/
static void ObtemTamanhosHuff(tArvoreHuff raiz, int tabela[], int n)
{
      /* A �rvore n�o pode estar vazia */
   ASSEGURA(raiz, "A raiz da arvore e' nula");

      /* Se for poss�vel, caminha na sub�rvore esquerda do pr�ximo n�vel */
   if (raiz->filhoEsquerda)
      ObtemTamanhosHuff(raiz->filhoEsquerda, tabela, n + 1);

      /* Se for poss�vel, caminha na sub�rvore direita do pr�ximo n�vel */
   if (raiz->filhoDireita)
      ObtemTamanhosHuff(raiz->filhoDireita, tabela, n + 1);

      /* Se o n� corrente for uma folha, ele  */
      /* armazena um valor de byte do arquivo */
   if (!raiz->filhoEsquerda && !raiz->filhoDireita)
         /* Armazena no �ndice da tabela correspondente ao valor do byte       */
         /* armazenado nessa folha o tamanho que foi acumulado no caminhamento */
      tabela[raiz->byteFreq.byte] = n;
}

/****
 *
 * ComparaTamanhosHuff(): Fun��o de compara��o usada por qsort() para
 *                    ordenar a lista de c�digos can�nicos por
 *                    tamanhos de c�digos e por valores de bytes
 *
 * Par�metros:
 *     e1, e2 (entrada) - ponteiros para elementos de uma lista can�nica
 *
 * Retorno:
 *      1 se e1 > e2
 *     -1 se e1 < e2
 *
 * Observa��es:
 *      1. Se os tamanhos forem iguais, os valores dos bytes ser�o
 *         usados para desempatar
 *      2. Esta fun��o nunca retorna zero
 *
 ****/
static int ComparaTamanhosHuff(const void *e1, const void *e2)
{
   int nBits1 = ((tNoListaCanHuff *)e1)->nBits,
       nBits2 = ((tNoListaCanHuff *)e2)->nBits,
       valor1 = ((tNoListaCanHuff *)e1)->valor,
       valor2 = ((tNoListaCanHuff *)e2)->valor;

   if (nBits1 > nBits2)
      return 1; /* e1 > e2 */
   else if (nBits1 < nBits2)
      return -1; /* e1 < e2 */

      /* Se ainda n�o houve retorno, os tamanhos s�o iguais e */
      /* o maior elemento ser� aquele que tiver o maior valor */
   if (valor1 > valor2)
      return 1;  /* e1 > e2 */
   else if (valor1 < valor2)
      return -1;  /* e1 < e2 */

      /* J� deveria ter havido retorno */
   ASSEGURA(0, "Erro: houve empate em ComparaTamanhosHuff()");
}

/****
 *
 * ObtemCodigosCanonicosHuff(): Atribui um c�digo can�nico para cada
 *                          byte de uma lista de bytes ordenados
 *                          de acordo com seus tamanhos de c�digos e seus valores
 *
 * Par�metros:
 *     lista (entrada e sa�da ) - a lista ordenada de bytes
 *     nItens (entrada) - n�mero de elementos da lista
 *
 * Retorno: Nada
 *
 ****/
static void ObtemCodigosCanonicosHuff(tNoListaCanHuff lista[], int nItens)
{
   int   i;
   tByte tamanho;
   tByte codigo[MAX_BYTES] = {0};

      /* Obt�m o tamanho do c�digo armazenado no primeiro elemento da lista */
   tamanho = lista[0].nBits;

      /* Atribui c�digos aos elementos da lista */
   for(i = 0; i < nItens; ++i) {
         /* Verifica se o tamanho do c�digo � v�lido */
      ASSEGURA(lista[i].nBits, "Tamanho de codigo e' zero");

      IncrementaBits(codigo); /* Soma 1 ao c�digo do byte anterior */

         /* Ajusta o c�digo se seu tamanho for menor */
         /* do que o tamanho do c�digo anterior      */
      if (lista[i].nBits > tamanho) {
         DeslocaBitsEsquerda(codigo, lista[i].nBits - tamanho);
         tamanho = lista[i].nBits;
      }

         /* Copia o c�digo obtido para o respectivo */
         /* elemento da lista e alinha-o � esquerda */
      memmove(lista[i].cod, codigo, MAX_BYTES);
      DeslocaBitsEsquerda(lista[i].cod, MAX_BITS - tamanho);
   }
}

/****
 *
 * ExibeTamanhosCodigosHuff(): Exibe os tamanhos dos c�digos
 *
 * Par�metros:
 *     tamanhos (entrada) - tabela de consulta contendo os tamanhos dos c�digos
 *     nItens (entrada) - n�mero de elementos da tabela
 *
 * Retorno: Nada
 *
 * Observa��o: Usada apenas em depura��o
 *
 ****/
void ExibeTamanhosCodigosHuff(int *tamanhos, int nItens)
{
   int i;

   for (i = 0; i < nItens; ++i)
      if (tamanhos[i])
         printf( "0x%02X\t%d\n",i, tamanhos[i]);
}

/****
 *
 * CriaListaCanonicaHuff(): Constr�i uma lista de c�digos can�nicos
 *
 * Par�metros:
 *     arvore (entrada) - ponteiro para raiz da �rvore a partir
 *                        da qual a lista ser� criada
 *     nItens (entrada) - n�mero de elementos que a lista ter�
 *
 * Retorno: Endere�o da lista de c�digos can�nicos
 *
 ****/
static tNoListaCanHuff *CriaListaCanonicaHuff(tArvoreHuff arvore, int nItens)
{
   tNoListaCanHuff *listaCanonica;
   int             *tamanhos, i, j;

      /* Aloca um array para conter os tamanhos dos */
      /* c�digos associados aos valores de bytes    */
   tamanhos = calloc(UCHAR_MAX + 1, sizeof(int));
   ASSEGURA(tamanhos, "Array de tamanhos nao foi alocado");

      /* Obt�m os tamanhos dos c�digos dos bytes */
   ObtemTamanhosHuff(arvore, tamanhos, 0);

      /* Aloca a lista de c�digos can�nicos */
   listaCanonica = calloc(nItens, sizeof(tNoListaCanHuff));
   ASSEGURA(listaCanonica, "Lista canonica nao foi alocada");

      /* Copia os tamanhos dos c�digos para a lista de c�digos can�nicos */
   for (i = 0, j = 0; i < UCHAR_MAX + 1; ++i)
         /* A lista can�nica cont�m apenas tamanhos n�o nulos */
      if (tamanhos[i]) {
         listaCanonica[j].valor = i;
         listaCanonica[j].nBits = tamanhos[i];
         ++j;
      }

   free(tamanhos); /* O array que cont�m os tamanhos n�o � mais necess�rio */

      /* Ordena a lista de c�digos can�nicos por */
      /* tamanho de c�digo e valor de byte       */
   qsort( listaCanonica, nItens, sizeof(tNoListaCanHuff), ComparaTamanhosHuff );

      /* Obt�m os c�digos can�nicos */
   ObtemCodigosCanonicosHuff(listaCanonica, nItens);

   return listaCanonica;
}

/****
 *
 * EscreveCabecalhoHuff(): Escreve o cabe�alho da codifica��o no
 *                     in�cio do arquivo de sa�da
 *
 * Par�metros:
 *     cabecalho (entrada) - ponteiro para o cabe�alho que ser� escrito
 *     stream (entrada) - stream associado ao arquivo de sa�da
 *
 * Retorno: Nada
 *
 ****/
static void EscreveCabecalhoHuff(const tCabecalhoHuff *cabecalho, FILE *stream)
{
      /* Escreve o n�mero de elementos da lista */
   fwrite( &cabecalho->tamLista, sizeof(cabecalho->tamLista), 1, stream );

      /* Escreve a lista de c�digos can�nicos */
   fwrite(cabecalho->lista, sizeof(tNoListaCanHuff), cabecalho->tamLista, stream);
}

/****
 *
 * LeCabecalhoHuff(): L� o cabe�alho da codifica��o num arquivo de entrada codificado
 *
 * Par�metros:
 *     cabecalho (sa�da) - ponteiro para o cabe�alho que ser� lido
 *     stream (entrada) - stream associado ao arquivo de sa�da
 *
 * Retorno: Endere�o da lista de c�digos can�nicos lida no
 *          arquivo ou NULL, se ocorrer algum erro
 *
 ****/
static tNoListaCanHuff *LeCabecalhoHuff(tCabecalhoHuff *cabecalho, FILE *stream)
{
      /* L� o n�mero de elementos da lista */
   fread( &cabecalho->tamLista, sizeof(cabecalho->tamLista), 1, stream );

   if (ferror(stream))
      return NULL; /* Ocorreu erro de leitura */

      /* Aloca espa�o para a lista de c�digos can�nicos */
   cabecalho->lista = calloc( cabecalho->tamLista, sizeof(tNoListaCanHuff) );
   if(!cabecalho->lista)
      return NULL; /* Aloca��o falhou */

      /* L� a lista de c�digos can�nicos */
   fread(cabecalho->lista, sizeof(tNoListaCanHuff), cabecalho->tamLista, stream);

   if (ferror(stream))
      return NULL; /* Ocorreu erro de leitura */

   return cabecalho->lista;
}

/****
 *
 * ObtemByteHuff(): Obt�m o valor de um byte usando o c�digo associado a ele
 *
 * Par�metros:
 *     arvore (entrada) - �rvore de codifica��o
 *     codigo (entrada) - array de bytes contendo a codifica��o
 *                        de (pelo menos) um byte
 *     tamCodigo (entrada) - n�mero de bytes no array
 *    *indice (entrada e sa�da) - �ndice do primeiro bit a ser examinado
 *
 * Retorno: O valor do byte se for encontrada uma folha da �rvore
 *          de codifica��o durante uma descida nessa �rvore; em caso contr�rio, -1
 *
 ****/
static int ObtemByteHuff( tArvoreHuff arvore, tByte *codigo, int tamCodigo,
                          int *indice )
{
   static tNoArvoreHuff *pNoInicial = NULL; /* N� no qual a descida */
                                            /* na �rvore ser� iniciada */
   int                   byte; /* Byte que ser� retornado */

   if (!pNoInicial)
      pNoInicial = arvore;

      /* Desce a �rvore at� encontrar uma folha de */
      /* acordo com o valor de cada bit do c�digo  */
   for ( ; *indice < tamCodigo; ++*indice) {
      if (ConsultaBitEmArray(codigo, *indice))
            /* Bit = 1: desce pela direita */
         pNoInicial = pNoInicial->filhoDireita;
      else
            /* Bit = 0: desce pela esquerda */
         pNoInicial = pNoInicial->filhoEsquerda;

         /* Verifica se j� se chegou a uma folha */
      if (!pNoInicial->filhoEsquerda && !pNoInicial->filhoDireita){
         byte = pNoInicial->byteFreq.byte; /* Um byte foi decodificado */

         pNoInicial = NULL; /* A pr�xima descida na �rvore come�ar� na raiz */

         return byte;
      }
   }

      /* Se ainda n�o houve retorno, n�o se atingiu uma folha */
      /* e a busca ser� reiniciada no n� no qual ela parou    */
   return -1;
}

/********************* Fun��es Globais *********************/

/****
 *
 * CodificaHuff(): Codifica um arquivo seguindo o algoritmo de
 *                 codifica��o de Huffman
 *
 * Par�metros:
 *    *arqEntrada (entrada) - nome do arquivo que ser� codificado
 *    *arqSaida (entrada) - nome do arquivo que conter� a codifica��o
 *
 * Retorno: Nada
 *
 ****/
void CodificaHuff(const char *arqEntrada, const char *arqSaida)
{
   FILE          *streamE, /* Associado ao arquivo a ser codificado */
                 *streamS; /* Associado ao arquivo resultante */
   tHeapHuff     *pHeap; /* Apontar� para o heap que armazena     */
                         /* temporariamente os n�s da codifica��o */
   tNoArvoreHuff *no1, /* Ponteiro para o n� com a */
                       /* menor frequ�ncia no heap */
                 *no2, /* Ponteiro para o n� com a segunda */
                       /* menor frequ�ncia no heap         */
                 *noNovo; /* Ponteiro para um novo n� da �rvore */
                          /* que ter� no1 e no2 como filhos     */
   tNoListaCanHuff *lista; /* Lista que conter� os c�digos can�nicos */
   tCabecalhoHuff   cabecalho; /* Cabe�alho da codifica��o */
   int              i, j,
                    c, /* Armazenar� um byte lido */
                    bits, /* Empacotar� os bits */
                    nFolhas, /* N�mero de valores de bytes distintos no arquivo */
                    contaBits, /* Conta os bits de um byte */
                    tamArquivo; /* N�mero de bytes do arquivo */

   streamE = AbreArquivo(arqEntrada, "rb"); /* Abre o arquivo que ser� codificado */

      /* Cria o heap que armazena temporariamente os n�s da �rvore de codifica��o */
   pHeap = CriaHeapHuff(streamE, &tamArquivo);

      /* O n�mero de folhas da �rvore de codifica��o ser� igual ao tamanho */
      /* inicial do heap. Esse valor � igual ao n�mero de valores de bytes */
      /* distintos encontrados no arquivo.                                 */
   nFolhas = TamanhoHeapHuff(pHeap);

      /* Constr�i a �rvore de codifica��o */
   while (!HeapVazioHuff(pHeap)) {
         /* Remove o n� com a menor frequ�ncia do heap */
      no1 = RemoveMinHeapHuff(pHeap, ComparaNosHeapHuff);

         /* Se o heap ficou vazio, o �ltimo n� removido � */
         /* a raiz da �rvore que representa a codifica��o */
      if (HeapVazioHuff(pHeap))
         break; /* Constru��o de �rvore conclu�da */

         /* Remove o n� com a segunda menor frequ�ncia do heap  */
      no2 = RemoveMinHeapHuff(pHeap, ComparaNosHeapHuff);

      noNovo = ConstroiNoArvoreHuff(); /* Tenta alocar um novo n� */

         /* Armazena a frequ�ncia do novo n�. O conte�do do campo 'byte' n�o */
         /* tem import�ncia porque este n� n�o � uma folha.                  */
      noNovo->byteFreq.frequencia = no1->byteFreq.frequencia +
                                    no2->byteFreq.frequencia;

         /* O filho esquerdo do novo n� � o primeiro n� removido do heap */
         /* e o filho direito � o segundo n� removido do heap            */
      noNovo->filhoEsquerda = no1;
      noNovo->filhoDireita = no2;

      InsereEmHeapHuff(pHeap, noNovo, ComparaNosHeapHuff); /* Insere o n� no heap */
   }

         /*                                      */
         /* Neste ponto, a �rvore de codifica��o */
         /* j� foi conclu�da e sua raiz � no1    */
         /*                                      */

      /* Obt�m a lista de c�digos can�nicos */
   lista = CriaListaCanonicaHuff(no1, nFolhas);

      /* Nem o heap nem a �rvore s�o mais necess�rios */
   DestroiHeapHuff(pHeap);
   free(pHeap);
   DestroiArvoreHuff(no1);

   streamS = AbreArquivo(arqSaida, "wb"); /* Abre o arquivo de sa�da */

      /* Cria o cabe�alho da codifica��o */
   cabecalho.tamLista = nFolhas;
   cabecalho.lista = lista;

      /* Escreve o cabe�alho da codifica��o no arquivo de sa�da */
   EscreveCabecalhoHuff(&cabecalho, streamS);

   bits = 0; /* Zera o empacotador de bits */
   contaBits = 0; /* Inicia a contagem de bits */

      /* Faz o apontador de posi��o do arquivo apontar para o seu primeiro byte */
   rewind(streamE);

      /* L� cada byte no arquivo de entrada, codifica-o usando a lista */
      /* de c�digos e escreve a codifica��o no arquivo de sa�da        */
   while(1) {
      c = fgetc(streamE); /* L� um byte no arquivo de entrada */

         /* Verifica se ocorreu erro */
      ASSEGURA(!ferror(streamE), "Erro de leitura");

      if (feof(streamE))
         break; /* Acabou a leitura */

         /* Encontra o valor do byte lido na lista de c�digos. */
      for (i = 0; i < nFolhas; ++i)
         if (lista[i].valor == c)
            break;

         /* O valor do byte deve ter sido encontrado */
      ASSEGURA(i < nFolhas, "Valor de byte nao encontrado");

         /* Empacota os bits do c�digo deste byte */
      for(j = 0; j < lista[i].nBits; ++j) {
         contaBits++; /* Mais um bit neste c�digo */

            /* Armazena bit na vari�vel que empacota os bits */
         bits = (bits << 1) | ConsultaBitEmArray(lista[i].cod, j);

            /* Se j� houver um byte na vari�vel de          */
            /* empacotamento, escreve-o no arquivo de sa�da */
         if (contaBits == CHAR_BIT) {
            fputc(bits, streamS); /* Escreve byte no arquivo */

               /* Verifica se ocorreu erro de escrita */
            ASSEGURA(!ferror(streamS), "Erro de escrita");

            contaBits = 0; /* Reinicia a contagem de bits */
         }
      }
   }

      /* Escreve os bits que restaram no �ltimo empacotamento */
   if (contaBits) {
      bits <<= CHAR_BIT - contaBits; /* Alinha bits � esquerda */
      fputc(bits, streamS); /* Escreve-os no arquivo */
      ASSEGURA(!ferror(streamS), "Erro de escrita"); /* Testa */
   }

   free(lista); /* Libera o espa�o ocupado pela lista de c�digos */

      /* Fecha os arquivos */
   FechaArquivo(streamE, arqEntrada);
   FechaArquivo(streamS, arqSaida);
}

/****
 *
 * DecodificaHuff(): Decodifica um arquivo e escreve o resultado no
 *               arquivo especificado
 *
 * Par�metros:
 *     arqEntrada (entrada) - nome do arquivo a ser decodificado
 *     arqSaida (entrada) - nome do arquivo no qual o resultado
 *                          ser� escrito
 *
 * Retorno: Nada
 *
 ****/
void DecodificaHuff(const char *arqEntrada, const char *arqSaida)
{
   tNoListaCanHuff *listaCanonica; /* Lista de c�digos can�nicos */
   tByte            buffer[MAX_BYTES]; /* Buffer que armazenar� cada leitura */
                                       /* efetuada no arquivo de entrada     */
   int              i,
                    byte; /* Decodifica��o de um byte */
   FILE            *streamE, /* Stream associado ao arquivo a ser decodificado */
                   *streamS; /* Associado ao arquivo resultante */
   tCabecalhoHuff   cabecalho; /* Cabe�alho da codifica��o */
   tArvoreHuff      arvore; /* Raiz da �rvore de codifica��o */

   streamE = AbreArquivo(arqEntrada, "rb"); /* Tenta abrir o arquivo de entrada */

   streamS = AbreArquivo(arqSaida, "wb"); /* Tenta abrir o arquivo de sa�da */

      /* L� o cabe�alho no arquivo de entrada */
   listaCanonica = LeCabecalhoHuff(&cabecalho, streamE);

      /* Verifica se ocorreu erro na leitura do cabe�alho */
   if (!listaCanonica) {
         /* Fecha os arquivos e aborta o programa */
      FechaArquivo(streamE, arqEntrada);
      FechaArquivo(streamS, arqSaida);
      ASSEGURA(0, "Impossivel ler lista de codigos");
   }

      /* Constr�i a �rvore de codifica��o usando a lista de c�digos can�nicos */
   arvore = ReconstroiArvoreHuff(listaCanonica, cabecalho.tamLista);

      /* Decodifica o arquivo de entrada */
   while(1) {
         /* L� um array de bytes no arquivo codificado. O tamanho desse array */
         /* � igual ao n�mero m�ximo de bytes que pode constituir um c�digo   */
      fread(buffer, sizeof(buffer), 1, streamE);

         /* Verifica se houve erro de leitura */
      ASSEGURA(!ferror(streamE), "Erro de leitura");

         /* Verifica se a leitura acabou */
      if (feof(streamE))
         break;

         /* Decodifica os bytes lidos e armazenados no buffer */
         /* e escreve o resultado no arquivo de sa�da         */
      for (i = 0; i < MAX_BITS; ++i) {
            /* Obt�m um byte da decodifica��o do buffer */
         byte = ObtemByteHuff( arvore, buffer, MAX_BITS, &i );

            /* Se um byte foi obtido, escreve-o no arquivo de sa�da; caso */
            /* contr�rio, ele dever� ser completado na pr�xima leitura    */
         if (byte > 0)
            fputc(byte, streamS);
      }
   }

      /* Fecha arquivos */
   FechaArquivo(streamE, arqEntrada);
   FechaArquivo(streamS, arqSaida);

   free(listaCanonica); /* A lista de c�digos can�nicos n�o � mais necess�ria */
}
