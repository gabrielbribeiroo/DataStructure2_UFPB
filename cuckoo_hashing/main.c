/****
 *
 * Exemplo: Dispers�o Cuco
 *
 * Se��o: 7.5.3
 *
 * Descri��o:
 *    -> A tabela de busca � implementada usando dispers�o cuco
 *       em que cada elemento cont�m uma chave e a posi��o do
 *       respectivo registro no arquivo
 *
 *    -> Os registros s�o mantidos em arquivo
 *
 *    -> Tipo de busca: linear
 *
 *    -> Arquivo de dados: CEPs.bin
 *
 ****/

/*********************** Includes *************************/

#include <math.h>      /* Fun��es matem�ticas   */
#include <stdlib.h>    /* Miscel�nea de fun��es */
#include <stdio.h>     /* Fun��es de Entrada/Sa�da    */
#include <string.h>    /* Fun��es memXXX() e strXXX() */
#include <time.h>      /* Medi��o de tempo         */
#include <limits.h>    /* Limites dos tipos inteiros */
#include "Macros.h"    /* Macros usadas pelo programa */
#include "Tipos.h"     /* Tipos usados pelo programa  */
#include "TabelaCuco.h" /* Fun��es que lidam com      */
                       /* dispers�o com encadeamento */
#include "Registros.h" /* Processamento de registros     */
#include "leitura.h"   /* Fun��es de leitura via teclado */

/******************* Alus�es de Fun��es *******************/

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

/********************* Fun��es de dispers�o ********************/

/****
 *
 * DispersaoPolinomial(): Calcula o valor de dispers�o de um
 *                        string usando o m�todo polinomial
 *
 * Par�metros:
 *      chave (entrada) - o string
 *      tam (entrada) - tamanho da tabela
 *
 * Retorno: Nada
 *
 * Observa��o: Esta fun��o n�o testa se o ponteiro que
 *             representa o string � NULL (como deveria)
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
 * DispersaoRotativa(): Calcula o valor de dispers�o de um
 *                      string usando o m�todo rotativo
 *
 * Par�metros:
 *      chave (entrada) - o string
 *      tam (entrada) - tamanho da tabela
 *
 * Retorno: Nada
 *
 * Observa��o: Esta fun��o n�o testa se o ponteiro que
 *             representa o string � NULL (como deveria)
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

/********************* Fun��es Locais *********************/

/****
 *
 * EhPrimo(): Verifica se um n�mero inteiro � primo
 *
 * Par�metros: valor (entrada) - o n�mero que ser� verificado
 *
 * Retorno: 1, se o n�mero for primo; 0, em caso contr�rio
 *
 ****/
int EhPrimo(int valor)
{
   int metade, divisor = 2;

   if (valor < 2)
      return 0;

      /* No m�ximo, um n�mero qualquer � divis�vel por sua metade */
   metade = valor/2;

      /* Tenta encontrar um divisor do valor recebido */
   while (divisor <= metade) {
      if ( !(valor % divisor) ) /* Se foi encontrado um divisor, ... */
         return 0; /* ...o n�mero n�o � primo */

      divisor++;
   }

   return 1; /* N�o foi encontrado nenhum divisor */
}

/****
 *
 * ProximoPrimo(): Retorna o menor inteiro primo positivo que �
 *                 maior do que ou igual ao valor recebido como
 *                 par�metro
 *
 * Par�metros: valor (entrada) - o n�mero que ser� verificado
 *
 * Retorno: O menor inteiro primo positivo que � maior do que
 *          ou igual ao valor recebido como par�metro; ou 0,
 *          se o valor recebido for menor do que ou igual a 1,
 *          ou se n�o for poss�vel encontrar um n�mero primo
 *          que caiba no tipo int.
 *
 ****/
int ProximoPrimo(int valor)
{
      /* Esta fun��o n�o lida com valores */
      /* menores do que ou iguais a 1     */
   if (valor <= 1)
      return 0;

      /* Tenta encontrar um n�mero primo por meio de tentativas */
   while (valor <= INT_MAX) {
      if ( EhPrimo(valor) ) /* Se for encontrado um n�mero   */
         return valor;      /* primo, retorna-se ele pr�prio */

      valor++;
   }

      /* N�o foi encontrado nenhum n�mero */
      /* primo que caiba no tipo int      */
   return 0;
}

/****
 *
 * ConstroiTabela(): L� arquivo de dados (bin�rio) contendo
 *                   os registros e constr�i uma tabela de
 *                   dispers�o cuco
 *
 * Par�metros:
 *      stream (entrada) - stream associado ao arquivo de
 *                         registros
 *      tamTabela (sa�da) - tamanho da tabela criada
 *
 * Retorno: Ponteiro para a tabela criada, se a fun��o
 *          for bem sucedida; NULL, em caso contr�rio
 *
 ****/
void ConstroiTabela( tTabelaCuco *aTabela, FILE *stream,
                     tFDispersaoCuco fDisp1, tFDispersaoCuco fDisp2 )
{
   int          pos = 0,
                tamTabela;
   tCEP_Ind chaveEIndice;
   tRegistroCEP    umRegistro;

      /* Verifica se o stream � v�lido */
   ASSEGURA(stream, "\nStream e' nulo em ConstroiTabela()\n");

         /* Calcula o tamanho da tabela */
   tamTabela = NumeroDeRegistros(stream, sizeof(umRegistro)) /
               FATOR_DE_CARGA;

   tamTabela = ProximoPrimo(tamTabela);

      /* Aloca e inicializa a tabela */
   CriaTabelaCuco(aTabela, tamTabela, fDisp1, fDisp2);

      /* Chamando sempre rewind() antes de qualquer  */
      /* processamento sequencial pode trazer grande */
      /* economia em tempo de depura��o!             */
   rewind(stream);

   while ( fread(&umRegistro, sizeof(umRegistro), 1, stream) ) {
         /* Cria o conte�do do par chave/�ndice, A chave */
         /* n�o � armazenada como string no registro.    */
         /* Portanto, n�o se pode usar strcpy().         */
      strncpy(chaveEIndice.chave, umRegistro.CEP, TAM_CEP);

         /* Transforma a chave num string. Isso n�o � */
         /* essencial, mas facilita o processamnto.   */
      chaveEIndice.chave[TAM_CEP] = '\0';

         /* �ndice do registro que cont�m a chave no arquivo */
      chaveEIndice.indice = pos++;

      (void) InsereCuco( aTabela, chaveEIndice );
   }

   ASSEGURA( !ferror(stream),
             "\nOcorreu um erro de leitura no arquivo\n" );
}

/****
 *
 * ApresentaMenu(): Apresenta um menu de op��es para o usu�rio
 *
 * Par�metros:
 *      opcoes[] (entrada) - array de strings contendo as
 *                           op��es do usu�rio
 *      nOpcoes (entrada) - o n�mero de op��es
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
 * AtualizaArquivoBin(): Cria um novo arquivo bin�rio contendo
 *                       cada registro de outro arquivo bin�rio
 *                       cuja chave n�o fa�a parte de uma lista
 *                       de registros que devem ser removidos
 *
 * Par�metros:
 *      nomeArquivo (entrada) - nome do arquivo bin�rio original
 *      removidos (entrada) - lista contendo as estruturas que
 *                            N�O ser�o escritas no novo arquivo
 *
 * Retorno: Nenhum
 *
 ****/
void AtualizaArquivoBin( const char *nomeArquivo,
                         tTabelaCuco tabela )
{
   char      ar[L_tmpnam]; /* Nome de um arquivo tempor�rio */
   FILE     *stream, /* Stream associado ao arquivo original */
            *streamTmp; /* Stream associado ao tempor�rio */
   tRegistroCEP umRegistro; /* Um registro do arquivo */
   tCEP    umaChave;
   int       ocorreuErro = 0; /* Indicar� se ocorreu erro */

      /* Tenta abrir o arquivo original para leitura */
   ASSEGURA( stream = fopen(nomeArquivo, "rb"),
             "Nao foi possivel atualizar arquivo" );

      /* Obt�m o nome de um arquivo tempor�rio */
   tmpnam(ar);

      /* Tenta abrir o arquivo tempor�rio */
      /* para escrita em modo bin�rio     */
   ASSEGURA( streamTmp = fopen(ar, "wb"),
             "Nao foi possivel criar arquivo temporario" );

      /* O la�o encerra quando n�o for */
      /* mais poss�vel ler um registro */
   while (1) {
         /* L� um registro no arquivo de entrada */
      fread(&umRegistro, sizeof(umRegistro), 1, stream);

         /* Testa se o final do arquivo de entrada  */
         /* foi atingido ou ocorreu erro de leitura */
      if (feof(stream) || ferror(stream)) {
         break; /* Processamento encerrado */
      }

         /* Cria uma chave (string). O campo CEP */
         /* do registro n�o � string.            */
      strncpy(umaChave, umRegistro.CEP, TAM_CEP);
      umaChave[TAM_CEP] = '\0';

         /* Se a chave do registro lido n�o faz parte */
         /* da tabela, escreve-o no novo arquivo      */
      if (BuscaCuco(tabela, umaChave) >= 0) {
            /* Escreve o registro lido no arquivo tempor�rio */
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

      /* Tenta rebatizar o arquivo tempor�rio */
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
 * Par�metros: Nenhum
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
void MedidaDeTempo(void)
{
   static int    numeroDeChamadas = 0;
   static time_t t0; /* Instante inicial */
   time_t        tf; /* Instante final   */

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

/********************* Fun��o main() **********************/

/****
 *
 * main(): Testa fun��es de busca, inser��o, remo��o e
 *         atualiza��o usando dispers�o com encadeamento
 *
 * Par�metros: nenhum
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
                primeiroByte, /* Indica a posi��o do */
                              /* primeiro byte de um */
                              /* registro no arquivo */
                atualizarArq = 0; /* Indica se o arquivo */
                                  /* de registros ser�   */
                                  /* atualizado          */

      /* Tenta abrir o arquivo de dados */
   stream = fopen(NOME_ARQUIVO_BIN, "r+b");

      /* Garante que o arquivo foi aberto */
   ASSEGURA(stream,
            "\nImpossivel abrir arquivo de registros\n");

   MedidaDeTempo();
   printf("\nCriando tabela...");

      /* L� o arquivo e cria a tabela de dispers�o */
   ConstroiTabela( &tabela, stream, DispersaoPolinomial,
                   DispersaoRotativa );

   printf("\n...tabela criada com sucesso\n");
   MedidaDeTempo();

      /* Obt�m o n�mero de registros do arquivo */
   nRegistros = NumeroDeRegistros( stream,
                                   sizeof(umRegistro) );

      /* O la�o a seguir encerra quando o usu�rio */
      /* escolher a op��o de encerramento         */
   while (1) {
         /* Apresenta menu de op��es do programa */
         /* e l� a op��o do usu�rio              */
      ApresentaMenu(opcoes, nOpcoes);
      op = LeOpcao("12345");

         /* Verifica se o usu�rio quer encerrar o programa */
      if (op == '5') {
            /* Fecha o arquivo que cont�m os registros */
         fclose(stream);

            /* Libera o espa�o ocupado pela tabela */
         DestroiTabelaCuco(&tabela);

         break; /* Encerra o la�o*/
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
               /* L� um CEP introduzido pelo usu�rio */
            LeMatricula("CEP", umCEP, TAM_CEP + 1);

               /* Procura o CEP na tabela de busca */
            pos = BuscaCuco(tabela, umCEP);

               /* Se a chave for encontrada, utiliza-se */
               /* seu �ndice para recuperar o registro  */
               /* correspondente no arquivo             */
            if (pos < 0) {
               printf("\n>>> CEP nao foi encontrado\n");
            } else {
                  /* Utilizando o �ndice do registro, */
                  /* calcula-se a posi��o no arquivo  */
                  /* do primeiro byte do registro     */
               primeiroByte = pos*sizeof(tRegistroCEP);

                  /* Move apontador de posi��o para o     */
                  /* primeiro byte do registro. Se isso   */
                  /* n�o for poss�vel, aborta o programa. */
               ASSEGURA( !fseek(stream, primeiroByte, SEEK_SET),
                         "Erro de posicionamento em arquivo" );

                  /* L� o registro no arquivo */
               fread(&umRegistro, sizeof(umRegistro), 1, stream);

                  /* Se ocorreu erro de leitura aborta */
               ASSEGURA(!ferror(stream), "Erro de leitura");

                  /* Apresenta o registro ao usu�rio */
               ExibeRegistro(&umRegistro);

               putchar('\n'); /* Embelezamento */
            }

            break;

         case '3': /* Acrescenta um CEP */
               /* L� dados do novo CEP */
            LeRegistro(&umRegistro);

               /* O valor do campo 'numero' corresponde � */
               /* posi��o do registro no arquivo de CEPs  */
               /* (isso � coisa dos Correios)             */
            umRegistro.numero = nRegistros + 1;

               /* Cria a chave a ser inserida na tabela de */
               /* busca. Na tabela, a chabe � um string    */
               /* mas no registro CEP n�o � string         */
            strncpy(indiceChave.chave, umRegistro.CEP, TAM_CEP);

               /* Torna a chave um string */
            indiceChave.chave[TAM_CEP] = '\0';

            indiceChave.indice = nRegistros + 1;

               /* Acrescemta novo elemento � tabela de busca */
            InsereCuco( &tabela, indiceChave );

               /* Move apontador de posi��o para o final */
               /* do arquivo. Se isso n�o for poss�vel,  */
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
               /* L� um CEP introduzido pelo usu�rio */
            LeMatricula("CEP", umCEP, TAM_CEP + 1);

               /* Tenta remover o CEP da tabela de busca */
            pos = RemoveCuco(&tabela, umCEP);

               /* Se a chave foi removida acrescenta-se   */
               /* um novo elemento na lista de removidos. */
               /* A remo��odo registro do arquivo s� ser� */
               /* efetuada ao final do programa.          */

            if (pos >= 0) { /* O CEP foi removido */
                  /* Arquivo de registros precisar� */
                  /* ser atualizado                 */
               atualizarArq = 1;

               printf("\n>>> Remocao bem sucedida\n");
            } else { /* O CEP n�o se encontra na tabela */
               printf("\n>>> CEP nao encontrado\n");
            }

            break;

         default: /* O programa n�o deve chegar at� aqui */
            printf("\nEste programa contem um erro\n");
            return 1;
      } /* switch */
   } /* while */

      /* Atualiza arquivo bin�rio se for necess�rio */
   if (atualizarArq) {
      MedidaDeTempo();
      printf("\nAtualizando arquivo...");

         /* Atualiza arquivo bin�rio */
      AtualizaArquivoBin(NOME_ARQUIVO_BIN, tabela);

      printf("\n... Atualizacao bem sucedida");
      MedidaDeTempo();
   }

   printf("\nObrigado por usar este programa. Bye.\n");

   return 0;
}

/******************* Exemplo de Execu��o ******************


**********************************************************/
