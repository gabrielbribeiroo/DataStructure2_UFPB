/****
 *
 * Arquivo Macros.h
 *
 * Autor: Ulysses de Oliveira
 *
 * Data de Cria��o: 07/06/2010
 * �ltima modifica��o: 07/06/2010
 *
 * Descri��o: Macros utilizadas pelo programa de dispers�o
 *            com encadeamento
 *
 ****/

#ifndef _Macros_H_   /* Previne inclus�o m�ltipla deste arquivo */
#define _Macros_H_

#include <stdlib.h> /* Fun��o exit()   */
#include <stdio.h>  /* Fun��o printf() */

   /* Macro utilizada em tratamento */
   /* de condi��es de exce��o       */
#define ASSEGURA(condicao, msg) if (!(condicao)) {\
                                   fprintf(stderr, "\n%s\n", msg);\
                                   exit(1); \
                                }

   /* Fator de carga da tabela de dispers�o */
#define FATOR_DE_CARGA 0.8

   /* Nome do arquivo de dados */
#define NOME_ARQUIVO_BIN "I:\\Dados\\CEPs\\CEPs.bin"

   /* Nome do arquivo contendo as chaves da tabela de busca */
#define NOME_ARQUIVO_CHAVES "ListaDeChaves.txt"

   /* Nome do arquivo que armazena o n�mero */
   /* de chaves em cada cont�iner           */
#define NOME_ARQ_CHAVES  "ChavesEmConteineres.txt"

#define MAX_NOME  40 /* N�mero m�ximo de caracteres num nome */
#define TAM_UF     2 /* N�mero de d�gitos num UF */
#define TAM_CEP    8 /* N�mero de d�gitos num CEP */
#define MAX_COMP  60 /* N�mero m�ximo de caracteres */
                     /* num complemento             */
#define MAX_TIPO_LOG 10 /* N�mero m�ximo de caracteres */
                        /* num tipo de logradouro      */
#define TAM_DNE   16 /* N�mero de d�gitos numa chave DNE */

   /* Tamanho do acr�scimo do bloco usado para conter */
   /* uma linha a cada chamada de realloc()           */
#define TAMANHO_BLOCO 256

#endif
