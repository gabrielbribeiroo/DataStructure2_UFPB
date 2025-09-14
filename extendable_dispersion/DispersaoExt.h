/****
 *
 * Arquivo DispersaoExt.h
 *
 * Autor: Ulysses de Oliveira
 *
 * Data de Criação: 04/08/2017
 * Última modificação: 26/08/2017
 *
 * Descrição: Iterface das funções que implementam operações
 *            de busca, inserção e remoção usando dispersão
 *            extensível em memória secundária
 *
 ****/

#ifndef _DispersaoExt_H_ /* Previne inclusão múltipla */
#define _DispersaoExt_H_

#include "Registros.h" /* Tipos tRegistroMEC e tChave */

/************ Definições de Tipos e Constantes ************/

   /* Posição nula (inválida) em arquivo */
#define POSICAO_NULA -1

#define TB 4096 /* Tamanho do bloco lido ou escrito  */
#define TR (int) sizeof(tRegistroMEC) /* Tamanho de um registro   */
                                      /* armazenado nos coletores */
#define TI (int) sizeof(int) /* Tamanho de um campo do tipo int */

   /* Cálculo do número máximo de registros num coletor */
#define M (((TB - (2*TI))/TR) - 1)

   /* Tipo de uma variável que armazena um coletor */
typedef struct {
           tRegistroMEC registros[M]; /* Array de registros  */
           int          nRegistros;   /* Número de registros num coletor */
           int          profLocal;    /* Profundidade local  */
        } tColetorDExt;

   /* Tipo de uma variável que representa uma  */
   /* tabela de busca com dispersão extensível */
typedef struct {
           long *dir;        /* Diretório            */
           int   profGlobal; /* Profundidade global  */
           int   tamDir;     /* Tamanho do diretório */
           int   nReg;       /* Número de registros  */
        } tTabelaDExt;

/************************* Alusões ************************/

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
