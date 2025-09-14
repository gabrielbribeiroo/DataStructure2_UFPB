/****
 *
 * Arquivo DispersaoExt.c
 *
 * Autor: Ulysses de Oliveira
 *
 * Data de Cria��o: 04/08/2017
 * �ltima modifica��o: 06/09/2017
 *
 * Descri��o: Implementa��o de opera��es de busca, inser��o e
 *            remo��o usando dispers�o extens�vel em mem�ria
 *            secund�ria
 *
 ****/

/*********************** Includes *************************/

#include <limits.h>     /* Limites de tipos inteiros   */
#include <stdlib.h>     /* Miscel�nea de fun��es       */
#include <stdio.h>      /* Fun��es de E/S              */
#include <string.h>     /* Fun��es strxxx() e memxxx() */
#include <math.h>       /* Fun��es strxxx() e memxxx() */
#include "DispersaoExt.h" /* Interface deste m�dulo    */
#include "Registros.h"  /* Processamento de registros  */
#include "ASSEGURA.h"   /* Macro ASSEGURA              */

/********************* Fun��es Locais *********************/

/****
 *
 * ValorBit(): Retorna o valor de um bit de um valor inteiro
 *
 * Par�metros:
 *      valor (entrada) - valor que ter� seu bit retornado
 *      pos (entrada) - posi��o do bit que ser� retornado
 *
 * Retorno: O valor do bit
 *
 ****/
int ValorBit(int valor, int pos)
{
   return 1 & valor >> pos;
}

/****
 *
 * ObtemBitsDeChave(): Obt�m os bits menos significativos de uma chave
 *
 * Par�metros:
 *      chave (entrada) - a chave da qual os bits ser�o obtidos
 *      nBits (entrada) - o n�mero de bits que ser�o extra�dos da chave
 *
 * Retorno: O valor inteiro que representa esses bits
 *
 ****/
static int ObtemBitsDeChave(tChave chave, int nBits)
{
   int mascara = (1 << nBits) - 1;

   return chave & mascara;
}

/****
 *
 * NovoColetorDExt(): Cria um novo coletor num arquivo
 *
 * Par�metros:
 *     stream (entrada) - stream associado ao arquivo que cont�m os coletores
 *
 * Retorno: O �ndice do coletor criado
 *
 ****/
static int NovoColetorDExt(FILE *stream)
{
   tColetorDExt coletor;
   int          indice;

   coletor.nRegistros = 0;
   coletor.profLocal = 0;

      /* Move o apontador de posi��o do arquivo para seu final */
   MoveApontador(stream, 0, SEEK_END);

      /* Obt�m o �ndice do coletor */
   indice = ftell(stream)/sizeof(coletor);

      /* Escreve o coletor no arquivo */
   fwrite(&coletor, sizeof(coletor), 1, stream);

   return indice;
}

/****
 *
 * InsereEmDiretorioDExt(): Insere num diret�rio uma refer�ncia para
 *                      um coletor de numa tabela de busca com
 *                      dispers�o extens�vel
 *
 * Par�metros:
 *      tab (entrada/sa�da) - a tabela de busca
 *      pColetor (entrada) - endere�o do coletor que ser� inserido
 *      iColetor (entrada) - �ndice do coletor no arquivo de
 *                           coletores
 *
 * Retorno: Nada
 *
 ****/
static void InsereEmDiretorioDExt(tTabelaDExt *tab, const tColetorDExt *pColetor,
                                  int iColetor)
{
   int    i, bits, pLocal;
   tChave chave;

      /* Obt�m a chave do primeiro item do coletor */
   chave = ObtemChave(pColetor->registros);

   pLocal = pColetor->profLocal; /* Obt�m a profundidade local do coletor */

      /* Duplica o tamanho do diret�rio enquanto sua profundidade */
      /* global for menor do que a profundidade local do coletor  */
   while (tab->profGlobal < pLocal) {
         /* Duplica o tamanho do diret�rio */
      tab->dir = realloc(tab->dir, 2*tab->tamDir*sizeof(long));
      ASSEGURA(tab->dir, "Impossivel alocar novo diretorio");

         /* Copia as refer�ncias do antigo diret�rio */
         /* para a metade final do novo diret�rio    */
      memcpy(tab->dir + tab->tamDir, tab->dir, tab->tamDir*sizeof(int));

         /* Atualiza a profundidade global e o tamanho do diret�rio */
      ++tab->profGlobal;
      tab->tamDir = 2*tab->tamDir;
   }

   /***                                                                     ***/
   /* Se a profundidade local do novo coletor for menor do que a profundidade */
   /* global, ser� preciso atualizar mais de uma refer�ncia do diret�rio      */
   /***                                                                     ***/

      /* Obt�m os bits menos significativos coincidentes no coletor */
   bits = ObtemBitsDeChave(chave, pLocal);

      /* Corrige as refer�ncias do diret�rio para o novo coletor */
   for (i = 0; i < tab->tamDir; ++i)
      if (ObtemBitsDeChave(i, pLocal) == bits)
         tab->dir[i] = iColetor;
}

/****
 *
 * PodeDividirColetorDExt(): Verifica se um coletor pode ser dividido
 *
 * Par�metros:
 *     coletor (entrada) - o coletor que ser� examinado
 *
 * Retorno: 1, se o coletor pode ser dividido; 0, em caso contr�rio
 *
 ****/
static int PodeDividirColetorDExt(const tColetorDExt *coletor)
{
   tChave chave;
   int    i;

      /* Esta fun��o deve ser chamada apenas quando o coletor est� repleto */
   ASSEGURA( coletor->nRegistros == M,
             "PodeDividirColetorDExt() nao deveria ter sido chamada" );

   chave = ObtemChave(coletor->registros); /* Obt�m a chave do primeiro registro */

   for (i = 1; i < coletor->nRegistros; ++i)
      if ( chave != ObtemChave(coletor->registros + i) )
         return 1; /* Coletor pode ser dividido */

      /* As chaves deste coletor s�o todas iguais e, */
      /* portanto, ele n�o pode ser dividido         */
   return 0;
}

/****
 *
 * InsereColetorVazioDExt(): Insere num diret�rio uma refer�ncia para
 *                       um coletor vazio de numa tabela de busca
 *                       com dispers�o extens�vel
 *
 * Par�metros:
 *      tab (entrada/sa�da) - a tabela de busca
 *      pColetor (entrada) - endere�o do coletor que deu origem ao
 *                           coletor vazio
 *      iColetor (entrada) - �ndice do coletor que deu origem ao
 *                           coletor vazio no arquivo de coletores
 *      iVazio (entrada) - �ndice do coletor vazio no arquivo de
 *                         coletores
 *
 * Retorno: Nada
 *
 ****/
static void InsereColetorVazioDExt( tTabelaDExt *tab, const tColetorDExt *pColetor,
                                    int iColetor, int iVazio )
{
   int    i, bits, pLocal;
   tChave chave;

   pLocal = pColetor->profLocal; /* Obt�m a profundidade local do coletor */

      /* Duplica o tamanho do diret�rio enquanto sua profundidade global */
      /* for menor do que a profundidade local do coletor                */
   while (tab->profGlobal < pLocal) {
         /* Duplica o tamanho do diret�rio */
      tab->dir = realloc(tab->dir, 2*tab->tamDir*sizeof(int));
      ASSEGURA(tab->dir, "Impossivel alocar novo diretorio");

         /* Copia as refer�ncias do antigo diret�rio */
         /* para a metade final do novo diret�rio    */
      memcpy( tab->dir + tab->tamDir, tab->dir,
              tab->tamDir*sizeof(int) );

         /* Atualiza a profundidade global e o tamanho do diret�rio */
      ++tab->profGlobal;
      tab->tamDir = 2*tab->tamDir;
   }

      /* Obt�m a chave do primeiro item do coletor que deu origem ao coletor vazio */
   chave = ObtemChave(pColetor->registros);

      /* Obt�m os bits menos significativos coincidentes no */
      /* coletor que deu origem ao coletor vazio            */
   bits = ObtemBitsDeChave(chave, pLocal);

      /* Corrige as refer�ncias para o coletor vazio */
   for (i = 0; i < tab->tamDir; ++i)
      if (tab->dir[i] == iColetor && ObtemBitsDeChave(i, pLocal) != bits)
         tab->dir[i] = iVazio;
}

/****
 *
 * DivideColetorDExt(): Divide um coletor de uma tabela de busca
 *                     com dispers�o extens�vel
 *
 * Par�metros:
 *      tab (entrada/sa�da) - a tabela de busca que cont�m a
 *                            refer�ncia para o coletor que ser� dividido
 *      coletor (entrada/sa�da) - o coletor que ser� dividido
 *      iColetor (entrada) - �ndice do coletor que ser� dividido
 *                           no arquivo de coletores
 *      stream (entrada) - stream associado ao arquivo contendo os coletores
 *
 * Retorno: Nada
 *
 ****/
static void DivideColetorDExt( tTabelaDExt *tab, tColetorDExt *coletor,
                               int iColetor, FILE *stream )
{
   int          i, iNovo, chave, bits;
   tColetorDExt novo;

      /* Cria um novo coletor e obt�m seu �ndice em arquivo */
   iNovo = NovoColetorDExt(stream);

   LeColetorDExt(stream, iNovo, &novo); /* L� o novo coletor em arquivo */

      /* Divide registros do antigo coletor com o novo coletor */
   while (coletor->nRegistros == M) {
         /* Zera os registros do antigo e do novo coletor */
      coletor->nRegistros = 0;
      novo.nRegistros = 0;

         /* Distribui registros entre o antigo e o novo coletor */
      for (i = 0; i < M; i++) {
         /* Para cada item do antigo coletor, verifica-se  */
         /* se o bit de ordem profLocal + 1 de sua chave   */
         /* � 0 ou 1. Se ele for 0, o respectivo item fica */
         /* no antigo coletor. Caso contr�rio, esse item   */
         /* vai para o novo coletor.                       */

            /* Obt�m a chave do item corrente */
         chave = ObtemChave(coletor->registros + i);

            /* Obt�m profLocal + 1 bits menos significativos */
         bits = ObtemBitsDeChave(chave, coletor->profLocal + 1);

            /* Obt�m o valor do bit mais significativo    */
            /* dessa sequ�ncia de bits e testa esse valor */
         if (ValorBit(bits, coletor->profLocal) == 0)
               /* Este item fica no antigo coletor */
            coletor->registros[coletor->nRegistros++] = coletor->registros[i];
         else
               /* Este item vai para o novo coletor */
            novo.registros[novo.nRegistros++] = coletor->registros[i];
      }

         /* O novo e o antigo coletor ter�o a mesma profundidade local, */
         /* que � a profundidade local do antigo coletor mais um        */
      novo.profLocal = ++coletor->profLocal;

         /* Se n�o ficou nenhum item no coletor antigo, � preciso  */
         /* repetir o processo usando uma profundidade local maior */
      if (novo.nRegistros == M)
            /* Copia os registros do novo coleto de volta para o antigo coletor */
         *coletor = novo;
      else if (novo.nRegistros == 0) {
            /* Verifica se o antigo coletor pode ser dividido.  */
            /* Se ele n�o pode ser dividido, o novo registro    */
            /* deve ser inserido no novo coletor que dever� se  */
            /* tornar um coletor excedente. Como esta implemen- */
            /* ta��o n�o lida com esse caso, o programa ser�    */
            /* abortado se o coletor n�o puder ser dividido.    */
         ASSEGURA( PodeDividirColetorDExt(coletor),
                   "Erro: O coletor nao pode ser dividido\n" );

            /* Insere uma refer�ncia para o novo coletor vazio no diret�rio */
         InsereColetorVazioDExt(tab, coletor, iColetor, iNovo);

            /* Escreve o coletor vazio no arquivo */
         EscreveColetorDExt(stream, iNovo, &novo);

            /* Cria um novo coletor e e o l� em arquivo */
         iNovo = NovoColetorDExt(stream);
         LeColetorDExt(stream, iNovo, &novo);
      }
   }

      /* Escreve os dois coletores no arquivo */
   EscreveColetorDExt(stream, iColetor, coletor);
   EscreveColetorDExt(stream, iNovo, &novo);

      /* Insere uma refer�ncia para o novo coletor no diret�rio */
   InsereEmDiretorioDExt(tab, &novo, iNovo);
}

/********************* Fun��es Globais ********************/

/****
 *
 * LeColetorDExt(): L� um coletor em arquivo
 *
 * Par�metros:
 *     stream (entrada) - stream associado ao arquivo contendo
 *                        os coletores no qual ser� feita a leitura
 *     pos (entrada) - posi��o no arquivo na qual ser� feita a
 *                     leitura
 *    *coletor (sa�da) - ponteiro para o coletor que conter� o
 *                       resultado da leitura
 *
 * Retorno: Nada
 *
 ****/
void LeColetorDExt(FILE *stream, int pos, tColetorDExt *coletor)
{
      /* Verifica a validade do stream */
   ASSEGURA(stream, ERRO_STREAM_NULL(stream, LeColetorDExt));

      /* Tenta mover o apontador de arquivo para o local */
      /* de leitura; se n�o conseguir aborta o programa  */
   MoveApontador(stream, sizeof(tColetorDExt)*pos, SEEK_SET);

      /* Efetua a leitura */
   fread(coletor, sizeof(tColetorDExt), 1, stream);

      /* Certifica-se que n�o houve erro de leitura */
   ASSEGURA(!ferror(stream),  "Erro de leitura em LeColetorDExt()");
}

/****
 *
 * EscreveColetorDExt(): Escreve um coletor em arquivo
 *
 * Par�metros:
 *     stream (entrada) - stream associado ao arquivo contendo os
 *                        coletores no qual ser� feita a escrita
 *     pos (entrada) - posi��o no arquivo no qual ser� feita a
 *                     escrita
 *    *coletor (entrada) - coletor que ser� escrito no arquivo
 *
 * Retorno: Nada
 *
 ****/
void EscreveColetorDExt(FILE *stream, int pos, const tColetorDExt *coletor)
{
      /* Verifica se o stream � v�lido */
   ASSEGURA(stream, "Erro em EscreveColetorDExt(): stream e' NULL");

      /* Tenta mover o apontador de arquivo para o local */
      /* de escrita; se n�o conseguir aborta o programa  */
   MoveApontador(stream, sizeof(tColetorDExt)*pos, SEEK_SET);

      /* Efetua a escrita */
   fwrite(coletor, sizeof(tColetorDExt), 1, stream);

      /* Certifica-se que n�o houve erro */
   ASSEGURA( !ferror(stream), "Erro de escrita em EscreveColetorDExt()" );
}

/****
 *
 * IniciaTabelaDExt(): Inicia uma tabela de busca com dispers�o extens�vel
 *
 * Par�metros:
 *      tabela (sa�da) - a tabela que ser� iniciada
 *      stream (entrada) - stream associado ao arquivo que cont�m os coletores
 *
 * Retorno: Nada
 *
 ****/
void IniciaTabelaDExt(tTabelaDExt *tabela, FILE *stream)
{
      /*                                                 */
      /* A tabela � iniciada com um tamanho de diret�rio */
      /* igual a 1 que aponta para um coletor vazio      */
      /*                                                 */

    tabela->profGlobal = 0;
    tabela->nReg = 0;
    tabela->tamDir = 1;

    tabela->dir = malloc(tabela->tamDir*sizeof(int));

    tabela->dir[0] = NovoColetorDExt(stream);
}

/****
 *
 * BuscaDExt(): Efetua uma busca numa tabela de busca com dispers�o extens�vel
 *
 * Par�metros:
 *      stream (entrada) - stream associado ao arquivo que cont�m os coletores
 *      tab (entrada) - a tabela
 *      chave (entrada) - a chave de busca
 *
 * Retorno: O �ndice do registro no arquivo, se ele for encontrado;
 *          -1, em caso contr�rio
 *
 * Observa��o: Os registros s�o ordenados em cada coletor, de
 *             modo que busca bin�ria poderia ser utilizada
 *
 ****/
tRegistroMEC *BuscaDExt( FILE *stream, const tTabelaDExt *tab, tChave chave,
                         tRegistroMEC *reg )
{
   int      iColetor, i;
   tColetorDExt coletor;

      /* Obt�m �ndice do coletor que cont�m o �ndice do registro */
   iColetor = tab->dir[ObtemBitsDeChave(chave, tab->profGlobal)];

      /* L� o coletor que cont�m o �ndice do registro */
   LeColetorDExt(stream, iColetor, &coletor);

      /* Efetua uma busca sequencial pelo */
      /* registro em mem�ria principal    */
   for (i = 0; i < coletor.nRegistros; i++)
         /* Verifica se a chave foi encontrada */
      if (chave == ObtemChave(coletor.registros + i)) {
            /* Chave encontrada */
         *reg = coletor.registros[i]; /* Chave encontrada */
         return reg;
      }

   return NULL; /* A chave n�o foi encontrada */
}

/****
 *
 * InsereDExt(): Insere um item numa tabela de busca com dispers�o extens�vel
 *
 * Par�metros:
 *      tab (entrada/sa�da) - a tabela na qual ser� efetuada a inser��o
 *      item (entrada) - o item que ser� inserido
 *      stream (entrada) - stream associado ao arquivo que cont�m os coletores
 *
 * Retorno: Nada
 *
 * Observa��o: Para facilitar a implementa��o, o n�mero m�ximo
 *             de registros em qualquer coletor � M - 1, de modo
 *             que sempre h� espa�o para inser��o
 *
 ****/
void InsereDExt( tTabelaDExt *tab, const tRegistroMEC *item, FILE *stream )
{
   int          i, j, iColetor;
   tChave       chave;
   tColetorDExt coletor;

   chave = ObtemChave(item); /* Obt�m a chave do item */

      /* Obt�m o �ndice do coletor */
   iColetor = tab->dir[ObtemBitsDeChave(chave, tab->profGlobal)];


   LeColetorDExt(stream, iColetor, &coletor); /* L� o coletor em arquivo */

      /* O n�mero de registros num coletor deve ser menor do que M */
   ASSEGURA( coletor.nRegistros < M, "Excesso de registros em coletor" );

      /* Como os registros s�o ordenado, � precico encontrar */
      /* a posi��o de inser��o do novo item no coletor   */
   for (i = 0; i < coletor.nRegistros; i++)
      if ( chave < ObtemChave(coletor.registros + i) )
         break;

      /* Abre espa�o no coletor para o novo item */
   for (j = coletor.nRegistros; j > i; j--)
      coletor.registros[j] = coletor.registros[j - 1];

   coletor.registros[i] = *item; /* Insere o novo item no coletor */

      /* O n�mero de registros no coletor aumentou */
   coletor.nRegistros++;

   ++tab->nReg; /* O n�mero de registros na tabela tamb�m aumentou */

      /* Verifica se o coletor ficou repleto */
   if (coletor.nRegistros == M)
         /* O coletor ficou repleto e � preciso dividi-lo */
      DivideColetorDExt(tab, &coletor, iColetor, stream);
   else
         /* O coletor foi alterado e � necess�rio reescrev�-lo no arquivo */
      EscreveColetorDExt(stream, iColetor, &coletor);
}

/****
 *
 * RemoveDExt(): Remove um item de uma tabela de dispers�o extens�vel
 *
 * Par�metros:
 *      tab (entrada/sa�da) - a tabela de dispers�o
 *      chave (entrada) - a chave do item que ser� removido
 *      stream (entrada) - stream associado ao arquivo que cont�m os coletores
 *
 * Retorno: 1, se a remo��o foi ok; 0, caso contr�rio
 *
 ****/
int RemoveDExt(tTabelaDExt *tab, tChave chave, FILE *stream)
{
   int          i, j, iColetor;
   tColetorDExt coletor;

      /* Obt�m o �ndice do coletor */
   iColetor = tab->dir[ObtemBitsDeChave(chave, tab->profGlobal)];

   LeColetorDExt(stream, iColetor, &coletor); /* L� o coletor em arquivo */

      /* Efetua uma busca sequencial pelo item em mem�ria principal */
   for (i = 0; i < coletor.nRegistros; i++)
         /* Verifica se a chave foi encontrada */
      if (chave == ObtemChave(coletor.registros + i))
         break;

      /* Verifica se a chave foi encontrada */
   if (i >= coletor.nRegistros)
      return 0;  /* A chave n�o foi encontrada */

      /* O item a ser removido se encontra na posi��o i. � necess�rio mover uma */
      /* posi��o para tr�s todos os registros que se encontram � sua frente.    */
   for (j = i; j < coletor.nRegistros - 1; ++j)
      coletor.registros[j] = coletor.registros[j + 1];

   --coletor.nRegistros; /* O n�mero de registros no coletor diminuiu */

   --tab->nReg; /* O n�mero de registros na tabela tamb�m diminuiu */

      /* Verifica se o coletor ficou vazio */
   if (coletor.nRegistros == 0)
         /* O coletor ficou vazio. Talvez o tamanho do        */
         /* diret�rio possa ser reduzido, mas � poss�vel que, */
         /* mesmo assim, essa opera��o n�o valha a pena.      */
         /* Substituir a instru��o vazia a seguir pela        */
         /* implementa��o de uma opera��o que reduza o        */
         /* tamanho do diret�rio � deixada como exerc�cio.    */
      ;

      /* O coletor foi alterado e � preciso reescrev�-lo no arquivo */
   EscreveColetorDExt(stream, iColetor, &coletor);

   return 1;  /* Remo��o bem-sucedida */
}

/****
 *
 * NRegistrosDExt(): Calcula o n�mero de registros numa tabela
 *                     de dispers�o extens�vel
 *
 * Par�metros:
 *      tabela (entrada) - a tabela de dispers�o
 *      stream (entrada) - stream associado ao arquivo que cont�m os coletores
 *
 * Retorno: O n�mero de registros na tabela de dispers�o
 *
 ****/
int NRegistrosDExt(const tTabelaDExt *tabela, FILE *stream)
{
   int          nRegistros = 0;
   tColetorDExt umColetor;

   rewind(stream); /* Assegura que a leitura come�a no in�cio do arquivo */

   while (1) {
      fread(&umColetor, sizeof(umColetor), 1, stream); /* L� um coletor */

         /* Se ocorreu erro de leitura ou o final  */
         /* do arquivo foi atingido encerra o la�o */
      if (feof(stream) || ferror(stream))
         break;

      nRegistros += umColetor.nRegistros;
   }

      /* Verifica se ocorreu erro de leitura */
   ASSEGURA( !ferror(stream), "Erro de leitura em NRegistrosDExt" );

      /* Checa a consist�ncia */
   ASSEGURA( nRegistros == tabela->nReg, "Erro de consistencia" );

   return nRegistros;
}

/****
 *
 * NColetoresDExt(): Calcula o n�mero de coletores numa tabela
 *                   de dispers�o extens�vel
 *
 * Par�metros:
 *      stream (entrada) - stream associado ao arquivo que cont�m os coletores
 *
 * Retorno: O n�mero de coletores na tabela de dispers�o
 *
 ****/
int NColetoresDExt(FILE *stream)
{
   int          n = 0;
   tColetorDExt umColetor;

   rewind(stream); /* Assegura que a leitura come�a no in�cio do arquivo */

   while (1) {
         /* L� um coletor */
      fread(&umColetor, sizeof(umColetor), 1, stream);

         /* Se ocorreu erro de leitura ou o final  */
         /* do arquivo foi atingido encerra o la�o */
      if (feof(stream) || ferror(stream))
         break;

      ++n;
   }

      /* Verifica se ocorreu erro de leitura */
   ASSEGURA( !ferror(stream), "Erro de leitura em NColetoresDExt" );

   return n;
}

/****
 *
 * NColetoresVaziosDExt(): Calcula o n�mero de coletores vazios
 *                         numa tabela de dispers�o extens�vel
 *
 * Par�metros:
 *      stream (entrada) - stream associado ao arquivo que cont�m os coletores
 *
 * Retorno: O n�mero de coletores vazios
 *
 ****/
int NColetoresVaziosDExt(FILE *stream)
{
   int      n = 0;
   tColetorDExt umColetor;

   rewind(stream); /* Assegura que a leitura come�a no in�cio do arquivo */

   while (1) {
         /* L� um coletor */
      fread(&umColetor, sizeof(umColetor), 1, stream);

         /* Se ocorreu erro de leitura ou o final  */
         /* do arquivo foi atingido encerra o la�o */
      if (feof(stream) || ferror(stream))
         break;

      if (!umColetor.nRegistros)
         ++n;
   }

      /* Verifica se ocorreu erro de leitura */
   ASSEGURA( !ferror(stream), "Erro de leitura em NColetoresVaziosDExt" );

   return n;
}

/****
 *
 * NColetoresRepletosDExt(): Calcula o n�mero de coletores repletos
 *                           numa tabela de dispers�o extens�vel
 *
 * Par�metros:
 *      stream (entrada) - stream associado ao arquivo que cont�m os coletores
 *
 * Retorno: O n�mero de coletores repletos
 *
 ****/
int NColetoresRepletosDExt(FILE *stream)
{
   int      n = 0;
   tColetorDExt umColetor;

   rewind(stream); /* Assegura que a leitura come�a no in�cio do arquivo */

   while (1) {
      fread(&umColetor, sizeof(umColetor), 1, stream); /* L� um coletor */

         /* Se ocorreu erro de leitura ou o final  */
         /* do arquivo foi atingido encerra o la�o */
      if (feof(stream) || ferror(stream))
         break;

      if (umColetor.nRegistros == M)
         ++n;
   }

      /* Verifica se ocorreu erro de leitura */
   ASSEGURA( !ferror(stream), "Erro de leitura em NColetoresRepletosDExt" );

   return n;
}

/****
 *
 * NColetoresOcupadosDExt(): Calcula o n�mero de coletores ocupados
 *                           numa tabela de dispers�o extens�vel
 *
 * Par�metros:
 *      stream (entrada) - stream associado ao arquivo que cont�m os coletores
 *
 * Retorno: O n�mero de coletores ocupados
 *
 ****/
int NColetoresOcupadosDExt(FILE *stream)
{
   int      n = 0;
   tColetorDExt umColetor;

   rewind(stream); /* Assegura que a leitura come�a no in�cio do arquivo */

   while (1) {
         /* L� um coletor */
      fread(&umColetor, sizeof(umColetor), 1, stream);

         /* Se ocorreu erro de leitura ou o final  */
         /* do arquivo foi atingido encerra o la�o */
      if (feof(stream) || ferror(stream))
         break;

      if (umColetor.nRegistros)
         ++n;
   }

      /* Verifica se ocorreu erro de leitura */
   ASSEGURA( !ferror(stream), "Erro de leitura em NColetoresOcupadosDExt" );

   return n;
}

/****
 *
 * ProfLocalMinDExt(): Calcula a menor profundidade local
 *
 * Par�metros:
 *      stream (entrada) - stream associado ao arquivo que cont�m os coletores
 *
 * Retorno: A profundidade local m�nima
 *
 ****/
int ProfLocalMinDExt(FILE *stream)
{
   int      min;
   tColetorDExt umColetor;

   rewind(stream); /* Assegura que a leitura come�a no in�cio do arquivo */

   fread(&umColetor, sizeof(umColetor), 1, stream); /* L� um coletor */

      /* Verifica se ocorreu erro de leitura */
   ASSEGURA( !ferror(stream),
             "Erro de leitura em ProfLocalMinDExt" );

   min = umColetor.profLocal;

   while (1) {
      fread(&umColetor, sizeof(umColetor), 1, stream); /* L� um coletor */

         /* Se ocorreu erro de leitura ou o final  */
         /* do arquivo foi atingido encerra o la�o */
      if (feof(stream) || ferror(stream))
         break;

      if (umColetor.profLocal < min)
         min = umColetor.profLocal;
   }

      /* Verifica se ocorreu erro de leitura */
   ASSEGURA( !ferror(stream), "Erro de leitura em ProfLocalMinDExt" );

   return min;
}

/****
 *
 * ProfLocalMaxDExt(): Calcula a maior profundidade local
 *
 * Par�metros:
 *      stream (entrada) - stream associado ao arquivo que cont�m os coletores
 *
 * Retorno: A profundidade local m�xima
 *
 ****/
int ProfLocalMaxDExt(FILE *stream)
{
   int          max;
   tColetorDExt umColetor;

   rewind(stream); /* Assegura que a leitura come�a no in�cio do arquivo */

      /* L� um coletor */
   fread(&umColetor, sizeof(umColetor), 1, stream);

      /* Verifica se ocorreu erro de leitura */
   ASSEGURA( !ferror(stream), "Erro de leitura em ProfLocalMaxDExt" );

   max = umColetor.profLocal;

   while (1) {
      fread(&umColetor, sizeof(umColetor), 1, stream); /* L� um coletor */

         /* Se ocorreu erro de leitura ou o final  */
         /* do arquivo foi atingido encerra o la�o */
      if (feof(stream) || ferror(stream))
         break;

      if (umColetor.profLocal > max)
         max = umColetor.profLocal;
   }

      /* Verifica se ocorreu erro de leitura */
   ASSEGURA( !ferror(stream), "Erro de leitura em ProfLocalMaxDExt" );

   return max;
}

