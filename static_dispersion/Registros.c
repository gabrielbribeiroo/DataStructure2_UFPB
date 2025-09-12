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
#include "Leitura.h"  /* Leitura via teclado        */
#include "ASSEGURA.h" /* Macro ASSEGURA             */
#include <time.h>     /* Medi��o de tempo           */

/********************* Fun��es Locais *********************/

/****
 *
 * EscreveArrayDeChars(): Escreve num stream um array de elementos
 *                        do tipo char que n�o � string
 *
 * Par�metros:
 *      *nome (entrada) - string que ser� exibido antes do array
 *       ar (entrada) - endere�o do array
 *       tam (entrada) - tamanho do array
 *
 * Retorno: Nada
 *
 ****/
static void EscreveArrayDeChars( FILE *stream, const char *nome,
                                 const char *ar, int tam )
{
   fputc('\n', stream);

   if (*nome)
      fprintf(stream, "%s: ", nome);

   for (int i = 0; i < tam && *ar; ++i)
      fputc(ar[i], stream);
}

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

/****
*
* ObtemTokens(): Separa um string em partes (tokens)
*
* Par�metros:
*     str (entrada/sa�da) - o string que ser� separado
*                           ou NULL para continuar uma
*                           separa��o anterior
*     sep (entrada) - string contendo os separadores de tokens
*
* Retorno: Endere�o do pr�ximo token do string, se este
*          existir, ou NULL, em caso contr�rio
*
* Observa��o: Esta fun��o N�O � funcionalmente equivalente �
*             fun��o strtok() da biblioteca padr�o de C
*
****/
static char *ObtemTokens(char *str, char const *sep)
{
   static char *proximoToken; /* Aponta para o pr�ximo */
                              /* token, se ele existir */
   char        *s, /* Apontar� para o string no qual a */
                   /* busca pelo token ser� efetuada   */
               *inicio; /* Guardar� o in�cio */
                        /* do token corrente */

   if (str != NULL)
      proximoToken = str;

   if (proximoToken == NULL)
      return NULL;

   if ((s = strpbrk(proximoToken, sep)) != NULL) {
      *s  = '\0';
      inicio = proximoToken;
      proximoToken = ++s;
   } else if (*proximoToken) {
      inicio = proximoToken;
      proximoToken = NULL;
   }

   return inicio;
}

/****
 *
 * LinhaEmRegistro(): Converte uma linha do arquivo numa
 *                    estrutura do tipo tRegistroMEC.
 *
 * Par�metros: aluno (sa�da) - ponteiro para a estrutura
 *                             resultante da convers�o
 *             linha (entrada/sa�da) - linha que ser� convertida
 *
 * Retorno: Endere�o da estrutura que armazena o resultado
 *          da convers�o
 *
 * Observa��es:
 *     1. Para facilitar o entendimento, esta fun��o assume
 *        que o par�metro 'linha' realmente � um string no
 *        formato das linhas do arquivo. Portanto, os
 *        valores retornados por ObtemTokens() n�o s�o testados
 *        como deveriam.
 *
 *     2. O par�metro 'linha' � alterado por ObtemTokens().
 *
 *     3. A fun��o ObtemTokens() � usada em lugar de strtok()
 *        devido � presen�a de campos vazios no arquivo de
 *        texto, o que faz com que dois separadores apare�am
 *        juntos. Nesse caso, a fun��o strtok() consideraria
 *        os dois separadores como sendo apenas um.
 *
 *     4. O caractere '\r' faz parte das chamadas da fun��o
 *        ObtemTokens() para satisfazer uma bizarrice do
 *        sistema Mac OS X, que � o �nico sistema Unix que
 *        usa '\r' como quebra de linha.
 *
 ****/
static tRegistroMEC *LinhaEmRegistro(tRegistroMEC *aluno, char *linha)
{
   char *str, /* Apontar� para cada token da linha */
        *final; /* Final de uma convers�o num�rica */

      /* Obt�m o c�digo do aluno, converte-o e         */
      /* acrescenta-o ao respectivo campo da estrutura */
   str = ObtemTokens(linha, "\t\n\r;");
   ++str; /* Salta espa�o em branco inicial */
   aluno->codigoAluno = strtoll(str, &final, 10);
   ASSEGURA(!*final, "Erro de conversao numerica");

      /* Obt�m o c�digo de v�nculo do aluno ao */
      /* curso, converte-o e acrescenta-o ao   */
      /* respectivo campo da estrutura         */
   str = ObtemTokens(NULL, "\t\n\r;");
   aluno->chave = strtol(str, &final, 10);
   ASSEGURA(!*final, "Erro de conversao numerica");

      /* Obt�m o nome da IES e acrescenta-a */
      /* ao respectivo campo da estrutura   */
   str = ObtemTokens(NULL, "\t\n\r;");
   strncpy(aluno->nomeIES, str, MAX_NOME_IES);

      /* Obt�m o nome do curso e acrescenta-o */
      /* ao respectivo campo da estrutura     */
   str = ObtemTokens(NULL, "\t\n\r;");
   strncpy(aluno->nomeCurso, str, MAX_NOME_CURSO);

      /* Obt�m o sexo do aluno e acrescenta-o */
      /* ao respectivo campo da estrutura     */
   str = ObtemTokens(NULL, "\t\n\r;");
   ASSEGURA(*str == '0' || *str == '1', "Sexo indefinido");
   aluno->sexoAluno = *str == '0' ? 0 : 1;

      /* Obt�m a idade do aluno e acrescenta-o */
      /* ao respectivo campo da estrutura      */
   str = ObtemTokens(NULL, "\t\n\r;");
   aluno->idadeAluno = strtol(str, &final, 10);
   ASSEGURA(!*final, "Erro de conversao numerica");

      /* Obt�m a UF de nascimento e acrescenta-a */
      /* ao respectivo campo da estrutura        */
   str = ObtemTokens(NULL, "\t\n\r;");
   strncpy(aluno->UFNascimento, str, MAX_UF_NASCIMENTO);

      /* Obt�m o ano de ingresso do aluno e acrescenta-o */
      /* ao respectivo campo da estrutura                */
   str = ObtemTokens(NULL, "\t\n\r;");
   aluno->anoIngresso = strtol(str, &final, 10);
   ASSEGURA(!*final, "Erro de conversao numerica");

   return aluno;
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
 * ExibeRegistro(): Exibe na tela um registro do tipo tRegistroMEC
 *
 * Par�metros: *registro (entrada) - registro que ser� exibido
 *
 * Retorno: Nada
 *
 ****/
void ExibeRegistro(const tRegistroMEC *registro)
{
   printf("Codigo do aluno: %ld", registro->codigoAluno);

   printf("\nCodigo do curso: %ld", registro->chave);

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
tRegistroMEC *LeRegistro(tRegistroMEC *aluno)
{
   char ar[MAX_NOME_IES + 1];
   int  op;

   aluno->codigoAluno = LeChave();

   printf("\n\t>>> Codigo do curso: ");
   aluno->chave = LeNatural();

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
tRegistroMEC *SubstituiRegistro( tRegistroMEC *novo,
                              const tRegistroMEC *antigo )
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
   ASSEGURA(stream, "Erro em TamanhoDeArquivo(): stream e' NULL" );

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
tRegistroMEC *LeRegistroEmArquivo( tRegistroMEC *umRegistro,
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
   tRegistroMEC reg;

   ASSEGURA( stream, "Arquivo de registros e' vazio" );

   reg.chave = chave;

   printf("\nChave: %ld", chave);

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

/****
 *
 * CriaArquivoBin(): L� cada linha de um arquivo no formato
 *                   especificado, converte-a numa estrutura
 *                   do tipo tRegistroMEC e armazena a estrutura
 *                   num arquivo bin�rio
 *
 * Par�metros:
 *     streamTexto (entrada) - stream associado ao arquivo que
 *                             ser� lido
 *     streamBin (entrada) - stream associado ao arquivo que ser�
 *                           escrito
 *     maxRegistros (entrada) - n�mero m�ximo de registros que
 *                              ser�o gerados
 *
 * Retorno: N�mero de estruturas escritas no arquivo
 *          bin�rio ou um valor negativo, se ocorrer erro
 *
 * Observa��es:
 *     1. O stream de entrada deve estar aberto em modo de
 *        texto que permite leitura
 *     2. O stream de sa�da deve estar aberto em modo "wb"
 *
 ****/
int CriaArquivoBin( const char *nomeTexto, const char *nomeBin,
                    int maxRegistros )
{
   char  *linha; /* Apontar� para cada linha lida */
   tRegistroMEC umAluno; /* Dados de uma linha      */
                   /* convertida em estrutura */
   int   nRegistros = 0, /* N�mero de estruturas escritas */
                         /* no arquivo bin�rio            */
         ocorreuErro = 0;
   FILE *streamTexto,
        *streamBin;

      /* Tenta abrir arquivo para leitura em modo texto */
   streamTexto = AbreArquivo(nomeTexto, "r");

      /* Tenta abrir arquivo para escrita em modo bin�rio */
   streamBin = AbreArquivo(nomeBin, "w");

      /* L� cada linha do arquivo, cria um registro do tipo     */
      /* tRegistroMEC e armazena-o no arquivo bin�rio. O la�o encerra */
      /* quando h� tentativa de leitura al�m do final do        */
      /* arquivo de entrada, ou ocorre erro de leitura/escrita  */
   while (1) {
         /* L� uma linha no arquivo de entrada */
      linha = LeLinhaIlimitada(NULL, streamTexto);

         /* Verifica se o final do arquivo foi  */
         /* atingido ou ocorreu erro de leitura */
      if (feof(streamTexto) || ferror(streamTexto))
         break;

         /* Converte a linha lida em estrutura */
      LinhaEmRegistro(&umAluno, linha);

         /* Escreve a estrutura resultante da */
         /* convers�o no arquivo bin�rio      */
      fwrite(&umAluno, sizeof(umAluno), 1, streamBin);

         /* Verifica se ocorreu erro de escrita */
      if (ferror(streamBin))
         break;

      ++nRegistros; /* Mais um registro lido */

      if (maxRegistros && nRegistros >= maxRegistros)
         break;
   }

   ocorreuErro = ferror(streamTexto) || ferror(streamBin);

      /* Fecha os arquivos */
   FechaArquivo(streamTexto, nomeTexto);
   FechaArquivo(streamBin, nomeBin);

   ASSEGURA(!ocorreuErro,
            "Erro de processamento de arquivo em CriaArquivoBin");

   return nRegistros; /* N�o ocorreu nenhum erro */
}

/****
 *
 * EscreveRegistro(): Escreve num stream um registro do tipo tRegistroMEC
 *
 * Par�metros: *registro (entrada) - registro que ser� exibido
 *
 * Retorno: Nada
 *
 ****/
void EscreveRegistro(const tRegistroMEC *registro, FILE *stream)
{
   fprintf(stream, "Codigo do aluno: %ld", registro->codigoAluno);

   fprintf(stream, "\nCodigo do curso: %ld", registro->chave);

   EscreveArrayDeChars(stream, "Nome da IES", registro->nomeIES,
                       MAX_NOME_IES);

   EscreveArrayDeChars(stream, "Nome do curso", registro->nomeCurso,
                       MAX_NOME_CURSO);

   fprintf(stream, "\nSexo do aluno: %s",
          registro->sexoAluno ? "Masculino" : "Feminino");

   fprintf(stream, "\nIdade do aluno: %d", registro->idadeAluno);

   EscreveArrayDeChars(stream, "UF de nascimento",
                       registro->UFNascimento, MAX_UF_NASCIMENTO);

   fprintf(stream, "\nAno de ingresso: %d", registro->anoIngresso);
}

/****
 *
 * ObtemChave(): Retorna a chave de um registro
 *
 * Par�metros: registro (entrada) - o referido registro
 *
 * Retorno: A chave do registro recebido como par�metro
 *
 ****/
tChave ObtemChave(const tRegistroMEC *registro)
{
   return registro->chave;
}

/****
 *
 * MedidaDeTempo(): Mede intervalos de tempo entre duas chamadas
 *                  consecutivas
 *
 * Par�metros:
 *     info (entrada) - informa��o que ser� apresentada na tela
 *
 * Retorno: Nada
 *
 * Observa��o: Esta fun��o deve ser chamada aos pares; i.e.,
 *             na primeira chamada ela marca o in�cio da
 *             contagem de tempo e na segunda chamada ela
 *             apresenta o intervalo de tempo decorrido
 *             desde a �ltima chamada
 *
 ****/
void MedidaDeTempo(const char *info)
{
   static int    numeroDeChamadas = 0;
   static time_t t0; /* Instante inicial */
   time_t        tf; /* Instante final   */

   if (info) {
      printf("\n>>> %s\n", info);
      fflush(stdout);
   }

      /* Esta fun��o foi chamada mais uma vez */
   ++numeroDeChamadas;

      /* Se o n�mero da chamada for �mpar, esta */
      /* chamada � a primeira de um par         */
   if (numeroDeChamadas%2) { /* Calcula o instante inicial */
      t0 = time(NULL);
   } else { /* Calcula e apresenta o intervalo de   */
            /* tempo entre as duas �ltimas chamadas */
      tf = time(NULL);

      printf( "\n\n\t*** Tempo gasto na operacao: "
              "%5.2f segundos ***\n", difftime(tf, t0) );
   }
}
