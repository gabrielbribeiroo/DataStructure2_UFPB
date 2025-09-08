/****
 *
 * CriaArquivoBin.c: Programa que cria o arquivo binário "CEPs.bin"
 *
 ****/

/*********************** Includes *************************/

#include <stdio.h>    /* Funções de Entrada/Saída */
#include <string.h>   /* Processamento de strings */
#include <stdlib.h>   /* exit() e strtod()        */
#include <time.h>     /* Medição de tempo         */

/***************** Constantes Simbólicas ******************/

#define MAX_NOME  40 /* Número máximo de caracteres num nome */
#define TAM_UF     2 /* Número de dígitos num UF */
#define TAM_CEP    8 /* Número de dígitos num CEP */
#define MAX_COMP  60 /* Número máximo de caracteres */
                     /* num complemento             */
#define MAX_TIPO_LOG 10 /* Número máximo de caracteres */
                        /* num tipo de logradouro      */
#define TAM_DNE   16 /* Número de dígitos numa chave DNE */

   /* Tamanho do acréscimo do bloco usado para conter */
   /* uma linha a cada chamada de realloc()           */
#define TAMANHO_BLOCO 256

#define SEPARADORES "\t;\n" /* Possíveis separadores de campos */

   /* Nome dos arquivos binário */
#define NOME_ARQUIVO_BIN "CEPs.bin"

   /* Altere o string a seguir para incluir o diretório */
   /* em que se encontra o arquivo "CEPs.txt"           */
#define NOME_ARQUIVO_TXT "I:\\Dados\\CEPs\\CEPs.txt"

   /* Se o arquivo "CEPs.txt" encontra-se no diretório */
   /* corrente, comente a linha anterior e remova o    */
   /* comentário da linha a seguir                     */
//#define NOME_ARQUIVO_TXT "CEPs.txt"

   /* Múltiplos de bytes */
#define KIBIBYTE 1024             /* 1 Kibibyte */
#define MEBIBYTE (1024*1024)      /* 1 Mebibyte */
#define GIBIBYTE (1024*1024*1024) /* 1 Gibibyte */

/****************** Definições de Macros ******************/

   /* Macro utilizada em tratamento */
   /* de condições de exceção       */
#define ASSEGURA(cond, msg) if (!(cond)) {\
                               fprintf(stderr, "\n%s\n", msg);\
                               exit(1); \
                            }

/****************** Definições de Tipos *******************/

   /* Tipo de uma estrutura que representa um registro de CEP */
typedef struct {
           int  numero;
           char UF[TAM_UF];
           int  localidadeNumero;
           char nomeAbr[MAX_NOME];
           char nome[MAX_NOME];
           int  bairroInicio;
           int  bairroFim;
           char CEP[TAM_CEP];
           char complemento[MAX_COMP];
           char tipoLogradouro[MAX_TIPO_LOG];
           char statusLogradouro;
           char nomeSemAcento[MAX_NOME];
           char chaveDNE[TAM_DNE];
         } tCEP;

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
static char *LeLinhaIlimitada(int *tam, FILE *stream)
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
      if (feof(stream) || c == '\n')
         break; /* Leitura encerrada */

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
   if (tam)
         /* i é o índice do caractere terminal do */
         /* string e corresponde ao seu tamanho   */
      *tam = i;

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
static char *ObtemTokens(char *str, char const *sep)
{
   static char *proximoToken; /* Aponta para o próximo */
                              /* token, se ele existir */
   char        *s, /* Apontará para o string no qual a */
                   /* busca pelo token será efetuada   */
               *inicio = NULL; /* Guardará o início */
                               /* do token corrente */

      /* Se 'str' não for um ponteiro nulo, o próximo  */
      /* token será obtido a partir do início de 'str' */
   if (str)
      proximoToken = str;

      /* Se 'proximoToken' for um ponteiro nulo, o */
      /* string ora explorado não tem mais tokens  */
   if (!proximoToken)
      return NULL; /* Não há mais token nesse string */

      /* Obtém o endereço do primeiro separador */
      /* encontrado em 'proximoToken'           */
   s = strpbrk(proximoToken, sep);

      /* Verifica se foi encontrado algum separador */
      /* no string ora sendo explorado              */
   if (s) {
         /* Termina o token corrente na posição em */
         /* que se encontra o separador encontrado */
      *s = '\0';

         /* Guarda o início do token corrente */
      inicio = proximoToken;

         /* O próximo token começará no primeiro */
         /* caractere após o separador           */
      proximoToken = ++s;
   } else
      if (*proximoToken) {
            /* Não foi encontrado nenhum separador, mas o string  */
            /* corrente não é vazio e seu endereço será retornado */
         inicio = proximoToken;

         /* Não foi encontrado nenhum separador, de modo que */
         /* não haverá mais nenhum token na próxima chamada  */
      proximoToken = NULL;
   }

   return inicio;
}

/****
 *
 * CopiaNStr(): Copia um determinado número de caracteres de um
 *              string para um array de caracteres
 *
 * Parâmetros:
 *     dest (saída) - o array que receberá os caracteres
 *     origem (entrada) - string doador de caracteres
 *     n  (entrada) - número máximo de caracteres que serão copiados
 *
 * Retorno: O primeiro parâmetro
 *
 * Observação: Esta função é similar à strncpy(). A diferença é
 *             que, quando o segundo parâmetro é NULL, um string
 *             vazio é copiado para o primeiro parâmetro e o
 *             programa não é abortado.
 *
 ****/
char *CopiaNStr(char *dest, const char *origem, size_t n)
{
   return origem && *origem ? strncpy(dest, origem, n)
                            : strcpy(dest, "");
}

/****
 *
 * LinhaEmRegistro(): Converte uma linha do arquivo numa estrutura
 *                    do tipo tCEP
 *
 * Parâmetros: regCEP (saída) - ponteiro para a estrutura
 *                             resultante da conversão
 *             linha (entrada/saída) - linha que será convertida
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
 ****/
static tCEP *LinhaEmRegistro(tCEP *regCEP, char *linha)
{
   char *str; /* Apontará para cada token da linha */
   char *final; /* Final de uma conversão numérica */

      /* Obtém o número e acrescenta-o ao */
      /* respectivo campo da estrutura    */
   str = ObtemTokens(linha, SEPARADORES);
   ASSEGURA(str, "Token nulo: numero");
   regCEP->numero = strtol(str, &final, 10);
   ASSEGURA(!*final, "Erro de conversao numerica");

      /* Obtém a UF e acrescenta-o ao  */
      /* respectivo campo da estrutura */
   str = ObtemTokens(NULL, SEPARADORES);
   CopiaNStr(regCEP->UF, str, TAM_UF);

      /* Obtém o número de localidade e acrescenta-o */
      /* ao respectivo campo da estrutura            */
   str = ObtemTokens(NULL, SEPARADORES);
   ASSEGURA(str, "Token nulo: localidadeNumero");
   regCEP->localidadeNumero = (int) strtol(str, &final, 10);
   ASSEGURA(!*final || regCEP->localidadeNumero < 0,
            "Erro de conversao numerica");

      /* Obtém o nome abreviado e acrescenta-o */
      /* ao respectivo campo da estrutura      */
   str = ObtemTokens(NULL, SEPARADORES);
   CopiaNStr(regCEP->nomeAbr, str, MAX_NOME);

      /* Obtém o nome e acrescenta-o ao */
      /* respectivo campo da estrutura  */
   str = ObtemTokens(NULL, SEPARADORES);
   CopiaNStr(regCEP->nome, str, MAX_NOME);

      /* Obtém o início do bairro e acrescenta-o */
      /* ao respectivo campo da estrutura        */
   str = ObtemTokens(NULL, SEPARADORES);
   ASSEGURA(str, "Token nulo: bairroInicio");
   regCEP->bairroInicio = (int) strtol(str, &final, 10);
   ASSEGURA(!*final || regCEP->bairroInicio < 0,
            "Erro de conversao numerica");

      /* Obtém o final do bairro e acrescenta-o */
      /* ao respectivo campo da estrutura       */
   str = ObtemTokens(NULL, SEPARADORES);
   ASSEGURA(str, "Token nulo: bairroFim");
   regCEP->bairroFim = (int) strtol(str, &final, 10);
   ASSEGURA(!*final || regCEP->bairroFim < 0,
            "Erro de conversao numerica");

      /* Obtém o CEP e acrescenta-o ao */
      /* respectivo campo da estrutura */
   str = ObtemTokens(NULL, SEPARADORES);
   CopiaNStr(regCEP->CEP, str, TAM_CEP);

      /* Obtém o complemento e acrescenta-o */
      /* ao respectivo campo da estrutura   */
   str = ObtemTokens(NULL, SEPARADORES);
   CopiaNStr(regCEP->complemento, str, MAX_COMP);

      /* Obtém o tipo de logradouro e acrescenta-o */
      /* ao respectivo campo da estrutura          */
   str = ObtemTokens(NULL, SEPARADORES);
   CopiaNStr(regCEP->tipoLogradouro, str, MAX_TIPO_LOG);

      /* Obtém o status de logradouro e acrescenta-o */
      /* ao respectivo campo da estrutura            */
   str = ObtemTokens(NULL, SEPARADORES);
   regCEP->statusLogradouro = *str;

      /* Obtém o nome sem acento e acrescenta-o */
      /* ao respectivo campo da estrutura       */
   str = ObtemTokens(NULL, SEPARADORES);
   CopiaNStr(regCEP->nomeSemAcento, str, MAX_NOME);

      /* Obtém a chave DNE e acrescenta-o */
      /* ao respectivo campo da estrutura */
   str = ObtemTokens(NULL, SEPARADORES);
   CopiaNStr(regCEP->chaveDNE, str, TAM_DNE);

   return regCEP;
}

/****
 *
 * CriaArquivoBin(): Lê cada linha de um arquivo no formato
 *                   especificado, converte-a numa estrutura do
 *                   tipo tCEP e armazena a estrutura num arquivo
 *                   binário
 *
 * Parâmetros:
 *     streamTexto (entrada) - stream associado ao arquivo que será
 *                             lido
 *     streamBin (entrada) - stream associado ao arquivo que será
 *                           escrito
 *     linhasVazias (saída) - linhas do arquivo de entrada que não
 *                            são levadas em consideração
 *
 * Retorno: Número de estruturas escritas no arquivo binário ou um
 *          valor negativo, se ocorrer erro
 *
 * Observações:
 *     1. O stream de entrada deve estar aberto em modo de texto
 *        que permite leitura
 *     2. O stream de saída deve estar aberto em modo "wb"
 *
 ****/
int CriaArquivoBin( FILE *streamTexto, FILE *streamBin,
                    int *linhasVazias )
{
   char  *linha; /* Apontará para cada linha lida */
   tCEP   umCEP; /* Dados de uma linha      */
                 /* convertida em estrutura */
   int    nRegistros = 0; /* Número de estruturas escritas */
                          /* no arquivo binário            */

   *linhasVazias = 0; /* Ainda não foi lida nenhuma linha vazia */

      /* Garante leitura a partir do início do arquivo */
   rewind(streamTexto);

      /* Lê cada linha do arquivo, cria um registro do tipo tCEP */
      /* e armazena-o no arquivo binário. O laço encerra quando  */
      /* há tentativa de leitura além do final do arquivo de     */
      /* entrada, ou ocorre erro de leitura/escrita.             */
   while (1) {
         /* Lê uma linha no arquivo de entrada */
      linha = LeLinhaIlimitada(NULL, streamTexto);

         /* Verifica se o final do arquivo foi atingido */
      if (feof(streamTexto))
         break;

         /* Verifica se ocorreu erro de leitura */
      if (ferror(streamTexto))
         return -1; /* Operação mal sucedida */

         /* Verifica se a linha lida é vazia */
      if (!linha || !*linha || *linha == '\n') {
         ++*linhasVazias; /* Foi lida uma linha vazia */

         free(linha); /* A linha não contém dados relevantes */

         continue; /* Salta o restante do corpo do laço */
      }

         /* Converte a linha lida em estrutura */
      LinhaEmRegistro(&umCEP, linha);

         /* Escreve a estrutura resultante da */
         /* conversão no arquivo binário      */
      fwrite(&umCEP, sizeof(umCEP), 1, streamBin);

         /* Verifica se ocorreu erro de escrita */
      if (ferror(streamBin))
         return -1; /* Operação mal sucedida */

      ++nRegistros; /* Mais um registro lido */

      free(linha); /* Libera o espaço ocupado pela linha corrente */
   }

   return nRegistros; /* Não ocorreu nenhum erro */
}

/****
 *
 * MedidaDeTempo(): Mede intervalos de tempo entre duas chamadas
 *                  consecutivas
 *
 * Parâmetros:
 *     info (entrada) - informação que será apresentada na tela
 *
 * Retorno: Nada
 *
 * Observação: Esta função deve ser chamada aos pares; i.e., na
 *             primeira chamada ela marca o início da contagem de
 *             tempo e na segunda chamada ela apresenta o intervalo
 *             de tempo decorrido desde a última chamada
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

      /* Esta função foi chamada mais uma vez */
   ++numeroDeChamadas;

      /* Se o número da chamada for ímpar, esta */
      /* chamada é a primeira de um par         */
   if (numeroDeChamadas%2) { /* Calcula o instante inicial */
      t0 = time(NULL);
   } else { /* Calcula e apresenta o intervalo de   */
            /* tempo entre as duas últimas chamadas */
      tf = time(NULL);

      printf( "\n\t*** Tempo gasto na operacao: "
              "%5.2f segundos ***\n", difftime(tf, t0) );
   }
}

/****
 *
 * EscreveNumeroBytes(): Escreve um determinado número de bytes
 *                       (valor) num stream de texto
 *
 * Parâmetros:
 *     nBytes (entrada) - o valor a ser escrito
 *     stream (entrada) - o stream de texto
 *
 * Retorno: Nenhum
 *
 ****/
void EscreveNumeroBytes(int nBytes, FILE *stream)
{
   fprintf(stream, " %d bytes", nBytes);

   if (nBytes > GIBIBYTE)
      fprintf(stream, " (%.2f GiB)", (double) nBytes/GIBIBYTE);
   else if (nBytes > MEBIBYTE)
      fprintf(stream, " (%.2f MiB)", (double) nBytes/MEBIBYTE);
   else if (nBytes > KIBIBYTE)
      fprintf(stream, " (%.2f KiB)", (double) nBytes/KIBIBYTE);

   fprintf(stream, "\n");
}

/****
 *
 * main(): Executa a função CriaArquivoBin()
 *
 * Parâmetros: Nenhum
 *
 * Retorno: Zero
 *
 ****/
int main(void)
{
   int   nRegistros, /* Número de registros nos arquivos  */
         tamArquivo, /* Tamanho do arquivo binário        */
         vazias;     /* Linhas que não serão consideradas */
   FILE *streamTexto, /* Associado ao arquivo de texto */
        *streamBin;   /* Associado ao arquivo binário */

   printf("\n>>> Tamanho de um registro (tCEP): %d\n", sizeof(tCEP));

      /* Tenta abrir arquivo para leitura em modo texto */
   streamTexto = fopen(NOME_ARQUIVO_TXT, "r");

      /* Se o arquivo não pode ser aberto, */
      /* nada mais pode ser feito          */
   if (!streamTexto) {
      printf( "O arquivo %s nao pode ser aberto\n",
              NOME_ARQUIVO_TXT );
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

   MedidaDeTempo("Criando arquivo...");

      /* Lê cada linha do arquivo de texto,   */
      /* converte-a em estrutura do tipo tCEP */
      /* e armazena-a no arquivo binário.     */
   nRegistros = CriaArquivoBin(streamTexto, streamBin, &vazias);

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

   MedidaDeTempo("...Arquivo criado com sucesso");

   tamArquivo = nRegistros*sizeof(tCEP);

      /* Apresenta o resultado da conversão */
   printf( "\n>>> Foram armazenados %d registros no arquivo "
           "\"%s\"\n", nRegistros, NOME_ARQUIVO_BIN );

   printf( "\n>>> Tamanho do arquivo \"%s\":", NOME_ARQUIVO_BIN );
   EscreveNumeroBytes(tamArquivo, stdout);

   printf("\n>>> Linhas vazias: %d\n", vazias);

      /* Fecha os arquivos */
   fclose(streamTexto);
   fclose(streamBin);

   return 0; /* Tudo ocorreu bem */
}

/****************** Resultado do Programa *****************

>>> Tamanho de um registro (tCEP): 236

>>> Criando arquivo...

>>> ...Operacao encerrada. Arquivo criado com sucesso

        *** Tempo gasto na operacao:  5.00 segundos ***

>>> Foram armazenados 673580 registros no arquivo "CEPs.bin"

>>> Tamanho do arquivo "CEPs.bin": 158964880 bytes (151.60 MiB)

>>> Linhas vazias: 0

**********************************************************/

