/****
 *
 * Arquivo DispersaoEstatica.c
 *
 * Autor: Ulysses de Oliveira
 *
 * Data de Criação: 04/08/2017
 * Última modificação: 06/09/2017
 *
 * Descrição: Implementação de operações de busca, inserção e
 *            remoção usando dispersão estática em memória
 *            secundária
 *
 ****/

/*********************** Includes *************************/

#include <limits.h>
#include <stdlib.h>     /* Miscelânea de funções       */
#include <stdio.h>      /* Funções de E/S              */
#include <string.h>     /* Funções strxxx() e memxxx() */
#include <math.h>       /* Funções strxxx() e memxxx() */
#include "DispersaoEstatica.h" /* Interface deste módulo */
#include "Registros.h"  /* Processamento de registros  */
#include "ASSEGURA.h"   /* Macro ASSEGURA              */

/********************* Funções Locais *********************/

/****
 *
 * FDispersao(): Obtém o valor de dispersão de uma chave inteira
 *
 * Parâmetros:
 *      chave (entrada) - a chave
 *      nColetores (entrada) - o número de coletores primários
 *
 * Retorno: O valor de dispersão
 *
 ****/
static int FDispersao(tChave chave, int nColetores)
{
//   return DispersaoRotativa(chave) % nColetores;
   return chave % nColetores;
}

/****
 *
 * NovoColetorDEst(): Cria um novo coletor num arquivo
 *
 * Parâmetros:
 *     stream (entrada) - stream associado ao arquivo que contém
 *                        os coletores
 *     tipo (entrada) - tipo do coletor que será criado
 *
 * Retorno: O índice do coletor criado
 *
 ****/
static int NovoColetorDEst(FILE *stream, tTipoDeColetorDEst tipo)
{
   tColetorDEst coletor;
   int          indice;

   coletor.nRegistros = 0;
   coletor.proximo = POSICAO_NULA;
   coletor.tipo = tipo;

      /* Move o apontador de posição do arquivo para seu final */
   MoveApontador(stream, 0, SEEK_END);

   indice = ftell(stream)/sizeof(coletor); /* Obtém o índice do coletor */

   fwrite(&coletor, sizeof(coletor), 1, stream); /* Escreve o coletor no arquivo */

   return indice;
}

/****
 *
 * LeColetorDEst(): Lê um coletor em arquivo
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
static void LeColetorDEst(FILE *stream, int pos, tColetorDEst *coletor)
{
      /* Tenta mover o apontador de arquivo para o local */
      /* de leitura; se não conseguir, aborta o programa */
   MoveApontador(stream, sizeof(tColetorDEst)*pos, SEEK_SET);

   fread(coletor, sizeof(tColetorDEst), 1, stream); /* Efetua a leitura */

      /* Certifica-se que não houve erro de leitura */
   ASSEGURA(!ferror(stream),  "Erro de leitura em LeColetorDEst()");
}

/****
 *
 * EscreveColetorDEst(): Escreve um coletor em arquivo
 *
 * Parâmetros:
 *     stream (entrada) - stream associado ao arquivo contendo os
 *                        coletores no qual será feita a escrita
 *     pos (entrada) - posição no arquivo no qual será feita a escrita
 *    *coletor (entrada) - coletor que será escrito no arquivo
 *
 * Retorno: Nada
 *
 ****/
static void EscreveColetorDEst(FILE *stream, int pos, const tColetorDEst *coletor)
{
      /* Tenta mover o apontador de arquivo para o local */
      /* de escrita; se não conseguir, aborta o programa */
   MoveApontador(stream, sizeof(tColetorDEst)*pos, SEEK_SET);

   fwrite(coletor, sizeof(tColetorDEst), 1, stream); /* Efetua a escrita */

      /* Certifica-se que não houve erro */
   ASSEGURA(!ferror(stream), "Erro de escrita em EscreveColetorDEst()");
}

/********************* Funções Globais ********************/

/****
 *
 * IniciaTabDEst(): Inicia uma tabela de busca com dispersão estática
 *
 * Parâmetros:
 *      streamCol (entrada) - stream associado ao arquivo que
 *                            contém os coletores
 *      nRegistros (entrada) - número de registros iniciais da tabela
 *      maxRegs (entrada) - número máximo de registros num coletor
 *
 * Retorno: O número de coletores primários
 *
 ****/
int IniciaTabDEst( FILE *streamCol, int nRegistros, int maxRegs )
{
   int i,
       nColetores;

   nColetores = nRegistros/maxRegs; /* Obtém o número de coletores */

      /* Aloca os coletores primários no arquivo */
   for (i = 0; i < nColetores; ++i)
      (void) NovoColetorDEst(streamCol, PRIMARIO);

   return nColetores;
}

/****
 *
 * BuscaDEst(): Efetua uma busca numa tabela de dispersão estática
 *
 * Parâmetros:
 *      stream (entrada) - stream associado ao arquivo que contém os coletores
 *      nColetores (entrada) - número de coletores primários da tabela
 *      chave (entrada) - a chave de busca
 *      reg (saída) - o registro encontrado
 *
 * Retorno: O endereço do registro, se ele for encontrado; NULL, em caso contrário
 *
 ****/
tRegistroMEC *BuscaDEst( FILE *stream, int nColetores, tChave chave,
                         tRegistroMEC *reg )
{
   int          iColetor, i;
   tColetorDEst coletor;

      /* Obtém o índice do coletor primário que pode conter o registro */
   iColetor = FDispersao(chave, nColetores);

      /* Efetua uma busca no coletor primário e em seus coletores excedentes */
   while (iColetor != POSICAO_NULA) {
         /* Lê um coletor que pode conter o registro */
      LeColetorDEst(stream, iColetor, &coletor);

         /* Efetua uma busca sequencial pelo registro em memória principal */
      for (i = 0; i < coletor.nRegistros; i++)
            /* Verifica se a chave foi encontrada */
         if (chave == ObtemChave(&coletor.registros[i])) {
            *reg = coletor.registros[i]; /* Chave encontrada */
            return reg; /* Serviço completo */
         }

      iColetor = coletor.proximo; /* Passa para o próximo coletor excedente */
   }

   return NULL; /* A chave não foi encontrada */
}

/****
 *
 * InsereDEst(): Insere um registro numa tabela de busca com dispersão estática
 *
 * Parâmetros:
 *      nColetores (entrada) - número de coletores primários da
 *                             tabela na qual será efetuada a inserção
 *      registro (entrada) - o registro que será inserido
 *      stream (entrada) - stream associado ao arquivo que contém os coletores
 *
 * Retorno: Nada
 *
 ****/
void InsereDEst(int nColetores, const tRegistroMEC *registro, FILE *stream)
{
   int          iColetor;
   tChave       chave;
   tColetorDEst coletor;

   chave = ObtemChave(registro); /* Obtém a chave do registro */

   iColetor = FDispersao(chave, nColetores); /* Obtém o índice do coletor */

      /* Lê o coletor primário em arquivo */
   LeColetorDEst(stream, iColetor, &coletor);

      /* Encontra o coletor no qual será efetuada a inserção */
   while (coletor.nRegistros == M) {
         /* O coletor está repleto. Portanto passa-se para o    */
         /* próximo coletor excedente. Se o próximo coletor     */
         /* excedente ainda não existe, cria-se um novo coletor */
      if (coletor.proximo == POSICAO_NULA) {
            /* Cria um novo coletor excedente */
         coletor.proximo = NovoColetorDEst(stream, EXCEDENTE);

            /* O coletor foi alterado e precisa ser reescrito */
         EscreveColetorDEst(stream, iColetor, &coletor);
      }

         /* Guarda a posição do próximo coletor a ser lido */
      iColetor = coletor.proximo;

         /* Lê um coletor excedente em arquivo */
      LeColetorDEst(stream, coletor.proximo, &coletor);
   }

      /* Acrescenta o novo registro ao final do coletor */
   coletor.registros[coletor.nRegistros] = *registro;

   coletor.nRegistros++; /* O número de registros no coletor aumentou */

      /* O coletor foi alterado e é preciso reescrevê-lo no arquivo */
   EscreveColetorDEst(stream, iColetor, &coletor);
}

/****
 *
 * RemoveDEst(): Remove um registro de uma tabela de dispersão estática
 *
 * Parâmetros:
 *      nColetores (entrada/saída) - número de coletores primários
 *                                   da tabela de dispersão
 *      chave (entrada) - a chave do registro que será removido
 *      stream (entrada) - stream associado ao arquivo que contém os coletores
 *
 * Retorno: 1, se a remoção foi ok; 0, caso contrário
 *
 ****/
int RemoveDEst(int nColetores, tChave chave, FILE *stream)
{
   int          i, j, iColetor, encontrada = 0;
   tColetorDEst coletor;

      /* Obtém o índice do coletor que pode conter o registro */
   iColetor = FDispersao(chave, nColetores);

      /* Efetua uma busca no coletor primário e em seus coletores excedentes */
   while (iColetor != POSICAO_NULA) {
         /* Lê o coletor que pode conter o registro */
      LeColetorDEst(stream, iColetor, &coletor);

         /* Efetua uma busca sequencial pelo registro em memória principal */
      for (i = 0; i < coletor.nRegistros && !encontrada; i++)
            /* Verifica se a chave foi encontrada */
         if (chave == ObtemChave(&coletor.registros[i]))
            encontrada = 1;

         /* Se a chave foi encontrada, encerra a busca */
      if (encontrada)
         break;

      iColetor = coletor.proximo; /* Passa para o próximo coletor excedente */
   }

      /* Verifica se a chave foi encontrada */
   if (!encontrada)
      return 0;  /* A chave não foi encontrada */

      /* Corrige o valor de i que foi incrementado e deixou */
      /* de ser o índice do elemento a ser removido         */
   --i;

      /* O registro a ser removido encontra-se na posição i do */
      /* coletor corrente. É necessário mover uma posição para */
      /* trás todos os registros que se encontram à sua frente */
   for (j = i; j < coletor.nRegistros - 1; ++j)
      coletor.registros[j] = coletor.registros[j + 1];

   --coletor.nRegistros; /* O número de registros no coletor diminuiu */

      /* O coletor foi alterado e é preciso reescrevê-lo no arquivo */
   EscreveColetorDEst(stream, iColetor, &coletor);

   return 1;  /* Remoção bem-sucedida */
}

/****
 *
 * SubstituiDEst(): Substitui o conteúdo de um registro que
 *                      tem uma determinada chave
 *
 * Parâmetros:
 *      stream (entrada) - stream associado ao arquivo que contém os coletores
 *      nColetores (entrada) - número de coletores primários da tabela
 *      reg (entrada) - o registro substituto
 *
 * Retorno: Nada
 *
 * Observações:
 *      1. A chave deve ser primária
 *      2. Se a chave não for encontrada, o programa será abortado
 *
 ****/
void SubstituiDEst( FILE *stream, int nColetores, const tRegistroMEC *reg )
{
   int          iColetor, i;
   tColetorDEst coletor;
   tChave       chave = ObtemChave(reg);

      /* Obtém o índice do coletor primário que pode conter o registro */
   iColetor = FDispersao(chave, nColetores);

      /* Efetua uma busca no coletor primário e em seus coletores excedentes */
   while (iColetor != POSICAO_NULA) {
         /* Lê um coletor que pode conter o registro */
      LeColetorDEst(stream, iColetor, &coletor);

         /* Efetua uma busca sequencial pelo registro em memória principal */
      for (i = 0; i < coletor.nRegistros; i++)
            /* Verifica se a chave foi encontrada */
         if (chave == ObtemChave(&coletor.registros[i])) {
               /* A chave encontrada. Altera o conteúdo do registro que a contém */
            coletor.registros[i] = *reg;

            return; /* Serviço completo */
         }

      iColetor = coletor.proximo; /* Passa para o próximo coletor excedente */
   }

      /* A chave não foi encontrada */
   ASSEGURA(0, "A chave deveria ter sido encontrada");
}

/****
 *
 * NRegistrosTabDEst(): Calcula o número de registros numa tabela de dispersão estática
 *
 * Parâmetros:
 *      stream (entrada) - stream associado ao arquivo que contém os coletores
 *
 * Retorno: O número de registros na tabela de dispersão
 *
 ****/
int NRegistrosTabDEst(FILE *stream)
{
   int          nRegistros = 0;
   tColetorDEst umColetor;

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
   ASSEGURA( !ferror(stream), "Erro de leitura em NRegistrosTabDEst" );

   return nRegistros;
}

/****
 *
 * NColetoresDEst(): Calcula o número de coletores numa tabela de dispersão estática
 *
 * Parâmetros:
 *      stream (entrada) - stream associado ao arquivo que contém os coletores
 *
 * Retorno: O número de coletores na tabela de dispersão
 *
 ****/
int NColetoresDEst(FILE *stream)
{
   int          n = 0;
   tColetorDEst umColetor;

   rewind(stream); /* Assegura que a leitura começa no início do arquivo */

   while (1) {
      fread(&umColetor, sizeof(umColetor), 1, stream); /* Lê um coletor */

         /* Se ocorreu erro de leitura ou o final  */
         /* do arquivo foi atingido encerra o laço */
      if (feof(stream) || ferror(stream))
         break;

      ++n;
   }

      /* Verifica se ocorreu erro de leitura */
   ASSEGURA( !ferror(stream), "Erro de leitura em NColetoresDEst" );

   return n;
}

/****
 *
 * NColetoresVaziosDEst(): Calcula o número de coletores vazios
 *                         numa tabela de dispersão estática
 *
 * Parâmetros:
 *      stream (entrada) - stream associado ao arquivo que contém os coletores
 *
 * Retorno: O número de coletores vazios
 *
 ****/
int NColetoresVaziosDEst(FILE *stream)
{
   int          n = 0;
   tColetorDEst umColetor;

   rewind(stream); /* Assegura que a leitura começa no início do arquivo */

   while (1) {
      fread(&umColetor, sizeof(umColetor), 1, stream); /* Lê um coletor */

         /* Se ocorreu erro de leitura ou o final  */
         /* do arquivo foi atingido encerra o laço */
      if (feof(stream) || ferror(stream))
         break;

      if (!umColetor.nRegistros)
         ++n;
   }

      /* Verifica se ocorreu erro de leitura */
   ASSEGURA( !ferror(stream), "Erro de leitura em NColetoresVaziosDEst" );

   return n;
}

/****
 *
 * NColetoresRepletosDEst(): Calcula o número de coletores repletos
 *                       numa tabela de dispersão estática
 *
 * Parâmetros:
 *      stream (entrada) - stream associado ao arquivo que contém os coletores
 *
 * Retorno: O número de coletores repletos
 *
 ****/
int NColetoresRepletosDEst(FILE *stream)
{
   int      n = 0;
   tColetorDEst umColetor;

      /* Assegura que a leitura começa no início do arquivo */
   rewind(stream);

   while (1) {
         /* Lê um coletor */
      fread(&umColetor, sizeof(umColetor), 1, stream);

         /* Se ocorreu erro de leitura ou o final  */
         /* do arquivo foi atingido encerra o laço */
      if (feof(stream) || ferror(stream))
         break;

      if (umColetor.nRegistros == M)
         ++n;
   }

      /* Verifica se ocorreu erro de leitura */
   ASSEGURA( !ferror(stream), "Erro de leitura em NColetoresRepletosDEst" );

   return n;
}

/****
 *
 * NColetoresOcupadosDEst(): Calcula o número de coletores ocupados
 *                           numa tabela de dispersão estática
 *
 * Parâmetros:
 *      stream (entrada) - stream associado ao arquivo que contém os coletores
 *
 * Retorno: O número de coletores ocupados
 *
 ****/
int NColetoresOcupadosDEst(FILE *stream)
{
   int          n = 0;
   tColetorDEst umColetor;

   rewind(stream); /* Assegura que a leitura começa no início do arquivo */

   while (1) {
      fread(&umColetor, sizeof(umColetor), 1, stream); /* Lê um coletor */

         /* Se ocorreu erro de leitura ou o final  */
         /* do arquivo foi atingido encerra o laço */
      if (feof(stream) || ferror(stream))
         break;

      if (umColetor.nRegistros)
         ++n;
   }

      /* Verifica se ocorreu erro de leitura */
   ASSEGURA( !ferror(stream), "Erro de leitura em NColetoresOcupadosDEst" );

   return n;
}

/****
 *
 * NColetoresExcedentesDEst(): Calcula o número de coletores excedentes
 *                         numa tabela de dispersão estática
 *
 * Parâmetros:
 *      nColetores (entrada) - número de coletores primários
 *      stream (entrada) - stream associado ao arquivo que contém os coletores
 *
 * Retorno: O número de coletores excedentes
 *
 ****/
int NColetoresExcedentesDEst(int nColetores, FILE *stream)
{
   int          i, n = 0;
   tColetorDEst coletor;

      /* Para cada coletor primário, conta quantos coletores excedentes ele possui */
   for (i = 0; i < nColetores; ++i) {
      LeColetorDEst(stream, i, &coletor);

         /* Conta os coletores excedentes do coletor primário corrente  */
      while (coletor.proximo != POSICAO_NULA) {
          ++n;

          LeColetorDEst(stream, coletor.proximo, &coletor);
      }
   }

   return n;
}

/****
 *
 * MaiorCadeiaExcedentesDEst(): Calcula o tamanho da maior cadeia de
 *                              coletores excedentes
 *
 * Parâmetros:
 *      nColetores (entrada) - número de coletores primários
 *      stream (entrada) - stream associado ao arquivo que contém os coletores
 *
 * Retorno: O número de coletores excedentes
 *
 ****/
int MaiorCadeiaExcedentesDEst(int nColetores, FILE *stream)
{
   int          i, tamCadeia, maiorCadeia = 0;
   tColetorDEst coletor;

   for (i = 0; i < nColetores; ++i) {
      LeColetorDEst(stream, i, &coletor);

      tamCadeia = 0;

      while (coletor.proximo != POSICAO_NULA) {
          ++tamCadeia;

          LeColetorDEst(stream, coletor.proximo, &coletor);
      }

      if (tamCadeia > maiorCadeia)
         maiorCadeia = tamCadeia;
   }

   return maiorCadeia;
}

