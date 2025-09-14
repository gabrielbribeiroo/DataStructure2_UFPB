/****
 *
 * Arquivo Arquivos.h: Interface do módulo de processamento
 *                     de arquivos
 *
 * Autor: Ulysses de Oliveira
 *
 * Data de Criação: 20/04/2004
 * Última modificação: 19/07/2008
 *
 ****/

#ifndef _Arquivos_H_  /* Previne inclusão       */
#define _Arquivos_H_  /* múltipla deste arquivo */

/*********************** Includes *************************/

#include <stdio.h>

/****************** Definições de Tipos *******************/


/******************* Alusões de Funções *******************/

extern FILE *AbreArquivo(const char *nome, const char *modo);
extern void FechaArquivo(FILE *stream, const char *nome);
extern void MoveApontador(FILE *stream, long bytes, int DeOnde);
extern int ArquivoExiste(const char *nomeArq);

#endif

