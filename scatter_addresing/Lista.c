/****
 *
 * Exemplo: Listas Simplesmente Encadeadas sem Ordenação
 *
 * Seção: 10.2
 *
 * Autor: Ulysses de Oliveira
 *
 * Data de Criação: 19/05/2012
 * Última modificação: 25/05/2012
 *
 * Entrada: V. exemplo de execução
 *
 * Saída: V. exemplo de execução
 *
 ****/

/************************ Includes ************************/

#include <stdio.h>    /* Entrada e saída         */
#include <stdlib.h>   /* exit()                  */
#include <string.h>   /* Funções strXXX()        */
#include "Macros.h"   /* Macro ASSEGURA          */
#include "Lista.h"    /* Interface deste arquivo */

/****************** Definições de Funções *****************/

/****
 *
 * IniciaListaSE(): Inicia uma lista simplesmente encadeada
 *
 * Parâmetros: *lista (saída) - a lista que será iniciada
 *
 * Retorno: Nada
 *
 * Observação: Esta função independe do tipo de conteúdo
 *             efetivo armazenado em cada nó da lista
 *
 ****/
void IniciaListaSE(tListaSE *lista)
{
   *lista = NULL;
}

/****
 *
 * ComprimentoListaSE(): Calcula o número de nós de uma lista
 *                       simplesmente encadeada
 *
 * Parâmetros:
 *     lista (entrada) - ponteiro para o primeiro nó da lista
 *                       cujo comprimento será calculado
 *
 * Retorno: O número de nós da referida lista
 *
 * Observação: Esta função independe do tipo de conteúdo
 *             efetivo armazenado em cada nó da lista
 *
 ****/
int ComprimentoListaSE(tListaSE lista)
{
   int tamanho = 0; /* Armazena o número de nós da lista */

      /* Acessa cada nó da lista e conta */
      /* quantos nós são acessados       */
   while (lista) {
      ++tamanho; /* Mais um nó foi encontrado */
      lista = lista->proximo; /* Passa para o próximo nó */
   }

   return tamanho;
}

/****
 *
 * InsereListaSE(): Insere um novo nó no início de uma
 *                lista simplesmente encadeada
 *
 * Parâmetros:
 *    *lista (entrada/saída) - ponteiro para a lista na qual
 *                             será feita a inserção
 *     conteudo (entrada) - conteúdo do nó que será inserido
 *
 * Retorno: Nada
 *
 * Observação: Como a lista não obedece nenhuma ordem, pode-se
 *             inserir um nó em qualquer posição da lista.
 *             Escolheu-se inserção no início da lista porque
 *             é mais simples e eficiente.
 *
 ****/
void InsereListaSE(tListaSE *lista, const tCEP_Ind *conteudo)
{
   tNoListaSE *ptrNovoNo; /* Apontará para o novo nó alocado */

      /* Tenta alocar um novo nó */
   ASSEGURA( ptrNovoNo = malloc(sizeof(tNoListaSE)),
             "Nao foi possivel alocar no'" );

      /* Armazena no novo nó os dados recebidos como parâmetro */
   ptrNovoNo->conteudo = *conteudo;

      /* O novo nó apontará para o início corrente da lista */
   ptrNovoNo->proximo = *lista;

      /* O início da lista passa a apontar para o novo nó */
   *lista = ptrNovoNo;
}

/****
 *
 * RemoveListaSE(): Remove de uma lista simplesmente encadeada
 *                um nó que contém um determinado valor
 *                especificado como parâmetro
 *
 * Parâmetros:
 *     lista (entrada/saída) - endereço do ponteiro que representa
 *                             a lista na qual será feita a remoção
 *     conteudo (entrada) - conteúdo do nó a ser removido
 *
 * Retorno: 0, se a remoção for bem sucedida, ou 1, se o nó a
 *          ser removido não for encontrado.
 *
 ****/
int RemoveListaSE(tListaSE *lista, tCEP chave)
{
   tListaSE p = *lista, /* p aponta para o nó corrente     */
            q = NULL;   /* q aponta para o nó anterior a p */

      /*                                                     */
      /* O laço while procura um nó que cujo conteúdo case   */
      /* com o parâmetro 'conteudo' e tem duas condições de  */
      /* parada: (1) todos os nós são acessados sem que o    */
      /* referido nó tenha sido encontrado (neste  caso, p   */
      /* assume NULL); (2) o nó que contém o valor procurado */
      /* é encontrado.                                       */
      /*                                                     */
   while (p && strcmp(p->conteudo.chave, chave)) {
      q = p; /* q passa a apontar para o nó corrente */
      p = p->proximo; /* p passa a apontar para o próximo nó */
   }

      /* Se p for NULL, a lista foi totalmente examinada   */
      /* sem que o nó a ser removido tenha sido encontrado */
   if (!p) {
      return 1; /* Tentativa de remoção de nó inexistente */
   }

      /****************************************************/
      /* Neste ponto, p aponta para o nó a ser removido e */
      /* q aponta para o nó imediatamente anterior a ele  */
      /****************************************************/

      /* Verifica se p aponta para o primeiro */
      /* nó da lista porque remoção no início */
      /* deve ser tratada separadamente       */
   if (p == *lista) { /* Remoção será no início da lista */
      (*lista) = p->proximo; /* Altera início da lista */
   } else {/* Nó a ser removido NÃO é o primeiro da lista */
      q->proximo = p->proximo; /* Desvia o nó anterior */
                               /* do nó a ser removido */
   }

      /* Neste ponto, o nó desejado não faz mais parte da */
      /* lista, mas ele ainda ocupa espaço em memória.    */
      /* Portanto, é necessário liberar este espaço.      */
   free(p); /* Libera o nó removido */

   return 0;
}

/****
 *
 * EstaVaziaListaSE(): Verifica se uma lista simplesmente
 *                 encadeada está vazia
 *
 * Parâmetros:
 *     lista (entrada) - a lista que será verificada
 *
 * Retorno: 1, quando a lista está vazia, ou 0, em caso contrário
 *
 ****/
int EstaVaziaListaSE(tListaSE lista)
{
   return lista == NULL;
}

/****
 *
 * BuscaListaSE(): Retorna o endereço do conteúdo
 *                            efetivo do nó que possui uma
 *                            determinado chave numa lista
 *                            simplesmente encadeada
 *
 * Parâmetros:
 *      *lista (entrada) - lista que será pesquisada
 *       chave (entrada) - chave de busca
 *
 * Retorno: Endereço dos dados que contêm o referido conteúdo,
 *          se ele for encontrado. Caso contrário, NULL.
 *
 ****/
int BuscaListaSE(tListaSE *lista, tCEP chave)
{
   tListaSE p = *lista; /* p apontará para o nó corrente     */

      /* Enquanto p não assume NULL ou a chave de um nó não */
      /* casa com o parâmetro 'chave', a busca prossegue    */
   while (p && strcmp(p->conteudo.chave, chave)) {
      p = p->proximo;
   }

      /* Se 'p' assume NULL é porque a chave especificada */
      /* como parâmetro não foi encontrado                */
   if(!p) {
      return -1; /* Chave não foi encontrada */
   }

      /* Neste ponto, p aponta para o nó encontrado. Então, */
      /* retorna-se o endereço de seu campo 'conteudo'.     */
   return p->conteudo.indice;
}

/****
 *
 * DestroiListaSE(): Libera o espaço ocupado pelos nós de
 *                   uma lista encadeada, tornando-a vazia
 *
 * Parâmetros:
 *     lista (entrada/saída) - endereço do ponteiro para a
 *                             lista simplesmente encadeada
 *                             que será destruída
 *
 * Retorno: Nada
 *
 ****/
void DestroiListaSE(tListaSE *lista)
{
   tListaSE p; /* Aponta para o próximo nó a ser liberado */

   if (!*lista) { /* Verifica se a lista está vazia */
      return; /* Lista vazia não precisa ser destruída */
   }

   p = *lista; /* Faz p apontar para o início da lista */

      /* Visita cada nó da lista liberando-o */
   do {
         /* Passa para o próximo nó antes que o nó corrente */
         /* seja destruído. Não importa que o ponteiro para */
         /*o início da lista seja alterado. Afinal, este    */
         /* ponteiro será mesmo anulado.                    */
      *lista = (*lista)->proximo;

      free(p); /* Libera o espaço do nó corrente */

         /* Faz p apontar para o próximo nó, cujo */
         /* endereço está armazenado em *lista    */
      p = *lista;
   } while (p);

      /* Neste ponto, a lista ficou vazia, pois o  */
      /* último valor assumido por *lista foi NULL */
}

/****
 *
 * ProximoListaSE(): Encontra o próximo nó de uma lista
 *                   simplesmente encadeada
 *
 * Parâmetros:
 *     lista (entrada) - ponteiro para a lista
 *                       simplesmente encadeada
 *
 * Retorno: Endereço do conteúdo do próximo nó
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
