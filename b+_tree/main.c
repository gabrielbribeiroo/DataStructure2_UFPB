/****
 *
 * Título do Programa: Árvore B+ em Arquivo
 *
 * Autor: Ulysses de Oliveira
 *
 * Data de Criação: 25/04/2004
 * Última modificação: 21/07/2008
 *
 * Descrição Geral: Este programa demonstra várias
 *                  operações utilizando árvores de busca
 *                  multidirecionais armazenadas em arquivo
 *
 * Entrada: Opções escolhidas através de menu
 *
 * Saída: Resultados das operações escolhidas pelo usuário
 *        (v. exemplo de execução do programa no final
 *        deste arquivo)
 *
 * Arquivos:
 *     ASSERT.h - macros de tratamento de exceção usadas pelo
 *                programa
 *     ArvoreBM.h e - interface e implementação de operações
 *     ArvoreBM.c     operações sobre árvores B+
 *     Registros.h e - interface e implementação de operações
 *     Registros.c     sobre registros armazenados em arquivos
 *     main.c (este arquivo) - contém main() e outras funções
 *                             que testam as operações sobre
 *                             árvores de B+
 *
 ****/

/*********************** Includes *************************/

#include <stdio.h>     /* Funções de Entrada/Saída        */
#include <stdlib.h>    /* Miscelânea                      */
#include <string.h>    /* Funções strXXX() e memXXX()     */
#include <time.h>      /* Medição de tempo                */
#include "ArvoreBM.h"  /* Funções que lidam com árvores B */
#include "Registros.h" /* Funções de processamento        */
                       /* de registros                    */
#include "Leitura.h"   /* Funções de leitura via teclado  */
#include "ASSEGURA.h"  /* Macro ASSEGURA e outras macros  */
                       /* de tratamento de exceção        */

/***************** Constantes Simbólicas ******************/

   /* Nome do arquivo de texto contendo os registros */
#define NOME_ARQUIVO_REGS "../CensoMEC2012.txt"

   /* Nome do arquivo binário de registros Windows */
//#define NOME_ARQ_BIN "I:\\Dados\\CensoMEC\\CensoMEC2012.bin"

   /* Nome do arquivo binário de registros Unix/Linux */
//#define NOME_ARQ_BIN "CensoMEC2012.bin"

   /* Nome do arquivo binário de registros (Teste) */
#define NOME_ARQ_BIN "../MiniCensoMEC2012.bin"

   /* Nome do arquivo contendo a árvore */
#define NOME_ARQ_ARV "ArvoreBM.dat"

   /* Nome do arquivo de texto que contém as chaves ordenadas */
//#define NOME_ARQ_CHAVES  "I:\\Dados\\CensoMEC\\ChavesOrdenadasBM.txt"
#define NOME_ARQ_CHAVES "ChavesOrdenadasBM.txt"

   /* Nome do arquivo de texto que contém o resultado de uma busca de intervalo */
//#define NOME_ARQ_INTER  "I:\\Dados\\CensoMEC\\BuscaIntervaloBM.txt"
#define NOME_ARQ_INTER "BuscaIntervaloBM.txt"

   /* Nome inicial do arquivo de texto contendo os nós da árvore */
#define NOME_ARQ_NOS "NosArvoreBM"

   /* Nome do arquivo de texto contendo registros em formato legível */
#define NOME_ARQ_DEBUG   "RegistrosDebug.txt"

/****************** Definições de Macros ******************/

#define MAX(a,b) (((a) > (b)) ? (a) : (b))

/**************** Compilação Condidicional ****************/

#define DEPURACAO 0

/******************** Funções de Locais *******************/

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
static void ApresentaMenu(const char *opcoes[], int nOpcoes)
{
   int i;

   printf("\n>>> Escolha uma das opcoes a seguir:\n");

   for (i = 0; i < nOpcoes; ++i)
      printf("\n\t[%d] %s", i + 1, opcoes[i]);

   printf("\n\n\t>>> ");
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
static void MedidaDeTempo(void)
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

/****
 *
 * TamanhoNoSemPreenchimento(): Calcula o tamanho de um nó sem
 *                              o preenchimento de estrutura
 *                              efetuado pelo compilador
 *
 * Parâmetros: Nenhum
 *
 * Retorno: O tamanho de um nó sem preenchimento
 *
 * Observação: O propósito desta função é apenas depuração e
 *             melhor entendimento do programa
 *
 ****/
static int TamanhoNoSemPreenchimento(void)
{
   int tni, /* Tamanho de um nó interno sem preenchimento */
       tnf, /* Tamanho de uma folha sem preenchimento     */
       ti,  /* largura do tipo int                        */
       tc,  /* Tamanho de uma chave                       */
       tci, /* Tamanho de um par chave/índice             */
       tt,  /* Tamanho de uma constante de enumeração     */
       t;   /* Tamanho de um nó sem preenchimento         */

   ti = sizeof(int);
   tc = sizeof(tChave);
   tci = sizeof(tChave) + sizeof(int);
   tt = sizeof(tTipoDoNo);

   tni = ti + (G - 1)*tc + G*ti;

   tnf = ti + F*tci + ti;

   t = tt + MAX(tni,tnf);

   return t;
}

/****
 *
 * InfoArvoreBM(): Apresenta informações gerais sobre a árvore B+
 *
 * Parâmetros:
 *    *streamArv (entrada) - stream associado ao arquivo que
 *                           contém a árvore
 *
 * Retorno: Nada
 *
 ****/
void InfoArvoreBM(FILE *streamArv)
{
   int   menorChave,
         maiorChave,
         nosInternos = 0,
         nosFolhas = 0,
         nosVazios = 0,
         nosDesconhecidos = 0,
         nChaves = 0,
         tg = (int) TG,
         tf = (int) TF,
         tamNoCom, /* Tamanho de um nó com preenchimento */
         tamNoSem; /* Tamanho de um nó sem preenchimento */
   tNoBM no;

      /* Verifica se streamArv é válido */
   ASSEGURA( streamArv,
             ERRO_STREAM_NULL(streamArv, InfoArvoreBM) );

   printf("\n\n\t>>>  Informacoes sobre a Arvore B+  <<<\n");

   printf("\n\t*** Parametros da arvore ***");

   printf("\n\t\t>>> G  = %d", (int) G);
   printf("\n\t\t>>> TG = %d", tg);
   printf("\n\t\t>>> F  = %d", (int) F);
   printf("\n\t\t>>> TF = %d\n", tf);

   tamNoCom = (int) sizeof(tNoBM);
   printf( "\n\t>>> Tamanho de um no' COM preenchimento: %d bytes",
           tamNoCom );

   tamNoSem = TamanhoNoSemPreenchimento();
   printf( "\n\t>>> Tamanho de um no' SEM preenchimento: %d bytes",
           tamNoSem );

   if (tamNoCom - tamNoSem > 0)
      printf( "\n\t>>> Espaco desperdicado por no': %d bytes",
              tamNoCom - tamNoSem );
   else if (tamNoCom - tamNoSem == 0)
      printf( "\n\t>>> Nao ha' espaco desperdicado com "
              "preenchimento" );
   else
      ASSEGURA(0, "O calculo de preenchimento esta' errado\n");

   printf("\n\t>>> Tamanho de um bloco: %d bytes", TB);
   printf( "\n\t>>> Espaco de bloco nao utilizado: %d bytes\n\n",
           TB - tamNoCom );

   printf( "\n\t>>> Numero maximo de filhos em no' interno: %d",
           (int) G );
   printf( "\n\t>>> Numero minimo de filhos em no' interno: %d",
           tg );

   printf( "\n\t>>> Numero maximo de chaves em folha: %d",
           (int) F );
   printf("\n\t>>> Numero minimo de chaves em folha: %d\n\n", tf);

   rewind(streamArv);

   while (1) {
      fread(&no, sizeof(no), 1, streamArv);

      if (feof(streamArv) || ferror(streamArv))
         break;

      if (no.tipoDoNo == INTERNO)
         ++nosInternos;
      else if (no.tipoDoNo == FOLHA) {
         ++nosFolhas;
         nChaves += no.conteudo.noFolha.nChaves;
      } else if (no.tipoDoNo == NO_VAZIO)
         ++nosVazios;
      else
         ++nosDesconhecidos;
   }

   printf("\t>>> Numero de nos internos: %d", nosInternos);
   printf("\n\t>>> Numero de folhas: %d", nosFolhas);
   printf("\n\t>>> Numero de nos vazios: %d", nosVazios);
   printf( "\n\t>>> Numero de nos desconhecidos: %d",
           nosDesconhecidos );
   printf( "\n\t>>> Numero total de nos (inclui vazios): %d",
           nosInternos + nosFolhas + nosVazios + nosDesconhecidos );
   printf("\n\t>>> Numero total de chaves: %d\n", nChaves);

   printf( "\n\t>>> Profundidade da arvore (incluindo folhas): "
           "%d\n", ProfundidadeBM(streamArv) );

   menorChave = MenorChaveBM(streamArv);

   printf( "\n\t>>> Menor chave armazenada na arvore: %d",
           menorChave );

   maiorChave = MaiorChaveBM(streamArv);

   printf( "\n\t>>> Maior chave armazenada na arvore: %d",
           maiorChave );

   ASSEGURA(!nosDesconhecidos, "Nao deveria haver nos desconhecidos\n");
}

/****
 *
 * ConstroiArvore(): Lê arquivo de dados (binário) contendo
 *                   os registros e constrói uma árvore B+
 *                   em outro arquivo
 *
 * Parâmetros:
 *     streamReg (entrada) - stream binário associado ao arquivo de
 *                           registros
 *     streamArv (entrada) - stream associado ao arquivo que
 *                           conterá a árvore
 *    *raiz (entrada/saída) - a raiz da árvore criada
 *
 * Retorno: Nada
 *
 ****/
void ConstroiArvore(FILE *streamReg, FILE *streamArv, tNoBM *raiz)
{
   tRegistro    registro;
   int          pos = 0;
   tChaveIndice chaveIndice;

   ASSEGURA( raiz, "Erro em ConstroiArvore(): raiz e' NULL" );

      /* Os registros serão acessados sequencialmente.   */
      /* Portanto, deve-se assegurar que o processamento */
      /* começa no início do arquivo.                    */
   MoveApontador(streamReg, 0, SEEK_SET);

      /* Inicia a raiz da árvore */
   raiz->tipoDoNo = NO_VAZIO;

   while (1) {
         /* Lê um registro */
      fread(&registro, sizeof(registro), 1, streamReg);

         /* Se ocorreu erro de leitura ou o final  */
         /* do arquivo foi atingido encerra o laço */
      if (feof(streamReg) || ferror(streamReg))
         break;

         /* Cria o conteúdo do par chave/índice */
      chaveIndice.chave = registro.codAlunoCurso;
      chaveIndice.indice = pos++; /* Cuidado com o incremento */

         /* Verifica se houve inserção */
      ASSEGURA( InsereBM( raiz, &chaveIndice, streamArv ),
                "Nao houve insercao em ConstroiArvore" );

//      if (!InsereBM( raiz, &chaveIndice, streamArv)) {
//         printf("\nA chave %d ja' existe\n", chaveIndice.chave);
//         printf("\nchaveIndice.indice = %d\n", chaveIndice.indice);
//      }
   }

      /* Processamento terminado. Verifica se  */
      /* ocorreu algum erro de leitura/escrita */
   ASSEGURA( !ferror(streamReg) && !ferror(streamArv),
             "Erro de leitura ou escrita em ConstroiArvore" );
}

/****
 *
 * IniciaArvoreBM(): Verifica se a árvore de busca já existe.
 *                   Se ela já existir e o usuário desejar
 *                   reconstrui-la ou se ela não existir,
 *                   ela será construída.
 *
 * Parâmetros:
 *     nomeArqArvore (entrada) - nome do arquivo que armazena a
 *                               árvore
 *     streamRegistros (entrada/saída) - stream associado ao
 *                                       arquivo de registros
 *    *raiz (saída) - a raiz da árvore
 *
 * Retorno: Stream associado ao arquivo que contém a árvore
 *
 * Observação: O stream de registros deve estar aberto num modo
 *             binário que permita leitura
 *
 ****/
FILE *IniciaArvoreBM( const char *nomeArqArvore,
                      FILE *streamRegistros, tNoBM *raiz )
{
   int   op; /* Opção do usuário */
   FILE *streamArvore; /* Stream associado ao arquivo da árvore */

      /* Verifica se o arquivo que contém a árvore já existe */
   if (ArquivoExiste(nomeArqArvore)) {
      /*                                */
      /* Árvore já existe. Se o usuário */
      /* desejar, ela será reconstruída */
      /*                                */

         /* Oferece a opção ao usuário */
      op = LeSimNao("Arvore ja existe. Deseja substitui-la");

         /* Verifica se o usuário deseja reconstruir a árvore */
      if ( op == 'n' || op == 'N') { /* Ele não deseja isso   */
            /* Tenta abrir o arquivo que contém a árvore */
         streamArvore = AbreArquivo(nomeArqArvore, "r+b");

         LeNoBM(streamArvore, 0, raiz);

         return streamArvore; /* Serviço completo */
      }
   }

   /*                                        */
   /* Neste ponto, ou a árvore ainda não     */
   /* existe ou usuário deseja reconstrui-la */
   /*                                        */

   printf("\nCriando arvore...\n");
   fflush(stdout);

      /* Cria o arquivo e checa se abertura foi OK */
   streamArvore = AbreArquivo(nomeArqArvore, "w+b");

      /* Constrói a árvore */
   ConstroiArvore(streamRegistros, streamArvore, raiz);

   return streamArvore;
}

/****
 *
 * CriaArquivoRegistros(): Verifica se o arquivo de registros já
 *                         existe. Se ela já existir e o usuário
 *                         desejar reconstrui-lo ou se ele não
 *                         existir, ele será construído.
 *
 * Parâmetros:
 *     nomeArqTexto (entrada) - nome do arquivo texto de registros
 *     nomeArqBin (entrada) - nome do arquivo binário de registros
 *
 * Retorno: Stream associado ao arquivo binário de registros
 *
 ****/
FILE *CriaArquivoRegistros( const char *nomeArqTexto,
                            const char *nomeArqBin )
{
   int   op, /* Opção do usuário */
         nRegistros, /* Número de registros no arquivo */
         maxRegistros; /* Número máximo de registros no arquivo */

      /* Verifica se o arquivo binário de registros já existe */
   if (ArquivoExiste(nomeArqBin)) {
      /*                                */
      /* Arquivo já existe. Se o usuário */
      /* desejar, ele será reconstruído */
      /*                                */

         /* Oferece a opção ao usuário */
      op = LeSimNao( "Arquivo de registros ja' existe. "
                      "Deseja substitui-lo" );

         /* Verifica se o usuário deseja reconstruir o arquivo */
      if (op == 'n') /* Ele não deseja isso   */
         return AbreArquivo(nomeArqBin, "r+b");
   }

   /*                                        */
   /* Neste ponto, ou o arquivo ainda não    */
   /* existe ou usuário deseja reconstrui-lo */
   /*                                        */

   printf("\n\t>>> Digite o numero maximo de registros (0 = TODOS): ");
   maxRegistros = LeNatural();

   printf("\nCriando arquivo...\n");
   fflush(stdout);

      /* Cria o arquivo binário */
   nRegistros = CriaArquivoBin( nomeArqTexto, nomeArqBin,
                                maxRegistros );

      /* Apresenta o resultado da conversão */
   printf( "\n\t>>> Foram armazenados %d registros no arquivo"
           "\n\t>>> \"%s\"\n", nRegistros, nomeArqBin );

      /* Abre e retorna stream assoaciado ao arquivo binário */
   return AbreArquivo(nomeArqBin, "r+b");
}

/****
 *
 * EscreveArrayChavesIndices(): Escreve num stream um array de
 *                              pares chave/índice
 *
 * Parâmetros:
 *      ci[] (entrada) - o array
 *      n (entrada) - número de elementos do array
 *      stream (entrada) - o stream no qual ocorrerá a escrita
 *
 * Retorno: Nada
 *
 ****/
void EscreveArrayChavesIndices( const tChaveIndice ci[],
                                int n, FILE *stream )
{
   int i;

   fprintf(stream, "\nChaves e posicoes = { ");

   for (i = 0; i < n; ++i)
      fprintf(stream, "(%ld, %d) ", ci[i].chave, ci[i].indice);

   fprintf(stream, "}");
}

/****
 *
 * EscreveArrayChaves(): Escreve num stream um array de chaves
 *
 * Parâmetros:
 *      c[] (entrada) - o array
 *      n (entrada) - número de elementos do array
 *      stream (entrada) - o stream no qual ocorrerá a escrita
 *
 * Retorno: Nada
 *
 ****/
void EscreveArrayChaves(const tChave c[], int n, FILE *stream)
{
   int i;

   fprintf(stream, "\nChaves = {%ld", c[0]);

   for (i = 1; i < n; ++i)
      fprintf(stream, ", %ld", c[i]);

   fprintf(stream, "}");
}

/****
 *
 * EscreveArrayFilhos(): Escreve num stream um array de ints que são
 *                       índices que representam filhos de um nó
 *
 * Parâmetros:
 *      filhos[] (entrada) - o array
 *      n (entrada) - número de elementos do array
 *      stream (entrada) - o stream no qual ocorrerá a escrita
 *
 * Retorno: Nada
 *
 ****/
void EscreveArrayFilhos(const int filhos[], int n, FILE *stream)
{
   int i;

   fprintf(stream, "\nFilhos = {%d", filhos[0]);

   for (i = 1; i < n; ++i)
      fprintf(stream, ", %d", filhos[i]);

   fprintf(stream, "}");
}

/****
 *
 * ExibeNoBM(): Escreve um nó de uma árvore B+ num stream
 *
 * Parâmetros:
 *      *no (entrada) - o nó
 *      stream (entrada) - o stream no qual ocorrerá a escrita
 *
 * Retorno: Um valor que indica o tipo de nó escrito (interno
 *          ou folha)
 *
 ****/
tTipoDoNo ExibeNoBM(const tNoBM *no, FILE *stream)
{
   if (no->tipoDoNo == NO_VAZIO) {
      fprintf(stream, "No' Vazio");

      return NO_VAZIO;
   }

   if (no->tipoDoNo == FOLHA) {
      fprintf(stream, "Tipo de no': Folha");
      fprintf(stream, "\nNumero de chaves: %d",
              no->conteudo.noFolha.nChaves);
      EscreveArrayChavesIndices( no->conteudo.noFolha.chaves,
                                 no->conteudo.noFolha.nChaves,
                                 stream );
      fprintf ( stream, "\nProxima folha: %d",
                no->conteudo.noFolha.proximaFolha );

      return FOLHA;
   }

   if (no->tipoDoNo == INTERNO) {
      fprintf(stream, "Tipo de no': Interno");
      fprintf(stream, "\nNumero de chaves: %d",
              no->conteudo.noInterno.nFilhos - 1);
      fprintf(stream, "\nNumero de filhos: %d",
              no->conteudo.noInterno.nFilhos);
      EscreveArrayChaves(no->conteudo.noInterno.chaves,
                         no->conteudo.noInterno.nFilhos - 1, stream);
      EscreveArrayFilhos(no->conteudo.noInterno.filhos,
                         no->conteudo.noInterno.nFilhos, stream);

      return INTERNO;
   }

   fprintf(stream, "Tipo de no': desconhecido (%d)", no->tipoDoNo);

   return -1;
}

/****
 *
 * CriaArquivoDeNosBM(): Apresenta os nós de uma árvore B+ num
 *                       arquivo de texto
 *
 * Parâmetros:
 *      streamArv (entrada) - stream associado ao arquivo que
 *                            contém a árvore
 *      nomeIni (entrada) - nome inicial do arquivo que conterá
 *                          as informações sobre os nós da árvore
 *
 * Retorno: Nada
 *
 ****/
void CriaArquivoDeNosBM(FILE *streamArv, const char *nomeIni)
{
   static int ordem = 0;
   int        i = 0,
              nosInternos = 0,
              nosFolhas = 0,
              nosVazios = 0,
              nosDesconhecidos = 0,
              nChaves = 0;
   tTipoDoNo  tipo;
   tNoBM      no;
   FILE      *streamNos;
   char      *nomeArqNos;

      /* O limite de arquivos criados por seção é 99 */
   if (++ordem > 99) {
      printf("\n\t\a>>> Ja' criei arquivos demais <<<\n");
      return;
   }

      /* Aloca espaço para o nome do arquivo de nós: */
      /* 1 (terminal) + 2 (ordem) + 4 (".txt") = 7   */
   nomeArqNos = malloc(strlen(nomeIni) + 7);
   ASSEGURA( nomeArqNos,
             "Nao foi possivel criar nome de arquivo de nos" );

      /* Cria o nome completo do arquivo */
   sprintf(nomeArqNos, "%s%.2d%s", nomeIni, ordem, ".txt");

      /* Tenta criar o aquivo */
   streamNos = fopen(nomeArqNos, "w");
   ASSEGURA(streamNos, "Nao foi possivel criar arquivo de nos");

      /* A leitura deve começar no início */
      /* do arquivo que contém a árvore   */
   rewind(streamArv);

      /* Lê cada nó no arquivo que contém a árvore e escreve */
      /* as informações sobre eles num arquivo de texto      */
   while (1) {
      fread(&no, sizeof(no), 1, streamArv);

      if (feof(streamArv) || ferror(streamArv))
         break;

      fprintf(streamNos, "%d.\n\n", i++);

      tipo = ExibeNoBM(&no, streamNos);

      if (tipo == INTERNO)
         ++nosInternos;
      else if (tipo == FOLHA) {
         ++nosFolhas;
         nChaves += no.conteudo.noFolha.nChaves;
      } else if (tipo == NO_VAZIO)
         ++nosVazios;
      else
         ++nosDesconhecidos;

      fprintf(streamNos, "\n\n----------\n\n");
   }

   fprintf(streamNos, "\t>>> Grau da arvore (G): %d", (int) G);
   fprintf( streamNos, "\n\t>>> Max chaves em folha (F): %d",
            (int) F );
   fprintf( streamNos, "\n\t>>> Profundidade da arvore: %d\n\n",
            ProfundidadeBM(streamArv) );

   fprintf( streamNos, "\t>>> Numero de nos internos: %d",
            nosInternos );
   fprintf( streamNos, "\n\t>>> Numero de folhas: %d",
            nosFolhas );
   fprintf( streamNos, "\n\t>>> Numero de nos vazios: %d",
            nosVazios );
   fprintf( streamNos, "\n\t>>> Numero total de nos (inclui "
            "vazios): %d", nosInternos + nosFolhas + nosVazios );
   fprintf( streamNos, "\n\t>>> Numero total de chaves: %d\n\n",
            nChaves );

   fclose(streamNos);

   printf( "\n\t>>> Resultado escrito no arquivo: \"%s\"\n",
           nomeArqNos );

   free(nomeArqNos);

   if (!nosDesconhecidos)
      return;

   printf( "\n\a\t\t>>> Foram encontrados %d nos desconhecidos!",
           nosDesconhecidos );

      /* Não deveria haver nós desconhecidos */
   ASSEGURA( !nosDesconhecidos,
             "Nao deveria haver nos desconhecidos" );
}

/****
 *
 * EscreveRegistrosDebug(): Escreve o conteúdo de um arquivo
 *                          binário de registros num arquivo de
 *                          texto de modo legível
 *
 * Parâmetros: Nenhum
 *
 * Retorno: Nada
 *
 ****/
void EscreveRegistrosDebug(FILE *streamReg, const char *nomeArqTxt)
{
   int     i = 0, /* Índice do registro no arquivo */
           ocorreuErro = 0;
   tRegistroMEC  reg;
   FILE   *streamTxt;

   streamTxt = AbreArquivo(nomeArqTxt, "w");

   MoveApontador(streamReg, 0L, SEEK_SET);

   while (1) {
      fread(&reg, sizeof(reg), 1, streamReg);

      if (feof(streamReg) || ferror(streamReg))
         break;

      fprintf(streamTxt, "%d.\n\n", i++);

      if (ferror(streamTxt))
         break;

      EscreveRegistro(&reg, streamTxt);

      fprintf(streamTxt, "\n\n----------\n\n");

      if (ferror(streamTxt))
         break;
   }

   ocorreuErro = ferror(streamReg) || ferror(streamTxt);

   FechaArquivo(streamTxt, nomeArqTxt);

   if (ocorreuErro) {
      fclose(streamReg);
      printf("\n\t\a>>> Ocorreu erro de leitura ou escrita\n");
      exit(1);
   }
}

/************************ Função main() *************************/

/****
 *
 * main(): Testa funções de busca multidirecional com a
 *         árvore B armazenada em arquivo
 *
 * Parâmetros: nenhum
 *
 * Retorno: 0
 *
 ****/
int main(void)
{
   tRegistro   umRegistro;
   const char *opcoes[] = { "Consulta dados de um aluno",           /* 1 */
                            "Insere um aluno",                      /* 2 */
                            "Remove um aluno",                      /* 3 */
                            "Escreve as chaves em ordem",           /* 4 */
                            "Busca de intervalo",                   /* 5 */
                            "Escreve nos da arvore B+ (depuracao)", /* 6 */
                            "Escreve registros (depuracao)",        /* 7 */
                            "Encerra o programa"                    /* 8 */
                          };
   int          op,  /* Opção do usuário  */
                pos, /* Posição de um registro em arquivo */
                teste, /* Testa o retorno de uma função */
                nOpcoes = sizeof(opcoes)/sizeof(opcoes[0]),
                nRegs = 0, /* Número de registros no arquivo */
                sizeofLong = (int) sizeof(long);
   tNoBM        raiz; /* Armazena a raiz da árvore */
   tChave       umaChave, /* Uma chave de busca */
                outraChave; /* Outra chave de busca */
   tChaveIndice chaveEIndice;
   FILE        *streamReg, /* Stream bonário associado */
                           /* ao arquivo de registros  */
               *streamArv, /* Stream associado ao       */
                           /* arquivo contendo a árvore */
               *streamChaves; /* Stream associado a um     */
                              /* arquivo que armazena as   */
                              /* chaves em ordem crescente */

   printf("\n\t>>> sizeof(long) = %d", sizeofLong);

   if (sizeofLong >= 8) {
      printf("... OK\n");
   } else {
      printf("\n\t>>> Este programa pode nao funcionar adequadamente "
             "\n\t>>> quando sizeof(long) e' menor do que 8");
      op = LeSimNao("Deseja prosseguir");

      if (op == 'n') {
         printf("\n\t>>> Adquira um compilador decente e recompile "
                "\n\t>>> o programa. Bye.");
         return 1;
      }
   }

   MedidaDeTempo();
   printf("\nIniciando arvore...\n");

      /* Abre ou cria o arquivo de registros */
   streamReg = CriaArquivoRegistros(NOME_ARQUIVO_REGS, NOME_ARQ_BIN);

      /* Inicia a árvore B+ e obtém a raiz da árvore */
   streamArv = IniciaArvoreBM( NOME_ARQ_ARV, streamReg, &raiz );

   printf("\n\t>>> Arvore criada com sucesso");

      /* Calcula o número de registros */
   nRegs = NumeroDeRegistros(streamReg, sizeof(umRegistro));

   MedidaDeTempo();

   MedidaDeTempo();
   printf("\nChecando a consistencia do programa...");

   InfoArvoreBM(streamArv);

   printf("\n\n...consistencia OK");
   MedidaDeTempo();

   while (1) { /* Laço principal do programa */
      ApresentaMenu(opcoes, nOpcoes);

      op = LeOpcao("12345678");

         /* Verifica se o usuário deseja sair do programa */
      if (op == '8') {
         break; /* Encerra o laço */
      }

      if (op <= '3') { /* Estas opções precisam de uma chave */
         printf("\n\t>>> Digite a chave (codigo aluno/curso): ");
         umaChave = LeNatural();
      }

      switch (op) {
         case '1': /* Procura o registro que */
                   /* possui uma dada chave  */

               /* Tenta encontrar a posição do registro */
               /* que contém a chave de busca           */
            pos = BuscaBM(streamArv, &raiz, umaChave);

               /* Verifica se a chave foi encontrada */
            if (pos < 0)
               printf("\n\t>>> A chave nao foi encontrada\n");
            else if (LeRegistroEmArquivo( &umRegistro, streamReg,
                                          pos) ) {
               printf( "\n\t>>> Registro com a chave %ld <<<\n",
                       umaChave);
               putchar('\n'); /* Embelezamento */
               ExibeRegistro(&umRegistro);
               putchar('\n'); /* Embelezamento */
            } else
               printf("\n\t>>> Ocorreu um erro ao tentar"
                      " recuperar registro\n");

            break;

         case '2': /* Insere um novo registro, se a */
                   /* chave dada ainda não existe   */

               /* Tenta inserir o par chave/índice na árvore. */
               /* Se inserção for bem-sucedida, insere o      */
               /* respectivo registro no arquivo de registros */
            chaveEIndice.chave = umaChave;
            chaveEIndice.indice = nRegs;

            if (!InsereBM( &raiz, &chaveEIndice, streamArv))
               printf("\n\t>>> A chave ja' existe na arvore\n");
            else {
                  /* A chave foi inserida na árvore. Resta    */
                  /* inserir o respectivo registro no arquivo */
               pos = chaveEIndice.indice;

                  /* A gravação do respectivo registro */
                  /* no arquivo não pode falhar        */
               InsereRegistroEmArquivo( chaveEIndice.chave,
                                        streamReg );

               nRegs++; /* O número de registros aumentou */

                  /* Garante que os buffers associados */
                  /* aos arquivos são descarregados    */
               fflush(streamArv);
               fflush(streamReg);

               printf("\n\t>>> Insercao bem-sucedida\n");
            }

            break;

         case '3': /* Remove uma chave da árvore */
            if (RemoveBM(streamArv, &raiz, umaChave)) {
                  /* Se a árvore ficou vazia, cria um novo */
                  /* arquivo vazio para conter a árvore    */
               if (raiz.tipoDoNo == NO_VAZIO)
                  streamArv = AbreArquivo(NOME_ARQ_ARV, "w+b");

               printf( "\n\t>>> Remocao bem sucedida\n");
            } else {
               printf( "\n\t>>> A remocao MAL sucedida\n");
            }

            break;

         case '4':   /* Escreve em arquivo de texto  */
                     /* as chaves em ordem crescente */
            streamChaves = AbreArquivo(NOME_ARQ_CHAVES, "w");

               /* Faz um caminhamento em ordem infixa na */
               /* árvore e escreve as chaves encontradas */
               /* nesse processo num arquivo de texto    */
            EscreveChavesOrdenadas(&raiz, streamArv, streamChaves);
            printf( "\n\t>>> As chaves foram gravados em ordem "
                    "crescente no arquivo: \"%s\"\n",
                    NOME_ARQ_CHAVES );

               /* Fecha o arquivo de chaves ordenadas */
            FechaArquivo(streamChaves, NOME_ARQ_CHAVES);

            break;

         case '5': /* Busca de intervalo */
            printf("\n\t>>> Digite a primeira chave: ");
            umaChave = LeNatural();

            printf("\n\t>>> Digite a segunda chave: ");
            outraChave = LeNatural();

            streamChaves = AbreArquivo(NOME_ARQ_INTER, "w");

            teste = BuscaIntervaloBM( umaChave, outraChave, &raiz,
                                      streamArv, streamChaves );

            if (teste) {
               printf( "\n\t>>> O resultado da busca foi armazenado"
                       " no arquivo: \"%s\"\n", NOME_ARQ_INTER );
            } else {
               printf("\n\t>>> A busca nao produziu resultado\n");
            }

               /* Fecha o arquivo com o resultado da busca */
            FechaArquivo(streamChaves, NOME_ARQ_INTER);

            break;

         case '6': /* Apresenta os nós da árvore B+ num */
                   /* arquivo de texto (depuração)      */
            CriaArquivoDeNosBM(streamArv, NOME_ARQ_NOS);

            break;

         case '7': /* Apresenta os registros num arquivo */
                   /* de texto para depuração             */
            EscreveRegistrosDebug(streamReg, NOME_ARQ_DEBUG);
               printf( "\n\t>>> Os registros foram escritos no "
                       "arquivo: \"%s\"\n", NOME_ARQ_DEBUG );

            break;
      } /* switch */
   } /* while */

      /* Fecha os arquivos */
   FechaArquivo(streamReg, NOME_ARQ_BIN);
   FechaArquivo(streamArv, NOME_ARQ_ARV);

   printf("\n\t>>> Obrigado por usar este programa. Bye.\n\n");

   return 0;
}

/******************* Exemplo de Execução ******************

        >>> sizeof(long) = 8... OK

Iniciando arvore...

        >>> Arquivo de registros ja' existe. Deseja substitui-lo (s/n)? s

        >>> Digite o numero maximo de registros (0 = TODOS): 0

Criando arquivo...

        >>> Foram armazenados 9565483 registros no arquivo
        >>> "../MiniCensoMEC2012.bin"

        >>> Arvore ja existe. Deseja substitui-la (s/n)? s

Criando arvore...

        >>> Arvore criada com sucesso

        *** Tempo gasto na operacao: 902.00 segundos ***

Checando a consistencia do programa...

        >>>  Informacoes sobre a Arvore B+  <<<

        *** Parametros da arvore ***
                >>> G  = 340
                >>> TG = 170
                >>> F  = 254
                >>> TF = 127

        >>> Tamanho de um no' COM preenchimento: 4088 bytes
        >>> Tamanho de um no' SEM preenchimento: 4080 bytes
        >>> Espaco desperdicado por no': 8 bytes
        >>> Tamanho de um bloco: 4096 bytes
        >>> Espaco de bloco nao utilizado: 8 bytes


        >>> Numero maximo de filhos em no' interno: 340
        >>> Numero minimo de filhos em no' interno: 170
        >>> Numero maximo de chaves em folha: 254
        >>> Numero minimo de chaves em folha: 127

        >>> Numero de nos internos: 243
        >>> Numero de folhas: 53883
        >>> Numero de nos vazios: 0
        >>> Numero de nos desconhecidos: 0
        >>> Numero total de nos (inclui vazios): 54126
        >>> Numero total de chaves: 9565483

        >>> Profundidade da arvore (incluindo folhas): 3

        >>> Menor chave armazenada na arvore: 1
        >>> Maior chave armazenada na arvore: 21569970

...consistencia OK

        *** Tempo gasto na operacao:  0.00 segundos ***

>>> Escolha uma das opcoes a seguir:

        [1] Consulta dados de um aluno
        [2] Insere um aluno
        [3] Remove um aluno
        [4] Escreve as chaves em ordem
        [5] Busca de intervalo
        [6] Escreve nos da arvore B+ (depuracao)
        [7] Escreve registros (depuracao)
        [8] Encerra o programa

        >>> 1

        >>> Digite a chave (codigo aluno/curso): 1

        >>> Registro com a chave 1 <<<

Codigo do aluno: 400010594975
Codigo do curso: 1
Nome da IES: FACULDADE DE CINCIAS SOCIAIS APLICADAS DE BELO HORIZONTE
Nome do curso: PEDAGOGIA
Sexo do aluno: Masculino
Idade do aluno: 46
UF de nascimento: MINAS GERAIS
Ano de ingresso: 2010

>>> Escolha uma das opcoes a seguir:

        [1] Consulta dados de um aluno
        [2] Insere um aluno
        [3] Remove um aluno
        [4] Escreve as chaves em ordem
        [5] Busca de intervalo
        [6] Escreve nos da arvore B+ (depuracao)
        [7] Escreve registros (depuracao)
        [8] Encerra o programa

        >>> 6

        >>> Resultado escrito no arquivo: "NosArvoreBM01.txt"

>>> Escolha uma das opcoes a seguir:

        [1] Consulta dados de um aluno
        [2] Insere um aluno
        [3] Remove um aluno
        [4] Escreve as chaves em ordem
        [5] Busca de intervalo
        [6] Escreve nos da arvore B+ (depuracao)
        [7] Escreve registros (depuracao)
        [8] Encerra o programa

        >>> 3

        >>> Digite a chave (codigo aluno/curso): 21569970

        >>> Remocao bem sucedida

>>> Escolha uma das opcoes a seguir:

        [1] Consulta dados de um aluno
        [2] Insere um aluno
        [3] Remove um aluno
        [4] Escreve as chaves em ordem
        [5] Busca de intervalo
        [6] Escreve nos da arvore B+ (depuracao)
        [7] Escreve registros (depuracao)
        [8] Encerra o programa

        >>> 6

        >>> Resultado escrito no arquivo: "NosArvoreBM02.txt"

>>> Escolha uma das opcoes a seguir:

        [1] Consulta dados de um aluno
        [2] Insere um aluno
        [3] Remove um aluno
        [4] Escreve as chaves em ordem
        [5] Busca de intervalo
        [6] Escreve nos da arvore B+ (depuracao)
        [7] Escreve registros (depuracao)
        [8] Encerra o programa

        >>> 5

        >>> Digite a primeira chave: 15896466

        >>> Digite a segunda chave: 18806574

        >>> O resultado da busca foi armazenado no arquivo "BuscaIntervaloBM.txt"

>>> Escolha uma das opcoes a seguir:

        [1] Consulta dados de um aluno
        [2] Insere um aluno
        [3] Remove um aluno
        [4] Escreve as chaves em ordem
        [5] Busca de intervalo
        [6] Escreve nos da arvore B+ (depuracao)
        [7] Escreve registros (depuracao)
        [8] Encerra o programa

        >>> 8

        >>> Obrigado por usar este programa. Bye.

**********************************************************/

