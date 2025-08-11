/****
 *
 * Descrição: Escreve os registros do arquivo binário
 *            "MiniCensoMEC2012.bin" num arquivo de texto
 *
 * Observações:
 *     1. Este programa é útil em depuração de árvores
 *        multidirecionais implementadas em arquivo.
 *
 *     2. Este programa não contém tratamento de exceção ou
 *        comentários adequados. Ou pior, muitos comentários
 *        não correspondem aquilo que é comentado.
 *
 *     3. IMPORATANTE: Não utilize este programa com o
 *                     arquivo "CensoMEC2012.bin" (completo)
 *                     ou você irá precisar comprar um novo HD.
 *
 * Autor: Ulysses de Oliveira
 *
 * Data de Criação: 15/03/2017
 * Última modificação: 15/03/2017
 *
 * Entrada: Arquivo cujo nome é especificado pela constante
 *          NOME_ARQ_REG
 *
 * Saída: Arquivo cujo nome é especificado pela constante
 *        NOME_ARQ_TXT
 *
 ****/

/*********************** Includes *************************/

#include <stdio.h>

/***************** Constantes Simbólicas ******************/

#define TAM_CODIGO_ALUNO  13 /* Número de dígitos num   */
                             /* código de aluno */
#define TAM_ALUNO_CURSO   8 /* Número de dígitos num         */
                            /* código de aluno/curso (chave) */
#define MAX_NOME_IES     200 /* Tamanho máximo de */
                             /* um nome de IES    */
#define MAX_NOME_CURSO   200 /* Tamanho máximo de */
                             /* um nome de curso  */

#define MAX_UF_NASCIMENTO 30 /* Tamanho máximo   */
                             /* de um nome de UF */

   /* Tamanho do acréscimo do bloco usado para conter */
   /* uma linha a cada chamada de realloc()           */
#define TAMANHO_BLOCO 256

   /* Nome do arquivo de registros */
#define NOME_ARQ_REG     "MiniCensoMEC2012.bin"

   /* Nome do arquivo de texto contendo registros */
#define NOME_ARQ_TXT     "RegistrosTeste.txt"

/****************** Definições de Tipos *******************/

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
 *                      do tipo char que não é string
 *
 * Parâmetros:
 *      *nome (entrada) - string que será exibido antes do array
 *       ar (entrada) - endereço do array
 *       tam (entrada) - tamanho do array
 *
 * Retorno: Nada
 *
 ****/
static void ExibeArrayDeChars(FILE *stream, const char *nome, const char *ar,
                              int tam)
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
 * Parâmetros: *registro (entrada) - registro que será exibido
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
 * main(): Escreve o conteúdo de um arquivo binário
 *         num arquivo de texto
 *
 * Parâmetros: Nenhum
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
