/****
 *
 * Arquivo TabelaCuco.c
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
#include <math.h>     /* Funções strxxx() e memxxx() */
#include "TabelaCuco.h"  /* Interface deste módulo      */
#include "Registros.h"  /* Processamento de registros  */

/********************* Funções Locais *********************/

/****
 *
 * RedimensionaCuco(): Redimensiona uma tabela de dispersão cuco
 *
 * Parâmetros:
 *     tabela (entrada/saída) - a tabela de dispersão
 *     chaveEIndice (entrada) - o par chave/índice que será inserido
 *                              após o redimensionamento da tabela
 *
 * Retorno: Nada
 *
 ****/
static void RedimensionaCuco(tTabelaCuco *tabela, tCEP_Ind chaveEIndice)
{
   tTabelaCuco novaTabela; /* A nova tabela */
   int         i,
               teste, /* Resultado de uma inserção */
               novoTamanho; /* Tamanho da nova tabela */

      /* O novo tamanho será 50% maior do que o antigo valor */
   novoTamanho = 1.5*tabela->tam;

      /* Cria uma nova tabela de dispersão cuco */
   CriaTabelaCuco(&novaTabela, novoTamanho, tabela->fD1, tabela->fD2);

      /* Insere as chaves da antiga tabela na nova tabela */
   for (i = 0; i < tabela->tam; ++i) {
         /* Verifica se a posição corrente da primeira tabela contém uma chave */
      if (tabela->tab1[i].status == OCUPADO) {
            /* Existe uma chave nesta posição */
         teste = InsereCuco( &novaTabela, tabela->tab1[i].chaveEIndice );
            /* Esta inserção não pode falhar */
         ASSEGURA( teste, "Falha de insercao em redimensionamento" );
      }

         /* Verifica se a posição corrente da segunda tabela contém uma chave */
      if (tabela->tab2[i].status == OCUPADO) {
            /* Existe uma chave nesta posição */
         teste = InsereCuco( &novaTabela, tabela->tab2[i].chaveEIndice );
            /* Esta inserção não pode falhar */
         ASSEGURA( teste, "Falha de insercao em redimensionamento" );
      }
   }

      /* Neste ponto, todas as chaves da antiga tabela foram inseridas, */
      /* de modo que a tabela antiga pode ser extinta                   */
   DestroiTabelaCuco(tabela);

   *tabela = novaTabela; /* Substitui a tabela antiga com a nova tabela */

    teste = InsereCuco(tabela, chaveEIndice); /* Insere a chave que restou */

      /* Esta inserção não pode falhar */
    ASSEGURA(teste, "Falha de insercao em redimensionamento");
}

/********************* Funções Globais ********************/

/****
 *
 * CriaTabelaCuco(): Cria e inicializa uma tabela de dispersão cuco
 *
 * Parâmetros:
 *     tabelas (saída) - a tabela cuco criada e iniciada
 *     nElementos (entrada) - número de posições da tabela
 *                            de dispersão
 *     fD1, fD2 (entrada) - as funções de dispersão utilizadas
 *
 * Retorno: Nada
 *
 ****/
void CriaTabelaCuco( tTabelaCuco *tabelas, int nElementos,
                     tFDispersaoCuco fD1, tFDispersaoCuco fD2 )
{
   int i;

      /* Aloca espaço para as duas tabelas */
   tabelas->tab1 = calloc(nElementos, sizeof(tColetorCuco));
   tabelas->tab2 = calloc(nElementos, sizeof(tColetorCuco));
   ASSEGURA( tabelas->tab1 && tabelas->tab2,
             "\nImpossivel alocar a tabela de dispersão\n" );

      /* Inicia as duas tabelas */
   for (i = 0; i < nElementos; ++i) {
         /* Todos os elementos estão inicialmente desocupados */
      tabelas->tab1[i].status = VAZIO;
      tabelas->tab2[i].status = VAZIO;
   }

      /* Inicia as funções de dispersão */
   tabelas->fD1 = fD1;
   tabelas->fD2 = fD2;

      /* Inicia o número de chaves nas tabelas */
   tabelas->nChaves = 0;
}

/****
 *
 * BuscaCuco(): Executa uma busca simples numa tabela de dispersão cuco
 *
 * Parâmetros:
 *      tabela (entrada) - a tabela de dispersão
 *      chave (entrada) - a chave de busca
 *
 * Retorno: Índice do registro no arquivo de registros,
 *          se a chave for encontrada; -1, em caso contrário
 *
 ****/
int BuscaCuco( tTabelaCuco tabela, tCEP chave )
{
   int pos; /* Essa variável não é estritamente necessária, mas */
            /* ela facilita a escrita das expressões seguintes  */

   pos = tabela.fD1(chave, tabela.tam);

      /* Verifica se a chave se encontra na primeira tabela */
   if ( tabela.tab1[pos].status == OCUPADO &&
        !strcmp(tabela.tab1[pos].chaveEIndice.chave, chave) )
      return tabela.tab1[pos].chaveEIndice.indice; /* Chave encontrada */

   /* A chave ainda não foi encontrada. Verifica */
   /* se ela se encontra na segunda tabela.      */

   pos = tabela.fD2(chave, tabela.tam);

   if ( tabela.tab2[pos].status == OCUPADO &&
        !strcmp(tabela.tab2[pos].chaveEIndice.chave, chave) )
      return tabela.tab2[pos].chaveEIndice.indice; /* Chave encontrada */

   return -1; /* A chave não foi encontrada em nenhuma das tabelas */
}

/****
 *
 * InsereCuco(): Faz inserção numa tabela de dispersão cuco
 *
 * Parâmetros:
 *      tabela (entrada/saída) - a tabela de dispersão
 *      chaveEIndice (entrada) - o par chave/índice a ser inserido
 *
 * Retorno: 1, se houver inserção; 0, em caso contrário
 *
 ****/
int InsereCuco(tTabelaCuco *tabela, tCEP_Ind chaveEIndice)
{
   int          i,
                pos,
                maxDesalojamentos;
   tCEP_Ind     aux;

      /* Verifica se a chave já existe na tabela */
   if (BuscaCuco(*tabela, chaveEIndice.chave) >= 0)
      return 0; /* Chave já existe na tabela */

      /* Calcula o valor limite de desalojamentos. O valor   */
      /* nlog n é sugerido pelos criadores da dispersão cuco */
   maxDesalojamentos = tabela->nChaves*log2(tabela->nChaves);

      /* Tenta efetuar a inserção numa das tabelas */
   for (i = 0; i < maxDesalojamentos; ++i) {
      /*                                  */
      /* Tenta inserir na primeira tabela */

      pos = tabela->fD1(chaveEIndice.chave, tabela->tam);

      if (tabela->tab1[pos].status == VAZIO) {
         /*                                                                */
         /* Foi encontrado um espaço vazio na primeira tabela. Efetua-se a */
         /* inserção e retorna-se  indicando o sucesso da operação         */
         /*                                                                */

            /* Insere o par chave/índice */
         tabela->tab1[pos].chaveEIndice = chaveEIndice;

         tabela->tab1[pos].status = OCUPADO; /* Esta posição passa a ser ocupada */

         ++tabela->nChaves; /* O número de chaves aumentou */

         return 1;
      } else {
         /*                                                                     */
         /* Não foi encontrado um espaço vazio. É preciso desalojar a chave que */
         /* se encontra nessa posição e tentar inseri-la na segunda tabela      */
         /*                                                                     */

         aux = tabela->tab1[pos].chaveEIndice; /* Guarda chave a ser desalojada */
            /* Armazena a nova chave no lugar da chave desalojada */
         tabela->tab1[pos].chaveEIndice = chaveEIndice;
            /* Tentar-se-a inserir a chave desalojada na segunda tabela */
         chaveEIndice = aux;
      }

      /*                                  */
      /* Tenta inserir na segunda tabela */
      /*                                  */

      pos = tabela->fD2(chaveEIndice.chave, tabela->tam);

      if (tabela->tab2[pos].status == VAZIO) {
         /*                                                             */
         /* Foi encontrado um espaço vazio na segunda tabela. Efetua-se */
         /* a inserção e retorna-se indicando o sucesso da operação     */
         /*                                                             */

            /* Insere o par chave/índice */
         tabela->tab2[pos].chaveEIndice = chaveEIndice;

            /* Esta posição passa a ser ocupada */
         tabela->tab2[pos].status = OCUPADO;

         ++tabela->nChaves; /* O número de chaves aumentou */

         return 1;
      } else {
         /*                                                                     */
         /* Não foi encontrado um espaço vazio. É preciso desalojar a chave que */
         /* se encontra nessa posição e tentar inseri-la na primeira tabela     */
         /*                                                                     */

            /* Guarda a chave a ser desalojada */
         aux = tabela->tab2[pos].chaveEIndice;
            /* Armazena a nova chave no lugar da chave desalojada */
         tabela->tab2[pos].chaveEIndice = chaveEIndice;
            /* Tentar-se-á inserir a chave desalojada na primeira tabela */
         chaveEIndice = aux;
      }
   }

   /*                                                                   */
   /* Se o laço terminou sem que houvesse retorno, considera-se que     */
   /* o processo entrou em repetição  sem fim (de acordo com o critério */
   /* especificado). Portanto, a tabela será reconstruída e a chave     */
   /* restante será inserida nessa nova tabela.                         */
   /*                                                                   */

   RedimensionaCuco(tabela, chaveEIndice);

   return 1;
}

/****
 *
 * RemoveCuco(): Remove uma chave de uma tabela de dispersão cuco
 *
 * Parâmetros:
 *      tabela (entrada/saída) - a tabela de dispersão
 *      chave (entrada) - a chave de busca
 *
 * Retorno: 1, se a remoção foi ok; 0, caso contrário
 *
 ****/
int RemoveCuco(tTabelaCuco *tabela, tCEP chave)
{
   int pos; /* Essa variável não é estritamente necessária, mas */
            /* ela facilita a escrita das expressões seguintes  */

   pos = tabela->fD1(chave, tabela->tam);

      /* Tenta efetuar a remoção na primeira tabela */
   if ( tabela->tab1[pos].status == OCUPADO &&
        !strcmp(tabela->tab1[pos].chaveEIndice.chave, chave) ) {
      tabela->tab1[pos].status = VAZIO; /* Chave encontrada */

      --tabela->nChaves;

      return 1;
   }

   /*                                                                    */
   /* A chave ainda não foi removida. Tenta removê-la da segunda tabela. */
   /*                                                                    */

   pos = tabela->fD2(chave, tabela->tam);

   if ( tabela->tab2[pos].status == OCUPADO &&
        !strcmp(tabela->tab2[pos].chaveEIndice.chave, chave) ) {
      tabela->tab2[pos].status = VAZIO; /* Chave encontrada */

      --tabela->nChaves;

      return 1;
   }

   return 0;  /* A chave não foi encontrada */
}

/****
 *
 * NChavesCuco(): Calcula o número de chaves na tabela de dispersão
 *
 * Parâmetros:
 *      tabela (entrada) - a tabela de dispersão
 *
 * Retorno: O número de chaves na tabela de dispersão
 *
 ****/
int NChavesCuco(tTabelaCuco tabela)
{
   int i, nChaves = 0;

   for (i = 0; i < tabela.tam; ++i) {
      if (tabela.tab1[i].status == OCUPADO)
         ++nChaves;

      if (tabela.tab2[i].status == OCUPADO)
         ++nChaves;
   }

   ASSEGURA(tabela.nChaves == nChaves, "\nErro na contagem de numero de chaves\n");

   return nChaves;
}

/****
 *
 * NElementosVaziosCuco(): Calcula o número de elementos vazios
 *                     na tabela de dispersão
 *
 * Parâmetros:
 *      tabela (entrada) - a tabela de dispersão
 *
 * Retorno: O número de elementos vazios na tabela de dispersão
 *
 ****/
int NElementosVaziosCuco(tTabelaCuco tabela)
{
   int i, n = 0;

   for (i = 0; i < tabela.tam; ++i) {
      if (tabela.tab1[i].status == VAZIO)
         ++n;

      if (tabela.tab2[i].status == VAZIO)
         ++n;
   }

   return n;
}

/****
 *
 * DestroiTabelaCuco(): Libera o espaço ocupado por uma tabela
 *                  de dispersão com endereçamento aberto
 *
 * Parâmetros:
 *      tabela (entrada) - a tabela de dispersão
 *      tamTabela (entrada) - tamanho da tabela de dispersão
 *
 * Retorno: Nada
 *
 ****/
void DestroiTabelaCuco(tTabelaCuco *tabela)
{
      /* Libera o espaço ocupado por cada array */
   free(tabela->tab1);
   free(tabela->tab2);

      /* Os arrays não são mais válidos */
   tabela->tab1 = NULL;
   tabela->tab2 = NULL;
}
