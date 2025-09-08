/****
 *
 * Arquivo Tipos.h
 *
 * Autor: Ulysses de Oliveira
 *
 * Data de Cria��o: 07/06/2010
 * �ltima modifica��o: 07/06/2010
 *
 * Descri��o: Tipos utilizados pelo programa de dispers�o com
 *            encadeamento
 *
 ****/

#ifndef _Tipos_H_ /* Previne inclus�o m�ltipla deste arquivo */
#define _Tipos_H_

#include "Macros.h"  /* Macros usadas pelo programa */

typedef char tCEP[TAM_CEP + 1]; /* Tipo da chave */

   /* Tipo do conte�do armazenado em cada n� das listas (coletores) */
typedef struct {
           tCEP chave;  /* Chave de um registro          */
           int  indice; /* �ndice do registro no arquivo */
        } tCEP_Ind;

   /* Tipo de n� e tipo de ponteiro para n� de uma */
   /* lista encadeada que representa um coletor    */
typedef struct rotNoLSE {
           tCEP_Ind         conteudo;
           struct rotNoLSE *proximo;
        } tNoListaSE, *tListaSE;

   /* A tabela de dispers�o ser� um array de listas encadeadas */
typedef tListaSE *tTabelaDE;

   /* Tipo de um ponteiro para uma fun��o de dispers�o */
typedef unsigned (*tFDispersao)(const char *);

   /* Tipo usado para armazenamento de */
   /* registros no arquivo bin�rio     */
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

      /* Tipo usado para conter �ndices de listas */
      /* e respectivos n�meros de elementos       */
typedef struct {
           int iLista; /* �ndice da lista */
           int nElementos;
        } tElementosEmLista;

#endif
