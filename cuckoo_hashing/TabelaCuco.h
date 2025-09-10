/****
 *
 * Arquivo TabelaCuco.h
 *
 * Autor: Ulysses de Oliveira
 *
 * Data de Cria��o: 07/06/2010
 * �ltima modifica��o: 07/06/2010
 *
 * Descri��o: Iterface das fun��es que implementam opera��es
 *            de busca, inser��o e remo��o usando dispers�o
 *            com endere�amento aberto
 *
 ****/

#ifndef _TabelaCuco_H_ /* Previne inclus�o m�ltipla deste arquivo */
#define _TabelaCuco_H_

#include "Tipos.h"    /* Tipos usados pelo programa */

extern void CriaTabelaCuco( tTabelaCuco *tabelas, int nElementos,
                            tFDispersaoCuco fD1, tFDispersaoCuco fD2 );
extern int BuscaCuco( tTabelaCuco tabela, tCEP chave );
extern int InsereCuco( tTabelaCuco *tabela, tCEP_Ind chaveEIndice );
extern int RemoveCuco(tTabelaCuco *tabela, tCEP chave);
extern int NChavesCuco(tTabelaCuco tabela);
extern int NElementosVaziosCuco(tTabelaCuco tabela);
extern void DestroiTabelaCuco(tTabelaCuco *tabela);

#endif
