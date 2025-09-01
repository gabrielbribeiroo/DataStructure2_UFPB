/****
 *
 * Arquivo TabelaDEA.h
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

#ifndef _TabelaDEA_H_ /* Previne inclus�o m�ltipla deste arquivo */
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
