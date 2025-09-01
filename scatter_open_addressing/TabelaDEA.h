/****
 *
 * Arquivo TabelaDEA.h
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

#ifndef _TabelaDEA_H_ /* Previne inclusão múltipla deste arquivo */
#define _TabelaDEA_H_

#include "Tipos.h"    /* Tipos usados pelo programa */

extern tTabelaDEA CriaTabelaDEA(int nElementos);
extern int BuscaDEA( tTabelaDEA tabela, int tamanhoTab,
                     tCEP chave, tFDispersaoDEA fDispersao );
extern int InsereDEA( tTabelaDEA tabela, int tamanhoTab,
                      tCEP_Ind chaveEIndice,
                      tFDispersaoDEA fDispersao );
extern int RemoveDEA( tTabelaDEA tabela, int tamanhoTab,
                      tCEP chave, tFDispersaoDEA fDispersao );
extern int NChavesDEA(tTabelaDEA tabela, int tamanhoTab);
extern int NColetoresVaziosDEA(tTabelaDEA tabela, int tamanhoTab);
extern int NChavesRemovidasDEA(tTabelaDEA tabela, int tamanhoTab);
extern void DestroiTabelaDEA(tTabelaDEA *tabela);

#endif
