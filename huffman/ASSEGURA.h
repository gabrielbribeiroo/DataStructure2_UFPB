/****
 *
 * Arquivo ASSEGURA.h
 *
 * Autor: Ulysses de Oliveira
 *
 * Data de Criação: 07/06/2016
 * Última modificação: 07/06/2016
 *
 * Descrição: Definição da macro utilizada em tratamento de
 *            exceções
 *
 ****/

#ifndef _ASSEGURA_H_   /* Previne inclusão múltipla deste arquivo */
#define _ASSEGURA_H_

#include <stdlib.h> /* Função exit()   */
#include <stdio.h>  /* Função printf() */

   /* Macro utilizada em tratamento */
   /* de condições de exceção       */
#define ASSEGURA(condicao, msg) if (!(condicao)) {\
                                   fprintf(stderr, "\n%s\n", msg);\
                                   exit(1); \
                                }

#endif
