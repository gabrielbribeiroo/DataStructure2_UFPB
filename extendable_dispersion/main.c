/****
 *
 * T�tulo do Programa: Tabela de Dispers�o Extens�vel
 *                     (Vers�o alternativa)
 *
 * Se��o: 8.2.6
 *
 * Autor: Ulysses de Oliveira
 *
 * Data de Cria��o: 25/08/2017
 * �ltima modifica��o: 28/08/2017
 *
 * Descri��o Geral: Este programa demonstra v�rias opera��es
 *                  utilizando tabela de busca com dispers�o
 *                  extens�vel armazenada em arquivo
 *
 * Entrada: Op��es escolhidas atrav�s de menu
 *
 * Sa�da: Resultados das opera��es escolhidas pelo usu�rio
 *        (v. exemplo de execu��o do programa no final
 *        deste arquivo)
 *
 * Arquivos:
 *     ASSEGURA.h - macro de tratamento de exce��o usadas pelo
 *                  programa
 *     DispersaoExt.h e - interface e implementa��o de opera��es
 *     DispersaoExt.c     opera��es sobre tabelas de dispers�o
 *                        extens�vel
 *     Registros.h e - interface e implementa��o de opera��es
 *     Registros.c     sobre registros armazenados em arquivos
 *     main.c (este arquivo) - cont�m main() e outras fun��es
 *                             que testam as opera��es sobre
 *                             tabelas de dispers�o extens�vel
 *
 * Arquivo de dados: "MiniCensoMEC2012.bin" (para testes) ou
 *                   "CensoMEC2012.bin"
 *
 ****/

/*********************** Includes *************************/

#include <stdio.h>        /* Fun��es de Entrada/Sa�da       */
#include <string.h>       /* Fun��es strXXX() e memXXX()    */
#include "DispersaoExt.h" /* Opera��es com tabela de busca  */
#include "Registros.h"    /* Processamento de registros     */
#include "Leitura.h"      /* Leitura via teclado            */
#include "ASSEGURA.h"     /* Macro ASSEGURA                 */

/***************** Constantes Simb�licas ******************/

   /* Nome do arquivo de texto contendo os registros */
#define NOME_ARQUIVO_REGS "../CensoMEC2012.txt"

   /* Nome do arquivo de registros */
#define NOME_ARQ_BIN "../CensoMEC2012.bin"

   /* Nome do arquivo contendo os registros indexados */
#define NOME_ARQ_TAB "../ColetoresExt.dat"

   /* Nome do arquivo contendo o diret�rio */
#define NOME_ARQ_DIR "Diretorio.dat"

   /* Nome inicial do arquivo de texto   */
   /* contendo as chaves em cada coletor */
#define NOME_ARQ_CHAVES_DEBUG "ChavesEmColetoresDebug"

   /* Nome inicial do arquivo de texto contendo o diret�rio */
#define NOME_ARQ_DIR_DEBUG "DiretorioDebug"

   /* Nome do arquivo de texto contendo registros em formato leg�vel */
#define NOME_ARQ_DEBUG "RegistrosDebug.txt"

/**************** Compila��o Condidicional ****************/

#define DEPURACAO 0

/******************** Fun��es de Locais *******************/

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
 * ConstroiTabela(): L� arquivo de dados (bin�rio) contendo os
 *                   registros e constr�i outro arquivo contendo os
 *                   coletores de uma tabela de dispers�o extens�vel
 *
 * Par�metros:
 *     streamReg (entrada) - stream associado ao arquivo de
 *                           registros
 *     streamTab (entrada) - stream associado ao arquivo que
 *                           conter� os coletores
 *
 * Retorno: Endere�o de uma estrutura contendo o diret�rio da
 *          tabela de dispers�o extens�vel
 *
 ****/
tTabelaDExt *ConstroiTabela( tTabelaDExt *tabela, FILE *streamReg,
                             FILE *streamTab )
{
   tRegistroMEC registro;

   IniciaTabelaDExt(tabela, streamTab);

      /* Assegura que a leitura come�a no in�cio do arquivo */
   rewind(streamReg);

   while (1) {
         /* L� um registro */
      fread(&registro, sizeof(registro), 1, streamReg);

         /* Se ocorreu erro de leitura ou o final  */
         /* do arquivo foi atingido encerra o la�o */
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
 * ChecaTabela(): Verifica se a tabela de busca j� existe. Se ela
 *                j� existir e o usu�rio desejar reconstrui-la ou
 *                se ela n�o existir, ela ser� constru�da.
 *
 * Par�metros:
 *     nomeArqRegistros (entrada) - nome do arquivo de registros
 *     nomeArqTabela (entrada) - nome do arquivo que armazena a
 *                               tabela
 *     *streamRegistros (entrada/sa�da) - stream associado ao
 *                                        arquivo de registros
 *     *streamTabela (entrada/sa�da) - stream associado ao
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
   int op; /* Op��o do usu�rio */

      /* Tenta abrir o arquivo de registros */
   *streamRegistros = AbreArquivo(nomeArqRegistros, "r+b");

      /* Verifica se o arquivo que cont�m a tabela j� existe */
   if (ArquivoExiste(nomeArqTabela)) {
      /*                                */
      /* Tabela j� existe. Se o usu�rio */
      /* desejar, ela ser� reconstru�da */
      /*                                */

         /* Oferece a op��o ao usu�rio */
      printf( "\n\n\t>>> Tabela ja existe. "
              "Deseja substitui-la (s/n)? " );
      op = LeOpcao("sSnN");

         /* Verifica se o usu�rio deseja reconstruir a tabela */
      if ( op == 'n' || op == 'N') { /* Ele n�o deseja isso   */
            /* Tenta abrir o arquivo que cont�m a tabela */
         *streamTabela = AbreArquivo(nomeArqTabela, "r+b");

         return; /* Servi�o completo */
      }
   }

   /*                                        */
   /* Neste ponto, ou a tabela ainda n�o     */
   /* existe ou usu�rio deseja reconstrui-la */
   /*                                        */

      /* Cria o arquivo e checa se abertura foi OK */
   *streamTabela = AbreArquivo(nomeArqTabela, "w+b");

      /* Constr�i a tabela */
   ConstroiTabela(tabela, *streamRegistros, *streamTabela);
}

/****
 *
 * GravaDiretorio(): Armazena o diret�rio de uma tabela de busca
 *                   extens�vel em arquivo
 *
 * Par�metros:
 *     nomeArqDiretorio (entrada) - nome do arquivo que
 *                                  armazenar� o diret�rio
 *     tabela (entrada) - ponteiro para a estrutura que cont�m o
 *                        diret�rio
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
 * LeDiretorio(): Armazena o diret�rio de uma tabela de busca
 *                   extens�vel em arquivo
 *
 * Par�metros:
 *     nomeArqDiretorio (entrada) - nome do arquivo que
 *                                  armazenar� o diret�rio
 *     tabela (sa�da) - ponteiro para a estrutura que cont�m o
 *                      diret�rio
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
 * InfoTabela(): Apresenta informa��es gerais sobre a tabela
 *
 * Par�metros:
 *     arvore (entrada) - ponteiro para a raiz da �rvore
 *    *streamTab (entrada) - stream associado ao arquivo que
 *                           cont�m os coletores
 *    *streamRegs (entrada) - stream associado ao arquivo que
 *                            cont�m os registros
 *
 * Retorno: Nada
 *
 ****/
void InfoTabela( const tTabelaDExt* tabela, FILE *streamTab,
                 FILE *streamRegs )
{
   int       nRegs;
   tRegistroMEC registro;

      /* Verifica se os streams s�o v�lidos */
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

      /* Calcula o n�mero de registros no arquivo original */
   nRegs = NumeroDeRegistros(streamRegs, sizeof(registro));

   printf("\n\t>>> Numero de registros no arquivo original: %d",
          nRegs);

      /* Calcula o n�mero de registros na tabela */
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
 * CriaArquivoRegistros(): Verifica se o arquivo de registros j�
 *                         existe. Se ela j� existir e o usu�rio
 *                         desejar reconstrui-lo ou se ele n�o
 *                         existir, ele ser� constru�do.
 *
 * Par�metros:
 *     nomeArqTexto (entrada) - nome do arquivo texto de registros
 *     nomeArqBin (entrada) - nome do arquivo bin�rio de registros
 *
 * Retorno: Stream associado ao arquivo bin�rio de registros
 *
 ****/
FILE *CriaArquivoRegistros( const char *nomeArqTexto,
                            const char *nomeArqBin )
{
   int   op, /* Op��o do usu�rio */
         nRegistros, /* N�mero de registros no arquivo */
         maxRegistros; /* N�mero m�ximo de registros no arquivo */

      /* Verifica se o arquivo bin�rio de registros j� existe */
   if (ArquivoExiste(nomeArqBin)) {
      /*                                */
      /* Arquivo j� existe. Se o usu�rio */
      /* desejar, ele ser� reconstru�do */
      /*                                */

         /* Oferece a op��o ao usu�rio */
      op = LeSimNao( "Arquivo de registros ja' existe. "
                      "Deseja substitui-lo" );

         /* Verifica se o usu�rio deseja reconstruir o arquivo */
      if (op == 'n') /* Ele n�o deseja isso   */
         return AbreArquivo(nomeArqBin, "r+b");
   }

   /*                                        */
   /* Neste ponto, ou o arquivo ainda n�o    */
   /* existe ou usu�rio deseja reconstrui-lo */
   /*                                        */

   printf("\n\t>>> Digite o numero maximo de registros (0 = TODOS): ");
   maxRegistros = LeNatural();

   printf("\nCriando arquivo...\n");
   fflush(stdout);

      /* Cria o arquivo bin�rio */
   nRegistros = CriaArquivoBin( nomeArqTexto, nomeArqBin,
                                maxRegistros );

      /* Apresenta o resultado da convers�o */
   printf( "\n\t>>> Foram armazenados %d registros no arquivo"
           "\n\t>>> \"%s\"\n", nRegistros, nomeArqBin );

      /* Abre e retorna stream assoaciado ao arquivo bin�rio */
   return AbreArquivo(nomeArqBin, "r+b");
}

/****
 *
 * EscreveChavesColetor(): Escreve num stream as chaves de um coletor
 *
 * Par�metros:
 *     *coletor (entrada) - o coletor
 *      stream (entrada) - o stream no qual ocorrer� a escrita
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
 * ExibeColetor(): Escreve o conte�do de um coletor num stream
 *
 * Par�metros:
 *      *coletor (entrada) - o coletor
 *      stream (entrada) - o stream no qual ocorrer� a escrita
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
 *                                   dispers�o est�tica arquivo
 *                                   de texto
 *
 * Par�metros:
 *      streamTab (entrada) - stream associado ao arquivo que
 *                            cont�m a tabela
 *      nomeIni (entrada) - nome inicial do arquivo que conter� as
 *                          informa��es
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
   FILE      *streamCol; /* Stream de texto que cont�m o resultado */

      /* O limite de arquivos criados por se��o � 99 */
   if (++ordem > 99) {
      printf("\n\t\a>>> Ja' criei arquivos demais <<<\n");
      return;
   }

      /* Aloca espa�o para o nome do arquivo de n�s: */
      /* 1 (terminal) + 2 (ordem) + 4 (".txt") = 7   */
   nomeArq = malloc(strlen(nomeIni) + 7);
   ASSEGURA( nomeArq,
             "Nao foi possivel criar nome de arquivo de coletores" );

      /* Cria o nome completo do arquivo */
   sprintf(nomeArq, "%s%.2d%s", nomeIni, ordem, ".txt");

      /* Tenta criar o aquivo */
   ASSEGURA( streamCol = fopen(nomeArq, "w"),
             "Nao foi possivel criar arquivo de coletores" );

      /* A leitura deve come�ar no in�cio   */
      /* do arquivo que cont�m os coletores */
   rewind(streamTab);

      /* L� cada coletor no arquivo que os cont�m e escreve */
      /* as informa��es sobre eles num arquivo de texto     */
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
 * CriaArquivoDeDiretorio(): Apresenta o diret�rio de uma tabela
 *                           de dispers�o extens�vel num arquivo
 *                           de texto
 *
 * Par�metros:
 *      tabela (entrada) - stream associado ao arquivo que
 *                            cont�m a tabela
 *      nomeIni (entrada) - nome inicial do arquivo que conter� as
 *                          informa��es
 *
 * Retorno: Nada
 *
 ****/
void CriaArquivoDeDiretorio(tTabelaDExt *tab, const char *nomeIni)
{
   static int ordem = 0;
   int        i;
   char      *nomeArq;
   FILE      *streamCol; /* Stream de texto que cont�m o resultado */

      /* O limite de arquivos criados por se��o � 99 */
   if (++ordem > 99) {
      printf("\n\t\a>>> Ja' criei arquivos demais <<<\n");
      return;
   }

      /* Aloca espa�o para o nome do arquivo de n�s: */
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
   fprintf( streamCol, "\n>>> Tamanho do diret�rio: %d",
            tab->tamDir );
   fprintf( streamCol, "\n>>> N�mero de registros: %d\n",
            tab->nReg );
   fprintf(streamCol, "\n--------------------------------\n");

   fprintf(streamCol, "\n    *** Diretorio ***\n");

      /* Escreve os elementos do diret�rio */
   for (i = 0; i < tab->tamDir; ++i) {
      fprintf( streamCol, "\t%d ---> %ld\n", i, tab->dir[i] );
   }

   fclose(streamCol);

   printf("\n>> Resultado escrito no arquivo: \"%s\"\n", nomeArq);

   free(nomeArq);
}

/****
 *
 * EscreveRegistrosDebug(): Escreve o conte�do de um arquivo
 *                          bin�rio de registros num arquivo de
 *                          texto de modo leg�vel
 *
 * Par�metros: Nenhum
 *
 * Retorno: Nada
 *
 ****/
void EscreveRegistrosDebug(FILE *streamReg, const char *nomeArqTxt)
{
   int     i = 0, /* �ndice do registro no arquivo */
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

/********************* Fun��o main() **********************/

/****
 *
 * main(): Testa opera��es com tabela de dispers�o extens�vel
 *
 * Par�metros: nenhum
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
   int          op,  /* Op��o do usu�rio  */
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

      /* Inicia a tabela B e obt�m a posi��o da */
      /* raiz da tabela no arquivo que a cont�m */
//   ChecaTabela( NOME_ARQ_BIN, NOME_ARQ_TAB,
//                &streamReg, &streamTab );

         /* Tenta abrir o arquivo de registros */
   streamReg = AbreArquivo(NOME_ARQ_BIN, "r+b");

      /* Cria o arquivo e checa se abertura foi OK */
   streamTab = AbreArquivo(NOME_ARQ_TAB, "w+b");

      /* Constr�i a tabela */
   ConstroiTabela(&tabela, streamReg, streamTab);

   MedidaDeTempo("Tabela criada com sucesso");

   InfoTabela(&tabela, streamTab, streamReg);

   while (1) { /* La�o principal do programa */
      ApresentaMenu(opcoes, nOpcoes);

      op = LeOpcao("1234567");

         /* Verifica se o usu�rio deseja sair do programa */
      if (op == '7')
         break; /* Encerra o la�o */

      if (op <= '3') {
         printf("\n\t>>> Digite o codigo aluno/curso: ");
         umaChave = LeChave();
      }

      switch (op) {
         case '1': /* Procura o registro que */
                   /* possui uma dada chave  */

               /* Tenta encontrar o registro  */
               /* que cont�m a chave de busca */
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
                   /* chave dada ainda n�o existe   */
               /* Recebe do usu�rio os dados do registro */
            AtribuiChave(&registro, umaChave); /* Chave j� foi lida */
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
                   /* num arquivo de texto (depura��o)    */
            CriaArquivoDeChavesEmColetores( streamTab,
                                            NOME_ARQ_CHAVES_DEBUG );

            break;

         case '5': /* Apresenta o diret�rio num    */
                   /* arquivo de texto (depura��o) */
            CriaArquivoDeDiretorio(&tabela, NOME_ARQ_DIR_DEBUG);

            break;

         case '6': /* Apresenta os registros num arquivo */
                   /* de texto para depura��o             */
            EscreveRegistrosDebug(streamReg, NOME_ARQ_DEBUG);
               printf( "\n\t>>> Os registros foram escritos no "
                       "arquivo: \"%s\"\n", NOME_ARQ_DEBUG );

            break;
      } /* switch */
   } /* while */

      /* Fecha o arquivo que cont�m os coletores */
   FechaArquivo(streamTab, NOME_ARQ_TAB);

      /* Fecha o arquivo que cont�m os registros */
   FechaArquivo(streamReg, NOME_ARQ_BIN);

      /* Armazena o diret�rio em arquivo */
   GravaDiretorio(NOME_ARQ_DIR, &tabela);

   printf("\nObrigado por usar este programa. Bye.\n\n");

   return 0;
}

/******************* Exemplo de Execu��o ******************

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

