/****
 *
 * Arquivo Registros.h: Interface do m�dulo de processamento
 *                      de registros
 *
 * Autor: Ulysses de Oliveira
 *
 * Data de Cria��o: 20/04/2004
 * �ltima modifica��o: 19/07/2008
 *
 ****/

#ifndef _Registros_H_  /* Previne inclus�o       */
#define _Registros_H_  /* m�ltipla deste arquivo */

/*********************** Includes *************************/

#include <stdio.h>

/***************** Constantes Simb�licas ******************/

#define MAX_NOME_IES     200 /* Tamanho m�ximo de */
                             /* um nome de IES    */
#define MAX_NOME_CURSO   200 /* Tamanho m�ximo de */
                             /* um nome de curso  */

#define MAX_UF_NASCIMENTO 30 /* Tamanho m�ximo   */
                             /* de um nome de UF */

/****************** Defini��es de Tipos *******************/

typedef struct {
           long codigoAluno;
           int chave; /* Chave */
           char nomeIES[MAX_NOME_IES];
           char nomeCurso[MAX_NOME_CURSO];
           char sexoAluno;
           int  idadeAluno;
           char UFNascimento[MAX_UF_NASCIMENTO];
           int  anoIngresso;
         } tRegistroMEC;

typedef tRegistroMEC tRegistro;

typedef int tChave; /* Tipo da chave */

/******************* Alus�es de Fun��es *******************/

extern FILE *AbreArquivo(const char *nome, const char *modo);
extern void FechaArquivo(FILE *stream, const char *nome);
extern void MoveApontador(FILE *stream, long bytes, int DeOnde);
extern void ExibeRegistro(const tRegistroMEC *registro);
extern tRegistroMEC *LeRegistro(tRegistroMEC *regCEP);
extern tRegistroMEC *SubstituiRegistro( tRegistroMEC *novo,
                                     const tRegistroMEC *antigo );
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
extern void MedidaDeTempo(const char *info);

#endif
