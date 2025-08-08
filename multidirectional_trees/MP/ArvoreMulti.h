/****
 *
 * Arquivo ArvoreMulti.h
 *
 * Autor: Gabriel Ribeiro
 *
 * Descri��o: Interface das fun��es que implementam opera��es
 *            usando �rvores de busca multidirecional
 *
 ****/

#ifndef _ArvoreMulti_H_  /* Previne inclus�o m�ltipla deste arquivo */
#define _ArvoreMulti_H_

/*********************** Includes *************************/

#include <stdio.h>     /* Tipo FILE */
#include "Registros.h" /* Interface do m�dulo Registros */

/************ Defini��es de Tipos e Constantes ************/

   /* Tipo do par chave/�ndice armazenado nos n�s */
typedef struct  {
          tChave chave;  /* Chave do registro              */
          int    indice; /* Posi��o do registro no arquivo */
} tChaveIndice;

#define G  5

   /* Tipo de um n� da �rvore de busca multidirecional armazenada em arquivo */
typedef struct rotNoMulti {
           int                nFilhos; /* O n�mero de filhos do n� */
           tChaveIndice       chaves[G - 1]; /* array contendo chaves  */
                                             /* e �ndices de registros */
           struct rotNoMulti *filhos[G]; /* array contendo as posi��es dos filhos */
                                         /* no arquivo que cont�m a �rvore        */
        } tNoMulti, *tArvoreMulti;

   /* Tipo de n� e de ponteiro para n� de uma lista encadeada */
   /* contendo �ndices de registros de chaves removidas       */
typedef struct noLista {
           int             indice;
           struct noLista *proximo;
        } tNoIndice, *tListaRemocoes;

/************************* Alus�es ************************/

extern int BuscaMulti(tArvoreMulti arvore, tChave chave);
extern int InsereMulti( tArvoreMulti *arvore, tChaveIndice *chaveEIndice );
extern int RemoveChaveMulti(tArvoreMulti *arvore, tChave chave);
extern void CaminhamentoInfixoMulti(tArvoreMulti arvore, FILE *stream);
extern tChaveIndice *MenorChaveMulti(tArvoreMulti arvore);
extern tChaveIndice *MaiorChaveMulti(tArvoreMulti arvore);
extern int AlturaMulti(tArvoreMulti arvore);
extern int NumeroDeChavesMulti(tArvoreMulti arvore);
extern int NumeroDeNosMulti(tArvoreMulti arvore);

#endif
