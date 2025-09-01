/****
 *
 * Arquivo TabelaDEA.c
 *
 * Autor: Ulysses de Oliveira
 *
 * Data de Criação: 04/08/2008
 * Última modificação: 08/06/2010
 *
 * Descrição: Implementação de operações de busca, inserção e
 *            remoção usando dispersão aberto
 *
 ****/

/*********************** Includes *************************/

#include <stdlib.h>     /* Miscelânea de funções       */
#include <stdio.h>      /* Funções de E/S              */
#include <string.h>     /* Funções strxxx() e memxxx() */
#include "TabelaDEA.h"  /* Interface deste módulo      */
#include "Registros.h"  /* Processamento de registros  */

/***************** Definições de Funções ******************/

/****
 *
 * RedimensionaTabDEA(): Redimensiona uma tabela de dispersão com
 *                       endereçamento aberto
 *
 * Parâmetros:
 *      tabela (entrada) - a tabela de dispersão atual
 *      tamanhoTab (entrada) - tamanho da tabela de dispersão atual
 *      fDispersao (entrada) - ponteiro para a função de dispersão
 *                             a ser usada
 *
 * Retorno: Endereço da nova tabela
 *
 * Observação: O redimensionamento sempre dobra o tamanho atual da
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
         /* Todos os coletores da nova tabela estão inicialmente desocupados */
      tabela[i].status = VAZIO;

      /* Transfere as chaves da tabela antiga para a tabela nova */
   for (i = 0; i < tamanhoTab; ++i)
      if (tabela[i].status == OCUPADO)
         ASSEGURA( InsereDEA( tabelaNova, tamanhoNovo,
                              tabela[i].chaveEIndice, fDispersao ),
                   "Erro de insercao de redimensionamento");

   free(tabela); /* Libera o espaço ocupado pela tabela antiga */

   return tabelaNova;
}

/****
 *
 * CriaTabelaDEA(): Cria e inicializa uma tabela de dispersão
 *                  com endereçamento aberto
 *
 * Parâmetros:
 *      nElementos (entrada) - número de posições da tabela
 *                             de dispersão
 *
 * Retorno: A tabela de dispersão criada
 *
 ****/
tTabelaDEA CriaTabelaDEA(int nElementos)
{
   tTabelaDEA  tabela;
   int         i;

   tabela = calloc(nElementos, sizeof(tColetorDEA));
   ASSEGURA( tabela, "Impossivel alocar a tabela de dispersao" );

   for (i = 0; i < nElementos; ++i)
         /* Todos os elementos estão inicialmente desocupados */
      tabela[i].status = VAZIO;

   return tabela;
}

/****
 *
 * BuscaDEA(): Executa uma busca simples numa tabela de dispersão
 *             com endereçamento aberto
 *
 * Parâmetros:
 *      tabela (entrada) - a tabela de dispersão
 *      tamanhoTab (entrada) - tamanho da tabela de dispersão
 *      chave (entrada) - a chave de busca
 *      fDispersao (entrada) - ponteiro para a função de
 *                             dispersão a ser usada
 *
 * Retorno: Índice do registro no arquivo de registros,
 *          se a chave for encontrada; -1, em caso contrário
 *
 ****/
int BuscaDEA( tTabelaDEA tabela, int tamanhoTab,
              tCEP chave, tFDispersaoDEA fDispersao )
{
   int i, pos;

      /* Faz sucessivas sondagens até encontrar a chave, */
      /* uma posição vazia ou a última sondagem          */
   for (i = 0; i < tamanhoTab; ++i) {
      pos = fDispersao(chave, i, tamanhoTab);
      ASSEGURA(pos < tamanhoTab, "Dispersao invalida recebido por BuscaDEA()");

      if (tabela[pos].status == VAZIO)
         return -1; /* A chave não foi encontrada */

      if (tabela[pos].status == OCUPADO &&
          !memcmp( tabela[pos].chaveEIndice.chave, chave, sizeof(tCEP) ))
         return tabela[pos].chaveEIndice.indice; /* Chave encontrada */
   }

   return -1;  /* A chave não foi encontrada */
}

/****
 *
 * InsereDEA(): Faz inserção numa tabela de dispersão com
 *              endereçamento aberto
 *
 * Parâmetros:
 *      tabela (entrada) - a tabela de dispersão
 *      tamanhoTab (entrada) - tamanho da tabela de dispersão
 *      chaveEIndice (entrada) - a chave de busca e seu respectivo índice
 *      fDispersao (entrada) - ponteiro para a função de dispersão a ser usada
 *
 * Retorno: 1, se houver inserção; 0, em caso contrário
 *
 ****/
int InsereDEA( tTabelaDEA tabela, int tamanhoTab,
               tCEP_Ind chaveEIndice, tFDispersaoDEA fDispersao )
{
   int i, pos, posInsercao = -1;

      /* Faz sucessivas sondagens até encontrar a chave */
      /* ou um local onde se pode fazer a inserção.     */
   for (i = 0; i < tamanhoTab; ++i) {
      pos = fDispersao(chaveEIndice.chave, i, tamanhoTab);
      ASSEGURA( pos < tamanhoTab, "Valor invalido recebido por InsereDEA()" );

      if ( tabela[pos].status == REMOVIDO && posInsercao < 0 )
            /* Se a chave não for encontrada numa nova   */
            /* sondagem, esta será a posição de inserção */
         posInsercao = pos;

      if (tabela[pos].status == VAZIO && posInsercao < 0) {
         posInsercao = pos; /* A chave não foi encontrada */
         break; /* A chave será inserida fora do laço */
      }

      if ( tabela[pos].status == OCUPADO &&
           !memcmp( tabela[pos].chaveEIndice.chave,
                    chaveEIndice.chave, sizeof(tCEP) ) )
         return 0; /* Chave encontrada */
   }

      /* Insere a chave e respectivo índice na  */
      /* posição determinada dentro do laço for */
   tabela[posInsercao].chaveEIndice = chaveEIndice;
   tabela[posInsercao].status = OCUPADO;

   return 1;
}

/****
 *
 * RemoveDEA(): Remove uma chave de uma tabela de dispersão com
 *              endereçamento aberto
 *
 * Parâmetros:
 *      tabela (entrada) - a tabela de dispersão
 *      tamanhoTab (entrada) - tamanho da tabela de dispersão
 *      chave (entrada) - a chave de busca
 *      fDispersao (entrada) - ponteiro para a função de dispersão a ser usada
 *
 * Retorno: 1, se a remoção foi ok; 0, caso contrário
 *
 ****/
int RemoveDEA( tTabelaDEA tabela, int tamanhoTab, tCEP chave,
               tFDispersaoDEA fDispersao )
{
   int i, pos;

      /* Faz sucessivas sondagens até encontrar a chave, */
      /* uma posição vazia ou atingir a última sondagem. */
   for (i = 0; i < tamanhoTab; ++i) {
      pos = fDispersao(chave, i, tamanhoTab);
      ASSEGURA(pos < tamanhoTab, "Valor dispersao invalido em RemoveDEA()");

      if (tabela[pos].status == VAZIO)
         return 0; /* A chave não foi encontrada */

      if (tabela[pos].status == OCUPADO &&
          !memcmp( tabela[pos].chaveEIndice.chave, chave,sizeof(tCEP))) {
         tabela[pos].status = REMOVIDO; /* Marca como removido */

         return 1; /* Remoção foi OK */
      }
   }

   return 0;  /* A chave não foi encontrada */
}

/****
 *
 * NChavesDEA(): Calcula o número de chaves na tabela de dispersão
 *
 * Parâmetros:
 *      tabela (entrada) - a tabela de dispersão
 *      tamanhoTab (entrada) - tamanho da tabela de dispersão
 *
 * Retorno: O número de chaves na tabela de dispersão
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
 * NColetoresVaziosDEA(): Calcula o número de elementos vazios
 *                     na tabela de dispersão
 *
 * Parâmetros:
 *      tabela (entrada) - a tabela de dispersão
 *      tamanhoTab (entrada) - tamanho da tabela de dispersão
 *
 * Retorno: O número de elementos vazios na tabela de dispersão
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
 * NChavesRemovidasDEA(): Determina o número de chaves que foram removidas
 *
 * Parâmetros:
 *      tabela (entrada) - a tabela de dispersão
 *      tamanhoTab (entrada) - tamanho da tabela de dispersão
 *
 * Retorno: O número de conteiners que não são vazios
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
 * DestroiTabelaDEA(): Libera o espaço ocupado por uma tabela
 *                  de dispersão com endereçamento aberto
 *
 * Parâmetros:
 *      tabela (entrada) - a tabela de dispersão
 *      tamTabela (entrada) - tamanho da tabela de dispersão
 *
 * Retorno: Nada
 *
 ****/
void DestroiTabelaDEA(tTabelaDEA *tabela)
{
      /* Libera o espaço ocupado pelo array */
   free(*tabela);

   *tabela = NULL;
}
