/****
 *
 * Título do Programa: Tabela de Dispersão Extensível
 *                     (Versão alternativa)
 *
 * Seção: 8.2.6
 *
 * Autor: Ulysses de Oliveira
 *
 * Data de Criação: 25/08/2017
 * Última modificação: 28/08/2017
 *
 * Descrição Geral: Este programa demonstra várias operações
 *                  utilizando tabela de busca com dispersão
 *                  extensível armazenada em arquivo
 *
 * Entrada: Opções escolhidas através de menu
 *
 * Saída: Resultados das operações escolhidas pelo usuário
 *        (v. exemplo de execução do programa no final
 *        deste arquivo)
 *
 * Arquivos:
 *     ASSEGURA.h - macro de tratamento de exceção usadas pelo
 *                  programa
 *     DispersaoExt.h e - interface e implementação de operações
 *     DispersaoExt.c     operações sobre tabelas de dispersão
 *                        extensível
 *     Registros.h e - interface e implementação de operações
 *     Registros.c     sobre registros armazenados em arquivos
 *     main.c (este arquivo) - contém main() e outras funções
 *                             que testam as operações sobre
 *                             tabelas de dispersão extensível
 *
 * Arquivo de dados: "MiniCensoMEC2012.bin" (para testes) ou
 *                   "CensoMEC2012.bin"
 *
 ****/

/*********************** Includes *************************/

#include <stdio.h>        /* Funções de Entrada/Saída       */
#include <string.h>       /* Funções strXXX() e memXXX()    */
#include "DispersaoExt.h" /* Operações com tabela de busca  */
#include "Registros.h"    /* Processamento de registros     */
#include "Leitura.h"      /* Leitura via teclado            */
#include "ASSEGURA.h"     /* Macro ASSEGURA                 */

/***************** Constantes Simbólicas ******************/

   /* Nome do arquivo de texto contendo os registros */
#define NOME_ARQUIVO_REGS "../CensoMEC2012.txt"

   /* Nome do arquivo de registros */
#define NOME_ARQ_BIN "../CensoMEC2012.bin"

   /* Nome do arquivo contendo os registros indexados */
#define NOME_ARQ_TAB "../ColetoresExt.dat"

   /* Nome do arquivo contendo o diretório */
#define NOME_ARQ_DIR "Diretorio.dat"

   /* Nome inicial do arquivo de texto   */
   /* contendo as chaves em cada coletor */
#define NOME_ARQ_CHAVES_DEBUG "ChavesEmColetoresDebug"

   /* Nome inicial do arquivo de texto contendo o diretório */
#define NOME_ARQ_DIR_DEBUG "DiretorioDebug"

   /* Nome do arquivo de texto contendo registros em formato legível */
#define NOME_ARQ_DEBUG "RegistrosDebug.txt"

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
 * ConstroiTabela(): Lê arquivo de dados (binário) contendo os
 *                   registros e constrói outro arquivo contendo os
 *                   coletores de uma tabela de dispersão extensível
 *
 * Parâmetros:
 *     streamReg (entrada) - stream associado ao arquivo de
 *                           registros
 *     streamTab (entrada) - stream associado ao arquivo que
 *                           conterá os coletores
 *
 * Retorno: Endereço de uma estrutura contendo o diretório da
 *          tabela de dispersão extensível
 *
 ****/
tTabelaDExt *ConstroiTabela( tTabelaDExt *tabela, FILE *streamReg,
                             FILE *streamTab )
{
   tRegistroMEC registro;

   IniciaTabelaDExt(tabela, streamTab);

      /* Assegura que a leitura começa no início do arquivo */
   rewind(streamReg);

   while (1) {
         /* Lê um registro */
      fread(&registro, sizeof(registro), 1, streamReg);

         /* Se ocorreu erro de leitura ou o final  */
         /* do arquivo foi atingido encerra o laço */
      if (feof(streamReg) || ferror(streamReg))
         break;

      InsereDExt(tabela, &registro, streamTab);
   }

      /* Processamento terminado. Verifica se */
      /* ocorreu algum erro de leitura        */
   ASSEGURA(!ferror(streamReg), "Erro de leitura em ConstroiTabela");

   return tabela;
}

/****
 *
 * ChecaTabela(): Verifica se a tabela de busca já existe. Se ela
 *                já existir e o usuário desejar reconstrui-la ou
 *                se ela não existir, ela será construída.
 *
 * Parâmetros:
 *     nomeArqRegistros (entrada) - nome do arquivo de registros
 *     nomeArqTabela (entrada) - nome do arquivo que armazena a
 *                               tabela
 *     *streamRegistros (entrada/saída) - stream associado ao
 *                                        arquivo de registros
 *     *streamTabela (entrada/saída) - stream associado ao
 *                                     arquivo que armazena
 *                                     a tabela
 *
 * Retorno: Nada
 *
 ****/
void ChecaTabela( tTabelaDExt *tabela,
                  const char *nomeArqRegistros,
                  const char *nomeArqTabela,
                  FILE **streamRegistros, FILE **streamTabela )
{
   int op; /* Opção do usuário */

      /* Tenta abrir o arquivo de registros */
   *streamRegistros = AbreArquivo(nomeArqRegistros, "r+b");

      /* Verifica se o arquivo que contém a tabela já existe */
   if (ArquivoExiste(nomeArqTabela)) {
      /*                                */
      /* Tabela já existe. Se o usuário */
      /* desejar, ela será reconstruída */
      /*                                */

         /* Oferece a opção ao usuário */
      printf( "\n\n\t>>> Tabela ja existe. "
              "Deseja substitui-la (s/n)? " );
      op = LeOpcao("sSnN");

         /* Verifica se o usuário deseja reconstruir a tabela */
      if ( op == 'n' || op == 'N') { /* Ele não deseja isso   */
            /* Tenta abrir o arquivo que contém a tabela */
         *streamTabela = AbreArquivo(nomeArqTabela, "r+b");

         return; /* Serviço completo */
      }
   }

   /*                                        */
   /* Neste ponto, ou a tabela ainda não     */
   /* existe ou usuário deseja reconstrui-la */
   /*                                        */

      /* Cria o arquivo e checa se abertura foi OK */
   *streamTabela = AbreArquivo(nomeArqTabela, "w+b");

      /* Constrói a tabela */
   ConstroiTabela(tabela, *streamRegistros, *streamTabela);
}

/****
 *
 * GravaDiretorio(): Armazena o diretório de uma tabela de busca
 *                   extensível em arquivo
 *
 * Parâmetros:
 *     nomeArqDiretorio (entrada) - nome do arquivo que
 *                                  armazenará o diretório
 *     tabela (entrada) - ponteiro para a estrutura que contém o
 *                        diretório
 *
 * Retorno: Nada
 *
 ****/
void GravaDiretorio( const char *nomeArqDiretorio,
                     const tTabelaDExt *tabela )
{
   FILE *stream = AbreArquivo(nomeArqDiretorio, "wb");

   fwrite(&tabela->profGlobal, sizeof(int), 1, stream);
   fwrite(&tabela->tamDir, sizeof(int), 1, stream);
   fwrite(&tabela->nReg, sizeof(int), 1, stream);
   fwrite(tabela->dir, sizeof(int), tabela->tamDir, stream);

   ASSEGURA( !ferror(stream),
             "Erro de escrita em arquivo de diretorio" );

   FechaArquivo(stream, nomeArqDiretorio);
}

/****
 *
 * LeDiretorio(): Armazena o diretório de uma tabela de busca
 *                   extensível em arquivo
 *
 * Parâmetros:
 *     nomeArqDiretorio (entrada) - nome do arquivo que
 *                                  armazenará o diretório
 *     tabela (saída) - ponteiro para a estrutura que contém o
 *                      diretório
 *
 * Retorno: Nada
 *
 ****/
void LeDiretorio( const char *nomeArqDiretorio,
                  tTabelaDExt *tabela )
{
   FILE *stream = AbreArquivo(nomeArqDiretorio, "rb");

   fread(&tabela->profGlobal, sizeof(int), 1, stream);
   fread(&tabela->tamDir, sizeof(int), 1, stream);
   fread(&tabela->nReg, sizeof(int), 1, stream);
   fread(tabela->dir, sizeof(int), tabela->tamDir, stream);

   ASSEGURA( !ferror(stream),
             "Erro de leitura em arquivo de diretorio" );

   FechaArquivo(stream, nomeArqDiretorio);
}

/****
 *
 * InfoTabela(): Apresenta informações gerais sobre a tabela
 *
 * Parâmetros:
 *     arvore (entrada) - ponteiro para a raiz da árvore
 *    *streamTab (entrada) - stream associado ao arquivo que
 *                           contém os coletores
 *    *streamRegs (entrada) - stream associado ao arquivo que
 *                            contém os registros
 *
 * Retorno: Nada
 *
 ****/
void InfoTabela( const tTabelaDExt* tabela, FILE *streamTab,
                 FILE *streamRegs )
{
   int       nRegs;
   tRegistroMEC registro;

      /* Verifica se os streams são válidos */
   ASSEGURA( streamTab && streamRegs,
             "Um stream e' NULL em InfoTabela" );

   printf("\n\n\t\t>>>  Informacoes sobre coletores  <<<\n");

   printf("\n\t>>> Tamanho do bloco lido ou escrito: %d", TB);
   printf("\n\t>>> Tamanho de um registro: %d", TR);
   printf("\n\t>>> Tamanho de um campo do tipo int: %d", TI);
   printf("\n\t>>> Numero maximo de registros num coletor: %d", M);
   printf("\n\t>>> Tamanho de uma variavel que armazena um coletor: %d",
          (int) sizeof(tColetorDExt));
   printf( "\n\t>>> Espaco desperdicado em cada coletor: %d",
           (int) (TB - sizeof(tColetorDExt)) );

   printf("\n\n\t\t>>>  Informacoes sobre a tabela  <<<\n");

      /* Calcula o número de registros no arquivo original */
   nRegs = NumeroDeRegistros(streamRegs, sizeof(registro));

   printf("\n\t>>> Numero de registros no arquivo original: %d",
          nRegs);

      /* Calcula o número de registros na tabela */
   nRegs = NRegistrosDExt(tabela, streamTab);

   printf("\n\t>>> Numero de registros na tabela: %d", nRegs);

   printf( "\n\n\t>>> Tamanho do diretorio: %d",
           tabela->tamDir );

   printf( "\n\t>>> Profundidade global: %d",
           tabela->profGlobal );

   printf( "\n\n\t>>> Numero total de coletores: %d",
           NColetoresDExt(streamTab) );
   printf( "\n\t>>> Numero de coletores vazios: %d",
           NColetoresVaziosDExt(streamTab) );
   printf( "\n\t>>> Numero de coletores ocupados: %d",
           NColetoresOcupadosDExt(streamTab) );
   printf( "\n\t>>> Numero de coletores repletos: %d\n",
           NColetoresRepletosDExt(streamTab) );

   printf( "\n\t>>> Profundidade local minima: %d",
           ProfLocalMinDExt(streamTab) );
   printf( "\n\t>>> Profundidade local maxima: %d\n",
           ProfLocalMaxDExt(streamTab) );
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
 * EscreveChavesColetor(): Escreve num stream as chaves de um coletor
 *
 * Parâmetros:
 *     *coletor (entrada) - o coletor
 *      stream (entrada) - o stream no qual ocorrerá a escrita
 *
 * Retorno: Nada
 *
 ****/
void EscreveChavesColetor(const tColetorDExt *coletor, FILE *stream)
{
   int i;

   fprintf( stream, "\nChaves = {%ld",
            ObtemChave(coletor->registros) );

   for (i = 1; i < coletor->nRegistros; ++i)
      fprintf(stream, ", %ld", ObtemChave(coletor->registros + i));

   fprintf(stream, "}");
}

/****
 *
 * ExibeColetor(): Escreve o conteúdo de um coletor num stream
 *
 * Parâmetros:
 *      *coletor (entrada) - o coletor
 *      stream (entrada) - o stream no qual ocorrerá a escrita
 *
 * Retorno: Nada
 *
 ****/
void ExibeColetor( const tColetorDExt *coletor, FILE *stream )
{
   fprintf(stream, "\nNumero de chaves: %d", coletor->nRegistros);

   EscreveChavesColetor(coletor, stream);
}

/****
 *
 * CriaArquivoDeChavesEmColetores(): Apresenta as chaves em cada
 *                                   coletor de uma tabela de
 *                                   dispersão estática arquivo
 *                                   de texto
 *
 * Parâmetros:
 *      streamTab (entrada) - stream associado ao arquivo que
 *                            contém a tabela
 *      nomeIni (entrada) - nome inicial do arquivo que conterá as
 *                          informações
 *
 * Retorno: Nada
 *
 ****/
void CriaArquivoDeChavesEmColetores( FILE *streamTab,
                                     const char *nomeIni )
{
   static int ordem = 0;
   int        i = 0,
              nColetores = 0,
              nChaves = 0;
   tColetorDExt   coletor;
   char      *nomeArq;
   FILE      *streamCol; /* Stream de texto que contém o resultado */

      /* O limite de arquivos criados por seção é 99 */
   if (++ordem > 99) {
      printf("\n\t\a>>> Ja' criei arquivos demais <<<\n");
      return;
   }

      /* Aloca espaço para o nome do arquivo de nós: */
      /* 1 (terminal) + 2 (ordem) + 4 (".txt") = 7   */
   nomeArq = malloc(strlen(nomeIni) + 7);
   ASSEGURA( nomeArq,
             "Nao foi possivel criar nome de arquivo de coletores" );

      /* Cria o nome completo do arquivo */
   sprintf(nomeArq, "%s%.2d%s", nomeIni, ordem, ".txt");

      /* Tenta criar o aquivo */
   ASSEGURA( streamCol = fopen(nomeArq, "w"),
             "Nao foi possivel criar arquivo de coletores" );

      /* A leitura deve começar no início   */
      /* do arquivo que contém os coletores */
   rewind(streamTab);

      /* Lê cada coletor no arquivo que os contém e escreve */
      /* as informações sobre eles num arquivo de texto     */
   while (1) {
      fread(&coletor, sizeof(coletor), 1, streamTab);

      if (feof(streamTab) || ferror(streamTab))
         break;

      fprintf(streamCol, "%d.\n", i++);

      ExibeColetor(&coletor, streamCol);

      nChaves += coletor.nRegistros;

      ++nColetores;

      fprintf(streamCol, "\n\n----------\n\n");
   }

   fprintf( streamCol, ">>> Numero de coletores: %d", nColetores );
   fprintf( streamCol, "\n>>> Max chaves em coletor (M): %d",
            (int) M );

   fprintf( streamCol, "\n>>> Numero total de chaves: %d\n\n",
            nChaves );

   fclose(streamCol);

   printf("\n>> Resultado escrito no arquivo: \"%s\"\n", nomeArq);

   free(nomeArq);
}

/****
 *
 * CriaArquivoDeDiretorio(): Apresenta o diretório de uma tabela
 *                           de dispersão extensível num arquivo
 *                           de texto
 *
 * Parâmetros:
 *      tabela (entrada) - stream associado ao arquivo que
 *                            contém a tabela
 *      nomeIni (entrada) - nome inicial do arquivo que conterá as
 *                          informações
 *
 * Retorno: Nada
 *
 ****/
void CriaArquivoDeDiretorio(tTabelaDExt *tab, const char *nomeIni)
{
   static int ordem = 0;
   int        i;
   char      *nomeArq;
   FILE      *streamCol; /* Stream de texto que contém o resultado */

      /* O limite de arquivos criados por seção é 99 */
   if (++ordem > 99) {
      printf("\n\t\a>>> Ja' criei arquivos demais <<<\n");
      return;
   }

      /* Aloca espaço para o nome do arquivo de nós: */
      /* 1 (terminal) + 2 (ordem) + 4 (".txt") = 7   */
   nomeArq = malloc(strlen(nomeIni) + 7);
   ASSEGURA( nomeArq,
             "Nao foi possivel criar nome de arquivo de coletores" );

      /* Cria o nome completo do arquivo */
   sprintf(nomeArq, "%s%.2d%s", nomeIni, ordem, ".txt");

      /* Tenta criar o aquivo */
   ASSEGURA( streamCol = fopen(nomeArq, "w"),
             "Nao foi possivel criar arquivo de coletores" );

   fprintf( streamCol, ">>> Profundidade global: %d",
            tab->profGlobal );
   fprintf( streamCol, "\n>>> Tamanho do diretório: %d",
            tab->tamDir );
   fprintf( streamCol, "\n>>> Número de registros: %d\n",
            tab->nReg );
   fprintf(streamCol, "\n--------------------------------\n");

   fprintf(streamCol, "\n    *** Diretorio ***\n");

      /* Escreve os elementos do diretório */
   for (i = 0; i < tab->tamDir; ++i) {
      fprintf( streamCol, "\t%d ---> %ld\n", i, tab->dir[i] );
   }

   fclose(streamCol);

   printf("\n>> Resultado escrito no arquivo: \"%s\"\n", nomeArq);

   free(nomeArq);
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

/********************* Função main() **********************/

/****
 *
 * main(): Testa operações com tabela de dispersão extensível
 *
 * Parâmetros: nenhum
 *
 * Retorno: 0
 *
 ****/
int main(void)
{
   const char *opcoes[] = { "Consulta dados de um aluno",    /* 1 */
                            "Insere um aluno",               /* 2 */
                            "Remove um aluno",               /* 3 */
                            "Escreve coletores (depuracao)", /* 4 */
                            "Escreve diretorio (depuracao)", /* 5 */
                            "Escreve registros (depuracao)", /* 6 */
                            "Encerra o programa"             /* 7 */
                          };
   int          op,  /* Opção do usuário  */
                nOpcoes = sizeof(opcoes)/sizeof(opcoes[0]),
                sizeofLong = (int) sizeof(long);
   tChave       umaChave;  /* Uma chave de busca */
   FILE        *streamReg, /* Stream associado ao  */
                           /* arquivo de registros */
               *streamTab; /* Stream associado ao       */
                           /* arquivo contendo a tabela */
   tTabelaDExt  tabela;
   tRegistroMEC    registro,
               *pReg;

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

      /* Abre ou cria o arquivo de registros */
   streamReg = CriaArquivoRegistros(NOME_ARQUIVO_REGS, NOME_ARQ_BIN);

   MedidaDeTempo("Criando tabela...");

      /* Inicia a tabela B e obtém a posição da */
      /* raiz da tabela no arquivo que a contém */
//   ChecaTabela( NOME_ARQ_BIN, NOME_ARQ_TAB,
//                &streamReg, &streamTab );

         /* Tenta abrir o arquivo de registros */
   streamReg = AbreArquivo(NOME_ARQ_BIN, "r+b");

      /* Cria o arquivo e checa se abertura foi OK */
   streamTab = AbreArquivo(NOME_ARQ_TAB, "w+b");

      /* Constrói a tabela */
   ConstroiTabela(&tabela, streamReg, streamTab);

   MedidaDeTempo("Tabela criada com sucesso");

   InfoTabela(&tabela, streamTab, streamReg);

   while (1) { /* Laço principal do programa */
      ApresentaMenu(opcoes, nOpcoes);

      op = LeOpcao("1234567");

         /* Verifica se o usuário deseja sair do programa */
      if (op == '7')
         break; /* Encerra o laço */

      if (op <= '3') {
         printf("\n\t>>> Digite o codigo aluno/curso: ");
         umaChave = LeChave();
      }

      switch (op) {
         case '1': /* Procura o registro que */
                   /* possui uma dada chave  */

               /* Tenta encontrar o registro  */
               /* que contém a chave de busca */
            pReg = BuscaDExt(streamTab, &tabela, umaChave, &registro);

               /* Verifica se a chave foi encontrada */
            if (!pReg)
               printf("\n\t>>> A chave nao foi encontrada\n");
            else {
               printf( "\n\n\t>>> Registro com a chave %ld <<<\n\n",
                       umaChave );
               ExibeRegistro(&registro);
               putchar('\n'); /* Embelezamento */
               putchar('\n');
            }

            break;

         case '2': /* Insere um novo registro, se a */
                   /* chave dada ainda não existe   */
               /* Recebe do usuário os dados do registro */
            AtribuiChave(&registro, umaChave); /* Chave já foi lida */
            LeRegistro(&registro, 0);

            InsereDExt(&tabela, &registro, streamTab);

            printf("\n\t>>> Insercao bem-sucedida");

            break;

         case '3': /* Remove um registro */
            if (RemoveDExt(&tabela, umaChave, streamTab))
               printf( "\n\t>>> Remocao bem sucedida\n");
            else
               printf( "\n\t>>> A remocao MAL sucedida\n");

            break;

         case '4': /* Apresenta as chaves em cada coletor */
                   /* num arquivo de texto (depuração)    */
            CriaArquivoDeChavesEmColetores( streamTab,
                                            NOME_ARQ_CHAVES_DEBUG );

            break;

         case '5': /* Apresenta o diretório num    */
                   /* arquivo de texto (depuração) */
            CriaArquivoDeDiretorio(&tabela, NOME_ARQ_DIR_DEBUG);

            break;

         case '6': /* Apresenta os registros num arquivo */
                   /* de texto para depuração             */
            EscreveRegistrosDebug(streamReg, NOME_ARQ_DEBUG);
               printf( "\n\t>>> Os registros foram escritos no "
                       "arquivo: \"%s\"\n", NOME_ARQ_DEBUG );

            break;
      } /* switch */
   } /* while */

      /* Fecha o arquivo que contém os coletores */
   FechaArquivo(streamTab, NOME_ARQ_TAB);

      /* Fecha o arquivo que contém os registros */
   FechaArquivo(streamReg, NOME_ARQ_BIN);

      /* Armazena o diretório em arquivo */
   GravaDiretorio(NOME_ARQ_DIR, &tabela);

   printf("\nObrigado por usar este programa. Bye.\n\n");

   return 0;
}

/******************* Exemplo de Execução ******************

        >>> sizeof(long) = 8... OK

        >>> Digite o numero maximo de registros (0 = TODOS): 0

Criando arquivo...

        >>> Foram armazenados 9565483 registros no arquivo
        >>> "MiniCensoMEC2012.bin"

Criando tabela...

        >>> Tabela criada com sucesso

        *** Tempo gasto na operacao: 250.00 segundos ***


                >>>  Informacoes sobre coletores  <<<

        >>> Tamanho do bloco lido ou escrito: 4096
        >>> Tamanho de um registro: 16
        >>> Tamanho de um campo do tipo int: 4
        >>> Numero maximo de registros num coletor: 255
        >>> Tamanho de uma variavel que armazena um coletor: 4088
        >>> Espaco desperdicado em cada coletor: 8

                >>>  Informacoes sobre a tabela  <<<

        >>> Numero de registros no arquivo original: 9565483
        >>> Numero de registros na tabela: 9565483

        >>> Tamanho do diretorio: 65536
        >>> Profundidade global: 16

        >>> Numero total de coletores: 65536
        >>> Numero de coletores vazios: 0
        >>> Numero de coletores ocupados: 65536
        >>> Numero de coletores repletos: 0

        >>> Profundidade local minima: 16
        >>> Profundidade local maxima: 16

>>> Escolha uma das opcoes a seguir:

        [1] Consulta dados de um aluno
        [2] Insere um aluno
        [3] Remove um aluno
        [4] Encerra o programa

        >>>


**********************************************************/

