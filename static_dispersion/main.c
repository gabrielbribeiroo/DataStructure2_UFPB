/****
 *
 * Título do Programa: Dispersão Estática em Memória Secundária
 *
 * Seção: 8.1.3
 *
 * Autor: Ulysses de Oliveira
 *
 * Data de Criação: 25/08/2017
 * Última modificação: 28/08/2017
 *
 * Descrição Geral: Este programa demonstra várias operações
 *                  utilizando tabela de busca com dispersão
 *                  estática armazenada em arquivo
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
 *     DispersaoExt.c      sobre tabelas de dispersão estática
 *     Registros.h e - interface e implementação de operações
 *     Registros.c     sobre registros armazenados em arquivos
 *     main.c (este arquivo) - contém main() e outras funções
 *                             que testam as operações sobre
 *                             tabelas de dispersão estática
 *
 * Arquivo de dados: "CensoMEC2012.bin" (binário) e
 *                   "CensoMEC2012.txt" (texto)
 *
 ****/

/*********************** Includes *************************/

#include <stdio.h>        /* Funções de Entrada/Saída       */
#include <string.h>       /* Funções strXXX() e memXXX()    */
#include "DispersaoEstatica.h" /* Operações com tabela de busca */
#include "Registros.h"    /* Processamento de registros         */
#include "Leitura.h"      /* Leitura via teclado */
#include "ASSEGURA.h"     /* Macro ASSEGURA      */

/***************** Constantes Simbólicas ******************/

   /* Nome do arquivo de texto contendo os registros */
#define NOME_ARQUIVO_REGS "../CensoMEC2012.txt"

   /* Nome do arquivo binário de registros */
#define NOME_ARQ_BIN "../CensoMEC2012.bin"

   /* Nome do arquivo contendo os registros indexados */
#define NOME_ARQ_TAB "../Coletores.dat"

   /* Nome inicial do arquivo de texto   */
   /* contendo as chaves em cada coletor */
#define NOME_ARQ_CHAVES_DEBUG "ChavesEmColetoresDebug"

   /* Nome do arquivo de texto contendo registros em formato legível */
#define NOME_ARQ_DEBUG   "RegistrosDebug.txt"

/***************** Compilação Condicional *****************/

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
 * ConstroiTabela(): Lê arquivo de dados (binário) contendo
 *                   os registros e constrói outro arquivo
 *                   contendo os coletores de uma tabela de
 *                   dispersão estática
 *
 * Parâmetros:
 *     streamReg (entrada) - stream associado ao arquivo de
 *                           registros
 *     streamTab (entrada) - stream associado ao arquivo que
 *                           conterá os coletores
 *
 * Retorno: O tamanho da tabela de dispersão estática
 *
 ****/
int ConstroiTabela( FILE *streamReg, FILE *streamTab )
{
   tRegistroMEC registro;
   int       nRegistros, /* Número de registros */
             tamTabela; /* Número de coletores primários */

      /* Obtém o número de registros */
   nRegistros = NumeroDeRegistros(streamReg, sizeof(tRegistroMEC));

      /* Inicia a tabela e obtém seu tamanho */
   tamTabela = IniciaTabDEst(streamTab, nRegistros, M);

      /* Assegura que a leitura começa no início do arquivo */
   rewind(streamReg);

      /* Lê cada registro do arquivo e insere-o na tabela */
   while (1) {
         /* Lê um registro */
      fread(&registro, sizeof(registro), 1, streamReg);

         /* Se ocorreu erro de leitura ou o final  */
         /* do arquivo foi atingido encerra o laço */
      if (feof(streamReg) || ferror(streamReg))
         break;

         /* Insere o registro na tabela */
      InsereDEst(tamTabela, &registro, streamTab);
   }

      /* Processamento terminado. Verifica se */
      /* ocorreu algum erro de leitura        */
   ASSEGURA( !ferror(streamReg),
             "Erro de leitura em ConstroiTabela" );

   return tamTabela;
}

/****
 *
 * InfoTabela(): Apresenta informações gerais sobre a tabela
 *
 * Parâmetros:
 *     tamTabela (entrada) - tamanho da tabela (i.e., o número de
 *                           coletores primários)
 *    *streamTab (entrada) - stream associado ao arquivo que
 *                           contém os coletores
 *    *streamRegs (entrada) - stream associado ao arquivo que
 *                            contém os registros
 *
 * Retorno: Nada
 *
 ****/
void InfoTabela( int tamTabela, FILE *streamTab,
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
          (int) sizeof(tColetorDEst));
   printf( "\n\t>>> Espaco desperdicado em cada coletor: %d",
           TB - (int) sizeof(tColetorDEst) );

   printf("\n\n\t\t>>>  Informacoes sobre a tabela  <<<\n");

      /* Calcula o número de registros no arquivo original */
   nRegs = NumeroDeRegistros(streamRegs, sizeof(registro));

   printf("\n\t>>> Numero de registros no arquivo original: %d",
          nRegs);

      /* Calcula o número de registros na tabela */
   nRegs = NRegistrosTabDEst(streamTab);

   printf("\n\t>>> Numero de registros na tabela: %d", nRegs);

   printf( "\n\n\t>>> Numero total de coletores: %d",
           NColetoresDEst(streamTab) );
   printf( "\n\t>>> Numero de coletores vazios: %d",
           NColetoresVaziosDEst(streamTab) );
   printf( "\n\t>>> Numero de coletores ocupados: %d",
           NColetoresOcupadosDEst(streamTab) );
   printf( "\n\t>>> Numero de coletores repletos: %d",
           NColetoresRepletosDEst(streamTab) );
   printf( "\n\t>>> Numero de coletores excedentes: %d",
           NColetoresExcedentesDEst(tamTabela, streamTab) );
   printf( "\n\t>>> Tamanho da maior cadeia de coletores "
           "excedentes: %d\n",
           MaiorCadeiaExcedentesDEst(tamTabela, streamTab) );
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
void EscreveChavesColetor(const tColetorDEst *coletor, FILE *stream)
{
   int i;

   fprintf(stream, "\nChaves = {%ld", coletor->registros[0].chave);

   for (i = 1; i < coletor->nRegistros; ++i)
      fprintf(stream, ", %ld", coletor->registros[i].chave);

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
void ExibeColetor( const tColetorDEst *coletor, FILE *stream )
{
   ASSEGURA( coletor->tipo == PRIMARIO || coletor->tipo == EXCEDENTE,
             "\nTipo de coletor desconhecido\n" );

   if (coletor->tipo == PRIMARIO)
      fprintf(stream, "Tipo de coletor: Primario");
   else
      fprintf(stream, "Tipo de coletor: Excedente");

   fprintf(stream, "\nNumero de chaves: %d", coletor->nRegistros);

   EscreveChavesColetor(coletor, stream);

   if (coletor->proximo == POSICAO_NULA)
      fprintf(stream, "\nNao ha' proximo coletor");
   else
      fprintf ( stream, "\nProximo coletor excedente: %d",
                coletor->proximo );
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
              nChaves = 0,
              nColetoresPrimarios = 0,
              nColetoresExcedentes = 0,
              comExcedentes = 0,
              semExcedentes = 0;
   tColetorDEst   coletor;
   char      *nomeArq;
   FILE      *streamCol; /* */

      /* O limite de arquivos criados por seção é 99 */
   if (++ordem > 99) {
      printf("\n\t\a>>> Ja' criei arquivos demais <<<\n");
      return;
   }

      /* Aloca espaço para o nome do arquivo de nós: */
      /* 1 (terminal) + 2 (ordem) + 4 (".txt") = 7   */
   nomeArq = malloc(strlen(nomeIni) + 7);
   ASSEGURA( nomeArq,
             "Nao foi possivel criar nome de arquivo de nos" );

      /* Cria o nome completo do arquivo */
   sprintf(nomeArq, "%s%.2d%s", nomeIni, ordem, ".txt");

      /* Tenta criar o aquivo */
   ASSEGURA( streamCol = fopen(nomeArq, "w"),
             "Nao foi possivel criar arquivo de nos" );

      /* A leitura deve começar no início   */
      /* do arquivo que contém os coletores */
   rewind(streamTab);

      /* Lê cada coletor no arquivo que contém a tabela e       */
      /* escreve as informações sobre eles num arquivo de texto */
   while (1) {
      fread(&coletor, sizeof(coletor), 1, streamTab);

      if (feof(streamTab) || ferror(streamTab))
         break;

      ASSEGURA ( coletor.tipo == PRIMARIO ||
                 coletor.tipo == EXCEDENTE,
                 "Tipo de coletor desconhecido" )

      fprintf(streamCol, "%d.\n\n", i++);

      ExibeColetor(&coletor, streamCol);

      nChaves += coletor.nRegistros;

      if (coletor.tipo == PRIMARIO) {
         ++nColetoresPrimarios;

         if (coletor.proximo != POSICAO_NULA)
            ++comExcedentes;
         else
            ++semExcedentes;

      } else
         ++nColetoresExcedentes;

      fprintf(streamCol, "\n\n----------\n\n");
   }

   fprintf( streamCol, ">>> Tamanho da tabela: %d",
            nColetoresPrimarios );
   fprintf( streamCol, "\n>>> Max chaves em coletor (M): %d",
            (int) M );

   fprintf( streamCol, "\n>>> Numero de coletores primarios: %d",
            nColetoresPrimarios );
   fprintf( streamCol, "\n>>> Numero de coletores excedentes: %d",
            nColetoresExcedentes );
   fprintf( streamCol, "\n>>> Coletores primarios com excedentes: %d",
            comExcedentes );
   fprintf( streamCol, "\n>>> Coletores primarios sem excedentes: %d",
            semExcedentes );
   fprintf( streamCol, "\n>>> Numero total de coletores: %d",
            nColetoresPrimarios + nColetoresExcedentes );

   fprintf( streamCol, "\n>>> Numero total de chaves: %d\n\n",
            nChaves );

   fclose(streamCol);

   printf( "\n>> Resultado escrito no arquivo: \"%s\"\n",
           nomeArq );

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
 * main(): Testa operações com tabela de dispersão estática
 *
 * Parâmetros: nenhum
 *
 * Retorno: 0
 *
 ****/
int main(void)
{
   tRegistroMEC   umRegistro, *pRegistro;
   const char *opcoes[] = { "Consulta dados de um aluno",    /* 1 */
                            "Insere um aluno",               /* 2 */
                            "Remove um aluno",               /* 3 */
                            "Escreve coletores (depuracao)", /* 4 */
                            "Escreve registros (depuracao)", /* 5 */
                            "Encerra o programa"             /* 6 */
                          };
   int          op,  /* Opção do usuário  */
                nOpcoes = sizeof(opcoes)/sizeof(opcoes[0]),
                sizeofLong = (int) sizeof(long),
                tamTabela; /* Número de coletores primários */
   tChave       umaChave;  /* Uma chave de busca */
   FILE        *streamReg, /* Stream associado ao  */
                           /* arquivo de registros */
               *streamTab; /* Stream associado ao       */
                           /* arquivo contendo a tabela */

   printf("\n\t>>> sizeof(long) = %d", sizeofLong);

   if (sizeofLong >= 8)
      printf("... OK\n");
   else {
      printf("\n\t>>> Este programa pode nao funcionar adequadamente "
             "\n\t>>> quando sizeof(long) e' menor do que 8");
      op = LeSimNao("Deseja prosseguir");

      if (op == 'n') {
         printf("\n\t>>> Adquira um compilador decente e recompile "
                "\n\t>>> o programa. Bye.");
         return 1;
      }
   }

   MedidaDeTempo("Criando arquivo binario...");

      /* Abre ou cria o arquivo de registros */
   streamReg = CriaArquivoRegistros(NOME_ARQUIVO_REGS, NOME_ARQ_BIN);

   MedidaDeTempo("arquivo binario criado com sucesso");

   MedidaDeTempo("Criando tabela...");

      /* Cria o arquivo que conterá a tabela */
      /* e checa se abertura foi OK          */
   streamTab = AbreArquivo(NOME_ARQ_TAB, "w+b");

      /* Constrói a tabela */
   tamTabela = ConstroiTabela(streamReg, streamTab);

   MedidaDeTempo("Tabela criada com sucesso");

   InfoTabela(tamTabela, streamTab, streamReg);

   while (1) { /* Laço principal do programa */
      ApresentaMenu(opcoes, nOpcoes);

      op = LeOpcao("123456");

         /* Verifica se o usuário deseja sair do programa */
      if (op == '6')
         break; /* Encerra o laço */

      if (op <= '3') { /* Estas opções precisam de uma chave */
         printf("\n\t>>> Digite a chave (codigo aluno/curso): ");
         umaChave = LeNatural();
      }

      switch (op) {
         case '1': /* Procura o registro que */
                   /* possui uma dada chave  */

               /* Tenta encontrar o registro  */
               /* que contém a chave de busca */
            pRegistro = BuscaDEst( streamTab, tamTabela,
                                       umaChave, &umRegistro );

               /* Verifica se o registro foi encontrado */
            if (!pRegistro)
               printf("\n\t>>> A chave nao foi encontrada\n");
            else {
               printf( "\n\n\t>>> Registro com a chave %ld <<<\n",
                       umaChave );
               ExibeRegistro(&umRegistro);
               putchar('\n'); /* Embelezamento */
               putchar('\n');
            }

            break;

         case '2': /* Insere um novo registro, se a */
                   /* chave dada ainda não existe   */
               /* Recebe do usuário os dados */
               /* do registro a ser inserido */
            LeRegistro(&umRegistro);

            InsereDEst(tamTabela, &umRegistro, streamTab);

            printf("\n\t>>> Insercao bem-sucedida\n");

            break;

         case '3': /* Remove um registro */
            if (RemoveDEst( tamTabela, umaChave, streamTab))
               printf( "\n\t>>> Remocao bem-sucedida\n");
            else
               printf( "\n\t>>> A remocao malsucedida\n");

            break;

         case '4': /* Apresenta as chaves em cada coletor */
                   /* num arquivo de texto (depuração)    */
            CriaArquivoDeChavesEmColetores( streamTab,
                                            NOME_ARQ_CHAVES_DEBUG );

            break;

         case '5': /* Apresenta os registros num arquivo */
                   /* de texto para depuração             */
            EscreveRegistrosDebug(streamReg, NOME_ARQ_DEBUG);
               printf( "\n\t>>> Os registros foram escritos no "
                       "arquivo: \"%s\"\n", NOME_ARQ_DEBUG );

            break;
      } /* switch */
   } /* while */

      /* Fecha o arquivo que contém os coletores */
   FechaArquivo(streamTab, NOME_ARQ_TAB);

      /* Fecha o arquivo de registros */
   FechaArquivo(streamReg, NOME_ARQ_BIN);

   printf("\nBye.\n\n");

   return 0;
}

/******************* Exemplo de Execução ******************



**********************************************************/

