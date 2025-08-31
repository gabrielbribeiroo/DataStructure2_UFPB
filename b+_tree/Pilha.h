/****
 *
 * Arquivo Pilha.h
 *
 * Autor: Ulysses de Oliveira
 *
 * Data de Criação: 04/08/2008
 * Última modificação: 12/05/2010
 *
 * Descrição: Interface do módulo Pilha utilizado pelo
 *            programa de árvores B
 *
 ****/

#ifndef _Pilha2_H_ /* Previne inclusão múltipla deste arquivo */
#define _Pilha2_H_

/*********************** Includes *************************/

#include "ArvoreBM.h"   /* Interface d módulo ArvoreB */

/****************** Definições de Tipos *******************/

   /* Tipo usado para armazenamento do */
   /* caminho de um nó até outro nó    */
typedef struct {
           tNoBM no;       /* O nó              */
           int   endereco; /* Seu endereço      */
           int   pos;      /* Sua posição entre */
                           /* seus irmãos       */
        } tNoCaminho;

typedef tNoCaminho tElemento; /* Apenas por conveniência */

   /* Tipo da pilha usada para guardar caminhos */
typedef struct noPilha {
           tElemento       dado;
           struct noPilha *proximo;
        } tNoPilha, *tPilha;

/******************* Alusões de Funções *******************/

extern void CriaPilha(tPilha  *p);
extern int PilhaVazia(tPilha p);
extern tElemento ElementoTopo(tPilha *p);
extern void Empilha(tElemento item, tPilha  *p);
extern tElemento Desempilha(tPilha  *p);
extern void EsvaziaPilha(tPilha  *p);
extern void CopiaPilha(tPilha *copia, const tPilha *pilha);

#endif
