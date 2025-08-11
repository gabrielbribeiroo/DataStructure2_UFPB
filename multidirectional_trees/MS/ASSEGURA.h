/****
 *
 * Arquivo: ASSEGURA.h
 *
 * Autor: Ulysses de Oliveira
 *
 * Data de Criação: 10/04/2013
 * Última modificação: 10/04/2013
 *
 * Descrição: Define a macro ASSEGURA utilizada em
 *            tratamento de condições de exceção
 *
 ****/

   /* Previne inclusão múltipla deste arquivo */
#ifndef _ASSEGURA_H_
#define _ASSEGURA_H_

/*********************** Includes *************************/

#include <stdio.h>   /* fprintf() */
#include <stdlib.h>  /* exit()    */

/************************* Macros *************************/

   /* Macro utilizada em tratamento */
   /* de condições de exceção       */
#define ASSEGURA(condicao, msg) if (!(condicao)) {\
                                   fprintf(stderr, "\n%s\n", msg);\
                                   exit(1); \
                                }

   /* As seguintes macros são usadas em conjunto com a macro  */
   /* ASSEGURA para exibir mensagens de erro indicando o      */
   /* tipo de erro e em que função do programa o erro ocorreu */
#define ERRO_OPEN(f) \
        "ERRO: Impossivel abrir o arquivo em " #f "()"
#define ERRO_SEEK(f) \
        "ERRO: Impossivel mover apontador de arquivo em " #f "()"
#define ERRO_TELL(f) \
        "ERRO: Impossivel obter apontador de arquivo em " #f "()"
#define ERRO_FWRITE(f) \
        "ERRO: Impossivel escrever em arquivo em " #f "()"
#define ERRO_FREAD(f) \
        "ERRO: Impossivel ler arquivo em " #f "()"
#define ERRO_STREAM_NULL(s, f) \
        "ERRO: Stream " #s "' e' NULL em " #f "()"
#define ERRO_POSICAO(f) \
        "ERRO: Posicao invalida em " #f "()"

#endif
