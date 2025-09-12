/****
 *
 * Arquivo Registros.c: Arquivo de programa do módulo de
 *                      processamento de registros
 *
 * Autor: Ulysses de Oliveira
 *
 * Data de Criação: 20/04/2004
 * Última modificação: 30/03/2017
 *
 ****/

/*********************** Includes *************************/

#include <stdio.h>  /* Funções de Entrada e Saída   */
#include <string.h> /* Processamento de strings     */
#include <stdlib.h> /* strtod() e alocação dinâmica */
#include "Registros.h" /* Interface deste módulo    */
#include "Leitura.h"  /* Leitura via teclado        */
#include "ASSEGURA.h" /* Macro ASSEGURA             */
#include <time.h>     /* Medição de tempo           */

/********************* Funções Locais *********************/

/****
 *
 * EscreveArrayDeChars(): Escreve num stream um array de elementos
 *                        do tipo char que não é string
 *
 * Parâmetros:
 *      *nome (entrada) - string que será exibido antes do array
 *       ar (entrada) - endereço do array
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
static tRegistroMEC *LinhaEmRegistro(tRegistroMEC *aluno, char *linha)
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
   aluno->chave = strtol(str, &final, 10);
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

/********************* Funções Globais ********************/

/****
 *
 * AbreArquivo(): Tenta abrir um arquivo e, quando isso
 *                não é possível, aborta o programa
 *
 * Parâmetros:
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
 *                 não é possível, aborta o programa
 *
 * Parâmetros:
 *      stream (entrada) - stream associado ao arquivo
 *      nomeArq (entrada) - nome do arquivo ou NULL
 *
 * Retorno: Nada
 *
 * Observação: Se o segundo parâmetro não for NULL, o nome
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
 * MoveApontador(): Tenta mover o apontador de posição de um
 *                  arquivo e, quando isso não é possível,
 *                  aborta o programa
 *
 * Parâmetros:
 *      stream (entrada) - stream associado ao arquivo
 *      bytes (entrada) - quantidade de bytes que serão saltados
 *      DeOnde (entrada) - a partir de onde ocorrerá a movimentação
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
 * Parâmetros: *registro (entrada) - registro que será exibido
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
 * LeRegistro(): Lê os dados de um aluno
 *
 * Parâmetros: *aluno (saída) - estrutura que receberá os
 *                              dados introduzidos pelo
 *                              usuário
 * Retorno: O parâmetro
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
 * SubstituiRegistro(): Lê os dados de um registro que, se forem
 *                      válidos, substituirão valores antigos
 *
 * Parâmetros:
 *     *novo (saída) - estrutura que armazenará os novos
 *                     dados introduzidos pelo usuário
 *     *antigo (entrada) - estrutura contendo os dados antigos
 *
 * Retorno: O primeiro parâmetro
 *
 * Observação: Esta função não está rigorosamente bem
 *             implementada pois não faz parte do tema
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
//      /* Lê o valor de UF. Qualquer string que não seja  */
//      /* vazio é considerado válido, mas não deveria ser */
//      /* assim (v. Observação). O mesmo vale para os     */
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
 *                     bytes são lidos
 *
 * Parâmetros:
 *    stream (entrada) - stream binário associado ao arquivo cujo
 *                       tamanho será calculado
 *
 * Retorno: -1, se ocorrer algum erro.
 *          O tamanho do arquivo, em caso contrário.
 *
 * Observações:
 *     O arquivo deve estar aberto em modo binário que permite
 *     leitura
 *
 ****/
long TamanhoDeArquivo(FILE *stream)
{
   ASSEGURA(stream, "Erro em TamanhoDeArquivo(): stream e' NULL" );

      /* Tenta mover o apontador de posição do arquivo */
      /* para o final do mesmo; se não for possível,   */
      /* o programa será abortado                      */
   MoveApontador(stream, 0, SEEK_END);

   return ftell(stream);
}

/****
 *
 * NumeroDeRegistros(): Calcula o número de registros (i.e.,
 *                      partições de tamanho especificado) de um
 *                      arquivo binário
 *
 * Parâmetros:
 *    stream (entrada) - stream binário associado ao arquivo cujo
 *                       tamanho será calculado
 *    tamRegistro (entrada) - tamanho de cada partição do arquivo
 *
 * Retorno: O número de partições do arquivo, se não ocorrer
 *          nenhum erro, ou -1, em caso contrário.
 *
 * Observação: O arquivo deve estar aberto em modo binário que
 *             permite leitura
 *
 ****/
int NumeroDeRegistros( FILE *stream, int tamRegistro )
{
   long nBytes = 0; /* Número de bytes do arquivo */

      /* Calcula o número de bytes do arquivo */
   nBytes = TamanhoDeArquivo(stream);

      /* Verifica se ocorreu erro na contagem de bytes */
   ASSEGURA(nBytes >= 0, "Erro na contagem de bytes");

      /* O número de registros é o número de bytes do arquivo */
      /* dividido pelo número de bytes de cada registro       */
   return nBytes/tamRegistro;
}

/****
 *
 * LeRegistroEmArquivo(): Lê um registro no arquivo de registros
 *
 * Parâmetros:
 *     *umRegistro (saída) - registro lido
 *      stream (entrada) - stream associado ao arquivo
 *                         de registros
 *      pos (entrada) - posição do registro no arquivo
 *
 * Retorno: o primeiro parâmetro, se a recuperação for
 *           bem sucedida; NULL, em caso contrário.
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
 * Parâmetros:
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

      /* Recebe do usário os dados do */
      /* registro a ser inserido      */
   LeRegistro(&reg);

      /* A inserção será feita ao final do arquivo */
   MoveApontador(stream, 0, SEEK_END);

   fwrite(&reg, sizeof(reg), 1, stream);

   ASSEGURA(!ferror(stream), "Erro de escrita em arquivo");
}

/****
 *
 * ArquivoExiste(): Verifica se um arquivo existe
 *
 * Parâmetros:
 *     nomeArq (entrada) - nome do arquivo que se
 *                        verificará se existe
 *
 * Retorno: 1, se o arquivo existir; 0, em caso contrário
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
 * CriaArquivoBin(): Lê cada linha de um arquivo no formato
 *                   especificado, converte-a numa estrutura
 *                   do tipo tRegistroMEC e armazena a estrutura
 *                   num arquivo binário
 *
 * Parâmetros:
 *     streamTexto (entrada) - stream associado ao arquivo que
 *                             será lido
 *     streamBin (entrada) - stream associado ao arquivo que será
 *                           escrito
 *     maxRegistros (entrada) - número máximo de registros que
 *                              serão gerados
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
int CriaArquivoBin( const char *nomeTexto, const char *nomeBin,
                    int maxRegistros )
{
   char  *linha; /* Apontará para cada linha lida */
   tRegistroMEC umAluno; /* Dados de uma linha      */
                   /* convertida em estrutura */
   int   nRegistros = 0, /* Número de estruturas escritas */
                         /* no arquivo binário            */
         ocorreuErro = 0;
   FILE *streamTexto,
        *streamBin;

      /* Tenta abrir arquivo para leitura em modo texto */
   streamTexto = AbreArquivo(nomeTexto, "r");

      /* Tenta abrir arquivo para escrita em modo binário */
   streamBin = AbreArquivo(nomeBin, "w");

      /* Lê cada linha do arquivo, cria um registro do tipo     */
      /* tRegistroMEC e armazena-o no arquivo binário. O laço encerra */
      /* quando há tentativa de leitura além do final do        */
      /* arquivo de entrada, ou ocorre erro de leitura/escrita  */
   while (1) {
         /* Lê uma linha no arquivo de entrada */
      linha = LeLinhaIlimitada(NULL, streamTexto);

         /* Verifica se o final do arquivo foi  */
         /* atingido ou ocorreu erro de leitura */
      if (feof(streamTexto) || ferror(streamTexto))
         break;

         /* Converte a linha lida em estrutura */
      LinhaEmRegistro(&umAluno, linha);

         /* Escreve a estrutura resultante da */
         /* conversão no arquivo binário      */
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

   return nRegistros; /* Não ocorreu nenhum erro */
}

/****
 *
 * EscreveRegistro(): Escreve num stream um registro do tipo tRegistroMEC
 *
 * Parâmetros: *registro (entrada) - registro que será exibido
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
 * Parâmetros: registro (entrada) - o referido registro
 *
 * Retorno: A chave do registro recebido como parâmetro
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
 * Parâmetros:
 *     info (entrada) - informação que será apresentada na tela
 *
 * Retorno: Nada
 *
 * Observação: Esta função deve ser chamada aos pares; i.e.,
 *             na primeira chamada ela marca o início da
 *             contagem de tempo e na segunda chamada ela
 *             apresenta o intervalo de tempo decorrido
 *             desde a última chamada
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

      printf( "\n\n\t*** Tempo gasto na operacao: "
              "%5.2f segundos ***\n", difftime(tf, t0) );
   }
}
