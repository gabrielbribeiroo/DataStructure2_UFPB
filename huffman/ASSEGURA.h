/****
 *
 * Arquivo ASSEGURA.h
 *
 * Autor: Ulysses de Oliveira
 *
 * Data de Cria��o: 07/06/2016
 * �ltima modifica��o: 07/06/2016
 *
 * Descri��o: Defini��o da macro utilizada em tratamento de
 *            exce��es
 *
 ****/

#ifndef _ASSEGURA_H_   /* Previne inclus�o m�ltipla deste arquivo */
#define _ASSEGURA_H_

#include <stdlib.h> /* Fun��o exit()   */
#include <stdio.h>  /* Fun��o printf() */

   /* Macro utilizada em tratamento */
   /* de condi��es de exce��o       */
#define ASSEGURA(condicao, msg) if (!(condicao)) {\
                                   fprintf(stderr, "\n%s\n", msg);\
                                   exit(1); \
                                }

#endif
