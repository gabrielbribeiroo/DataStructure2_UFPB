/****
 *
 * Arquivo DispersaoEstatica.c
 *
 * Autor: Ulysses de Oliveira
 *
 * Data de Cria��o: 04/08/2017
 * �ltima modifica��o: 06/09/2017
 *
 * Descri��o: Implementa��o de opera��es de busca, inser��o e
 *            remo��o usando dispers�o est�tica em mem�ria
 *            secund�ria
 *
 ****/

/*********************** Includes *************************/

#include <limits.h>
#include <stdlib.h>     /* Miscel�nea de fun��es       */
#include <stdio.h>      /* Fun��es de E/S              */
#include <string.h>     /* Fun��es strxxx() e memxxx() */
#include <math.h>       /* Fun��es strxxx() e memxxx() */
#include "DispersaoEstatica.h" /* Interface deste m�dulo */
#include "Registros.h"  /* Processamento de registros  */
#include "ASSEGURA.h"   /* Macro ASSEGURA              */

/********************* Fun��es Locais *********************/

/****
 *
 * FDispersao(): Obt�m o valor de dispers�o de uma chave inteira
 *
 * Par�metros:
 *      chave (entrada) - a chave
 *      nColetores (entrada) - o n�mero de coletores prim�rios
 *
 * Retorno: O valor de dispers�o
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
 * Par�metros:
 *     stream (entrada) - stream associado ao arquivo que cont�m
 *                        os coletores
 *     tipo (entrada) - tipo do coletor que ser� criado
 *
 * Retorno: O �ndice do coletor criado
 *
 ****/
static int NovoColetorDEst(FILE *stream, tTipoDeColetorDEst tipo)
{
   tColetorDEst coletor;
   int          indice;

   coletor.nRegistros = 0;
   coletor.proximo = POSICAO_NULA;
   coletor.tipo = tipo;

      /* Move o apontador de posi��o do arquivo para seu final */
   MoveApontador(stream, 0, SEEK_END);

   indice = ftell(stream)/sizeof(coletor); /* Obt�m o �ndice do coletor */

   fwrite(&coletor, sizeof(coletor), 1, stream); /* Escreve o coletor no arquivo */

   return indice;
}

/****
 *
 * LeColetorDEst(): L� um coletor em arquivo
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
static void LeColetorDEst(FILE *stream, int pos, tColetorDEst *coletor)
{
      /* Tenta mover o apontador de arquivo para o local */
      /* de leitura; se n�o conseguir, aborta o programa */
   MoveApontador(stream, sizeof(tColetorDEst)*pos, SEEK_SET);

   fread(coletor, sizeof(tColetorDEst), 1, stream); /* Efetua a leitura */

      /* Certifica-se que n�o houve erro de leitura */
   ASSEGURA(!ferror(stream),  "Erro de leitura em LeColetorDEst()");
}

/****
 *
 * EscreveColetorDEst(): Escreve um coletor em arquivo
 *
 * Par�metros:
 *     stream (entrada) - stream associado ao arquivo contendo os
 *                        coletores no qual ser� feita a escrita
 *     pos (entrada) - posi��o no arquivo no qual ser� feita a escrita
 *    *coletor (entrada) - coletor que ser� escrito no arquivo
 *
 * Retorno: Nada
 *
 ****/
static void EscreveColetorDEst(FILE *stream, int pos, const tColetorDEst *coletor)
{
      /* Tenta mover o apontador de arquivo para o local */
      /* de escrita; se n�o conseguir, aborta o programa */
   MoveApontador(stream, sizeof(tColetorDEst)*pos, SEEK_SET);

   fwrite(coletor, sizeof(tColetorDEst), 1, stream); /* Efetua a escrita */

      /* Certifica-se que n�o houve erro */
   ASSEGURA(!ferror(stream), "Erro de escrita em EscreveColetorDEst()");
}

/********************* Fun��es Globais ********************/

/****
 *
 * IniciaTabDEst(): Inicia uma tabela de busca com dispers�o est�tica
 *
 * Par�metros:
 *      streamCol (entrada) - stream associado ao arquivo que
 *                            cont�m os coletores
 *      nRegistros (entrada) - n�mero de registros iniciais da tabela
 *      maxRegs (entrada) - n�mero m�ximo de registros num coletor
 *
 * Retorno: O n�mero de coletores prim�rios
 *
 ****/
int IniciaTabDEst( FILE *streamCol, int nRegistros, int maxRegs )
{
   int i,
       nColetores;

   nColetores = nRegistros/maxRegs; /* Obt�m o n�mero de coletores */

      /* Aloca os coletores prim�rios no arquivo */
   for (i = 0; i < nColetores; ++i)
      (void) NovoColetorDEst(streamCol, PRIMARIO);

   return nColetores;
}

/****
 *
 * BuscaDEst(): Efetua uma busca numa tabela de dispers�o est�tica
 *
 * Par�metros:
 *      stream (entrada) - stream associado ao arquivo que cont�m os coletores
 *      nColetores (entrada) - n�mero de coletores prim�rios da tabela
 *      chave (entrada) - a chave de busca
 *      reg (sa�da) - o registro encontrado
 *
 * Retorno: O endere�o do registro, se ele for encontrado; NULL, em caso contr�rio
 *
 ****/
tRegistroMEC *BuscaDEst( FILE *stream, int nColetores, tChave chave,
                         tRegistroMEC *reg )
{
   int          iColetor, i;
   tColetorDEst coletor;

      /* Obt�m o �ndice do coletor prim�rio que pode conter o registro */
   iColetor = FDispersao(chave, nColetores);

      /* Efetua uma busca no coletor prim�rio e em seus coletores excedentes */
   while (iColetor != POSICAO_NULA) {
         /* L� um coletor que pode conter o registro */
      LeColetorDEst(stream, iColetor, &coletor);

         /* Efetua uma busca sequencial pelo registro em mem�ria principal */
      for (i = 0; i < coletor.nRegistros; i++)
            /* Verifica se a chave foi encontrada */
         if (chave == ObtemChave(&coletor.registros[i])) {
            *reg = coletor.registros[i]; /* Chave encontrada */
            return reg; /* Servi�o completo */
         }

      iColetor = coletor.proximo; /* Passa para o pr�ximo coletor excedente */
   }

   return NULL; /* A chave n�o foi encontrada */
}

/****
 *
 * InsereDEst(): Insere um registro numa tabela de busca com dispers�o est�tica
 *
 * Par�metros:
 *      nColetores (entrada) - n�mero de coletores prim�rios da
 *                             tabela na qual ser� efetuada a inser��o
 *      registro (entrada) - o registro que ser� inserido
 *      stream (entrada) - stream associado ao arquivo que cont�m os coletores
 *
 * Retorno: Nada
 *
 ****/
void InsereDEst(int nColetores, const tRegistroMEC *registro, FILE *stream)
{
   int          iColetor;
   tChave       chave;
   tColetorDEst coletor;

   chave = ObtemChave(registro); /* Obt�m a chave do registro */

   iColetor = FDispersao(chave, nColetores); /* Obt�m o �ndice do coletor */

      /* L� o coletor prim�rio em arquivo */
   LeColetorDEst(stream, iColetor, &coletor);

      /* Encontra o coletor no qual ser� efetuada a inser��o */
   while (coletor.nRegistros == M) {
         /* O coletor est� repleto. Portanto passa-se para o    */
         /* pr�ximo coletor excedente. Se o pr�ximo coletor     */
         /* excedente ainda n�o existe, cria-se um novo coletor */
      if (coletor.proximo == POSICAO_NULA) {
            /* Cria um novo coletor excedente */
         coletor.proximo = NovoColetorDEst(stream, EXCEDENTE);

            /* O coletor foi alterado e precisa ser reescrito */
         EscreveColetorDEst(stream, iColetor, &coletor);
      }

         /* Guarda a posi��o do pr�ximo coletor a ser lido */
      iColetor = coletor.proximo;

         /* L� um coletor excedente em arquivo */
      LeColetorDEst(stream, coletor.proximo, &coletor);
   }

      /* Acrescenta o novo registro ao final do coletor */
   coletor.registros[coletor.nRegistros] = *registro;

   coletor.nRegistros++; /* O n�mero de registros no coletor aumentou */

      /* O coletor foi alterado e � preciso reescrev�-lo no arquivo */
   EscreveColetorDEst(stream, iColetor, &coletor);
}

/****
 *
 * RemoveDEst(): Remove um registro de uma tabela de dispers�o est�tica
 *
 * Par�metros:
 *      nColetores (entrada/sa�da) - n�mero de coletores prim�rios
 *                                   da tabela de dispers�o
 *      chave (entrada) - a chave do registro que ser� removido
 *      stream (entrada) - stream associado ao arquivo que cont�m os coletores
 *
 * Retorno: 1, se a remo��o foi ok; 0, caso contr�rio
 *
 ****/
int RemoveDEst(int nColetores, tChave chave, FILE *stream)
{
   int          i, j, iColetor, encontrada = 0;
   tColetorDEst coletor;

      /* Obt�m o �ndice do coletor que pode conter o registro */
   iColetor = FDispersao(chave, nColetores);

      /* Efetua uma busca no coletor prim�rio e em seus coletores excedentes */
   while (iColetor != POSICAO_NULA) {
         /* L� o coletor que pode conter o registro */
      LeColetorDEst(stream, iColetor, &coletor);

         /* Efetua uma busca sequencial pelo registro em mem�ria principal */
      for (i = 0; i < coletor.nRegistros && !encontrada; i++)
            /* Verifica se a chave foi encontrada */
         if (chave == ObtemChave(&coletor.registros[i]))
            encontrada = 1;

         /* Se a chave foi encontrada, encerra a busca */
      if (encontrada)
         break;

      iColetor = coletor.proximo; /* Passa para o pr�ximo coletor excedente */
   }

      /* Verifica se a chave foi encontrada */
   if (!encontrada)
      return 0;  /* A chave n�o foi encontrada */

      /* Corrige o valor de i que foi incrementado e deixou */
      /* de ser o �ndice do elemento a ser removido         */
   --i;

      /* O registro a ser removido encontra-se na posi��o i do */
      /* coletor corrente. � necess�rio mover uma posi��o para */
      /* tr�s todos os registros que se encontram � sua frente */
   for (j = i; j < coletor.nRegistros - 1; ++j)
      coletor.registros[j] = coletor.registros[j + 1];

   --coletor.nRegistros; /* O n�mero de registros no coletor diminuiu */

      /* O coletor foi alterado e � preciso reescrev�-lo no arquivo */
   EscreveColetorDEst(stream, iColetor, &coletor);

   return 1;  /* Remo��o bem-sucedida */
}

/****
 *
 * SubstituiDEst(): Substitui o conte�do de um registro que
 *                      tem uma determinada chave
 *
 * Par�metros:
 *      stream (entrada) - stream associado ao arquivo que cont�m os coletores
 *      nColetores (entrada) - n�mero de coletores prim�rios da tabela
 *      reg (entrada) - o registro substituto
 *
 * Retorno: Nada
 *
 * Observa��es:
 *      1. A chave deve ser prim�ria
 *      2. Se a chave n�o for encontrada, o programa ser� abortado
 *
 ****/
void SubstituiDEst( FILE *stream, int nColetores, const tRegistroMEC *reg )
{
   int          iColetor, i;
   tColetorDEst coletor;
   tChave       chave = ObtemChave(reg);

      /* Obt�m o �ndice do coletor prim�rio que pode conter o registro */
   iColetor = FDispersao(chave, nColetores);

      /* Efetua uma busca no coletor prim�rio e em seus coletores excedentes */
   while (iColetor != POSICAO_NULA) {
         /* L� um coletor que pode conter o registro */
      LeColetorDEst(stream, iColetor, &coletor);

         /* Efetua uma busca sequencial pelo registro em mem�ria principal */
      for (i = 0; i < coletor.nRegistros; i++)
            /* Verifica se a chave foi encontrada */
         if (chave == ObtemChave(&coletor.registros[i])) {
               /* A chave encontrada. Altera o conte�do do registro que a cont�m */
            coletor.registros[i] = *reg;

            return; /* Servi�o completo */
         }

      iColetor = coletor.proximo; /* Passa para o pr�ximo coletor excedente */
   }

      /* A chave n�o foi encontrada */
   ASSEGURA(0, "A chave deveria ter sido encontrada");
}

/****
 *
 * NRegistrosTabDEst(): Calcula o n�mero de registros numa tabela de dispers�o est�tica
 *
 * Par�metros:
 *      stream (entrada) - stream associado ao arquivo que cont�m os coletores
 *
 * Retorno: O n�mero de registros na tabela de dispers�o
 *
 ****/
int NRegistrosTabDEst(FILE *stream)
{
   int          nRegistros = 0;
   tColetorDEst umColetor;

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
   ASSEGURA( !ferror(stream), "Erro de leitura em NRegistrosTabDEst" );

   return nRegistros;
}

/****
 *
 * NColetoresDEst(): Calcula o n�mero de coletores numa tabela de dispers�o est�tica
 *
 * Par�metros:
 *      stream (entrada) - stream associado ao arquivo que cont�m os coletores
 *
 * Retorno: O n�mero de coletores na tabela de dispers�o
 *
 ****/
int NColetoresDEst(FILE *stream)
{
   int          n = 0;
   tColetorDEst umColetor;

   rewind(stream); /* Assegura que a leitura come�a no in�cio do arquivo */

   while (1) {
      fread(&umColetor, sizeof(umColetor), 1, stream); /* L� um coletor */

         /* Se ocorreu erro de leitura ou o final  */
         /* do arquivo foi atingido encerra o la�o */
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
 * NColetoresVaziosDEst(): Calcula o n�mero de coletores vazios
 *                         numa tabela de dispers�o est�tica
 *
 * Par�metros:
 *      stream (entrada) - stream associado ao arquivo que cont�m os coletores
 *
 * Retorno: O n�mero de coletores vazios
 *
 ****/
int NColetoresVaziosDEst(FILE *stream)
{
   int          n = 0;
   tColetorDEst umColetor;

   rewind(stream); /* Assegura que a leitura come�a no in�cio do arquivo */

   while (1) {
      fread(&umColetor, sizeof(umColetor), 1, stream); /* L� um coletor */

         /* Se ocorreu erro de leitura ou o final  */
         /* do arquivo foi atingido encerra o la�o */
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
 * NColetoresRepletosDEst(): Calcula o n�mero de coletores repletos
 *                       numa tabela de dispers�o est�tica
 *
 * Par�metros:
 *      stream (entrada) - stream associado ao arquivo que cont�m os coletores
 *
 * Retorno: O n�mero de coletores repletos
 *
 ****/
int NColetoresRepletosDEst(FILE *stream)
{
   int      n = 0;
   tColetorDEst umColetor;

      /* Assegura que a leitura come�a no in�cio do arquivo */
   rewind(stream);

   while (1) {
         /* L� um coletor */
      fread(&umColetor, sizeof(umColetor), 1, stream);

         /* Se ocorreu erro de leitura ou o final  */
         /* do arquivo foi atingido encerra o la�o */
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
 * NColetoresOcupadosDEst(): Calcula o n�mero de coletores ocupados
 *                           numa tabela de dispers�o est�tica
 *
 * Par�metros:
 *      stream (entrada) - stream associado ao arquivo que cont�m os coletores
 *
 * Retorno: O n�mero de coletores ocupados
 *
 ****/
int NColetoresOcupadosDEst(FILE *stream)
{
   int          n = 0;
   tColetorDEst umColetor;

   rewind(stream); /* Assegura que a leitura come�a no in�cio do arquivo */

   while (1) {
      fread(&umColetor, sizeof(umColetor), 1, stream); /* L� um coletor */

         /* Se ocorreu erro de leitura ou o final  */
         /* do arquivo foi atingido encerra o la�o */
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
 * NColetoresExcedentesDEst(): Calcula o n�mero de coletores excedentes
 *                         numa tabela de dispers�o est�tica
 *
 * Par�metros:
 *      nColetores (entrada) - n�mero de coletores prim�rios
 *      stream (entrada) - stream associado ao arquivo que cont�m os coletores
 *
 * Retorno: O n�mero de coletores excedentes
 *
 ****/
int NColetoresExcedentesDEst(int nColetores, FILE *stream)
{
   int          i, n = 0;
   tColetorDEst coletor;

      /* Para cada coletor prim�rio, conta quantos coletores excedentes ele possui */
   for (i = 0; i < nColetores; ++i) {
      LeColetorDEst(stream, i, &coletor);

         /* Conta os coletores excedentes do coletor prim�rio corrente  */
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
 * Par�metros:
 *      nColetores (entrada) - n�mero de coletores prim�rios
 *      stream (entrada) - stream associado ao arquivo que cont�m os coletores
 *
 * Retorno: O n�mero de coletores excedentes
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

