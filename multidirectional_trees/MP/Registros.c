/****
 *
 * Arquivo Registros.c: Arquivo de programa do m�dulo de
 *                      processamento de registros
 *
 * Autor: Ulysses de Oliveira
 *
 * Data de Cria��o: 20/04/2004
 * �ltima modifica��o: 30/03/2017
 *
 ****/

/*********************** Includes *************************/

#include <stdio.h>  /* Fun��es de Entrada e Sa�da   */
#include <string.h> /* Processamento de strings     */
#include <stdlib.h> /* strtod() e aloca��o din�mica */
#include "Registros.h" /* Interface deste m�dulo    */
#include "leitura.h"  /* Leitura via teclado        */
#include "ASSEGURA.h" /* Macro ASSEGURA             */

/********************* Fun��es Locais *********************/

/****
 *
 * ExibeArrayDeChars(): Exibe na tela um array de elementos
 *                      do tipo char que n�o � string
 *
 * Par�metros:
 *      *nome (entrada) - string que ser� exibido antes do array
 *       ar (entrada) - endere�o do array
 *       tam (entrada) - tamanho do array
 *
 * Retorno: Nada
 *
 ****/
static void ExibeArrayDeChars(const char *nome, const char *ar,
                              int tam)
{
   putchar('\n');

   if (*nome) {
      printf("%s: ", nome);
   }

   for (int i = 0; i < tam && *ar; ++i) {
      putchar(ar[i]);
   }
}

/********************* Fun��es Globais ********************/

/****
 *
 * AbreArquivo(): Tenta abrir um arquivo e, quando isso
 *                n�o � poss�vel, aborta o programa
 *
 * Par�metros:
 *      nome (entrada) - nome do arquivo
 *      modo (entrada) - modo de abertura
 *
 * Retorno: Stream associado ao arquivo que foi aberto
 *
 ****/
FILE *AbreArquivo(const char *nome, const char *modo)
{
   FILE *stream;

   stream = fopen(nome, modo);

   if (!stream) { /* Erro de abertura */
      fprintf( stderr, "\n>>> O arquivo %s nao pode ser aberto",
               nome);
      exit(1); /* Aborta o programa */
   }

   return stream;
}

/****
 *
 * FechaArquivo(): Tenta fechar um arquivo e, quando isso
 *                 n�o � poss�vel, aborta o programa
 *
 * Par�metros:
 *      stream (entrada) - stream associado ao arquivo
 *      nomeArq (entrada) - nome do arquivo ou NULL
 *
 * Retorno: Nada
 *
 * Observa��o: Se o segundo par�metro n�o for NULL, o nome
 *             do arquivo aparece na mensagem de erro
 *
 ****/
void FechaArquivo(FILE *stream, const char *nomeArq)
{
   /***********************************************************/
   /* Se fclose() retornar um valor diferente de zero ocorreu */
   /* algum erro na tentativa de fechamento do arquivo. Nesse */
   /* caso, apresenta mensagem de erro e aborta o programa.   */
   /***********************************************************/

   if (fclose(stream)) { /* Erro de fechamento */
      fprintf( stderr, "\a\n>>> Ocorreu erro no fechamento "
              "do arquivo %s.\n>>> O programa sera' "
              "encerrado.\n", nomeArq ? nomeArq : "");
      exit(1); /* Aborta o programa */
   }
}

/****
 *
 * MoveApontador(): Tenta mover o apontador de posi��o de um
 *                  arquivo e, quando isso n�o � poss�vel,
 *                  aborta o programa
 *
 * Par�metros:
 *      stream (entrada) - stream associado ao arquivo
 *      bytes (entrada) - quantidade de bytes que ser�o saltados
 *      DeOnde (entrada) - a partir de onde ocorrer� a movimenta��o
 *
 * Retorno: Nada
 *
 ****/
void MoveApontador(FILE *stream, long bytes, int DeOnde)
{
   int deuErrado;

   deuErrado = fseek(stream, (long) bytes, DeOnde);

      /* Verifica se houve erro */
   if (deuErrado) {
      fprintf( stderr, "\n>>> Erro na tentativa de mover "
                       "apontador de arquivo" );
      exit(1);
   }
}

/****
 *
 * ExibeRegistro(): Exibe na tela um registro do tipo tRegistro
 *
 * Par�metros: *registro (entrada) - registro que ser� exibido
 *
 * Retorno: Nada
 *
 ****/
void ExibeRegistro(const tRegistro *registro)
{
   printf("Codigo do aluno: %ld", registro->codigoAluno);

   printf("\nCodigo do curso: %d", registro->codAlunoCurso);

   ExibeArrayDeChars( "Nome da IES", registro->nomeIES,
                      MAX_NOME_IES );

   ExibeArrayDeChars("Nome do curso", registro->nomeCurso,
                     MAX_NOME_CURSO);

   printf("\nSexo do aluno: %s",
          registro->sexoAluno ? "Masculino" : "Feminino" );

   printf("\nIdade do aluno: %d", registro->idadeAluno);

   ExibeArrayDeChars("UF de nascimento",
                     registro->UFNascimento, MAX_UF_NASCIMENTO);

   printf("\nAno de ingresso: %d", registro->anoIngresso);
}

/****
 *
 * LeRegistro(): L� os dados de um aluno
 *
 * Par�metros: *aluno (sa�da) - estrutura que receber� os
 *                              dados introduzidos pelo
 *                              usu�rio
 * Retorno: O par�metro
 *
 ****/
tRegistro *LeRegistro(tRegistro *aluno)
{
   char ar[MAX_NOME_IES + 1];
   int  op;

   aluno->codigoAluno = LeChaveLonga( "Codigo do aluno",
                                      TAM_CODIGO_ALUNO );

   printf("\n\t>>> Codigo do curso: ");
   aluno->codAlunoCurso = LeNatural();

   printf("\n\t>>> Nome da IES: ");
   LeString(ar, MAX_NOME_IES + 1);
   strncpy(aluno->nomeIES, ar, MAX_NOME_IES);

   printf("\n\t>>> Nome do curso: ");
   LeString(ar, MAX_NOME_CURSO + 1);
   strncpy(aluno->nomeCurso, ar, MAX_NOME_CURSO);

   printf("\n\t>>> Sexo do aluno: ");
   op = LeOpcao("MmFF");
   aluno->sexoAluno = op == 'M' || op == 'm' ? 1 : 0;

   printf("\n\t>>> Idade do aluno: ");
   aluno->idadeAluno = LeNatural();

   printf("\n\t>>> UF de nascimento: ");
   LeString(ar, MAX_UF_NASCIMENTO + 1);
   strncpy(aluno->UFNascimento, ar, MAX_UF_NASCIMENTO);

   printf("\n\t>>> Ano de ingresso: ");
   aluno->anoIngresso = LeNatural();

   return aluno;
}

/****
 *
 * SubstituiRegistro(): L� os dados de um registro que, se forem
 *                      v�lidos, substituir�o valores antigos
 *
 * Par�metros:
 *     *novo (sa�da) - estrutura que armazenar� os novos
 *                     dados introduzidos pelo usu�rio
 *     *antigo (entrada) - estrutura contendo os dados antigos
 *
 * Retorno: O primeiro par�metro
 *
 * Observa��o: Esta fun��o n�o est� rigorosamente bem
 *             implementada pois n�o faz parte do tema
 *             central deste exemplo
 *
 ****/
tRegistro *SubstituiRegistro( tRegistro *novo,
                              const tRegistro *antigo )
{
//   char ar[MAX_COMP + 1];
//   int  num;

   printf("\n>>> ATENCAO: SubstituiRegistro() nao implementada <<<\n");

//   printf("\n>>> ATENCAO: Dados invalidos nao substituem "
//          "dados antigos <<<\n");
//
//      /* L� o valor de UF. Qualquer string que n�o seja  */
//      /* vazio � considerado v�lido, mas n�o deveria ser */
//      /* assim (v. Observa��o). O mesmo vale para os     */
//      /* demais strings.                                 */
//   printf("\n\t>>> UF: ");
//   LeString(ar, TAM_UF + 1);
//   strncpy(novo->UF, *ar ? ar : antigo->UF, TAM_UF);
//
//   printf("\n\t>>> Numero de localidade: ");
//   num = LeNatural();
//   novo->localidadeNumero = num >= 0 ?
//                            num :
//                            antigo->localidadeNumero;
//
//   printf("\n\t>>> Nome abreviado: ");
//   LeString(ar, MAX_NOME + 1);
//   strncpy(novo->nomeAbr, *ar ? ar : antigo->nomeAbr, MAX_NOME);
//
//   printf("\n\t>>> Nome: ");
//   LeString(ar, MAX_NOME + 1);
//   strncpy(novo->nome, *ar ? ar : antigo->nome, MAX_NOME);
//
//   printf("\n\t>>> Bairro inicio: ");
//   num = LeNatural();
//   novo->bairroInicio = num >= 0 ?
//                            num :
//                            antigo->bairroInicio;
//
//   printf("\n\t>>> Bairro fim: ");
//   num = LeNatural();
//   novo->bairroFim = num >= 0 ?
//                            num :
//                            antigo->bairroFim;
//
//   printf("\n\t>>> CEP: ");
//   LeString(ar, TAM_CEP + 1);
//   strncpy(novo->CEP, *ar ? ar : antigo->CEP, TAM_CEP);
//
//   printf("\n\t>>> Complemento: ");
//   LeString(ar, MAX_COMP + 1);
//   strncpy(novo->complemento, *ar ? ar : antigo->complemento,
//           MAX_COMP);
//
//   printf("\n\t>>> Tipo de logradouro: ");
//   LeString(ar, MAX_TIPO_LOG + 1);
//   strncpy(novo->tipoLogradouro,
//           *ar ? ar : antigo->tipoLogradouro,
//           MAX_TIPO_LOG);
//
//   printf("\n\t>>> Status tipo de logradouro: ");
//   num = LeCaractere();
//
//   if (num == 's' || num == 'S') {
//      novo->statusTipoLogradouro = 'S';
//   } else if (num == 'n' || num == 'N') {
//      novo->statusTipoLogradouro = 'N';
//   } else {
//      novo->statusTipoLogradouro = antigo->statusTipoLogradouro;
//   }
//
//   printf("\n\t>>> Nome sem acento: ");
//   LeString(ar, MAX_NOME + 1);
//   strncpy(novo->nomeSemAcento,
//           *ar ? ar : antigo->nomeSemAcento, MAX_NOME);
//
//   printf("\n\t>>> Chave DNE: ");
//   LeString(ar, TAM_DNE + 1);
//   strncpy(novo->chaveDNE, *ar ? ar : antigo->chaveDNE,
//           TAM_DNE);

   return novo;
}

/****
 *
 * TamanhoDeArquivo(): Calcula o tamanho de um arquivo lendo cada
 *                     byte que o constitui e contando quantos
 *                     bytes s�o lidos
 *
 * Par�metros:
 *    stream (entrada) - stream bin�rio associado ao arquivo cujo
 *                       tamanho ser� calculado
 *
 * Retorno: -1, se ocorrer algum erro.
 *          O tamanho do arquivo, em caso contr�rio.
 *
 * Observa��es:
 *     O arquivo deve estar aberto em modo bin�rio que permite
 *     leitura
 *
 ****/
long TamanhoDeArquivo(FILE *stream)
{
   ASSEGURA(stream, "Erro em TamanhoDeArquivo(): stream e' NULL");

      /* Tenta mover o apontador de posi��o do arquivo */
      /* para o final do mesmo; se n�o for poss�vel,   */
      /* o programa ser� abortado                      */
   MoveApontador(stream, 0, SEEK_END);

   return ftell(stream);
}

/****
 *
 * NumeroDeRegistros(): Calcula o n�mero de registros (i.e.,
 *                      parti��es de tamanho especificado) de um
 *                      arquivo bin�rio
 *
 * Par�metros:
 *    stream (entrada) - stream bin�rio associado ao arquivo cujo
 *                       tamanho ser� calculado
 *    tamRegistro (entrada) - tamanho de cada parti��o do arquivo
 *
 * Retorno: O n�mero de parti��es do arquivo, se n�o ocorrer
 *          nenhum erro, ou -1, em caso contr�rio.
 *
 * Observa��o: O arquivo deve estar aberto em modo bin�rio que
 *             permite leitura
 *
 ****/
int NumeroDeRegistros( FILE *stream, int tamRegistro )
{
   long nBytes = 0; /* N�mero de bytes do arquivo */

      /* Calcula o n�mero de bytes do arquivo */
   nBytes = TamanhoDeArquivo(stream);

      /* Verifica se ocorreu erro na contagem de bytes */
   ASSEGURA(nBytes >= 0, "Erro na contagem de bytes");

      /* O n�mero de registros � o n�mero de bytes do arquivo */
      /* dividido pelo n�mero de bytes de cada registro       */
   return nBytes/tamRegistro;
}

/****
 *
 * LeRegistroEmArquivo(): L� um registro no arquivo de registros
 *
 * Par�metros:
 *     *umRegistro (sa�da) - registro lido
 *      stream (entrada) - stream associado ao arquivo
 *                         de registros
 *      pos (entrada) - posi��o do registro no arquivo
 *
 * Retorno: o primeiro par�metro, se a recupera��o for
 *           bem sucedida; NULL, em caso contr�rio.
 *
 ****/
tRegistro *LeRegistroEmArquivo( tRegistro *umRegistro,
                                FILE *stream, int pos )
{
   ASSEGURA(stream, "Arquivo de registros eh nulo");

   MoveApontador(stream, pos*sizeof(*umRegistro), SEEK_SET);

   return fread(umRegistro, sizeof(*umRegistro), 1, stream)
          ? umRegistro
          : NULL;
}

/****
 *
 * InsereRegistroEmArquivo(): Insere um registro num arquivo
 *
 * Par�metros:
 *     chave (entrada) - a chave do registro
 *     stream (entrada) - stream associado ao arquivo de registros
 *
 * Retorno: Nada
 *
 ****/
void InsereRegistroEmArquivo(tChave chave, FILE *stream)
{
   tRegistro reg;

   ASSEGURA( stream, "Arquivo de registros e' vazio" );

   reg.codAlunoCurso = chave;

   printf("\nChave: %d", chave);

      /* Recebe do us�rio os dados do */
      /* registro a ser inserido      */
   LeRegistro(&reg);

      /* A inser��o ser� feita ao final do arquivo */
   MoveApontador(stream, 0, SEEK_END);

   fwrite(&reg, sizeof(reg), 1, stream);

   ASSEGURA(!ferror(stream), "Erro de escrita em arquivo");
}

/****
 *
 * ArquivoExiste(): Verifica se um arquivo existe
 *
 * Par�metros:
 *     nomeArq (entrada) - nome do arquivo que se
 *                        verificar� se existe
 *
 * Retorno: 1, se o arquivo existir; 0, em caso contr�rio
 *
 ****/
int ArquivoExiste(const char *nomeArq)
{
   FILE *stream;
   int   existe;

   stream = fopen(nomeArq, "rb");

   existe = stream ? 1 : 0;

   if (existe) {
      FechaArquivo(stream, nomeArq);
   }

   return existe;
}