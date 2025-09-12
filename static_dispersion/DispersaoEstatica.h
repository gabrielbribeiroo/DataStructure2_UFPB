/****
 *
 * Arquivo DispersaoEstatica.h
 *
 * Autor: Ulysses de Oliveira
 *
 * Data de Cria��o: 04/08/2017
 * �ltima modifica��o: 26/08/2017
 *
 * Descri��o: Iterface das fun��es que implementam opera��es
 *            de busca, inser��o e remo��o usando dispers�o
 *            est�tica em mem�ria secund�ria
 *
 ****/

#ifndef _DispersaoEstatica_H_ /* Previne inclus�o m�ltipla */
#define _DispersaoEstatica_H_

#include "Registros.h" /* Tipos tRegistroMEC e tChave */

/************ Defini��es de Tipos e Constantes ************/

   /* Posi��o nula (inv�lida) em arquivo */
#define POSICAO_NULA -1

   /* Tipo usado para classificar um coletor */
typedef enum {PRIMARIO, EXCEDENTE} tTipoDeColetorDEst;

#define TB 4096 /* Tamanho do bloco lido ou escrito  */
#define TT (int) sizeof(tTipoDeColetorDEst) /* Tamanho de tTipoDeColetorDEst */
#define TR (int) sizeof(tRegistroMEC) /* Tamanho de um registro */
#define TI (int) sizeof(int) /* Tamanho de um campo do tipo int */

   /* C�lculo do n�mero m�ximo de registros num coletor */
#define M (((TB - 2*TI - TT)/TR) - 1)

   /* Tipo de uma vari�vel que armazena um coletor */
typedef struct {
           tTipoDeColetorDEst tipo; /* Tipo de coletor */
           tRegistroMEC       registros[M]; /* Array de registros  */
           int                nRegistros; /* N�mero de registros num coletor */
           int                proximo; /* Pr�ximo coletor excedente */
        } tColetorDEst;

/************************* Alus�es ************************/

extern int IniciaTabDEst(FILE *streamCol, int nRegistros, int maxRegs);
extern tRegistroMEC *BuscaDEst( FILE *stream, int nColetores,
                                tChave chave, tRegistroMEC *reg );
extern void InsereDEst(int nColetores, const tRegistroMEC *registro, FILE *stream);
extern int RemoveDEst(int nColetores, tChave chave, FILE *stream);
extern void SubstituiDEst(FILE *stream, int nColetores, const tRegistroMEC *reg);
extern int NRegistrosTabDEst( FILE *stream );
extern int NColetoresDEst(FILE *stream);
extern int NColetoresVaziosDEst(FILE *stream);
extern int NColetoresRepletosDEst(FILE *stream);
extern int NColetoresOcupadosDEst(FILE *stream);
extern int NColetoresExcedentesDEst(int nColetores, FILE *stream);
extern int MaiorCadeiaExcedentesDEst(int nColetores, FILE *stream);

#endif
