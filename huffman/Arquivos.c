/****
 *
 * Arquivo Arquivos.c: Arquivo de programa do m�dulo de processamento de registros
 *
 * Autor: Ulysses de Oliveira
 *
 * Data de Cria��o: 20/04/2004
 * �ltima modifica��o: 30/03/2017
 *
 ****/

/*********************** Includes *************************/

#include <stdio.h>  /* Fun��es de Entrada e Sa�da   */
#include <string.h> /* Processamento de strings     */
#include <stdlib.h> /* strtod() e aloca��o din�mica */
#include "Arquivos.h" /* Interface deste m�dulo    */
#include "ASSEGURA.h" /* Macro ASSEGURA             */

/********************* Fun��es Globais ********************/

/****
 *
 * AbreArquivo(): Tenta abrir um arquivo e, quando isso
 *                n�o � poss�vel, aborta o programa
 *
 * Par�metros:
 *      nome (entrada) - nome do arquivo
 *      modo (entrada) - modo de abertura
 *
 * Retorno: Stream associado ao arquivo que foi aberto
 *
 ****/
FILE *AbreArquivo(const char *nome, const char *modo)
{
   FILE *stream;

   stream = fopen(nome, modo);

   if (!stream) { /* Erro de abertura */
      fprintf( stderr, "\n>>> O arquivo %s nao pode ser aberto",
               nome);
      exit(1); /* Aborta o programa */
   }

   return stream;
}

/****
 *
 * FechaArquivo(): Tenta fechar um arquivo e, quando isso
 *                 n�o � poss�vel, aborta o programa
 *
 * Par�metros:
 *      stream (entrada) - stream associado ao arquivo
 *      nomeArq (entrada) - nome do arquivo ou NULL
 *
 * Retorno: Nada
 *
 * Observa��o: Se o segundo par�metro n�o for NULL, o nome
 *             do arquivo aparece na mensagem de erro
 *
 ****/
void FechaArquivo(FILE *stream, const char *nomeArq)
{
   /***********************************************************/
   /* Se fclose() retornar um valor diferente de zero ocorreu */
   /* algum erro na tentativa de fechamento do arquivo. Nesse */
   /* caso, apresenta mensagem de erro e aborta o programa.   */
   /***********************************************************/

   if (fclose(stream)) { /* Erro de fechamento */
      fprintf( stderr, "\a\n>>> Ocorreu erro no fechamento "
              "do arquivo %s.\n>>> O programa sera' "
              "encerrado.\n", nomeArq ? nomeArq : "");
      exit(1); /* Aborta o programa */
   }
}

/****
 *
 * MoveApontador(): Tenta mover o apontador de posi��o de um
 *                  arquivo e, quando isso n�o � poss�vel, aborta o programa
 *
 * Par�metros:
 *      stream (entrada) - stream associado ao arquivo
 *      bytes (entrada) - quantidade de bytes que ser�o saltados
 *      DeOnde (entrada) - a partir de onde ocorrer� a movimenta��o
 *
 * Retorno: Nada
 *
 ****/
void MoveApontador(FILE *stream, long bytes, int DeOnde)
{
   int deuErrado;

   deuErrado = fseek(stream, (long) bytes, DeOnde);

      /* Verifica se houve erro */
   if (deuErrado) {
      fprintf(stderr, "\n>>> Erro na tentativa de mover apontador de arquivo");
      exit(1);
   }
}

/****
 *
 * ArquivoExiste(): Verifica se um arquivo existe
 *
 * Par�metros:
 *     nomeArq (entrada) - nome do arquivo que se verificar� se existe
 *
 * Retorno: 1, se o arquivo existir; 0, em caso contr�rio
 *
 ****/
int ArquivoExiste(const char *nomeArq)
{
   FILE *stream;
   int   existe;

   stream = fopen(nomeArq, "rb");

   existe = stream ? 1 : 0;

   if (existe)
      FechaArquivo(stream, nomeArq);

   return existe;
}
