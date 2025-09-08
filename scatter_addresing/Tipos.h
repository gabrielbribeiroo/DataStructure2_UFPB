/****
 *
 * Arquivo Tipos.h
 *
 * Autor: Ulysses de Oliveira
 *
 * Data de Criação: 07/06/2010
 * Última modificação: 07/06/2010
 *
 * Descrição: Tipos utilizados pelo programa de dispersão com
 *            encadeamento
 *
 ****/

#ifndef _Tipos_H_ /* Previne inclusão múltipla deste arquivo */
#define _Tipos_H_

#include "Macros.h"  /* Macros usadas pelo programa */

typedef char tCEP[TAM_CEP + 1]; /* Tipo da chave */

   /* Tipo do conteúdo armazenado em cada nó das listas (coletores) */
typedef struct {
           tCEP chave;  /* Chave de um registro          */
           int  indice; /* Índice do registro no arquivo */
        } tCEP_Ind;

   /* Tipo de nó e tipo de ponteiro para nó de uma */
   /* lista encadeada que representa um coletor    */
typedef struct rotNoLSE {
           tCEP_Ind         conteudo;
           struct rotNoLSE *proximo;
        } tNoListaSE, *tListaSE;

   /* A tabela de dispersão será um array de listas encadeadas */
typedef tListaSE *tTabelaDE;

   /* Tipo de um ponteiro para uma função de dispersão */
typedef unsigned (*tFDispersao)(const char *);

   /* Tipo usado para armazenamento de */
   /* registros no arquivo binário     */
typedef struct {
           int  numero;
           char UF[TAM_UF];
           int  localidadeNumero;
           char nomeAbr[MAX_NOME];
           char nome[MAX_NOME];
           int  bairroInicio;
           int  bairroFim;
           char CEP[TAM_CEP];
           char complemento[MAX_COMP];
           char tipoLogradouro[MAX_TIPO_LOG];
           char statusTipoLogradouro;
           char nomeSemAcento[MAX_NOME];
           char chaveDNE[TAM_DNE];
         } tRegistroCEP;

      /* Tipo usado para conter índices de listas */
      /* e respectivos números de elementos       */
typedef struct {
           int iLista; /* Índice da lista */
           int nElementos;
        } tElementosEmLista;

#endif
