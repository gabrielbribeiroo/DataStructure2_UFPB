/****
 *
 * Arquivo Arquivos.c: Arquivo de programa do módulo de processamento de registros
 *
 * Autor: Ulysses de Oliveira
 *
 * Data de Criação: 20/04/2004
 * Última modificação: 30/03/2017
 *
 ****/

/*********************** Includes *************************/

#include <stdio.h>  /* Funções de Entrada e Saída   */
#include <string.h> /* Processamento de strings     */
#include <stdlib.h> /* strtod() e alocação dinâmica */
#include "Arquivos.h" /* Interface deste módulo    */
#include "ASSEGURA.h" /* Macro ASSEGURA             */

/********************* Funções Globais ********************/

/****
 *
 * AbreArquivo(): Tenta abrir um arquivo e, quando isso
 *                não é possível, aborta o programa
 *
 * Parâmetros:
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
 *                 não é possível, aborta o programa
 *
 * Parâmetros:
 *      stream (entrada) - stream associado ao arquivo
 *      nomeArq (entrada) - nome do arquivo ou NULL
 *
 * Retorno: Nada
 *
 * Observação: Se o segundo parâmetro não for NULL, o nome
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
 * MoveApontador(): Tenta mover o apontador de posição de um
 *                  arquivo e, quando isso não é possível, aborta o programa
 *
 * Parâmetros:
 *      stream (entrada) - stream associado ao arquivo
 *      bytes (entrada) - quantidade de bytes que serão saltados
 *      DeOnde (entrada) - a partir de onde ocorrerá a movimentação
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
 * Parâmetros:
 *     nomeArq (entrada) - nome do arquivo que se verificará se existe
 *
 * Retorno: 1, se o arquivo existir; 0, em caso contrário
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
