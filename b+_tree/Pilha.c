/****
 *
 * Arquivo Pilha.c
 *
 * Autor: Ulysses de Oliveira
 *
 * Data de Cria��o: 04/08/2008
 * �ltima modifica��o: 12/05/2010
 *
 * Descri��o: Implementa��o do m�dulo Pilha usado pelo
 *            programa de busca usando �rvore B
 *
 ****/

#include <stdio.h>     /* Fun��es de entrada e sa�da     */
#include <stdlib.h>    /* Fun��es de aloca��o de mem�ria */
#include "Pilha.h"     /* Interface deste m�dulo         */
#include "ASSEGURA.h"  /* Macro ASSEGURA e outras macros */
                       /* de tratamento de exce��o       */

/***************** Defini��es de Fun��es ******************/

/****
 *
 * CriaPilha(): Inicia uma pilha
 *
 * Par�metros: p (sa�da) - ponteiro para a pilha que ser� iniciada
 *
 * Retorno: Nada
 *
 * Observa��o: O programa ser� abortado se o par�metro for NULL
 *
 ****/
void  CriaPilha(tPilha *p)
{
   ASSEGURA(p, "Erro: ponteiro nulo recebido por CriaPilha()");

   *p = NULL;
}

/****
 *
 * PilhaVazia(): Verifica se uma pilha est� vazia
 *
 * Par�metros: p (entrada) - a pilha que ser� verificada
 *
 * Retorno: 1, se a pilha recebida como par�metro estiver vazia;
 *          0, caso contr�rio.
 *
 ****/
int  PilhaVazia(tPilha p)
{
   return !p;
}

/****
 *
 * ElementoTopo(); Retorna o elemento do topo de uma pilha
 *                 sem desempilh�-lo
 *
 * Par�metros: p (entrada) - a pilha
 *
 * Retorno: O elemento que est� no topo da pilha
 *
 * Observa��o: Se a pilha estiver vazia, causa o
 *             aborto do programa.
 *
 ****/
tElemento ElementoTopo(tPilha *p)
{
   ASSEGURA(!PilhaVazia(*p),
            "Erro: Pilha vazia recebida por ElementoTopo()");

   return (*p)->dado;
}

/****
 *
 * Empilha(): Empilha um item numa pilha
 *
 * Par�metros:
 *      p (entrada/sa�da) - ponteiro para a pilha
 *      item (entrada) - o item que ser� empilhado
 *
 * Retorno: Nada
 *
 * Observa��o: Se o par�metro for NULL ou n�o for poss�vel
 *             alocar um n� para empilhar, o programa ser�
 *             abortado
 *
 ****/
void Empilha( tElemento item, tPilha *p)
{
   tPilha  aux;

   ASSEGURA(p, "Erro: ponteiro nulo recebido por Empilha()");

   aux = malloc(sizeof(tNoPilha));

   ASSEGURA(aux, "Erro de alocaca o de um elemento da pilha");

   aux->dado = item;
   aux->proximo = *p;
   *p = aux;
}

/****
 *
 * Desempilha(): Desempilha o elemento do topo de uma pilha
 *
 * Par�metros: p (entrada/sa�da) - ponteiro para a pilha
 *
 * Retorno: O elemento desempilhado
 *
 * Observa��o: Se o par�metro for NULL ou a pilha estiver
 *             vazia, o programa ser� abortado.
 *
 ****/
tElemento Desempilha(tPilha  *p)
{
   tElemento elementoNoTopo;
   tPilha    aux;

   ASSEGURA(p, "Erro: ponteiro nulo recebido por Desempilha()");

   ASSEGURA(!PilhaVazia(*p), "Erro: Pilha vazia em Desempilha()");

   elementoNoTopo = (*p)->dado;
   aux = *p;
   *p = (*p)->proximo;
   free(aux);

   return elementoNoTopo;
}

/****
 *
 * EsvaziaPilha(): Desempilha e descarta todos os elementos
 *                 da pilha
 *
 * Par�metros: p (sa�da) - ponteiro para a pilha
 *
 * Retorno: Nada
 *
 * Observa��o: Se o par�metro for NULL, o programa ser�
 *             abortado
 *
 ****/
void EsvaziaPilha(tPilha *p)
{
   ASSEGURA(p,
            "Erro: ponteiro nulo recebido por EsvaziaPilha()");

   while (*p)
      (void) Desempilha(p);
}

/****
 *
 * CopiaPilha(): Faz uma c�pia de uma pilha
 *
 * Par�metros:
 *    copia (sa�da) - ponteiro para a pilha resultante da c�pia
 *    pilha (entrada) - ponteiro para a pilha que ser� copiada
 *
 * Retorno: Nada
 *
 * Observa��o: Se o segundo par�metro for NULL, o programa ser�
 *             abortado
 *
 ****/
void CopiaPilha(tPilha *copia, const tPilha *pilha)
{
   tPilha p, q = NULL, lista = *pilha;

   ASSEGURA(pilha, "Erro: ponteiro nulo recebido por CopiaPilha()");

   CriaPilha(copia);

   if (PilhaVazia(lista))
      return; /* N�o h� o que copiar */

      /* Aloca o primeiro n� da c�pia */
   *copia = p = malloc(sizeof(tNoPilha));
   ASSEGURA(p, "Erro de alocaca o de um elemento da pilha");

      /* Copia o primeiro n� */
   p->dado = lista->dado;

      /* Passa para o pr�ximo n� */
   lista = lista->proximo;

   while (lista) {
         /* Aloca o pr�ximo n� */
      q = malloc(sizeof(tNoPilha));
      ASSEGURA(q, "Erro de alocaca o de um elemento da pilha");

         /* Copia o conte�do do n� */
      q->dado = lista->dado;

      p->proximo = q;

      p = q;
   }

   p->proximo = NULL;
}

