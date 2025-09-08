/****
 *
 * Arquivo TabelaDE.h
 *
 * Autor: Ulysses de Oliveira
 *
 * Data de Criação: 07/06/2010
 * Última modificação: 07/06/2010
 *
 * Descrição: Iterface das funções que implementam operações
 *            de busca, inserção e remoção usando dispersão
 *            com encadeamento
 *
 ****/

#ifndef _TabelaDE_H_ /* Previne inclusão múltipla deste arquivo */
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
