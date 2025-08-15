#ifndef _ARVOREB_H_
#define _ARVOREB_H_

#define POSICAO_NULA -1

#include <stdio.h>

typedef int tChave;

typedef struct  {
          tChave chave;  /* Chave do registro              */
          int    indice; /* Posição do registro no arquivo de registros */
    } tChaveIndice;

#define TB  4096
#define TCI  sizeof(tChaveIndice)
#define TI   sizeof(int)
//#define G  ((TB + TCI - TI)/(TCI + TI) - 1)
#define G 4

typedef struct {
           int          nFilhos; 
           tChaveIndice chaves[G - 1];
           int          filhos[G]; //posicao no arquivo da arvore
        } tNoB;

extern int BuscaB(tChave chave);
extern int InsereB(tChaveIndice *chaveEIndice);

extern void AbrirArquivoArvore(const char *nomeArquivoArvore);
extern void FechaArquivoArvore();

extern int CarregaPosicaoRaiz(const char *nomeArquivoRaiz);
extern void AtualizaPosicaoRaiz(int novaPosicaoRaiz);
extern void SalvaPosicaoRaiz(const char *nomeArquivoRaiz);



#endif