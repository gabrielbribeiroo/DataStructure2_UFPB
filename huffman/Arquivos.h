/****
 *
 * Arquivo Arquivos.h: Interface do m�dulo de processamento
 *                     de arquivos
 *
 * Autor: Ulysses de Oliveira
 *
 * Data de Cria��o: 20/04/2004
 * �ltima modifica��o: 19/07/2008
 *
 ****/

#ifndef _Arquivos_H_  /* Previne inclus�o       */
#define _Arquivos_H_  /* m�ltipla deste arquivo */

/*********************** Includes *************************/

#include <stdio.h>

/****************** Defini��es de Tipos *******************/


/******************* Alus�es de Fun��es *******************/

extern FILE *AbreArquivo(const char *nome, const char *modo);
extern void FechaArquivo(FILE *stream, const char *nome);
extern void MoveApontador(FILE *stream, long bytes, int DeOnde);
extern int ArquivoExiste(const char *nomeArq);

#endif

