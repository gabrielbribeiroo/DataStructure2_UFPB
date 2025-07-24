#ifndef _TABELAINDEXADA_H_
#define _TABELAINDEXADA_H_

#include "tipoFilme.h"

typedef tFilme tDados;
typedef struct rotTabIdx tTabelaIdx;

extern tTabelaIdx * CriaTabelaIdx(void);
extern void DestroiTabelaIdx(tTabelaIdx *tabela);
extern int ComprimentoIdx(tTabelaIdx *tabela);
extern tDados ObtemElementoIdx(tTabelaIdx *tabela, int indice);
extern tDados RemoveElementoIdx(tTabelaIdx *tabela, int indice);
extern int EstaVaziaIdx(tTabelaIdx *tabela);
extern void AcrescentaElementoIdx(tTabelaIdx *tabela, const tDados *elemento);

extern int BuscaSequencial(tTabelaIdx *tab, int chave);
extern int BuscaTransposicao(tTabelaIdx *tab, int chave);
extern int BuscaMovimentacaoInicio(tTabelaIdx *tab, int chave);
extern int BuscaDedilhada(tTabelaIdx *tab, int chave);
extern int BuscaBinaria(tTabelaIdx *tab, int chave);
extern int BuscaInterpolacao(tTabelaIdx *tab, int chave);

extern void Ordena(tTabelaIdx *tab);
extern int QuantOperacoes();
#endif