/****
 *
 * Arquivo TabelaDE.h
 *
 * Autor: Ulysses de Oliveira
 *
 * Data de Cria��o: 07/06/2010
 * �ltima modifica��o: 07/06/2010
 *
 * Descri��o: Iterface das fun��es que implementam opera��es
 *            de busca, inser��o e remo��o usando dispers�o
 *            com encadeamento
 *
 ****/

#ifndef _TabelaDE_H_ /* Previne inclus�o m�ltipla deste arquivo */
#define _TabelaDE_H_

#include "Tipos.h"    /* Tipos usados pelo programa */

extern tTabelaDE CriaTabelaDE(int nElementos);
extern int BuscaDE( tTabelaDE tabela, int tamTabela,
                    tCEP chave, tFDispersao fDispersao );
extern void InsereDE( tTabelaDE tabela, int tamTabela, const tCEP_Ind *conteudo,
                      tFDispersao fDispersao );
extern int RemoveDE( tTabelaDE tabela, int tamTabela, tCEP chave,
                     tFDispersao fDispersao );

extern void NChavesEmListasDE( tTabelaDE tabela, int tamTabela, FILE *stream );
extern int NChavesDE(tTabelaDE tabela, int tamTabela);
extern int NListasNaoVaziasDE( tTabelaDE tabela, int tamTabela );
extern void DestroiTabelaDE(tTabelaDE *tabela, int tamTabela);

#endif
