/****
 *
 * Arquivo DispersaoExt.c
 *
 * Autor: Ulysses de Oliveira
 *
 * Data de Criação: 04/08/2017
 * Última modificação: 06/09/2017
 *
 * Descrição: Implementação de operações de busca, inserção e
 *            remoção usando dispersão extensível em memória
 *            secundária
 *
 ****/

/*********************** Includes *************************/

#include <limits.h>     /* Limites de tipos inteiros   */
#include <stdlib.h>     /* Miscelânea de funções       */
#include <stdio.h>      /* Funções de E/S              */
#include <string.h>     /* Funções strxxx() e memxxx() */
#include <math.h>       /* Funções strxxx() e memxxx() */
#include "DispersaoExt.h" /* Interface deste módulo    */
#include "Registros.h"  /* Processamento de registros  */
#include "ASSEGURA.h"   /* Macro ASSEGURA              */

/********************* Funções Locais *********************/

/****
 *
 * ValorBit(): Retorna o valor de um bit de um valor inteiro
 *
 * Parâmetros:
 *      valor (entrada) - valor que terá seu bit retornado
 *      pos (entrada) - posição do bit que será retornado
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
 * ObtemBitsDeChave(): Obtém os bits menos significativos de uma chave
 *
 * Parâmetros:
 *      chave (entrada) - a chave da qual os bits serão obtidos
 *      nBits (entrada) - o número de bits que serão extraídos da chave
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
 * Parâmetros:
 *     stream (entrada) - stream associado ao arquivo que contém os coletores
 *
 * Retorno: O índice do coletor criado
 *
 ****/
static int NovoColetorDExt(FILE *stream)
{
   tColetorDExt coletor;
   int          indice;

   coletor.nRegistros = 0;
   coletor.profLocal = 0;

      /* Move o apontador de posição do arquivo para seu final */
   MoveApontador(stream, 0, SEEK_END);

      /* Obtém o índice do coletor */
   indice = ftell(stream)/sizeof(coletor);

      /* Escreve o coletor no arquivo */
   fwrite(&coletor, sizeof(coletor), 1, stream);

   return indice;
}

/****
 *
 * InsereEmDiretorioDExt(): Insere num diretório uma referência para
 *                      um coletor de numa tabela de busca com
 *                      dispersão extensível
 *
 * Parâmetros:
 *      tab (entrada/saída) - a tabela de busca
 *      pColetor (entrada) - endereço do coletor que será inserido
 *      iColetor (entrada) - índice do coletor no arquivo de
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

      /* Obtém a chave do primeiro item do coletor */
   chave = ObtemChave(pColetor->registros);

   pLocal = pColetor->profLocal; /* Obtém a profundidade local do coletor */

      /* Duplica o tamanho do diretório enquanto sua profundidade */
      /* global for menor do que a profundidade local do coletor  */
   while (tab->profGlobal < pLocal) {
         /* Duplica o tamanho do diretório */
      tab->dir = realloc(tab->dir, 2*tab->tamDir*sizeof(long));
      ASSEGURA(tab->dir, "Impossivel alocar novo diretorio");

         /* Copia as referências do antigo diretório */
         /* para a metade final do novo diretório    */
      memcpy(tab->dir + tab->tamDir, tab->dir, tab->tamDir*sizeof(int));

         /* Atualiza a profundidade global e o tamanho do diretório */
      ++tab->profGlobal;
      tab->tamDir = 2*tab->tamDir;
   }

   /***                                                                     ***/
   /* Se a profundidade local do novo coletor for menor do que a profundidade */
   /* global, será preciso atualizar mais de uma referência do diretório      */
   /***                                                                     ***/

      /* Obtém os bits menos significativos coincidentes no coletor */
   bits = ObtemBitsDeChave(chave, pLocal);

      /* Corrige as referências do diretório para o novo coletor */
   for (i = 0; i < tab->tamDir; ++i)
      if (ObtemBitsDeChave(i, pLocal) == bits)
         tab->dir[i] = iColetor;
}

/****
 *
 * PodeDividirColetorDExt(): Verifica se um coletor pode ser dividido
 *
 * Parâmetros:
 *     coletor (entrada) - o coletor que será examinado
 *
 * Retorno: 1, se o coletor pode ser dividido; 0, em caso contrário
 *
 ****/
static int PodeDividirColetorDExt(const tColetorDExt *coletor)
{
   tChave chave;
   int    i;

      /* Esta função deve ser chamada apenas quando o coletor está repleto */
   ASSEGURA( coletor->nRegistros == M,
             "PodeDividirColetorDExt() nao deveria ter sido chamada" );

   chave = ObtemChave(coletor->registros); /* Obtém a chave do primeiro registro */

   for (i = 1; i < coletor->nRegistros; ++i)
      if ( chave != ObtemChave(coletor->registros + i) )
         return 1; /* Coletor pode ser dividido */

      /* As chaves deste coletor são todas iguais e, */
      /* portanto, ele não pode ser dividido         */
   return 0;
}

/****
 *
 * InsereColetorVazioDExt(): Insere num diretório uma referência para
 *                       um coletor vazio de numa tabela de busca
 *                       com dispersão extensível
 *
 * Parâmetros:
 *      tab (entrada/saída) - a tabela de busca
 *      pColetor (entrada) - endereço do coletor que deu origem ao
 *                           coletor vazio
 *      iColetor (entrada) - índice do coletor que deu origem ao
 *                           coletor vazio no arquivo de coletores
 *      iVazio (entrada) - índice do coletor vazio no arquivo de
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

   pLocal = pColetor->profLocal; /* Obtém a profundidade local do coletor */

      /* Duplica o tamanho do diretório enquanto sua profundidade global */
      /* for menor do que a profundidade local do coletor                */
   while (tab->profGlobal < pLocal) {
         /* Duplica o tamanho do diretório */
      tab->dir = realloc(tab->dir, 2*tab->tamDir*sizeof(int));
      ASSEGURA(tab->dir, "Impossivel alocar novo diretorio");

         /* Copia as referências do antigo diretório */
         /* para a metade final do novo diretório    */
      memcpy( tab->dir + tab->tamDir, tab->dir,
              tab->tamDir*sizeof(int) );

         /* Atualiza a profundidade global e o tamanho do diretório */
      ++tab->profGlobal;
      tab->tamDir = 2*tab->tamDir;
   }

      /* Obtém a chave do primeiro item do coletor que deu origem ao coletor vazio */
   chave = ObtemChave(pColetor->registros);

      /* Obtém os bits menos significativos coincidentes no */
      /* coletor que deu origem ao coletor vazio            */
   bits = ObtemBitsDeChave(chave, pLocal);

      /* Corrige as referências para o coletor vazio */
   for (i = 0; i < tab->tamDir; ++i)
      if (tab->dir[i] == iColetor && ObtemBitsDeChave(i, pLocal) != bits)
         tab->dir[i] = iVazio;
}

/****
 *
 * DivideColetorDExt(): Divide um coletor de uma tabela de busca
 *                     com dispersão extensível
 *
 * Parâmetros:
 *      tab (entrada/saída) - a tabela de busca que contém a
 *                            referência para o coletor que será dividido
 *      coletor (entrada/saída) - o coletor que será dividido
 *      iColetor (entrada) - índice do coletor que será dividido
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

      /* Cria um novo coletor e obtém seu índice em arquivo */
   iNovo = NovoColetorDExt(stream);

   LeColetorDExt(stream, iNovo, &novo); /* Lê o novo coletor em arquivo */

      /* Divide registros do antigo coletor com o novo coletor */
   while (coletor->nRegistros == M) {
         /* Zera os registros do antigo e do novo coletor */
      coletor->nRegistros = 0;
      novo.nRegistros = 0;

         /* Distribui registros entre o antigo e o novo coletor */
      for (i = 0; i < M; i++) {
         /* Para cada item do antigo coletor, verifica-se  */
         /* se o bit de ordem profLocal + 1 de sua chave   */
         /* é 0 ou 1. Se ele for 0, o respectivo item fica */
         /* no antigo coletor. Caso contrário, esse item   */
         /* vai para o novo coletor.                       */

            /* Obtém a chave do item corrente */
         chave = ObtemChave(coletor->registros + i);

            /* Obtém profLocal + 1 bits menos significativos */
         bits = ObtemBitsDeChave(chave, coletor->profLocal + 1);

            /* Obtém o valor do bit mais significativo    */
            /* dessa sequência de bits e testa esse valor */
         if (ValorBit(bits, coletor->profLocal) == 0)
               /* Este item fica no antigo coletor */
            coletor->registros[coletor->nRegistros++] = coletor->registros[i];
         else
               /* Este item vai para o novo coletor */
            novo.registros[novo.nRegistros++] = coletor->registros[i];
      }

         /* O novo e o antigo coletor terão a mesma profundidade local, */
         /* que é a profundidade local do antigo coletor mais um        */
      novo.profLocal = ++coletor->profLocal;

         /* Se não ficou nenhum item no coletor antigo, é preciso  */
         /* repetir o processo usando uma profundidade local maior */
      if (novo.nRegistros == M)
            /* Copia os registros do novo coleto de volta para o antigo coletor */
         *coletor = novo;
      else if (novo.nRegistros == 0) {
            /* Verifica se o antigo coletor pode ser dividido.  */
            /* Se ele não pode ser dividido, o novo registro    */
            /* deve ser inserido no novo coletor que deverá se  */
            /* tornar um coletor excedente. Como esta implemen- */
            /* tação não lida com esse caso, o programa será    */
            /* abortado se o coletor não puder ser dividido.    */
         ASSEGURA( PodeDividirColetorDExt(coletor),
                   "Erro: O coletor nao pode ser dividido\n" );

            /* Insere uma referência para o novo coletor vazio no diretório */
         InsereColetorVazioDExt(tab, coletor, iColetor, iNovo);

            /* Escreve o coletor vazio no arquivo */
         EscreveColetorDExt(stream, iNovo, &novo);

            /* Cria um novo coletor e e o lê em arquivo */
         iNovo = NovoColetorDExt(stream);
         LeColetorDExt(stream, iNovo, &novo);
      }
   }

      /* Escreve os dois coletores no arquivo */
   EscreveColetorDExt(stream, iColetor, coletor);
   EscreveColetorDExt(stream, iNovo, &novo);

      /* Insere uma referência para o novo coletor no diretório */
   InsereEmDiretorioDExt(tab, &novo, iNovo);
}

/********************* Funções Globais ********************/

/****
 *
 * LeColetorDExt(): Lê um coletor em arquivo
 *
 * Parâmetros:
 *     stream (entrada) - stream associado ao arquivo contendo
 *                        os coletores no qual será feita a leitura
 *     pos (entrada) - posição no arquivo na qual será feita a
 *                     leitura
 *    *coletor (saída) - ponteiro para o coletor que conterá o
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
      /* de leitura; se não conseguir aborta o programa  */
   MoveApontador(stream, sizeof(tColetorDExt)*pos, SEEK_SET);

      /* Efetua a leitura */
   fread(coletor, sizeof(tColetorDExt), 1, stream);

      /* Certifica-se que não houve erro de leitura */
   ASSEGURA(!ferror(stream),  "Erro de leitura em LeColetorDExt()");
}

/****
 *
 * EscreveColetorDExt(): Escreve um coletor em arquivo
 *
 * Parâmetros:
 *     stream (entrada) - stream associado ao arquivo contendo os
 *                        coletores no qual será feita a escrita
 *     pos (entrada) - posição no arquivo no qual será feita a
 *                     escrita
 *    *coletor (entrada) - coletor que será escrito no arquivo
 *
 * Retorno: Nada
 *
 ****/
void EscreveColetorDExt(FILE *stream, int pos, const tColetorDExt *coletor)
{
      /* Verifica se o stream é válido */
   ASSEGURA(stream, "Erro em EscreveColetorDExt(): stream e' NULL");

      /* Tenta mover o apontador de arquivo para o local */
      /* de escrita; se não conseguir aborta o programa  */
   MoveApontador(stream, sizeof(tColetorDExt)*pos, SEEK_SET);

      /* Efetua a escrita */
   fwrite(coletor, sizeof(tColetorDExt), 1, stream);

      /* Certifica-se que não houve erro */
   ASSEGURA( !ferror(stream), "Erro de escrita em EscreveColetorDExt()" );
}

/****
 *
 * IniciaTabelaDExt(): Inicia uma tabela de busca com dispersão extensível
 *
 * Parâmetros:
 *      tabela (saída) - a tabela que será iniciada
 *      stream (entrada) - stream associado ao arquivo que contém os coletores
 *
 * Retorno: Nada
 *
 ****/
void IniciaTabelaDExt(tTabelaDExt *tabela, FILE *stream)
{
      /*                                                 */
      /* A tabela é iniciada com um tamanho de diretório */
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
 * BuscaDExt(): Efetua uma busca numa tabela de busca com dispersão extensível
 *
 * Parâmetros:
 *      stream (entrada) - stream associado ao arquivo que contém os coletores
 *      tab (entrada) - a tabela
 *      chave (entrada) - a chave de busca
 *
 * Retorno: O índice do registro no arquivo, se ele for encontrado;
 *          -1, em caso contrário
 *
 * Observação: Os registros são ordenados em cada coletor, de
 *             modo que busca binária poderia ser utilizada
 *
 ****/
tRegistroMEC *BuscaDExt( FILE *stream, const tTabelaDExt *tab, tChave chave,
                         tRegistroMEC *reg )
{
   int      iColetor, i;
   tColetorDExt coletor;

      /* Obtém índice do coletor que contém o índice do registro */
   iColetor = tab->dir[ObtemBitsDeChave(chave, tab->profGlobal)];

      /* Lê o coletor que contém o índice do registro */
   LeColetorDExt(stream, iColetor, &coletor);

      /* Efetua uma busca sequencial pelo */
      /* registro em memória principal    */
   for (i = 0; i < coletor.nRegistros; i++)
         /* Verifica se a chave foi encontrada */
      if (chave == ObtemChave(coletor.registros + i)) {
            /* Chave encontrada */
         *reg = coletor.registros[i]; /* Chave encontrada */
         return reg;
      }

   return NULL; /* A chave não foi encontrada */
}

/****
 *
 * InsereDExt(): Insere um item numa tabela de busca com dispersão extensível
 *
 * Parâmetros:
 *      tab (entrada/saída) - a tabela na qual será efetuada a inserção
 *      item (entrada) - o item que será inserido
 *      stream (entrada) - stream associado ao arquivo que contém os coletores
 *
 * Retorno: Nada
 *
 * Observação: Para facilitar a implementação, o número máximo
 *             de registros em qualquer coletor é M - 1, de modo
 *             que sempre há espaço para inserção
 *
 ****/
void InsereDExt( tTabelaDExt *tab, const tRegistroMEC *item, FILE *stream )
{
   int          i, j, iColetor;
   tChave       chave;
   tColetorDExt coletor;

   chave = ObtemChave(item); /* Obtém a chave do item */

      /* Obtém o índice do coletor */
   iColetor = tab->dir[ObtemBitsDeChave(chave, tab->profGlobal)];


   LeColetorDExt(stream, iColetor, &coletor); /* Lê o coletor em arquivo */

      /* O número de registros num coletor deve ser menor do que M */
   ASSEGURA( coletor.nRegistros < M, "Excesso de registros em coletor" );

      /* Como os registros são ordenado, é precico encontrar */
      /* a posição de inserção do novo item no coletor   */
   for (i = 0; i < coletor.nRegistros; i++)
      if ( chave < ObtemChave(coletor.registros + i) )
         break;

      /* Abre espaço no coletor para o novo item */
   for (j = coletor.nRegistros; j > i; j--)
      coletor.registros[j] = coletor.registros[j - 1];

   coletor.registros[i] = *item; /* Insere o novo item no coletor */

      /* O número de registros no coletor aumentou */
   coletor.nRegistros++;

   ++tab->nReg; /* O número de registros na tabela também aumentou */

      /* Verifica se o coletor ficou repleto */
   if (coletor.nRegistros == M)
         /* O coletor ficou repleto e é preciso dividi-lo */
      DivideColetorDExt(tab, &coletor, iColetor, stream);
   else
         /* O coletor foi alterado e é necessário reescrevê-lo no arquivo */
      EscreveColetorDExt(stream, iColetor, &coletor);
}

/****
 *
 * RemoveDExt(): Remove um item de uma tabela de dispersão extensível
 *
 * Parâmetros:
 *      tab (entrada/saída) - a tabela de dispersão
 *      chave (entrada) - a chave do item que será removido
 *      stream (entrada) - stream associado ao arquivo que contém os coletores
 *
 * Retorno: 1, se a remoção foi ok; 0, caso contrário
 *
 ****/
int RemoveDExt(tTabelaDExt *tab, tChave chave, FILE *stream)
{
   int          i, j, iColetor;
   tColetorDExt coletor;

      /* Obtém o índice do coletor */
   iColetor = tab->dir[ObtemBitsDeChave(chave, tab->profGlobal)];

   LeColetorDExt(stream, iColetor, &coletor); /* Lê o coletor em arquivo */

      /* Efetua uma busca sequencial pelo item em memória principal */
   for (i = 0; i < coletor.nRegistros; i++)
         /* Verifica se a chave foi encontrada */
      if (chave == ObtemChave(coletor.registros + i))
         break;

      /* Verifica se a chave foi encontrada */
   if (i >= coletor.nRegistros)
      return 0;  /* A chave não foi encontrada */

      /* O item a ser removido se encontra na posição i. É necessário mover uma */
      /* posição para trás todos os registros que se encontram à sua frente.    */
   for (j = i; j < coletor.nRegistros - 1; ++j)
      coletor.registros[j] = coletor.registros[j + 1];

   --coletor.nRegistros; /* O número de registros no coletor diminuiu */

   --tab->nReg; /* O número de registros na tabela também diminuiu */

      /* Verifica se o coletor ficou vazio */
   if (coletor.nRegistros == 0)
         /* O coletor ficou vazio. Talvez o tamanho do        */
         /* diretório possa ser reduzido, mas é possível que, */
         /* mesmo assim, essa operação não valha a pena.      */
         /* Substituir a instrução vazia a seguir pela        */
         /* implementação de uma operação que reduza o        */
         /* tamanho do diretório é deixada como exercício.    */
      ;

      /* O coletor foi alterado e é preciso reescrevê-lo no arquivo */
   EscreveColetorDExt(stream, iColetor, &coletor);

   return 1;  /* Remoção bem-sucedida */
}

/****
 *
 * NRegistrosDExt(): Calcula o número de registros numa tabela
 *                     de dispersão extensível
 *
 * Parâmetros:
 *      tabela (entrada) - a tabela de dispersão
 *      stream (entrada) - stream associado ao arquivo que contém os coletores
 *
 * Retorno: O número de registros na tabela de dispersão
 *
 ****/
int NRegistrosDExt(const tTabelaDExt *tabela, FILE *stream)
{
   int          nRegistros = 0;
   tColetorDExt umColetor;

   rewind(stream); /* Assegura que a leitura começa no início do arquivo */

   while (1) {
      fread(&umColetor, sizeof(umColetor), 1, stream); /* Lê um coletor */

         /* Se ocorreu erro de leitura ou o final  */
         /* do arquivo foi atingido encerra o laço */
      if (feof(stream) || ferror(stream))
         break;

      nRegistros += umColetor.nRegistros;
   }

      /* Verifica se ocorreu erro de leitura */
   ASSEGURA( !ferror(stream), "Erro de leitura em NRegistrosDExt" );

      /* Checa a consistência */
   ASSEGURA( nRegistros == tabela->nReg, "Erro de consistencia" );

   return nRegistros;
}

/****
 *
 * NColetoresDExt(): Calcula o número de coletores numa tabela
 *                   de dispersão extensível
 *
 * Parâmetros:
 *      stream (entrada) - stream associado ao arquivo que contém os coletores
 *
 * Retorno: O número de coletores na tabela de dispersão
 *
 ****/
int NColetoresDExt(FILE *stream)
{
   int          n = 0;
   tColetorDExt umColetor;

   rewind(stream); /* Assegura que a leitura começa no início do arquivo */

   while (1) {
         /* Lê um coletor */
      fread(&umColetor, sizeof(umColetor), 1, stream);

         /* Se ocorreu erro de leitura ou o final  */
         /* do arquivo foi atingido encerra o laço */
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
 * NColetoresVaziosDExt(): Calcula o número de coletores vazios
 *                         numa tabela de dispersão extensível
 *
 * Parâmetros:
 *      stream (entrada) - stream associado ao arquivo que contém os coletores
 *
 * Retorno: O número de coletores vazios
 *
 ****/
int NColetoresVaziosDExt(FILE *stream)
{
   int      n = 0;
   tColetorDExt umColetor;

   rewind(stream); /* Assegura que a leitura começa no início do arquivo */

   while (1) {
         /* Lê um coletor */
      fread(&umColetor, sizeof(umColetor), 1, stream);

         /* Se ocorreu erro de leitura ou o final  */
         /* do arquivo foi atingido encerra o laço */
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
 * NColetoresRepletosDExt(): Calcula o número de coletores repletos
 *                           numa tabela de dispersão extensível
 *
 * Parâmetros:
 *      stream (entrada) - stream associado ao arquivo que contém os coletores
 *
 * Retorno: O número de coletores repletos
 *
 ****/
int NColetoresRepletosDExt(FILE *stream)
{
   int      n = 0;
   tColetorDExt umColetor;

   rewind(stream); /* Assegura que a leitura começa no início do arquivo */

   while (1) {
      fread(&umColetor, sizeof(umColetor), 1, stream); /* Lê um coletor */

         /* Se ocorreu erro de leitura ou o final  */
         /* do arquivo foi atingido encerra o laço */
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
 * NColetoresOcupadosDExt(): Calcula o número de coletores ocupados
 *                           numa tabela de dispersão extensível
 *
 * Parâmetros:
 *      stream (entrada) - stream associado ao arquivo que contém os coletores
 *
 * Retorno: O número de coletores ocupados
 *
 ****/
int NColetoresOcupadosDExt(FILE *stream)
{
   int      n = 0;
   tColetorDExt umColetor;

   rewind(stream); /* Assegura que a leitura começa no início do arquivo */

   while (1) {
         /* Lê um coletor */
      fread(&umColetor, sizeof(umColetor), 1, stream);

         /* Se ocorreu erro de leitura ou o final  */
         /* do arquivo foi atingido encerra o laço */
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
 * Parâmetros:
 *      stream (entrada) - stream associado ao arquivo que contém os coletores
 *
 * Retorno: A profundidade local mínima
 *
 ****/
int ProfLocalMinDExt(FILE *stream)
{
   int      min;
   tColetorDExt umColetor;

   rewind(stream); /* Assegura que a leitura começa no início do arquivo */

   fread(&umColetor, sizeof(umColetor), 1, stream); /* Lê um coletor */

      /* Verifica se ocorreu erro de leitura */
   ASSEGURA( !ferror(stream),
             "Erro de leitura em ProfLocalMinDExt" );

   min = umColetor.profLocal;

   while (1) {
      fread(&umColetor, sizeof(umColetor), 1, stream); /* Lê um coletor */

         /* Se ocorreu erro de leitura ou o final  */
         /* do arquivo foi atingido encerra o laço */
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
 * Parâmetros:
 *      stream (entrada) - stream associado ao arquivo que contém os coletores
 *
 * Retorno: A profundidade local máxima
 *
 ****/
int ProfLocalMaxDExt(FILE *stream)
{
   int          max;
   tColetorDExt umColetor;

   rewind(stream); /* Assegura que a leitura começa no início do arquivo */

      /* Lê um coletor */
   fread(&umColetor, sizeof(umColetor), 1, stream);

      /* Verifica se ocorreu erro de leitura */
   ASSEGURA( !ferror(stream), "Erro de leitura em ProfLocalMaxDExt" );

   max = umColetor.profLocal;

   while (1) {
      fread(&umColetor, sizeof(umColetor), 1, stream); /* Lê um coletor */

         /* Se ocorreu erro de leitura ou o final  */
         /* do arquivo foi atingido encerra o laço */
      if (feof(stream) || ferror(stream))
         break;

      if (umColetor.profLocal > max)
         max = umColetor.profLocal;
   }

      /* Verifica se ocorreu erro de leitura */
   ASSEGURA( !ferror(stream), "Erro de leitura em ProfLocalMaxDExt" );

   return max;
}

