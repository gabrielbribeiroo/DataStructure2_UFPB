/****
 *
 * Arquivo Macros.h
 *
 * Autor: Ulysses de Oliveira
 *
 * Data de Criação: 07/06/2010
 * Última modificação: 07/06/2010
 *
 * Descrição: Macros utilizadas pelo programa de dispersão
 *            com encadeamento
 *
 ****/

#ifndef _Macros_H_   /* Previne inclusão múltipla deste arquivo */
#define _Macros_H_

#include <stdlib.h> /* Função exit()   */
#include <stdio.h>  /* Função printf() */

   /* Macro utilizada em tratamento */
   /* de condições de exceção       */
#define ASSEGURA(condicao, msg) if (!(condicao)) {\
                                   fprintf(stderr, "\n%s\n", msg);\
                                   exit(1); \
                                }

   /* Fator de carga da tabela de dispersão */
#define FATOR_DE_CARGA 0.8

   /* Nome do arquivo de dados */
#define NOME_ARQUIVO_BIN "I:\\Dados\\CEPs\\CEPs.bin"

   /* Nome do arquivo contendo as chaves da tabela de busca */
#define NOME_ARQUIVO_CHAVES "ListaDeChaves.txt"

   /* Nome do arquivo que armazena o número */
   /* de chaves em cada contêiner           */
#define NOME_ARQ_CHAVES  "ChavesEmConteineres.txt"

#define MAX_NOME  40 /* Número máximo de caracteres num nome */
#define TAM_UF     2 /* Número de dígitos num UF */
#define TAM_CEP    8 /* Número de dígitos num CEP */
#define MAX_COMP  60 /* Número máximo de caracteres */
                     /* num complemento             */
#define MAX_TIPO_LOG 10 /* Número máximo de caracteres */
                        /* num tipo de logradouro      */
#define TAM_DNE   16 /* Número de dígitos numa chave DNE */

   /* Tamanho do acréscimo do bloco usado para conter */
   /* uma linha a cada chamada de realloc()           */
#define TAMANHO_BLOCO 256

#endif
