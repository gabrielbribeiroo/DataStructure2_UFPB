/****
 *
 * Arquivo Pilha.c
 *
 * Autor: Ulysses de Oliveira
 *
 * Data de Criação: 04/08/2008
 * Última modificação: 12/05/2010
 *
 * Descrição: Implementação do módulo Pilha usado pelo
 *            programa de busca usando árvore B
 *
 ****/

#include <stdio.h>     /* Funções de entrada e saída     */
#include <stdlib.h>    /* Funções de alocação de memória */
#include "Pilha.h"     /* Interface deste módulo         */
#include "ASSEGURA.h"  /* Macro ASSEGURA e outras macros */
                       /* de tratamento de exceção       */

/***************** Definições de Funções ******************/

/****
 *
 * CriaPilha(): Inicia uma pilha
 *
 * Parâmetros: p (saída) - ponteiro para a pilha que será iniciada
 *
 * Retorno: Nada
 *
 * Observação: O programa será abortado se o parâmetro for NULL
 *
 ****/
void  CriaPilha(tPilha *p)
{
   ASSEGURA(p, "Erro: ponteiro nulo recebido por CriaPilha()");

   *p = NULL;
}

/****
 *
 * PilhaVazia(): Verifica se uma pilha está vazia
 *
 * Parâmetros: p (entrada) - a pilha que será verificada
 *
 * Retorno: 1, se a pilha recebida como parâmetro estiver vazia;
 *          0, caso contrário.
 *
 ****/
int  PilhaVazia(tPilha p)
{
   return !p;
}

/****
 *
 * ElementoTopo(); Retorna o elemento do topo de uma pilha
 *                 sem desempilhá-lo
 *
 * Parâmetros: p (entrada) - a pilha
 *
 * Retorno: O elemento que está no topo da pilha
 *
 * Observação: Se a pilha estiver vazia, causa o
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
 * Parâmetros:
 *      p (entrada/saída) - ponteiro para a pilha
 *      item (entrada) - o item que será empilhado
 *
 * Retorno: Nada
 *
 * Observação: Se o parâmetro for NULL ou não for possível
 *             alocar um nó para empilhar, o programa será
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
 * Parâmetros: p (entrada/saída) - ponteiro para a pilha
 *
 * Retorno: O elemento desempilhado
 *
 * Observação: Se o parâmetro for NULL ou a pilha estiver
 *             vazia, o programa será abortado.
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
 * Parâmetros: p (saída) - ponteiro para a pilha
 *
 * Retorno: Nada
 *
 * Observação: Se o parâmetro for NULL, o programa será
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
 * CopiaPilha(): Faz uma cópia de uma pilha
 *
 * Parâmetros:
 *    copia (saída) - ponteiro para a pilha resultante da cópia
 *    pilha (entrada) - ponteiro para a pilha que será copiada
 *
 * Retorno: Nada
 *
 * Observação: Se o segundo parâmetro for NULL, o programa será
 *             abortado
 *
 ****/
void CopiaPilha(tPilha *copia, const tPilha *pilha)
{
   tPilha p, q = NULL, lista = *pilha;

   ASSEGURA(pilha, "Erro: ponteiro nulo recebido por CopiaPilha()");

   CriaPilha(copia);

   if (PilhaVazia(lista))
      return; /* Não há o que copiar */

      /* Aloca o primeiro nó da cópia */
   *copia = p = malloc(sizeof(tNoPilha));
   ASSEGURA(p, "Erro de alocaca o de um elemento da pilha");

      /* Copia o primeiro nó */
   p->dado = lista->dado;

      /* Passa para o próximo nó */
   lista = lista->proximo;

   while (lista) {
         /* Aloca o próximo nó */
      q = malloc(sizeof(tNoPilha));
      ASSEGURA(q, "Erro de alocaca o de um elemento da pilha");

         /* Copia o conteúdo do nó */
      q->dado = lista->dado;

      p->proximo = q;

      p = q;
   }

   p->proximo = NULL;
}

