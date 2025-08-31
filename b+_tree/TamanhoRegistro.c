/****
 *
 * Título: Tamanho de estrutura com e sem preenchimento
 *
 * Autor: Ulysses de Oliveira
 *
 * Data de Criação: 16/09/2017
 * Última modificação: 16/09/2017
 *
 * Descrição: Calcula o tamanho de uma estrutura do tipo tRegistro
 *            com e sem preenchimento
 *
 * Entrada: Nada
 *
 * Saída: Tamanho de uma estrutura com e sem preenchimento
 *
 ****/

#include <stdio.h>     /* Funções de Entrada/Saída */

#define MAX_NOME_IES     200 /* Tamanho máximo de */
                             /* um nome de IES    */
#define MAX_NOME_CURSO   200 /* Tamanho máximo de */
                             /* um nome de curso  */

#define MAX_UF_NASCIMENTO 30 /* Tamanho máximo   */
                             /* de um nome de UF */

   /* Tipo usado para armazenamento de */
   /* registros no arquivo binário     */
typedef struct {
           long codigoAluno;
           long chave; /* Chave */
           char nomeIES[MAX_NOME_IES];
           char nomeCurso[MAX_NOME_CURSO];
           char sexoAluno;
           int  idadeAluno;
           char UFNascimento[MAX_UF_NASCIMENTO];
           int  anoIngresso;
         } tRegistro;

/****
 *
 * main(): Calcula o tamanho de uma estrutura do tipo tRegistro
 *         com e sem preenchimento
 *
 * Parâmetros: Nenhum
 *
 * Retorno: Zero
 *
 ****/
int main(void)
{
   int ts, /* Tamanho sem preenchimento */
       tc; /* Tamanho com preenchimento */

   ts = sizeof(long)        /* codigoAluno  */
        + sizeof(long)      /* Chave        */
        + MAX_NOME_IES      /* nomeIES      */
        + MAX_NOME_CURSO    /* nomeCurso    */
        + MAX_UF_NASCIMENTO /* UFNascimento */
        + 1                 /* sexoAluno    */
        + sizeof(int)       /* idadeAluno   */
        + sizeof(int);      /* anoIngresso  */

   printf("\n>>> Tamanho de no' SEM preenchimento: %d bytes", ts);

   tc = sizeof(tRegistro);
   printf("\n>>> Tamanho de no' COM preenchimento: %d bytes\n", tc);

   printf("\n>>> Tamanho do preenchimento: %d bytes\n\n", tc - ts);

   return 0;
}

/******************* Exemplo de Execução ******************

GCC/Windows:
===========

>>> Tamanho de no' SEM preenchimento: 447 bytes
>>> Tamanho de no' COM preenchimento: 452 bytes

>>> Tamanho do preenchimento: 5 bytes

4.8.5 (SUSE Linux):
==================

>>> Tamanho de no' SEM preenchimento: 455 bytes
>>> Tamanho de no' COM preenchimento: 464 bytes

>>> Tamanho do preenchimento: 9 bytes

**********************************************************/

