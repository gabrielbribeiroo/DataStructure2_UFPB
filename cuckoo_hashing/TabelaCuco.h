/****
 *
 * Arquivo TabelaCuco.h
 *
 * Autor: Ulysses de Oliveira
 *
 * Data de Criação: 07/06/2010
 * Última modificação: 07/06/2010
 *
 * Descrição: Iterface das funções que implementam operações
 *            de busca, inserção e remoção usando dispersão
 *            com endereçamento aberto
 *
 ****/

#ifndef _TabelaCuco_H_ /* Previne inclusão múltipla deste arquivo */
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
