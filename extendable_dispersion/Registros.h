/****
 *
 * Arquivo Registros.h: Interface do módulo de processamento
 *                      de registros
 *
 * Autor: Ulysses de Oliveira
 *
 * Data de Criação: 20/04/2004
 * Última modificação: 19/07/2008
 *
 ****/

#ifndef _Registros_H_  /* Previne inclusão       */
#define _Registros_H_  /* múltipla deste arquivo */

/*********************** Includes *************************/

#include <stdio.h>

/***************** Constantes Simbólicas ******************/

#define MAX_NOME_IES     200 /* Tamanho máximo de */
                             /* um nome de IES    */
#define MAX_NOME_CURSO   200 /* Tamanho máximo de */
                             /* um nome de curso  */
#define MAX_UF_NASCIMENTO 30 /* Tamanho máximo   */
                             /* de um nome de UF */

/****************** Definições de Tipos *******************/

typedef struct {
           long codigoAluno;
           long codAlunoCurso; /* Chave */
           char nomeIES[MAX_NOME_IES];
           char nomeCurso[MAX_NOME_CURSO];
           char sexoAluno;
           int  idadeAluno;
           char UFNascimento[MAX_UF_NASCIMENTO];
           int  anoIngresso;
         } tRegistroMEC;

typedef tRegistroMEC tRegistro;

typedef long tChave; /* Tipo da chave */

/******************* Alusões de Funções *******************/

extern FILE *AbreArquivo(const char *nome, const char *modo);
extern void FechaArquivo(FILE *stream, const char *nome);
extern void MoveApontador(FILE *stream, long bytes, int DeOnde);

extern void ExibeRegistro(const tRegistroMEC *registro);
extern tRegistroMEC *LeRegistro(tRegistroMEC *registro, int leChave);
extern long TamanhoDeArquivo(FILE *stream);
extern int NumeroDeRegistros( FILE *stream, int tamRegistro );
extern tRegistroMEC *LeRegistroEmArquivo( tRegistroMEC *umRegistro,
                                       FILE *stream, int pos );
extern void InsereRegistroEmArquivo(tChave chave, FILE *stream);
extern int ArquivoExiste(const char *nomeArq);
extern int CriaArquivoBin( const char *nomeTexto,
                           const char *nomeBin, int maxRegistros );
extern void EscreveRegistro(const tRegistroMEC *registro, FILE *stream);
extern tChave ObtemChave(const tRegistroMEC *registro);
extern void AtribuiChave(tRegistroMEC *registro, tChave chave);
extern void MedidaDeTempo(const char *info);

#endif

