/****
 *
 * Exemplo: Listas Simplesmente Encadeadas sem Ordena��o
 *
 * Se��o: 10.2
 *
 * Autor: Ulysses de Oliveira
 *
 * Data de Cria��o: 19/05/2012
 * �ltima modifica��o: 25/05/2012
 *
 * Entrada: V. exemplo de execu��o
 *
 * Sa�da: V. exemplo de execu��o
 *
 ****/

/************************ Includes ************************/

#include <stdio.h>    /* Entrada e sa�da         */
#include <stdlib.h>   /* exit()                  */
#include <string.h>   /* Fun��es strXXX()        */
#include "Macros.h"   /* Macro ASSEGURA          */
#include "Lista.h"    /* Interface deste arquivo */

/****************** Defini��es de Fun��es *****************/

/****
 *
 * IniciaListaSE(): Inicia uma lista simplesmente encadeada
 *
 * Par�metros: *lista (sa�da) - a lista que ser� iniciada
 *
 * Retorno: Nada
 *
 * Observa��o: Esta fun��o independe do tipo de conte�do
 *             efetivo armazenado em cada n� da lista
 *
 ****/
void IniciaListaSE(tListaSE *lista)
{
   *lista = NULL;
}

/****
 *
 * ComprimentoListaSE(): Calcula o n�mero de n�s de uma lista
 *                       simplesmente encadeada
 *
 * Par�metros:
 *     lista (entrada) - ponteiro para o primeiro n� da lista
 *                       cujo comprimento ser� calculado
 *
 * Retorno: O n�mero de n�s da referida lista
 *
 * Observa��o: Esta fun��o independe do tipo de conte�do
 *             efetivo armazenado em cada n� da lista
 *
 ****/
int ComprimentoListaSE(tListaSE lista)
{
   int tamanho = 0; /* Armazena o n�mero de n�s da lista */

      /* Acessa cada n� da lista e conta */
      /* quantos n�s s�o acessados       */
   while (lista) {
      ++tamanho; /* Mais um n� foi encontrado */
      lista = lista->proximo; /* Passa para o pr�ximo n� */
   }

   return tamanho;
}

/****
 *
 * InsereListaSE(): Insere um novo n� no in�cio de uma
 *                lista simplesmente encadeada
 *
 * Par�metros:
 *    *lista (entrada/sa�da) - ponteiro para a lista na qual
 *                             ser� feita a inser��o
 *     conteudo (entrada) - conte�do do n� que ser� inserido
 *
 * Retorno: Nada
 *
 * Observa��o: Como a lista n�o obedece nenhuma ordem, pode-se
 *             inserir um n� em qualquer posi��o da lista.
 *             Escolheu-se inser��o no in�cio da lista porque
 *             � mais simples e eficiente.
 *
 ****/
void InsereListaSE(tListaSE *lista, const tCEP_Ind *conteudo)
{
   tNoListaSE *ptrNovoNo; /* Apontar� para o novo n� alocado */

      /* Tenta alocar um novo n� */
   ASSEGURA( ptrNovoNo = malloc(sizeof(tNoListaSE)),
             "Nao foi possivel alocar no'" );

      /* Armazena no novo n� os dados recebidos como par�metro */
   ptrNovoNo->conteudo = *conteudo;

      /* O novo n� apontar� para o in�cio corrente da lista */
   ptrNovoNo->proximo = *lista;

      /* O in�cio da lista passa a apontar para o novo n� */
   *lista = ptrNovoNo;
}

/****
 *
 * RemoveListaSE(): Remove de uma lista simplesmente encadeada
 *                um n� que cont�m um determinado valor
 *                especificado como par�metro
 *
 * Par�metros:
 *     lista (entrada/sa�da) - endere�o do ponteiro que representa
 *                             a lista na qual ser� feita a remo��o
 *     conteudo (entrada) - conte�do do n� a ser removido
 *
 * Retorno: 0, se a remo��o for bem sucedida, ou 1, se o n� a
 *          ser removido n�o for encontrado.
 *
 ****/
int RemoveListaSE(tListaSE *lista, tCEP chave)
{
   tListaSE p = *lista, /* p aponta para o n� corrente     */
            q = NULL;   /* q aponta para o n� anterior a p */

      /*                                                     */
      /* O la�o while procura um n� que cujo conte�do case   */
      /* com o par�metro 'conteudo' e tem duas condi��es de  */
      /* parada: (1) todos os n�s s�o acessados sem que o    */
      /* referido n� tenha sido encontrado (neste  caso, p   */
      /* assume NULL); (2) o n� que cont�m o valor procurado */
      /* � encontrado.                                       */
      /*                                                     */
   while (p && strcmp(p->conteudo.chave, chave)) {
      q = p; /* q passa a apontar para o n� corrente */
      p = p->proximo; /* p passa a apontar para o pr�ximo n� */
   }

      /* Se p for NULL, a lista foi totalmente examinada   */
      /* sem que o n� a ser removido tenha sido encontrado */
   if (!p) {
      return 1; /* Tentativa de remo��o de n� inexistente */
   }

      /****************************************************/
      /* Neste ponto, p aponta para o n� a ser removido e */
      /* q aponta para o n� imediatamente anterior a ele  */
      /****************************************************/

      /* Verifica se p aponta para o primeiro */
      /* n� da lista porque remo��o no in�cio */
      /* deve ser tratada separadamente       */
   if (p == *lista) { /* Remo��o ser� no in�cio da lista */
      (*lista) = p->proximo; /* Altera in�cio da lista */
   } else {/* N� a ser removido N�O � o primeiro da lista */
      q->proximo = p->proximo; /* Desvia o n� anterior */
                               /* do n� a ser removido */
   }

      /* Neste ponto, o n� desejado n�o faz mais parte da */
      /* lista, mas ele ainda ocupa espa�o em mem�ria.    */
      /* Portanto, � necess�rio liberar este espa�o.      */
   free(p); /* Libera o n� removido */

   return 0;
}

/****
 *
 * EstaVaziaListaSE(): Verifica se uma lista simplesmente
 *                 encadeada est� vazia
 *
 * Par�metros:
 *     lista (entrada) - a lista que ser� verificada
 *
 * Retorno: 1, quando a lista est� vazia, ou 0, em caso contr�rio
 *
 ****/
int EstaVaziaListaSE(tListaSE lista)
{
   return lista == NULL;
}

/****
 *
 * BuscaListaSE(): Retorna o endere�o do conte�do
 *                            efetivo do n� que possui uma
 *                            determinado chave numa lista
 *                            simplesmente encadeada
 *
 * Par�metros:
 *      *lista (entrada) - lista que ser� pesquisada
 *       chave (entrada) - chave de busca
 *
 * Retorno: Endere�o dos dados que cont�m o referido conte�do,
 *          se ele for encontrado. Caso contr�rio, NULL.
 *
 ****/
int BuscaListaSE(tListaSE *lista, tCEP chave)
{
   tListaSE p = *lista; /* p apontar� para o n� corrente     */

      /* Enquanto p n�o assume NULL ou a chave de um n� n�o */
      /* casa com o par�metro 'chave', a busca prossegue    */
   while (p && strcmp(p->conteudo.chave, chave)) {
      p = p->proximo;
   }

      /* Se 'p' assume NULL � porque a chave especificada */
      /* como par�metro n�o foi encontrado                */
   if(!p) {
      return -1; /* Chave n�o foi encontrada */
   }

      /* Neste ponto, p aponta para o n� encontrado. Ent�o, */
      /* retorna-se o endere�o de seu campo 'conteudo'.     */
   return p->conteudo.indice;
}

/****
 *
 * DestroiListaSE(): Libera o espa�o ocupado pelos n�s de
 *                   uma lista encadeada, tornando-a vazia
 *
 * Par�metros:
 *     lista (entrada/sa�da) - endere�o do ponteiro para a
 *                             lista simplesmente encadeada
 *                             que ser� destru�da
 *
 * Retorno: Nada
 *
 ****/
void DestroiListaSE(tListaSE *lista)
{
   tListaSE p; /* Aponta para o pr�ximo n� a ser liberado */

   if (!*lista) { /* Verifica se a lista est� vazia */
      return; /* Lista vazia n�o precisa ser destru�da */
   }

   p = *lista; /* Faz p apontar para o in�cio da lista */

      /* Visita cada n� da lista liberando-o */
   do {
         /* Passa para o pr�ximo n� antes que o n� corrente */
         /* seja destru�do. N�o importa que o ponteiro para */
         /*o in�cio da lista seja alterado. Afinal, este    */
         /* ponteiro ser� mesmo anulado.                    */
      *lista = (*lista)->proximo;

      free(p); /* Libera o espa�o do n� corrente */

         /* Faz p apontar para o pr�ximo n�, cujo */
         /* endere�o est� armazenado em *lista    */
      p = *lista;
   } while (p);

      /* Neste ponto, a lista ficou vazia, pois o  */
      /* �ltimo valor assumido por *lista foi NULL */
}

/****
 *
 * ProximoListaSE(): Encontra o pr�ximo n� de uma lista
 *                   simplesmente encadeada
 *
 * Par�metros:
 *     lista (entrada) - ponteiro para a lista
 *                       simplesmente encadeada
 *
 * Retorno: Endere�o do conte�do do pr�ximo n�
 *
 ****/
tCEP_Ind *ProximoListaSE(tListaSE lista)
{
   static tNoListaSE *proximoNo = NULL;

   if (lista) {
      if (!proximoNo) {
         proximoNo = lista;
      } else {
         proximoNo = proximoNo->proximo;
      }
      return &proximoNo->conteudo;
   } else {
      return NULL;
   }
}
