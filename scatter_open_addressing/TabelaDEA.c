/****
 *
 * Arquivo TabelaDEA.c
 *
 * Autor: Ulysses de Oliveira
 *
 * Data de Cria��o: 04/08/2008
 * �ltima modifica��o: 08/06/2010
 *
 * Descri��o: Implementa��o de opera��es de busca, inser��o e
 *            remo��o usando dispers�o aberto
 *
 ****/

/*********************** Includes *************************/

#include <stdlib.h>     /* Miscel�nea de fun��es       */
#include <stdio.h>      /* Fun��es de E/S              */
#include <string.h>     /* Fun��es strxxx() e memxxx() */
#include "TabelaDEA.h"  /* Interface deste m�dulo      */
#include "Registros.h"  /* Processamento de registros  */

/***************** Defini��es de Fun��es ******************/

/****
 *
 * RedimensionaTabDEA(): Redimensiona uma tabela de dispers�o com
 *                       endere�amento aberto
 *
 * Par�metros:
 *      tabela (entrada) - a tabela de dispers�o atual
 *      tamanhoTab (entrada) - tamanho da tabela de dispers�o atual
 *      fDispersao (entrada) - ponteiro para a fun��o de dispers�o
 *                             a ser usada
 *
 * Retorno: Endere�o da nova tabela
 *
 * Observa��o: O redimensionamento sempre dobra o tamanho atual da
 *             tabela
 *
 ****/
static tTabelaDEA RedimensionaTabDEA( tTabelaDEA tabela, int tamanhoTab,
                                      tFDispersaoDEA fDispersao )
{
   tTabelaDEA tabelaNova;
   int        i, tamanhoNovo = 2*tamanhoTab;

   tabelaNova = calloc(tamanhoNovo, sizeof(tColetorDEA));
   ASSEGURA(tabelaNova, "Impossivel redimensionar a tabela");

      /* Inicia os coletores */
   for (i = 0; i < tamanhoNovo; ++i)
         /* Todos os coletores da nova tabela est�o inicialmente desocupados */
      tabela[i].status = VAZIO;

      /* Transfere as chaves da tabela antiga para a tabela nova */
   for (i = 0; i < tamanhoTab; ++i)
      if (tabela[i].status == OCUPADO)
         ASSEGURA( InsereDEA( tabelaNova, tamanhoNovo,
                              tabela[i].chaveEIndice, fDispersao ),
                   "Erro de insercao de redimensionamento");

   free(tabela); /* Libera o espa�o ocupado pela tabela antiga */

   return tabelaNova;
}

/****
 *
 * CriaTabelaDEA(): Cria e inicializa uma tabela de dispers�o
 *                  com endere�amento aberto
 *
 * Par�metros:
 *      nElementos (entrada) - n�mero de posi��es da tabela
 *                             de dispers�o
 *
 * Retorno: A tabela de dispers�o criada
 *
 ****/
tTabelaDEA CriaTabelaDEA(int nElementos)
{
   tTabelaDEA  tabela;
   int         i;

   tabela = calloc(nElementos, sizeof(tColetorDEA));
   ASSEGURA( tabela, "Impossivel alocar a tabela de dispersao" );

   for (i = 0; i < nElementos; ++i)
         /* Todos os elementos est�o inicialmente desocupados */
      tabela[i].status = VAZIO;

   return tabela;
}

/****
 *
 * BuscaDEA(): Executa uma busca simples numa tabela de dispers�o
 *             com endere�amento aberto
 *
 * Par�metros:
 *      tabela (entrada) - a tabela de dispers�o
 *      tamanhoTab (entrada) - tamanho da tabela de dispers�o
 *      chave (entrada) - a chave de busca
 *      fDispersao (entrada) - ponteiro para a fun��o de
 *                             dispers�o a ser usada
 *
 * Retorno: �ndice do registro no arquivo de registros,
 *          se a chave for encontrada; -1, em caso contr�rio
 *
 ****/
int BuscaDEA( tTabelaDEA tabela, int tamanhoTab,
              tCEP chave, tFDispersaoDEA fDispersao )
{
   int i, pos;

      /* Faz sucessivas sondagens at� encontrar a chave, */
      /* uma posi��o vazia ou a �ltima sondagem          */
   for (i = 0; i < tamanhoTab; ++i) {
      pos = fDispersao(chave, i, tamanhoTab);
      ASSEGURA(pos < tamanhoTab, "Dispersao invalida recebido por BuscaDEA()");

      if (tabela[pos].status == VAZIO)
         return -1; /* A chave n�o foi encontrada */

      if (tabela[pos].status == OCUPADO &&
          !memcmp( tabela[pos].chaveEIndice.chave, chave, sizeof(tCEP) ))
         return tabela[pos].chaveEIndice.indice; /* Chave encontrada */
   }

   return -1;  /* A chave n�o foi encontrada */
}

/****
 *
 * InsereDEA(): Faz inser��o numa tabela de dispers�o com
 *              endere�amento aberto
 *
 * Par�metros:
 *      tabela (entrada) - a tabela de dispers�o
 *      tamanhoTab (entrada) - tamanho da tabela de dispers�o
 *      chaveEIndice (entrada) - a chave de busca e seu respectivo �ndice
 *      fDispersao (entrada) - ponteiro para a fun��o de dispers�o a ser usada
 *
 * Retorno: 1, se houver inser��o; 0, em caso contr�rio
 *
 ****/
int InsereDEA( tTabelaDEA tabela, int tamanhoTab,
               tCEP_Ind chaveEIndice, tFDispersaoDEA fDispersao )
{
   int i, pos, posInsercao = -1;

      /* Faz sucessivas sondagens at� encontrar a chave */
      /* ou um local onde se pode fazer a inser��o.     */
   for (i = 0; i < tamanhoTab; ++i) {
      pos = fDispersao(chaveEIndice.chave, i, tamanhoTab);
      ASSEGURA( pos < tamanhoTab, "Valor invalido recebido por InsereDEA()" );

      if ( tabela[pos].status == REMOVIDO && posInsercao < 0 )
            /* Se a chave n�o for encontrada numa nova   */
            /* sondagem, esta ser� a posi��o de inser��o */
         posInsercao = pos;

      if (tabela[pos].status == VAZIO && posInsercao < 0) {
         posInsercao = pos; /* A chave n�o foi encontrada */
         break; /* A chave ser� inserida fora do la�o */
      }

      if ( tabela[pos].status == OCUPADO &&
           !memcmp( tabela[pos].chaveEIndice.chave,
                    chaveEIndice.chave, sizeof(tCEP) ) )
         return 0; /* Chave encontrada */
   }

      /* Insere a chave e respectivo �ndice na  */
      /* posi��o determinada dentro do la�o for */
   tabela[posInsercao].chaveEIndice = chaveEIndice;
   tabela[posInsercao].status = OCUPADO;

   return 1;
}

/****
 *
 * RemoveDEA(): Remove uma chave de uma tabela de dispers�o com
 *              endere�amento aberto
 *
 * Par�metros:
 *      tabela (entrada) - a tabela de dispers�o
 *      tamanhoTab (entrada) - tamanho da tabela de dispers�o
 *      chave (entrada) - a chave de busca
 *      fDispersao (entrada) - ponteiro para a fun��o de dispers�o a ser usada
 *
 * Retorno: 1, se a remo��o foi ok; 0, caso contr�rio
 *
 ****/
int RemoveDEA( tTabelaDEA tabela, int tamanhoTab, tCEP chave,
               tFDispersaoDEA fDispersao )
{
   int i, pos;

      /* Faz sucessivas sondagens at� encontrar a chave, */
      /* uma posi��o vazia ou atingir a �ltima sondagem. */
   for (i = 0; i < tamanhoTab; ++i) {
      pos = fDispersao(chave, i, tamanhoTab);
      ASSEGURA(pos < tamanhoTab, "Valor dispersao invalido em RemoveDEA()");

      if (tabela[pos].status == VAZIO)
         return 0; /* A chave n�o foi encontrada */

      if (tabela[pos].status == OCUPADO &&
          !memcmp( tabela[pos].chaveEIndice.chave, chave,sizeof(tCEP))) {
         tabela[pos].status = REMOVIDO; /* Marca como removido */

         return 1; /* Remo��o foi OK */
      }
   }

   return 0;  /* A chave n�o foi encontrada */
}

/****
 *
 * NChavesDEA(): Calcula o n�mero de chaves na tabela de dispers�o
 *
 * Par�metros:
 *      tabela (entrada) - a tabela de dispers�o
 *      tamanhoTab (entrada) - tamanho da tabela de dispers�o
 *
 * Retorno: O n�mero de chaves na tabela de dispers�o
 *
 ****/
int NChavesDEA(tTabelaDEA tabela, int tamanhoTab)
{
   int i, nChaves = 0;

   for (i = 0; i < tamanhoTab; ++i)
      if (tabela[i].status == OCUPADO)
         ++nChaves;

   return nChaves;
}

/****
 *
 * NColetoresVaziosDEA(): Calcula o n�mero de elementos vazios
 *                     na tabela de dispers�o
 *
 * Par�metros:
 *      tabela (entrada) - a tabela de dispers�o
 *      tamanhoTab (entrada) - tamanho da tabela de dispers�o
 *
 * Retorno: O n�mero de elementos vazios na tabela de dispers�o
 *
 ****/
int NColetoresVaziosDEA(tTabelaDEA tabela, int tamanhoTab)
{
   int i, n = 0;

   for (i = 0; i < tamanhoTab; ++i)
      if (tabela[i].status == VAZIO)
         ++n;

   return n;
}

/****
 *
 * NChavesRemovidasDEA(): Determina o n�mero de chaves que foram removidas
 *
 * Par�metros:
 *      tabela (entrada) - a tabela de dispers�o
 *      tamanhoTab (entrada) - tamanho da tabela de dispers�o
 *
 * Retorno: O n�mero de conteiners que n�o s�o vazios
 *
 ****/
int NChavesRemovidasDEA(tTabelaDEA tabela, int tamanhoTab)
{
   int i, n = 0;

   for (i = 0; i < tamanhoTab; ++i)
      if (tabela[i].status == REMOVIDO)
         ++n;

   return n;
}

/****
 *
 * DestroiTabelaDEA(): Libera o espa�o ocupado por uma tabela
 *                  de dispers�o com endere�amento aberto
 *
 * Par�metros:
 *      tabela (entrada) - a tabela de dispers�o
 *      tamTabela (entrada) - tamanho da tabela de dispers�o
 *
 * Retorno: Nada
 *
 ****/
void DestroiTabelaDEA(tTabelaDEA *tabela)
{
      /* Libera o espa�o ocupado pelo array */
   free(*tabela);

   *tabela = NULL;
}
