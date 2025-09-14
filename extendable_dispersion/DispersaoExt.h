/****
 *
 * Arquivo DispersaoExt.h
 *
 * Autor: Ulysses de Oliveira
 *
 * Data de Cria��o: 04/08/2017
 * �ltima modifica��o: 26/08/2017
 *
 * Descri��o: Iterface das fun��es que implementam opera��es
 *            de busca, inser��o e remo��o usando dispers�o
 *            extens�vel em mem�ria secund�ria
 *
 ****/

#ifndef _DispersaoExt_H_ /* Previne inclus�o m�ltipla */
#define _DispersaoExt_H_

#include "Registros.h" /* Tipos tRegistroMEC e tChave */

/************ Defini��es de Tipos e Constantes ************/

   /* Posi��o nula (inv�lida) em arquivo */
#define POSICAO_NULA -1

#define TB 4096 /* Tamanho do bloco lido ou escrito  */
#define TR (int) sizeof(tRegistroMEC) /* Tamanho de um registro   */
                                      /* armazenado nos coletores */
#define TI (int) sizeof(int) /* Tamanho de um campo do tipo int */

   /* C�lculo do n�mero m�ximo de registros num coletor */
#define M (((TB - (2*TI))/TR) - 1)

   /* Tipo de uma vari�vel que armazena um coletor */
typedef struct {
           tRegistroMEC registros[M]; /* Array de registros  */
           int          nRegistros;   /* N�mero de registros num coletor */
           int          profLocal;    /* Profundidade local  */
        } tColetorDExt;

   /* Tipo de uma vari�vel que representa uma  */
   /* tabela de busca com dispers�o extens�vel */
typedef struct {
           long *dir;        /* Diret�rio            */
           int   profGlobal; /* Profundidade global  */
           int   tamDir;     /* Tamanho do diret�rio */
           int   nReg;       /* N�mero de registros  */
        } tTabelaDExt;

/************************* Alus�es ************************/

extern void LeColetorDExt(FILE *stream, int pos, tColetorDExt *coletor);
extern void EscreveColetorDExt( FILE *stream, int pos,
                                const tColetorDExt *coletor );
extern void IniciaTabelaDExt(tTabelaDExt *tabela, FILE *stream);
extern tRegistroMEC *BuscaDExt( FILE *stream, const tTabelaDExt *tab,
                                tChave chave, tRegistroMEC *reg );
extern void InsereDExt(tTabelaDExt *tab, const tRegistroMEC *item, FILE *stream);
extern int RemoveDExt(tTabelaDExt *tab, tChave chave, FILE *stream);
extern int NRegistrosDExt(const tTabelaDExt *tabela, FILE *stream);
extern int NColetoresDExt(FILE *stream);
extern int NColetoresVaziosDExt(FILE *stream);
extern int NColetoresRepletosDExt(FILE *stream);
extern int NColetoresOcupadosDExt(FILE *stream);
extern int ProfLocalMinDExt(FILE *stream);
extern int ProfLocalMaxDExt(FILE *stream);

#endif
