/****
 *
 * Arquivo: ASSEGURA.h
 *
 * Autor: Gabriel Ribeiro
 *
 * Descri��o: Define a macro ASSEGURA utilizada em
 *            tratamento de condi��es de exce��o
 *
 ****/

   /* Previne inclus�o m�ltipla deste arquivo */
#ifndef _ASSEGURA_H_
#define _ASSEGURA_H_

/*********************** Includes *************************/

#include <stdio.h>   /* fprintf() */
#include <stdlib.h>  /* exit()    */

/************************* Macros *************************/

   /* Macro utilizada em tratamento */
   /* de condi��es de exce��o       */
#define ASSEGURA(condicao, msg) if (!(condicao)) {\
                                   fprintf(stderr, "\n%s\n", msg);\
                                   exit(1); \
                                }

   /* As seguintes macros s�o usadas em conjunto com a macro  */
   /* ASSEGURA para imprimir mensagens de erro indicando o    */
   /* tipo de erro e em que fun��o do programa o erro ocorreu */
#define ERRO_SEEK(f) \
        "ERRO: tentando mover apontador de arquivo na funcao " #f
#define ERRO_OPEN(f) \
        "ERRO: Nao foi possivel abrir o arquivo na funcao " #f
#define ERRO_LE_NO(f) \
        "ERRO: Nao foi possivel ler um no' na funcao " #f
#define ERRO_FWRITE(f) \
        "ERRO: Nao foi possivel gravar arquivo na funcao " #f
#define ERRO_FREAD(f) \
        "ERRO: Nao foi possivel ler arquivo na funcao " #f

#endif