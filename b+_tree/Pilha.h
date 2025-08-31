/****
 *
 * Arquivo Pilha.h
 *
 * Autor: Ulysses de Oliveira
 *
 * Data de Cria��o: 04/08/2008
 * �ltima modifica��o: 12/05/2010
 *
 * Descri��o: Interface do m�dulo Pilha utilizado pelo
 *            programa de �rvores B
 *
 ****/

#ifndef _Pilha2_H_ /* Previne inclus�o m�ltipla deste arquivo */
#define _Pilha2_H_

/*********************** Includes *************************/

#include "ArvoreBM.h"   /* Interface d m�dulo ArvoreB */

/****************** Defini��es de Tipos *******************/

   /* Tipo usado para armazenamento do */
   /* caminho de um n� at� outro n�    */
typedef struct {
           tNoBM no;       /* O n�              */
           int   endereco; /* Seu endere�o      */
           int   pos;      /* Sua posi��o entre */
                           /* seus irm�os       */
        } tNoCaminho;

typedef tNoCaminho tElemento; /* Apenas por conveni�ncia */

   /* Tipo da pilha usada para guardar caminhos */
typedef struct noPilha {
           tElemento       dado;
           struct noPilha *proximo;
        } tNoPilha, *tPilha;

/******************* Alus�es de Fun��es *******************/

extern void CriaPilha(tPilha  *p);
extern int PilhaVazia(tPilha p);
extern tElemento ElementoTopo(tPilha *p);
extern void Empilha(tElemento item, tPilha  *p);
extern tElemento Desempilha(tPilha  *p);
extern void EsvaziaPilha(tPilha  *p);
extern void CopiaPilha(tPilha *copia, const tPilha *pilha);

#endif
