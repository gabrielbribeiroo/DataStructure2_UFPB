/****
 *
 * Arquivo: ASSEGURA.h
 *
 * Autor: Ulysses de Oliveira
 *
 * Data de Cria��o: 10/04/2013
 * �ltima modifica��o: 10/04/2013
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
   /* ASSEGURA para exibir mensagens de erro indicando o      */
   /* tipo de erro e em que fun��o do programa o erro ocorreu */
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
        "ERRO: posicao invalida em " #f "()"
#define ERRO_LE_NO(f) \
        "ERRO: Nao foi possivel ler um no' na funcao " #f "\n"
#define ERRO_ESCREVE_NO(f) \
        "ERRO: Nao foi possivel escrever um no' na funcao " #f "\n"
#define ERRO_INSERE_REGISTRO(f) \
        "ERRO: Nao foi poss�vel inserir registro em arquivo na funcao " #f "\n"
#define ERRO_INCONSISTENCIA(f) \
        "ERRO: Foi detectada uma inconsistencia na funcao " #f "\n"

#endif
