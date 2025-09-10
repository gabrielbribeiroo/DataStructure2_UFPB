/****
 *
 * Exemplo: Dispersão Cuco
 *
 * Seção: 7.5.3
 *
 * Descrição:
 *    -> A tabela de busca é implementada usando dispersão cuco
 *       em que cada elemento contém uma chave e a posição do
 *       respectivo registro no arquivo
 *
 *    -> Os registros são mantidos em arquivo
 *
 *    -> Tipo de busca: linear
 *
 *    -> Arquivo de dados: CEPs.bin
 *
 ****/

/*********************** Includes *************************/

#include <math.h>      /* Funções matemáticas   */
#include <stdlib.h>    /* Miscelânea de funções */
#include <stdio.h>     /* Funções de Entrada/Saída    */
#include <string.h>    /* Funções memXXX() e strXXX() */
#include <time.h>      /* Medição de tempo         */
#include <limits.h>    /* Limites dos tipos inteiros */
#include "Macros.h"    /* Macros usadas pelo programa */
#include "Tipos.h"     /* Tipos usados pelo programa  */
#include "TabelaCuco.h" /* Funções que lidam com      */
                       /* dispersão com encadeamento */
#include "Registros.h" /* Processamento de registros     */
#include "leitura.h"   /* Funções de leitura via teclado */

/******************* Alusões de Funções *******************/

extern unsigned int DispersaoPolinomial(tCEP chave, int tam);
extern unsigned int DispersaoRotativa(tCEP chave, int tam);
extern int EhPrimo(int valor);
extern int ProximoPrimo(int valor);
extern void ConstroiTabela(tTabelaCuco *aTabela, FILE *stream,
                           tFDispersaoCuco fDisp1, tFDispersaoCuco fDisp2);
extern void ApresentaMenu(const char *opcoes[], int nOpcoes);
extern void AtualizaArquivoBin( const char *nomeArquivo,
                                tTabelaCuco tabela);
extern void MedidaDeTempo(void);

/********************* Funções de dispersão ********************/

/****
 *
 * DispersaoPolinomial(): Calcula o valor de dispersão de um
 *                        string usando o método polinomial
 *
 * Parâmetros:
 *      chave (entrada) - o string
 *      tam (entrada) - tamanho da tabela
 *
 * Retorno: Nada
 *
 * Observação: Esta função não testa se o ponteiro que
 *             representa o string é NULL (como deveria)
 *
 ****/
unsigned int DispersaoPolinomial(tCEP chave, int tam)
{
   unsigned int dispersao = 0,
                base = 2,
                primo = 31;

   while (*chave) {
      dispersao = (dispersao*base + *chave++)%primo;
   }

   return dispersao%tam;
}

/****
 *
 * DispersaoRotativa(): Calcula o valor de dispersão de um
 *                      string usando o método rotativo
 *
 * Parâmetros:
 *      chave (entrada) - o string
 *      tam (entrada) - tamanho da tabela
 *
 * Retorno: Nada
 *
 * Observação: Esta função não testa se o ponteiro que
 *             representa o string é NULL (como deveria)
 *
 ****/
unsigned int DispersaoRotativa(tCEP chave, int tam)
{
   unsigned int dispersao = 0;

   while (*chave) {
       dispersao = (dispersao << 4) ^
                   (dispersao >> 28) ^ (*chave++);
   }

   return dispersao%tam;
}

/********************* Funções Locais *********************/

/****
 *
 * EhPrimo(): Verifica se um número inteiro é primo
 *
 * Parâmetros: valor (entrada) - o número que será verificado
 *
 * Retorno: 1, se o número for primo; 0, em caso contrário
 *
 ****/
int EhPrimo(int valor)
{
   int metade, divisor = 2;

   if (valor < 2)
      return 0;

      /* No máximo, um número qualquer é divisível por sua metade */
   metade = valor/2;

      /* Tenta encontrar um divisor do valor recebido */
   while (divisor <= metade) {
      if ( !(valor % divisor) ) /* Se foi encontrado um divisor, ... */
         return 0; /* ...o número não é primo */

      divisor++;
   }

   return 1; /* Não foi encontrado nenhum divisor */
}

/****
 *
 * ProximoPrimo(): Retorna o menor inteiro primo positivo que é
 *                 maior do que ou igual ao valor recebido como
 *                 parâmetro
 *
 * Parâmetros: valor (entrada) - o número que será verificado
 *
 * Retorno: O menor inteiro primo positivo que é maior do que
 *          ou igual ao valor recebido como parâmetro; ou 0,
 *          se o valor recebido for menor do que ou igual a 1,
 *          ou se não for possível encontrar um número primo
 *          que caiba no tipo int.
 *
 ****/
int ProximoPrimo(int valor)
{
      /* Esta função não lida com valores */
      /* menores do que ou iguais a 1     */
   if (valor <= 1)
      return 0;

      /* Tenta encontrar um número primo por meio de tentativas */
   while (valor <= INT_MAX) {
      if ( EhPrimo(valor) ) /* Se for encontrado um número   */
         return valor;      /* primo, retorna-se ele próprio */

      valor++;
   }

      /* Não foi encontrado nenhum número */
      /* primo que caiba no tipo int      */
   return 0;
}

/****
 *
 * ConstroiTabela(): Lê arquivo de dados (binário) contendo
 *                   os registros e constrói uma tabela de
 *                   dispersão cuco
 *
 * Parâmetros:
 *      stream (entrada) - stream associado ao arquivo de
 *                         registros
 *      tamTabela (saída) - tamanho da tabela criada
 *
 * Retorno: Ponteiro para a tabela criada, se a função
 *          for bem sucedida; NULL, em caso contrário
 *
 ****/
void ConstroiTabela( tTabelaCuco *aTabela, FILE *stream,
                     tFDispersaoCuco fDisp1, tFDispersaoCuco fDisp2 )
{
   int          pos = 0,
                tamTabela;
   tCEP_Ind chaveEIndice;
   tRegistroCEP    umRegistro;

      /* Verifica se o stream é válido */
   ASSEGURA(stream, "\nStream e' nulo em ConstroiTabela()\n");

         /* Calcula o tamanho da tabela */
   tamTabela = NumeroDeRegistros(stream, sizeof(umRegistro)) /
               FATOR_DE_CARGA;

   tamTabela = ProximoPrimo(tamTabela);

      /* Aloca e inicializa a tabela */
   CriaTabelaCuco(aTabela, tamTabela, fDisp1, fDisp2);

      /* Chamando sempre rewind() antes de qualquer  */
      /* processamento sequencial pode trazer grande */
      /* economia em tempo de depuração!             */
   rewind(stream);

   while ( fread(&umRegistro, sizeof(umRegistro), 1, stream) ) {
         /* Cria o conteúdo do par chave/índice, A chave */
         /* não é armazenada como string no registro.    */
         /* Portanto, não se pode usar strcpy().         */
      strncpy(chaveEIndice.chave, umRegistro.CEP, TAM_CEP);

         /* Transforma a chave num string. Isso não é */
         /* essencial, mas facilita o processamnto.   */
      chaveEIndice.chave[TAM_CEP] = '\0';

         /* Índice do registro que contém a chave no arquivo */
      chaveEIndice.indice = pos++;

      (void) InsereCuco( aTabela, chaveEIndice );
   }

   ASSEGURA( !ferror(stream),
             "\nOcorreu um erro de leitura no arquivo\n" );
}

/****
 *
 * ApresentaMenu(): Apresenta um menu de opções para o usuário
 *
 * Parâmetros:
 *      opcoes[] (entrada) - array de strings contendo as
 *                           opções do usuário
 *      nOpcoes (entrada) - o número de opções
 *
 * Retorno: Nada
 *
 ****/
void ApresentaMenu(const char *opcoes[], int nOpcoes)
{
   int i;

   printf("\nEscolha uma das opcoes a seguir:\n");

   for (i = 0; i < nOpcoes; ++i)
      printf("\n\t(%d) %s", i + 1, opcoes[i]);

   printf("\n\n\t>>> ");
}

/****
 *
 * AtualizaArquivoBin(): Cria um novo arquivo binário contendo
 *                       cada registro de outro arquivo binário
 *                       cuja chave não faça parte de uma lista
 *                       de registros que devem ser removidos
 *
 * Parâmetros:
 *      nomeArquivo (entrada) - nome do arquivo binário original
 *      removidos (entrada) - lista contendo as estruturas que
 *                            NÃO serão escritas no novo arquivo
 *
 * Retorno: Nenhum
 *
 ****/
void AtualizaArquivoBin( const char *nomeArquivo,
                         tTabelaCuco tabela )
{
   char      ar[L_tmpnam]; /* Nome de um arquivo temporário */
   FILE     *stream, /* Stream associado ao arquivo original */
            *streamTmp; /* Stream associado ao temporário */
   tRegistroCEP umRegistro; /* Um registro do arquivo */
   tCEP    umaChave;
   int       ocorreuErro = 0; /* Indicará se ocorreu erro */

      /* Tenta abrir o arquivo original para leitura */
   ASSEGURA( stream = fopen(nomeArquivo, "rb"),
             "Nao foi possivel atualizar arquivo" );

      /* Obtém o nome de um arquivo temporário */
   tmpnam(ar);

      /* Tenta abrir o arquivo temporário */
      /* para escrita em modo binário     */
   ASSEGURA( streamTmp = fopen(ar, "wb"),
             "Nao foi possivel criar arquivo temporario" );

      /* O laço encerra quando não for */
      /* mais possível ler um registro */
   while (1) {
         /* Lê um registro no arquivo de entrada */
      fread(&umRegistro, sizeof(umRegistro), 1, stream);

         /* Testa se o final do arquivo de entrada  */
         /* foi atingido ou ocorreu erro de leitura */
      if (feof(stream) || ferror(stream)) {
         break; /* Processamento encerrado */
      }

         /* Cria uma chave (string). O campo CEP */
         /* do registro não é string.            */
      strncpy(umaChave, umRegistro.CEP, TAM_CEP);
      umaChave[TAM_CEP] = '\0';

         /* Se a chave do registro lido não faz parte */
         /* da tabela, escreve-o no novo arquivo      */
      if (BuscaCuco(tabela, umaChave) >= 0) {
            /* Escreve o registro lido no arquivo temporário */
         fwrite(&umRegistro, sizeof(umRegistro), 1, streamTmp);

            /* Verifica se ocorreu erro de escrita */
         if (ferror(streamTmp)) {
            break;
         }
      }
   }

         /* Verifica se ocorreu erro de leitura ou escrita */
   if ( ferror(stream) || ferror(streamTmp) ) {
      ocorreuErro = 1;
   }

      /* Fecha os arquivos */
   fclose(stream);
   fclose(streamTmp);

   ASSEGURA(!ocorreuErro, "Erro na atualizacao do arquivo");

   ASSEGURA( !remove(nomeArquivo),
             "Impossivel remover o arquivo antigo" );

      /* Tenta rebatizar o arquivo temporário */
      /* com o nome do arquivo original       */
   ocorreuErro = rename(ar, nomeArquivo);

   if (ocorreuErro) {
      printf( "\n\t>>> O resultado da atualizacao encontra-se"
              "no arquivo \"%s\". Nao foi possivel "
              "rebatiza-lo.\n", ar );
      ASSEGURA( 0, "Erro em batismo de arquivo" );
   }
}

/****
 *
 * MedidaDeTempo(): Mede intervalos de tempo entre duas
 *                  chamadas consecutivas
 *
 * Parâmetros: Nenhum
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
void MedidaDeTempo(void)
{
   static int    numeroDeChamadas = 0;
   static time_t t0; /* Instante inicial */
   time_t        tf; /* Instante final   */

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

/********************* Função main() **********************/

/****
 *
 * main(): Testa funções de busca, inserção, remoção e
 *         atualização usando dispersão com encadeamento
 *
 * Parâmetros: nenhum
 *
 * Retorno: 0
 *
 ****/
int main(void)
{
   tRegistroCEP    umRegistro;
   const char*  opcoes[] = { "Estatisticas do banco de dados",
                             "Consulta dados de um CEP",
                             "Acrescenta um CEP",
                             "Remove um CEP",
                             "Encerra o programa"
                           };
   tTabelaCuco  tabela;
   char         umCEP[TAM_CEP + 1]; /* Uma chave de busca */
   tCEP_Ind indiceChave;
   FILE        *stream;
   int          op, pos,
                nOpcoes = sizeof(opcoes)/sizeof(opcoes[0]),
                nRegistros,
                primeiroByte, /* Indica a posição do */
                              /* primeiro byte de um */
                              /* registro no arquivo */
                atualizarArq = 0; /* Indica se o arquivo */
                                  /* de registros será   */
                                  /* atualizado          */

      /* Tenta abrir o arquivo de dados */
   stream = fopen(NOME_ARQUIVO_BIN, "r+b");

      /* Garante que o arquivo foi aberto */
   ASSEGURA(stream,
            "\nImpossivel abrir arquivo de registros\n");

   MedidaDeTempo();
   printf("\nCriando tabela...");

      /* Lê o arquivo e cria a tabela de dispersão */
   ConstroiTabela( &tabela, stream, DispersaoPolinomial,
                   DispersaoRotativa );

   printf("\n...tabela criada com sucesso\n");
   MedidaDeTempo();

      /* Obtém o número de registros do arquivo */
   nRegistros = NumeroDeRegistros( stream,
                                   sizeof(umRegistro) );

      /* O laço a seguir encerra quando o usuário */
      /* escolher a opção de encerramento         */
   while (1) {
         /* Apresenta menu de opções do programa */
         /* e lê a opção do usuário              */
      ApresentaMenu(opcoes, nOpcoes);
      op = LeOpcao("12345");

         /* Verifica se o usuário quer encerrar o programa */
      if (op == '5') {
            /* Fecha o arquivo que contém os registros */
         fclose(stream);

            /* Libera o espaço ocupado pela tabela */
         DestroiTabelaCuco(&tabela);

         break; /* Encerra o laço*/
      }

      switch (op) {
         case '1': /* Estatisticas do banco de dados */
            printf("\n\n>>> Estatisticas do Banco "
                   "de Dados <<<\n");

            printf( "\n\t>>> Numero de registros no "
                   "arquivo: %d",
                    NumeroDeRegistros( stream,
                                       sizeof(umRegistro)) );

            printf( "\n\t>>> Fator de carga da tabela: %3.2f",
                    FATOR_DE_CARGA );

            printf( "\n\t>>> Numero de posicoes em cada "
                    "tabela: %d", tabela.tam );

            printf( "\n\t>>> Numero de chaves na tabela: %d",
                    NChavesCuco(tabela) );

            printf( "\n\t>>> Numero de elementos vazios "
                    "na tabela: %d",
                    NElementosVaziosCuco(tabela) );

            break;

         case '2': /* Consulta dados de um CEP */
               /* Lê um CEP introduzido pelo usuário */
            LeMatricula("CEP", umCEP, TAM_CEP + 1);

               /* Procura o CEP na tabela de busca */
            pos = BuscaCuco(tabela, umCEP);

               /* Se a chave for encontrada, utiliza-se */
               /* seu índice para recuperar o registro  */
               /* correspondente no arquivo             */
            if (pos < 0) {
               printf("\n>>> CEP nao foi encontrado\n");
            } else {
                  /* Utilizando o índice do registro, */
                  /* calcula-se a posição no arquivo  */
                  /* do primeiro byte do registro     */
               primeiroByte = pos*sizeof(tRegistroCEP);

                  /* Move apontador de posição para o     */
                  /* primeiro byte do registro. Se isso   */
                  /* não for possível, aborta o programa. */
               ASSEGURA( !fseek(stream, primeiroByte, SEEK_SET),
                         "Erro de posicionamento em arquivo" );

                  /* Lê o registro no arquivo */
               fread(&umRegistro, sizeof(umRegistro), 1, stream);

                  /* Se ocorreu erro de leitura aborta */
               ASSEGURA(!ferror(stream), "Erro de leitura");

                  /* Apresenta o registro ao usuário */
               ExibeRegistro(&umRegistro);

               putchar('\n'); /* Embelezamento */
            }

            break;

         case '3': /* Acrescenta um CEP */
               /* Lê dados do novo CEP */
            LeRegistro(&umRegistro);

               /* O valor do campo 'numero' corresponde à */
               /* posição do registro no arquivo de CEPs  */
               /* (isso é coisa dos Correios)             */
            umRegistro.numero = nRegistros + 1;

               /* Cria a chave a ser inserida na tabela de */
               /* busca. Na tabela, a chabe é um string    */
               /* mas no registro CEP não é string         */
            strncpy(indiceChave.chave, umRegistro.CEP, TAM_CEP);

               /* Torna a chave um string */
            indiceChave.chave[TAM_CEP] = '\0';

            indiceChave.indice = nRegistros + 1;

               /* Acrescemta novo elemento à tabela de busca */
            InsereCuco( &tabela, indiceChave );

               /* Move apontador de posição para o final */
               /* do arquivo. Se isso não for possível,  */
               /* aborta o programa.                     */
            ASSEGURA( !fseek(stream, 0, SEEK_END),
                      "Erro de posicionamento em arquivo");

               /* Escreve o novo registro no final do arquivo */
            fwrite(&umRegistro, sizeof(umRegistro), 1, stream);

               /* Se ocorreu erro de escrita, aborta */
            ASSEGURA(!ferror(stream),
                     "Erro de escrita em arquivo ");

            ++nRegistros; /* Mais um registro */

            printf("\n>>> Acrescimo bem sucedido\n");

            break;

         case '4': /* Remove um CEP */
               /* Lê um CEP introduzido pelo usuário */
            LeMatricula("CEP", umCEP, TAM_CEP + 1);

               /* Tenta remover o CEP da tabela de busca */
            pos = RemoveCuco(&tabela, umCEP);

               /* Se a chave foi removida acrescenta-se   */
               /* um novo elemento na lista de removidos. */
               /* A remoçãodo registro do arquivo só será */
               /* efetuada ao final do programa.          */

            if (pos >= 0) { /* O CEP foi removido */
                  /* Arquivo de registros precisará */
                  /* ser atualizado                 */
               atualizarArq = 1;

               printf("\n>>> Remocao bem sucedida\n");
            } else { /* O CEP não se encontra na tabela */
               printf("\n>>> CEP nao encontrado\n");
            }

            break;

         default: /* O programa não deve chegar até aqui */
            printf("\nEste programa contem um erro\n");
            return 1;
      } /* switch */
   } /* while */

      /* Atualiza arquivo binário se for necessário */
   if (atualizarArq) {
      MedidaDeTempo();
      printf("\nAtualizando arquivo...");

         /* Atualiza arquivo binário */
      AtualizaArquivoBin(NOME_ARQUIVO_BIN, tabela);

      printf("\n... Atualizacao bem sucedida");
      MedidaDeTempo();
   }

   printf("\nObrigado por usar este programa. Bye.\n");

   return 0;
}

/******************* Exemplo de Execução ******************


**********************************************************/
