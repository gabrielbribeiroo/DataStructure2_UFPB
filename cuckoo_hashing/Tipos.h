/****
 *
 * Arquivo Tipos.h
 *
 * Autor: Ulysses de Oliveira
 *
 * Data de Criação: 07/06/2010
 * Última modificação: 07/06/2010
 *
 * Descrição: Tipos utilizados pelo programa de dispersão cuco
 *
 ****/

#ifndef _Tipos_H_ /* Previne inclusão múltipla deste arquivo */
#define _Tipos_H_

#include "Macros.h"  /* Macros usadas pelo programa */

typedef char tCEP[TAM_CEP + 1]; /* Tipo da chave */

   /* Tipo dos pares chave/índice */
typedef struct  {
           tCEP chave;  /* Chave de um registro */
           int  indice; /* Índice do registro no arquivo */
        } tCEP_Ind;

   /* Tipo usado para indicar o status de um elemento da tabela */
typedef enum {VAZIO, OCUPADO} tStatusCuco;

   /* Tipo de um ponteiro para uma função de dispersão cuco */
typedef unsigned int (*tFDispersaoCuco) (tCEP, int);

   /* Tipo dos elementos de uma tabela de dispersão cuco */
typedef struct {
           tCEP_Ind    chaveEIndice;
           tStatusCuco status;
        } tColetorCuco;

   /* Tipo de uma tabela de dispersão cuco */
typedef struct {
           tColetorCuco   *tab1; /* Tabela 1 */
           tColetorCuco   *tab2; /* Tabela 2 */
           tFDispersaoCuco fD1;  /* Função de dispersão 1  */
           tFDispersaoCuco fD2;  /* Função de dispersão 2  */
           int             tam;  /* Tamanho de cada tabela */
           int             nChaves; /* Número de chaves    */
        } tTabelaCuco;

   /* Tipo usado para armazenamento de registros no arquivo binário */
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

#endif
