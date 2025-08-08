/****
 *
 * Descri��o: Escreve os registros do arquivo bin�rio
 *            "MiniCensoMEC2012.bin" num arquivo de texto
 *
 * Observa��es:
 *     1. Este programa � �til em depura��o de �rvores
 *        multidirecionais implementadas em arquivo.
 *
 *     2. Este programa n�o cont�m tratamento de exce��o ou
 *        coment�rios adequados. Ou pior, muitos coment�rios
 *        n�o correspondem aquilo que � comentado.
 *
 *     3. IMPORATANTE: N�o utilize este programa com o
 *                     arquivo "CensoMEC2012.bin" (completo)
 *                     ou voc� ir� precisar comprar um novo HD.
 *
 * Autor: Gabriel Ribeiro
 *
 * Entrada: Arquivo cujo nome � especificado pela constante
 *          NOME_ARQ_REG
 *
 * Sa�da: Arquivo cujo nome � especificado pela constante
 *        NOME_ARQ_TXT
 *
 ****/

/*********************** Includes *************************/

#include <stdio.h>

/***************** Constantes Simb�licas ******************/

#define TAM_CODIGO_ALUNO  13 /* N�mero de d�gitos num   */
                             /* c�digo de aluno */
#define TAM_ALUNO_CURSO   8 /* N�mero de d�gitos num         */
                            /* c�digo de aluno/curso (chave) */
#define MAX_NOME_IES     200 /* Tamanho m�ximo de */
                             /* um nome de IES    */
#define MAX_NOME_CURSO   200 /* Tamanho m�ximo de */
                             /* um nome de curso  */

#define MAX_UF_NASCIMENTO 30 /* Tamanho m�ximo   */
                             /* de um nome de UF */

   /* Tamanho do acr�scimo do bloco usado para conter */
   /* uma linha a cada chamada de realloc()           */
#define TAMANHO_BLOCO 256

   /* Nome do arquivo de registros */
#define NOME_ARQ_REG     "MiniCensoMEC2012.bin"

   /* Nome do arquivo de texto contendo registros */
#define NOME_ARQ_TXT     "RegistrosTeste.txt"

/****************** Defini��es de Tipos *******************/

typedef struct {
           long codigoAluno;
           int  codAlunoCurso;
           char nomeIES[MAX_NOME_IES];
           char nomeCurso[MAX_NOME_CURSO];
           char sexoAluno;
           int  idadeAluno;
           char UFNascimento[MAX_UF_NASCIMENTO];
           int  anoIngresso;
         } tRegistroMEC;

typedef tRegistroMEC tRegistro;

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
static void ExibeArrayDeChars( FILE *stream, const char *nome,
                               const char *ar, int tam )
{
   fputc('\n', stream);

   if (*nome) {
      fprintf(stream, "%s: ", nome);
   }

   for (int i = 0; i < tam && *ar; ++i) {
      fputc(ar[i], stream);
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
void ExibeRegistro(const tRegistro *registro, FILE *stream)
{
   fprintf(stream, "Codigo do aluno: %lld", registro->codigoAluno);

   fprintf(stream, "\nCodigo do curso: %d", registro->codAlunoCurso);

   ExibeArrayDeChars(stream, "Nome da IES", registro->nomeIES,
                     MAX_NOME_IES);

   ExibeArrayDeChars(stream, "Nome do curso", registro->nomeCurso,
                     MAX_NOME_CURSO);

   fprintf(stream, "\nSexo do aluno: %s",
          registro->sexoAluno ? "Masculino" : "Feminino");

   fprintf(stream, "\nIdade do aluno: %d", registro->idadeAluno);

   ExibeArrayDeChars(stream, "UF de nascimento",
                     registro->UFNascimento, MAX_UF_NASCIMENTO);

   fprintf(stream, "\nAno de ingresso: %d", registro->anoIngresso);
}

/****
 *
 * main(): Escreve o conte�do de um arquivo bin�rio
 *         num arquivo de texto
 *
 * Par�metros: Nenhum
 *
 * Retorno: 0
 *
 ****/
int main(void)
{
   int     i = 0;
   tRegistroMEC  reg;
   FILE   *streamReg = fopen(NOME_ARQ_REG, "rb"),
          *streamTxt = fopen(NOME_ARQ_TXT, "w");

   while (1) {
      fread(&reg, sizeof(reg), 1, streamReg);

      if (feof(streamReg) || ferror(streamReg)) {
         break;
      }

      fprintf(streamTxt, "%d.\n\n", i++);

      ExibeRegistro(&reg, streamTxt);

      fprintf(streamTxt, "\n\n----------\n\n");
   }

   return 0;
}