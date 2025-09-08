/****
 *
 * Arquivo TabelaDE.c
 *
 * Autor: Ulysses de Oliveira
 *
 * Data de Criação: 04/08/2008
 * Última modificação: 07/06/2016
 *
 * Descrição: Implementação de operações de busca, inserção e
 *            remoção usando dispersão com encadeamento
 *
 ****/

/*********************** Includes *************************/

#include "TabelaDE.h"   /* Interface deste módulo      */
#include <stdlib.h>     /* Miscelânea de funções       */
#include <stdio.h>      /* Funções de E/S              */
#include <string.h>     /* Funções strxxx() e memxxx() */
#include "Registros.h"  /* Processamento de registros  */
#include "Lista.h"      /* Lista encadeada */

/********************* Funções Locais *********************/

/****
 *
 * ComparaListas(): compara duas estruturas do tipo
 *                  tElementosEmLista
 *
 * Parâmetros:
 *      p1 (entrada) - ponteiro para a primeira estrutura
 *      p2 (entrada) - ponteiro para a segunda estrutura
 *
 * Retorno:
 *   0, se os campos nElementos das estruturas forem iguais
 *   > 0, se o campo nElementos da primeira estrutura for maior
 *   < 0, se o campo nElementos da primeira estrutura for menor
 *
 * Observação: Esta função é usada para ordenar as listas
 *             de acordo com seus números de elementos
 *
 ****/
static int ComparaListas(const void *e1, const void *e2)
{
   tElementosEmLista ec1 = *(tElementosEmLista *)e1,
                     ec2 = *(tElementosEmLista *)e2;

   return (int) ec1.nElementos - ec2.nElementos;
}

/********************* Funções Globais ********************/

/****
 *
 * CriaTabelaDE(): Cria e inicializa uma tabela de dispersão
 *                 com encadeamento
 *
 * Parâmetros:
 *      nElementos (entrada) - número de posições da tabela
 *                             de dispersão
 *
 * Retorno: A tabela de dispersão criada
 *
 ****/
tTabelaDE CriaTabelaDE(int nElementos)
{
   tTabelaDE tabela;
   int       i;

      /* Aloca o array que representa a tabela de dispersão */
   tabela = calloc(nElementos, sizeof(tListaSE));

      /* Certifica que houve alocação */
   ASSEGURA(tabela, "Impossivel alocar tabela de dispersao");

      /* Inicializa as listas encadeadas */
   for (i = 0; i < nElementos; ++i)
      IniciaListaSE(tabela + i);

   return tabela;
}

/****
 *
 * BuscaDE(): Executa uma busca simples numa tabela de
 *            dispersão com encadeamento
 *
 * Parâmetros:
 *      tabela (entrada) - a tabela de dispersão
 *      tamTabela (entrada) - tamanho da tabela de dispersão
 *      chave (entrada) - a chave de busca
 *      fDispersao (entrada) - ponteiro para a função de dispersão a ser usada
 *
 * Retorno: Índice do registro no arquivo de registros,
 *          se a chave for encontrada; -1, em caso contrário
 *
 ****/
int BuscaDE(tTabelaDE tabela, int tamTabela, tCEP chave, tFDispersao fDispersao)
{
   int posColetor;

      /* Encontra a respectiva lista encadeada */
   posColetor = fDispersao(chave)%tamTabela;

      /* A lista deve ter sido encontrada */
   ASSEGURA( posColetor >= 0 && posColetor < tamTabela,
             "ERRO: Impossivel localizar lista" );

      /* A função BuscaListaSE() completa o serviço */
   return BuscaListaSE(tabela + posColetor, chave);
}

/****
 *
 * InsereDE(): Insere uma nova chave numa tabela de dispersão
 *             com encadeamento
 *
 * Parâmetros:
 *      tabela (entrada) - a tabela de dispersão
 *      tamTabela (entrada) - tamanho da tabela de dispersão
 *      conteudo (entrada) - a chave de busca e seu
 *                           respectivo índice
 *      fDispersao (entrada) - ponteiro para a função de
 *                             dispersão a ser usada
 *
 * Retorno: Nada
 *
 ****/
void InsereDE( tTabelaDE tabela, int tamTabela,
               const tCEP_Ind *conteudo, tFDispersao fDispersao )
{
   int posColetor;

      /* Encontra a lista encadeada que deverá conter a chave */
   posColetor = fDispersao(conteudo->chave)%tamTabela;

      /* Garante que o índice é válido */
   ASSEGURA( posColetor >= 0 && posColetor < tamTabela,
             "ERRO: Impossivel localizar um lista" );

      /* A função InsereListaSE() completa o serviço */
   return InsereListaSE(&tabela[posColetor], conteudo );
}

/****
 *
 * RemoveDE(): Remove um item de uma tabela dispersão com
 *             encadeamento
 *
 * Parâmetros:
 *      tabela (entrada) - a tabela de dispersão
 *      tamTabela (entrada) - tamanho da tabela de dispersão
 *      chave (entrada) - a chave de busca
 *      fDispersao (entrada) - ponteiro para a função de
 *                             dispersão a ser usada
 *
 * Retorno: 1, se a remoção foi ok; 0, caso contrário
 *
 ****/
int RemoveDE(tTabelaDE tabela, int tamTabela, tCEP chave, tFDispersao fDispersao)
{
   int posColetor;

      /* Encontra a lista encadeada que contém a chave */
   posColetor = fDispersao(chave)%tamTabela;

      /* Garante que o índice é válido */
   ASSEGURA( posColetor > 0 && posColetor < tamTabela,
             "ERRO: Impossivel localizar lista" );

      /* A função RemoveListaSE() completa o serviço */
   return RemoveListaSE(tabela +posColetor, chave);
}

/****
 *
 * NChavesEmListasDE(): Escreve num arquivo o número de
 *                        elementos em cada contêiner da
 *                        tabela de dispersão
 *
 * Parâmetros:
 *      tabela (entrada) - a tabela de dispersão
 *      tamTabela (entrada) - tamanho da tabela de dispersão
 *      stream (entrada) - stream associado ao arquivo onde o
 *                         resultado da operação será escrito
 *
 * Retorno: Nada
 *
 ****/
void NChavesEmListasDE( tTabelaDE tabela, int tamTabela, FILE *stream )
{
   int                i, n, nMax, frequencia;
   tElementosEmLista *ar;

   ASSEGURA(stream, "\nStream recebido e' NULL\n");

      /* Aloca o array que irá armazenar índices de */
      /* listas e respectivos números de elementos  */
   ar = calloc(tamTabela, sizeof(tElementosEmLista));

   ASSEGURA(ar, "\nNao foi possivel alocar array\n");

      /* Preenche o conteúdo do array */
   for (i = 0; i < tamTabela; ++i) {
      ar[i].iLista = i;
      ar[i].nElementos = ComprimentoListaSE(tabela[i]);
   }

      /* Ordena o array de acordo com */
      /* os números de elementos      */
   qsort( ar, tamTabela, sizeof(tElementosEmLista),
          ComparaListas );

   for (i = 0; i < tamTabela; ++i)
      fprintf( stream, "%d\t%d\n", ar[i].iLista,
               ar[i].nElementos );

   nMax = ar[tamTabela - 1].nElementos;

   fprintf(stream, "\n");

      /* O código a seguir pode ser bastante melhorado, */
      /* uma vez que o array é ordenado pelo número de */
      /* elementos em cada contêiner                   */
   for (n = 0; n <= nMax; ++n) {
      frequencia = 0;

      for (i = 0; i < tamTabela; ++i)
         if (ar[i].nElementos == n)
            ++frequencia;

      if (frequencia)
         fprintf( stream, "Numero de listas com %d "
                  "elementos:\t%4.d\n", n, frequencia );
   }

   free(ar); /* O array não é mais necessário */
}

/****
 *
 * NChavesDE(): Calcula o número de chaves na tabela de dispersão
 *
 * Parâmetros:
 *      tabela (entrada) - a tabela de dispersão
 *      tamTabela (entrada) - tamanho da tabela de dispersão
 *
 * Retorno: O número de chaves na tabela de dispersão
 *
 ****/
int NChavesDE(tTabelaDE tabela, int tamTabela)
{
   int i, n = 0;

   for (i = 0; i < tamTabela; ++i)
      n += ComprimentoListaSE(tabela[i]);

   return n;
}

/****
 *
 * NListasNaoVaziasDE(): Determina o número de listas que não
 *                     são vazias numa tabela de dispersão
 *                     com encadeamento
 *
 * Parâmetros:
 *      tabela (entrada) - a tabela de dispersão
 *      tamTabela (entrada) - tamanho da tabela de dispersão
 *
 * Retorno: O número de conteiners que não são vazios
 *
 ****/
int NListasNaoVaziasDE( tTabelaDE tabela, int tamTabela )
{
   int i, n = 0;

   for (i = 0; i < tamTabela; ++i)
      if (tabela[i])
         n++;

   return n;
}

/****
 *
 * DestroiTabelaDE(): Libera o espaço ocupado por uma tabela
 *                  de dispersão com encadeamento
 *
 * Parâmetros:
 *      tabela (entrada) - a tabela de dispersão
 *      tamTabela (entrada) - tamanho da tabela de dispersão
 *
 * Retorno: Nada
 *
 ****/
void DestroiTabelaDE(tTabelaDE *tabela, int tamTabela)
{
   int i;

      /* Destrói as listas encadeadas */
   for (i = 0; i < tamTabela; ++i) {
      DestroiListaSE(*tabela + i);
   }

      /* Libera o espaço ocupado pelo array */
   free(*tabela);

   *tabela = NULL;
}
