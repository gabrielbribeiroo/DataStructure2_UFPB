/****
 *
 * Título: Convertendo um Arquivo de Registros de Texto para Binário
 *
 * Autor: Ulysses de Oliveira
 *
 * Data de Criação: 21/05/2012
 * Última modificação: 21/05/2012
 *
 * Descrição: Lê cada linha de um arquivo de texto, converte-a numa
 *            estrutura do tipo tRegistroMEC e armazena-a num arquivo
 *            binário
 *
 * Entrada: Arquivo cujo nome é especificado pela constante
 *          NOME_ARQUIVO_TEXTO
 *
 * Saída: Arquivo cujo nome é especificado pela constante
 *        NOME_ARQUIVO_BIN
 *
 ****/

/*********************** Includes *************************/

#include <stdio.h>  /* Funções de Entrada e Saída */
#include <string.h> /* Processamento de strings   */
#include <stdlib.h> /* Função strtod() e alocação dinâmica */

/***************** Constantes Simbólicas ******************/

#define MAX_NOME_IES     200 /* Tamanho máximo de */
                             /* um nome de IES    */
#define MAX_NOME_CURSO   200 /* Tamanho máximo de */
                             /* um nome de curso  */

#define MAX_UF_NASCIMENTO 30 /* Tamanho máximo   */
                             /* de um nome de UF */

   /* Nome do arquivo de texto */
#define NOME_ARQUIVO_TEXTO "I:\\Dados\\CensoMEC\\CensoMEC2012.txt"

   /* Nome do arquivo binário */
#define NOME_ARQUIVO_BIN "MiniCensoMEC2012.bin"

   /* Número máximo de registros que serão gerados */
#define MAX_REGISTTROS 50

   /* Tamanho do acréscimo do bloco usado para conter */
   /* uma linha a cada chamada de realloc()           */
#define TAMANHO_BLOCO 256

   /* Macro utilizada em tratamento */
   /* de condições de exceção       */
#define ASSEGURA(condicao, msg) if (!(condicao)) {\
                                   fprintf(stderr, "\n%s\n", msg);\
                                   exit(1); \
                                }

/****************** Definições de Tipos *******************/

typedef struct {
           long long codigoAluno;
           int       codAlunoCurso;
           char      nomeIES[MAX_NOME_IES];
           char      nomeCurso[MAX_NOME_CURSO];
           char      sexoAluno;
           int       idadeAluno;
           char      UFNascimento[MAX_UF_NASCIMENTO];
           int       anoIngresso;
         } tRegistroMEC;

/***************** Definições de Funções ******************/

/****
 *
 * LeLinhaIlimitada(): Lê uma linha de tamanho arbitrário num
 *                     stream de texto e armazena os caracteres
 *                     lidos num array alocado dinamicamente
 *
 * Parâmetros:
 *      tam (saída) - se não for NULL, apontará para uma variável
 *                    que armazenará o tamanho do string
 *                    constituído pelos caracteres da linha
 *      stream (entrada) - stream de texto no qual será feita a
 *                         leitura
 *
 * Retorno: Endereço do array contendo a linha lida. NULL, se
 *          ocorrer erro ou o final do arquivo for atingido
 *          antes da leitura de qualquer caractere
 *
 * Observações:
 *       1. O stream deve estar associado a um arquivo de texto
 *          aberto em modo de texto que permita leitura
 *       2. O caractere '\n' não é incluído no string resultante
 *          da leitura
 *       3. O primeiro parâmetro pode ser NULL. Nesse caso, o
 *          tamanho do string não será armazenado
 *
 ****/
char *LeLinhaIlimitada(int *tam, FILE *stream)
{
   char *ar = NULL,   /* Ponteiro para um array alocado */
                      /* dinamicamente que conterá os   */
                      /* caracteres lidos               */
        *p; /* Usado em chamada de realloc() */
   int   tamanho = 0, /* Tamanho do array alocado */
         c,  /* Armazenará cada caractere lido */
         i; /* Índice do próximo caractere */
            /* a ser inserido no array     */

      /* Lê caracteres a partir da posição corrente do      */
      /* indicador de posição do arquivo e armazena-os num  */
      /* array. A leitura encerra quando '\n' é encontrado, */
      /* o final do arquivo é atingido ou ocorre erro.      */
   for (i = 0; ; ++i) {
         /* Lê o próximo caractere no arquivo */
      c = fgetc(stream);

         /* Se ocorreu erro de leitura, libera o  */
         /* bloco eventualmente alocado e retorna */
      if (ferror(stream)) {
         free(ar); /* Libera o bloco apontado por 'ar' */
         return NULL; /* Ocorreu erro de leitura */
      }

         /* Verifica se array está completo. O maior valor  */
         /* que i poderia assumir deveria ser tamanho - 1.  */
         /* Mas, como ao final, o caractere '\0' deverá ser */
         /* inserido, limita-se o valor de i a tamanho - 2. */
      if (i > tamanho - 2) { /* Limite atingido */
            /* Tenta redimensionar o array */
         p = realloc(ar, tamanho + TAMANHO_BLOCO);

            /* Se o redimensionamento não foi     */
            /* possível, libera o bloco e retorna */
         if (!p) {
            free(ar); /* Libera o bloco apontado por 'ar' */
            return NULL; /* Ocorreu erro de alocação */
         }

            /* Redimensionamento foi OK. Então, faz-se */
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

      ar[i] = c; /* Acrescenta o último caractere lido ao array */
   }

      /* Se nenhum caractere foi lido, libera */
      /* o espaço alocado e retorna NULL      */
   if (feof(stream) && !i) {
      free(ar); /* Libera o bloco apontado por 'ar' */
      return NULL; /* Nenhum caractere foi armazenado no array */
   }

      /* Insere o caractere terminal de string. Neste    */
      /* instante, deve haver espaço para ele porque o   */
      /* array foi sempre redimensionado deixando um     */
      /* espaço a mais para o onipresente caractere '\0' */
   ar[i] = '\0';

      /* Atualiza o valor apontado pelo parâmetro */
      /* 'tam', se ele não for NULL               */
   if (tam) {
         /* i é o índice do caractere terminal do */
         /* string e corresponde ao seu tamanho   */
      *tam = i;
   }

            /*                                     */
            /* >>> NB: O tamanho do string não <<< */
            /* >>>     inclui o caractere '\0' <<< */
            /*                                     */

      /* Tenta ajustar o tamanho do array para não */
      /* haver desperdício de memória. Como i é o  */
      /* tamanho do string, o tamanho do array que */
      /* o contém deve ser i + 1.                  */
   p = realloc(ar, i + 1);

      /*                                                 */
      /* Se a realocação foi bem sucedida, retorna-se p. */
      /* Caso contrário, 'ar' ainda aponta para um bloco */
      /* válido. Talvez, haja desperdício de memória,    */
      /* mas, aqui, é melhor retornar 'ar' do que NULL.  */
      /*                                                 */
   return p ? p : ar;
}

/****
*
* ObtemTokens(): Separa um string em partes (tokens)
*
* Parâmetros:
*     str (entrada/saída) - o string que será separado
*                           ou NULL para continuar uma
*                           separação anterior
*     sep (entrada) - string contendo os separadores de tokens
*
* Retorno: Endereço do próximo token do string, se este
*          existir, ou NULL, em caso contrário
*
* Observação: Esta função NÃO é funcionalmente equivalente à
*             função strtok() da biblioteca padrão de C
*
****/
char *ObtemTokens(char *str, char const *sep)
{
   static char *proximoToken; /* Aponta para o próximo */
                              /* token, se ele existir */
   char        *s, /* Apontará para o string no qual a */
                   /* busca pelo token será efetuada   */
               *inicio; /* Guardará o início */
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
 * Parâmetros: aluno (saída) - ponteiro para a estrutura
 *                             resultante da conversão
 *             linha (entrada/saída) - linha que será convertida
 *             chave (entrada) - a chave do registro
 *
 * Retorno: Endereço da estrutura que armazena o resultado
 *          da conversão
 *
 * Observações:
 *     1. Para facilitar o entendimento, esta função assume
 *        que o parâmetro 'linha' realmente é um string no
 *        formato das linhas do arquivo. Portanto, os
 *        valores retornados por ObtemTokens() não são testados
 *        como deveriam.
 *
 *     2. O parâmetro 'linha' é alterado por ObtemTokens().
 *
 *     3. A função ObtemTokens() é usada em lugar de strtok()
 *        devido à presença de campos vazios no arquivo de
 *        texto, o que faz com que dois separadores apareçam
 *        juntos. Nesse caso, a função strtok() consideraria
 *        os dois separadores como sendo apenas um.
 *
 *     4. O caractere '\r' faz parte das chamadas da função
 *        ObtemTokens() para satisfazer uma bizarrice do
 *        sistema Mac OS X, que é o único sistema Unix que
 *        usa '\r' como quebra de linha.
 *
 ****/
tRegistroMEC *LinhaEmRegistro(tRegistroMEC *aluno, char *linha, int chave)
{
   char *str, /* Apontará para cada token da linha */
        *final; /* Final de uma conversão numérica */

      /* Obtém o código do aluno, converte-o e         */
      /* acrescenta-o ao respectivo campo da estrutura */
   str = ObtemTokens(linha, "\t\n\r;");
   ++str; /* Salta espaço em branco inicial */
   aluno->codigoAluno = strtoll(str, &final, 10);
   ASSEGURA(!*final, "Erro de conversao numerica");

      /* Obtém o código de vínculo do aluno ao */
      /* curso, converte-o e acrescenta-o ao   */
      /* respectivo campo da estrutura         */
   str = ObtemTokens(NULL, "\t\n\r;");
   aluno->codAlunoCurso = strtol(str, &final, 10);
   ASSEGURA(!*final, "Erro de conversao numerica");

      /* Obtém o nome da IES e acrescenta-a */
      /* ao respectivo campo da estrutura   */
   str = ObtemTokens(NULL, "\t\n\r;");
   strncpy(aluno->nomeIES, str, MAX_NOME_IES);

      /* Obtém o nome do curso e acrescenta-o */
      /* ao respectivo campo da estrutura     */
   str = ObtemTokens(NULL, "\t\n\r;");
   strncpy(aluno->nomeCurso, str, MAX_NOME_CURSO);

      /* Obtém o sexo do aluno e acrescenta-o */
      /* ao respectivo campo da estrutura     */
   str = ObtemTokens(NULL, "\t\n\r;");
   ASSEGURA(*str == '0' || *str == '1', "Sexo indefinido");
   aluno->sexoAluno = *str == '0' ? 0 : 1;

      /* Obtém a idade do aluno e acrescenta-o */
      /* ao respectivo campo da estrutura      */
   str = ObtemTokens(NULL, "\t\n\r;");
   aluno->idadeAluno = strtol(str, &final, 10);
   ASSEGURA(!*final, "Erro de conversao numerica");

      /* Obtém a UF de nascimento e acrescenta-a */
      /* ao respectivo campo da estrutura        */
   str = ObtemTokens(NULL, "\t\n\r;");
   strncpy(aluno->UFNascimento, str, MAX_UF_NASCIMENTO);

      /* Obtém o ano de ingresso do aluno e acrescenta-o */
      /* ao respectivo campo da estrutura                */
   str = ObtemTokens(NULL, "\t\n\r;");
   aluno->anoIngresso = strtol(str, &final, 10);
   ASSEGURA(!*final, "Erro de conversao numerica");

   return aluno;
}

/****
 *
 * CriaArquivoBin(): Lê cada linha de um arquivo no formato
 *                   especificado, converte-a numa estrutura
 *                   do tipo tRegistroMEC e armazena a estrutura
 *                   num arquivo binário.
 *
 * Parâmetros:
 *     streamTexto (entrada) - stream associado ao arquivo que
 *                             será lido
 *     streamBin (entrada) - stream associado ao arquivo que será
 *                           escrito
 *
 * Retorno: Número de estruturas escritas no arquivo
 *          binário ou um valor negativo, se ocorrer erro
 *
 * Observações:
 *     1. O stream de entrada deve estar aberto em modo de
 *        texto que permite leitura
 *     2. O stream de saída deve estar aberto em modo "wb"
 *
 ****/
int CriaArquivoBin(FILE *streamTexto, FILE  *streamBin)
{
   char  *linha; /* Apontará para cada linha lida */
   tRegistroMEC umAluno; /* Dados de uma linha      */
                   /* convertida em estrutura */
   int   nRegistros = 0, /* Número de estruturas escritas */
                         /* no arquivo binário            */
         chave = 0;

      /* Garante leitura a partir do início do arquivo */
   rewind(streamTexto);

      /* Lê cada linha do arquivo, cria um registro do tipo     */
      /* tRegistroMEC e armazena-o no arquivo binário. O laço encerra */
      /* quando há tentativa de leitura além do final do        */
      /* arquivo de entrada, ou ocorre erro de leitura/escrita  */
   while (1) {
         /* Lê uma linha no arquivo de entrada */
      linha = LeLinhaIlimitada(NULL, streamTexto);

         /* Verifica se o final do arquivo foi atingido */
      if (feof(streamTexto)) {
         break;
      }

         /* Verifica se ocorreu erro de leitura */
      if (ferror(streamTexto)) {
         return -1; /* Operação mal sucedida */
      }

      ++chave;

         /* Converte a linha lida em estrutura */
      LinhaEmRegistro(&umAluno, linha, chave);

         /* Escreve a estrutura resultante da */
         /* conversão no arquivo binário      */
      fwrite(&umAluno, sizeof(umAluno), 1, streamBin);

         /* Verifica se ocorreu erro de escrita */
      if (ferror(streamBin)) { /* Ocorreu */
         return -1; /* Operação mal sucedida */
      }

      ++nRegistros; /* Mais um registro lido */

      if (nRegistros >= MAX_REGISTTROS) {
         break;
      }
   }

   return nRegistros; /* Não ocorreu nenhum erro */
}

/****
 *
 * main(): Converte um arquivo de texto em arquivo binário
 *
 * Parâmetros: Nenhum
 *
 * Retorno: Zero, se não ocorrer nenhum erro.
 *          Um valor diferente de zero em caso contrário.
 *
 ****/
int main(void)
{
   int   nRegistros; /* Número de registros nos arquivos */
   FILE *streamTexto, /* Associado ao arquivo de texto */
        *streamBin;   /* Associado ao arquivo binário */

      /* Tenta abrir arquivo para leitura em modo texto */
   streamTexto = fopen(NOME_ARQUIVO_TEXTO, "r");

      /* Se o arquivo não pode ser aberto, */
      /* nada mais pode ser feito          */
   if (!streamTexto) {
      printf( "O arquivo %s nao pode ser aberto\n",
              NOME_ARQUIVO_TEXTO );
      return 1; /* Operação mal sucedida */
   }

      /* Tenta criar arquivo binário. Se um arquivo com esse   */
      /* nome existir no diretório corrente ele será destruído */
   streamBin = fopen(NOME_ARQUIVO_BIN, "wb");

      /* Se o arquivo não pode ser aberto, */
      /* nada mais pode ser feito.         */
   if (!streamBin) {
      printf("Arquivo %s nao pode ser aberto\n",NOME_ARQUIVO_BIN);

         /* Antes de retornar, fecha arquivo aberto */
      fclose(streamTexto);

      return 1; /* Operação mal sucedida */
   }

      /* Lê cada linha do arquivo de texto,     */
      /* converte-a em estrutura do tipo tRegistroMEC */
      /* e armazena-a no arquivo binário.       */
   nRegistros = CriaArquivoBin(streamTexto, streamBin);

      /* Verifica se ocorreu erro na criação do arquivo */
      /* binário. Se for o caso, informa o usuário,     */
      /* fecha os arquivos e encerra o programa.        */
   if (nRegistros < 0) {
      printf( "\n\t>>> Ocorreu erro na criacao do arquivo"
              "\n\t>>> \"%s\"\n", NOME_ARQUIVO_BIN );

         /* Fecha os arquivos antes de encerrar */
      fclose(streamTexto);
      fclose(streamBin);

      return 1; /* Ocorreu erro */
   }

      /* Apresenta o resultado da conversão */
   printf( "\n\t>>> Foram armazenados %d registros no arquivo"
           "\n\t>>> \"%s\"\n", nRegistros, NOME_ARQUIVO_BIN );

      /* Fecha os arquivos */
   fclose(streamTexto);
   fclose(streamBin);

   return 0; /* Tudo ocorreu bem */
}

/******************* Exemplo de Execução *******************

        >>> Foram armazenados 9565483 registros no arquivo
        >>> "AlunosMEC2012.bin"

execution time : 97.761 s

OLD: 4,361,863,168 bytes
NEW: 4,361,863,168 bytes

       >>> Foram armazenados 9565483 registros no arquivo
       >>> "AlunosMEC2012New.bin"

4,361,863,168 bytes

***********************************************************/
