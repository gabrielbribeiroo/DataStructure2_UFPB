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

/***************** Constantes Simb�licas ******************/

   /* Tamanho do acr�scimo do bloco usado para conter */
   /* uma linha a cada chamada de realloc()           */
#define TAMANHO_BLOCO 256

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
 * LeLinhaIlimitada(): L� uma linha de tamanho arbitr�rio num
 *                     stream de texto e armazena os caracteres
 *                     lidos num array alocado dinamicamente
 *
 * Par�metros:
 *      tam (sa�da) - se n�o for NULL, apontar� para uma vari�vel
 *                    que armazenar� o tamanho do string
 *                    constitu�do pelos caracteres da linha
 *      stream (entrada) - stream de texto no qual ser� feita a
 *                         leitura
 *
 * Retorno: Endere�o do array contendo a linha lida. NULL, se
 *          ocorrer erro ou o final do arquivo for atingido
 *          antes da leitura de qualquer caractere
 *
 * Observa��es:
 *       1. O stream deve estar associado a um arquivo de texto
 *          aberto em modo de texto que permita leitura
 *       2. O caractere '\n' n�o � inclu�do no string resultante
 *          da leitura
 *       3. O primeiro par�metro pode ser NULL. Nesse caso, o
 *          tamanho do string n�o ser� armazenado
 *
 ****/
static char *LeLinhaIlimitada(int *tam, FILE *stream)
{
   char *ar = NULL,   /* Ponteiro para um array alocado */
                      /* dinamicamente que conter� os   */
                      /* caracteres lidos               */
        *p; /* Usado em chamada de realloc() */
   int   tamanho = 0, /* Tamanho do array alocado */
         c,  /* Armazenar� cada caractere lido */
         i; /* �ndice do pr�ximo caractere */
            /* a ser inserido no array     */

      /* L� caracteres a partir da posi��o corrente do      */
      /* indicador de posi��o do arquivo e armazena-os num  */
      /* array. A leitura encerra quando '\n' � encontrado, */
      /* o final do arquivo � atingido ou ocorre erro.      */
   for (i = 0; ; ++i) {
         /* L� o pr�ximo caractere no arquivo */
      c = fgetc(stream);

         /* Se ocorreu erro de leitura, libera o  */
         /* bloco eventualmente alocado e retorna */
      if (ferror(stream)) {
         free(ar); /* Libera o bloco apontado por 'ar' */
         return NULL; /* Ocorreu erro de leitura */
      }

         /* Verifica se array est� completo. O maior valor  */
         /* que i poderia assumir deveria ser tamanho - 1.  */
         /* Mas, como ao final, o caractere '\0' dever� ser */
         /* inserido, limita-se o valor de i a tamanho - 2. */
      if (i > tamanho - 2) { /* Limite atingido */
            /* Tenta redimensionar o array */
         p = realloc(ar, tamanho + TAMANHO_BLOCO);

            /* Se o redimensionamento n�o foi     */
            /* poss�vel, libera o bloco e retorna */
         if (!p) {
            free(ar); /* Libera o bloco apontado por 'ar' */
            return NULL; /* Ocorreu erro de aloca��o */
         }

            /* Redimensionamento foi OK. Ent�o, faz-se */
            /* 'ar' apontar para o novo bloco.         */
         ar = p;

            /* O array aumentou de tamanho */
         tamanho = tamanho + TAMANHO_BLOCO;
      }

         /* Se final do arquivo for atingido ou o caractere */
         /* '\n' for lido,  encerra-se a leitura            */
      if (feof(stream) || c == '\n') {
         break; /* Leitura encerrada */
      }

      ar[i] = c; /* Acrescenta o �ltimo caractere lido ao array */
   }

      /* Se nenhum caractere foi lido, libera */
      /* o espa�o alocado e retorna NULL      */
   if (feof(stream) && !i) {
      free(ar); /* Libera o bloco apontado por 'ar' */
      return NULL; /* Nenhum caractere foi armazenado no array */
   }

      /* Insere o caractere terminal de string. Neste    */
      /* instante, deve haver espa�o para ele porque o   */
      /* array foi sempre redimensionado deixando um     */
      /* espa�o a mais para o onipresente caractere '\0' */
   ar[i] = '\0';

      /* Atualiza o valor apontado pelo par�metro */
      /* 'tam', se ele n�o for NULL               */
   if (tam) {
         /* i � o �ndice do caractere terminal do */
         /* string e corresponde ao seu tamanho   */
      *tam = i;
   }

            /*                                     */
            /* >>> NB: O tamanho do string n�o <<< */
            /* >>>     inclui o caractere '\0' <<< */
            /*                                     */

      /* Tenta ajustar o tamanho do array para n�o */
      /* haver desperd�cio de mem�ria. Como i � o  */
      /* tamanho do string, o tamanho do array que */
      /* o cont�m deve ser i + 1.                  */
   p = realloc(ar, i + 1);

      /*                                                 */
      /* Se a realoca��o foi bem sucedida, retorna-se p. */
      /* Caso contr�rio, 'ar' ainda aponta para um bloco */
      /* v�lido. Talvez, haja desperd�cio de mem�ria,    */
      /* mas, aqui, � melhor retornar 'ar' do que NULL.  */
      /*                                                 */
   return p ? p : ar;
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
   aluno->codAlunoCurso = strtol(str, &final, 10);
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
      fprintf( stderr, "\n>>> O arquivo \"%s\" nao pode ser aberto\n",
               nome );
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

   deuErrado = fseek(stream, bytes, DeOnde);

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

   printf("\nCodigo do curso: %ld", registro->codAlunoCurso);

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
 * EscreveRegistro(): Escreve num stream um registro do tipo tRegistro
 *
 * Par�metros: *registro (entrada) - registro que ser� exibido
 *
 * Retorno: Nada
 *
 ****/
void EscreveRegistro(const tRegistro *registro, FILE *stream)
{
   fprintf(stream, "Codigo do aluno: %ld", registro->codigoAluno);

   fprintf(stream, "\nCodigo do curso: %ld", registro->codAlunoCurso);

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
