#ifndef _TABELAENCADEADA_H_
#define _TABELAENCADEADA_H_

#include "tipoFilme.h"

typedef tFilme tDado;
typedef struct rotTabEnc tTabelaEnc;

extern void CriaTabelaEnc(tTabelaEnc **tabela);
extern void DestroiTabelaEnc(tTabelaEnc * tabela);
extern int ComprimentoEnc(tTabelaEnc * tabela);
extern tDado * RemoveElementoEnc(tTabelaEnc ** tabela, int id);
extern int EstaVaziaEnc(tTabelaEnc * tabela);
extern void AcrescentaElementoEnc(tTabelaEnc ** tabela, const tDado *elemento);
extern tDado * BuscaSequencialEnc(tTabelaEnc *tab, int chave);
extern tDado * BuscaDedilhadaEnc(tTabelaEnc *tab, int chave);
extern tDado * BuscaTransposicaoEnc(tTabelaEnc **tab, int chave);
extern tDado * BuscaMovimentacaoInicioEnc(tTabelaEnc **tab, int chave);
extern int QuantOperacoesEnc();
#endif