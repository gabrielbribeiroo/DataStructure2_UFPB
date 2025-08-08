/****
 *
 * T�tulo: Convertendo um Arquivo de Registros de Texto para Bin�rio
 *
 * Autor: Gabriel Ribeiro

 * Descri��o: L� cada linha de um arquivo de texto, converte-a numa
 *            estrutura do tipo tRegistroMEC e armazena-a num arquivo
 *            bin�rio
 *
 * Entrada: Arquivo cujo nome � especificado pela constante
 *          NOME_ARQUIVO_TEXTO
 *
 * Sa�da: Arquivo cujo nome � especificado pela constante
 *        NOME_ARQUIVO_BIN
 *
 ****/

/*********************** Includes *************************/

#include <stdio.h>  /* Fun��es de Entrada e Sa�da */
#include <string.h> /* Processamento de strings   */
#include <stdlib.h> /* Fun��o strtod() e aloca��o din�mica */

/***************** Constantes Simb�licas ******************/

#define MAX_NOME_IES     200 /* Tamanho m�ximo de */
                             /* um nome de IES    */
#define MAX_NOME_CURSO   200 /* Tamanho m�ximo de */
                             /* um nome de curso  */

#define MAX_UF_NASCIMENTO 30 /* Tamanho m�ximo   */
                             /* de um nome de UF */

   /* Nome do arquivo de texto */
#define NOME_ARQUIVO_TEXTO "CensoMEC2012.txt"

   /* Nome do arquivo bin�rio */
#define NOME_ARQUIVO_BIN "MiniCensoMEC2012.bin"

   /* N�mero m�ximo de registros que ser�o gerados */
#define MAX_REGISTTROS 50

   /* Tamanho do acr�scimo do bloco usado para conter */
   /* uma linha a cada chamada de realloc()           */
#define TAMANHO_BLOCO 256

   /* Macro utilizada em tratamento */
   /* de condi��es de exce��o       */
#define ASSEGURA(condicao, msg) if (!(condicao)) {\
                                   fprintf(stderr, "\n%s\n", msg);\
                                   exit(1); \
                                }

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

/***************** Defini��es de Fun��es ******************/

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
char *LeLinhaIlimitada(int *tam, FILE *stream)
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
char *ObtemTokens(char *str, char const *sep)
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
 *             chave (entrada) - a chave do registro
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
tRegistroMEC *LinhaEmRegistro(tRegistroMEC *aluno, char *linha, int chave)
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

/****
 *
 * CriaArquivoBin(): L� cada linha de um arquivo no formato
 *                   especificado, converte-a numa estrutura
 *                   do tipo tRegistroMEC e armazena a estrutura
 *                   num arquivo bin�rio.
 *
 * Par�metros:
 *     streamTexto (entrada) - stream associado ao arquivo que
 *                             ser� lido
 *     streamBin (entrada) - stream associado ao arquivo que ser�
 *                           escrito
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
int CriaArquivoBin(FILE *streamTexto, FILE  *streamBin)
{
   char  *linha; /* Apontar� para cada linha lida */
   tRegistroMEC umAluno; /* Dados de uma linha      */
                   /* convertida em estrutura */
   int   nRegistros = 0, /* N�mero de estruturas escritas */
                         /* no arquivo bin�rio            */
         chave = 0;

      /* Garante leitura a partir do in�cio do arquivo */
   rewind(streamTexto);

      /* L� cada linha do arquivo, cria um registro do tipo     */
      /* tRegistroMEC e armazena-o no arquivo bin�rio. O la�o encerra */
      /* quando h� tentativa de leitura al�m do final do        */
      /* arquivo de entrada, ou ocorre erro de leitura/escrita  */
   while (1) {
         /* L� uma linha no arquivo de entrada */
      linha = LeLinhaIlimitada(NULL, streamTexto);

         /* Verifica se o final do arquivo foi atingido */
      if (feof(streamTexto)) {
         break;
      }

         /* Verifica se ocorreu erro de leitura */
      if (ferror(streamTexto)) {
         return -1; /* Opera��o mal sucedida */
      }

      ++chave;

         /* Converte a linha lida em estrutura */
      LinhaEmRegistro(&umAluno, linha, chave);

         /* Escreve a estrutura resultante da */
         /* convers�o no arquivo bin�rio      */
      fwrite(&umAluno, sizeof(umAluno), 1, streamBin);

         /* Verifica se ocorreu erro de escrita */
      if (ferror(streamBin)) { /* Ocorreu */
         return -1; /* Opera��o mal sucedida */
      }

      ++nRegistros; /* Mais um registro lido */

      if (nRegistros >= MAX_REGISTTROS) {
         break;
      }
   }

   return nRegistros; /* N�o ocorreu nenhum erro */
}

/****
 *
 * main(): Converte um arquivo de texto em arquivo bin�rio
 *
 * Par�metros: Nenhum
 *
 * Retorno: Zero, se n�o ocorrer nenhum erro.
 *          Um valor diferente de zero em caso contr�rio.
 *
 ****/
int main(void)
{
   int   nRegistros; /* N�mero de registros nos arquivos */
   FILE *streamTexto, /* Associado ao arquivo de texto */
        *streamBin;   /* Associado ao arquivo bin�rio */

      /* Tenta abrir arquivo para leitura em modo texto */
   streamTexto = fopen(NOME_ARQUIVO_TEXTO, "r");

      /* Se o arquivo n�o pode ser aberto, */
      /* nada mais pode ser feito          */
   if (!streamTexto) {
      printf( "O arquivo %s nao pode ser aberto\n",
              NOME_ARQUIVO_TEXTO );
      return 1; /* Opera��o mal sucedida */
   }

      /* Tenta criar arquivo bin�rio. Se um arquivo com esse   */
      /* nome existir no diret�rio corrente ele ser� destru�do */
   streamBin = fopen(NOME_ARQUIVO_BIN, "wb");

      /* Se o arquivo n�o pode ser aberto, */
      /* nada mais pode ser feito.         */
   if (!streamBin) {
      printf("Arquivo %s nao pode ser aberto\n",NOME_ARQUIVO_BIN);

         /* Antes de retornar, fecha arquivo aberto */
      fclose(streamTexto);

      return 1; /* Opera��o mal sucedida */
   }

      /* L� cada linha do arquivo de texto,     */
      /* converte-a em estrutura do tipo tRegistroMEC */
      /* e armazena-a no arquivo bin�rio.       */
   nRegistros = CriaArquivoBin(streamTexto, streamBin);

      /* Verifica se ocorreu erro na cria��o do arquivo */
      /* bin�rio. Se for o caso, informa o usu�rio,     */
      /* fecha os arquivos e encerra o programa.        */
   if (nRegistros < 0) {
      printf( "\n\t>>> Ocorreu erro na criacao do arquivo"
              "\n\t>>> \"%s\"\n", NOME_ARQUIVO_BIN );

         /* Fecha os arquivos antes de encerrar */
      fclose(streamTexto);
      fclose(streamBin);

      return 1; /* Ocorreu erro */
   }

      /* Apresenta o resultado da convers�o */
   printf( "\n\t>>> Foram armazenados %d registros no arquivo"
           "\n\t>>> \"%s\"\n", nRegistros, NOME_ARQUIVO_BIN );

      /* Fecha os arquivos */
   fclose(streamTexto);
   fclose(streamBin);

   return 0; /* Tudo ocorreu bem */
}

/******************* Exemplo de Execu��o *******************

        >>> Foram armazenados 9565483 registros no arquivo
        >>> "AlunosMEC2012.bin"

execution time : 97.761 s

OLD: 4,361,863,168 bytes
NEW: 4,361,863,168 bytes

       >>> Foram armazenados 9565483 registros no arquivo
       >>> "AlunosMEC2012New.bin"

4,361,863,168 bytes

***********************************************************/