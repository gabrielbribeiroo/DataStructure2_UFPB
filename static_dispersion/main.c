/****
 *
 * T�tulo do Programa: Dispers�o Est�tica em Mem�ria Secund�ria
 *
 * Se��o: 8.1.3
 *
 * Autor: Ulysses de Oliveira
 *
 * Data de Cria��o: 25/08/2017
 * �ltima modifica��o: 28/08/2017
 *
 * Descri��o Geral: Este programa demonstra v�rias opera��es
 *                  utilizando tabela de busca com dispers�o
 *                  est�tica armazenada em arquivo
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
 *     DispersaoExt.c      sobre tabelas de dispers�o est�tica
 *     Registros.h e - interface e implementa��o de opera��es
 *     Registros.c     sobre registros armazenados em arquivos
 *     main.c (este arquivo) - cont�m main() e outras fun��es
 *                             que testam as opera��es sobre
 *                             tabelas de dispers�o est�tica
 *
 * Arquivo de dados: "CensoMEC2012.bin" (bin�rio) e
 *                   "CensoMEC2012.txt" (texto)
 *
 ****/

/*********************** Includes *************************/

#include <stdio.h>        /* Fun��es de Entrada/Sa�da       */
#include <string.h>       /* Fun��es strXXX() e memXXX()    */
#include "DispersaoEstatica.h" /* Opera��es com tabela de busca */
#include "Registros.h"    /* Processamento de registros         */
#include "Leitura.h"      /* Leitura via teclado */
#include "ASSEGURA.h"     /* Macro ASSEGURA      */

/***************** Constantes Simb�licas ******************/

   /* Nome do arquivo de texto contendo os registros */
#define NOME_ARQUIVO_REGS "../CensoMEC2012.txt"

   /* Nome do arquivo bin�rio de registros */
#define NOME_ARQ_BIN "../CensoMEC2012.bin"

   /* Nome do arquivo contendo os registros indexados */
#define NOME_ARQ_TAB "../Coletores.dat"

   /* Nome inicial do arquivo de texto   */
   /* contendo as chaves em cada coletor */
#define NOME_ARQ_CHAVES_DEBUG "ChavesEmColetoresDebug"

   /* Nome do arquivo de texto contendo registros em formato leg�vel */
#define NOME_ARQ_DEBUG   "RegistrosDebug.txt"

/***************** Compila��o Condicional *****************/

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
 * ConstroiTabela(): L� arquivo de dados (bin�rio) contendo
 *                   os registros e constr�i outro arquivo
 *                   contendo os coletores de uma tabela de
 *                   dispers�o est�tica
 *
 * Par�metros:
 *     streamReg (entrada) - stream associado ao arquivo de
 *                           registros
 *     streamTab (entrada) - stream associado ao arquivo que
 *                           conter� os coletores
 *
 * Retorno: O tamanho da tabela de dispers�o est�tica
 *
 ****/
int ConstroiTabela( FILE *streamReg, FILE *streamTab )
{
   tRegistroMEC registro;
   int       nRegistros, /* N�mero de registros */
             tamTabela; /* N�mero de coletores prim�rios */

      /* Obt�m o n�mero de registros */
   nRegistros = NumeroDeRegistros(streamReg, sizeof(tRegistroMEC));

      /* Inicia a tabela e obt�m seu tamanho */
   tamTabela = IniciaTabDEst(streamTab, nRegistros, M);

      /* Assegura que a leitura come�a no in�cio do arquivo */
   rewind(streamReg);

      /* L� cada registro do arquivo e insere-o na tabela */
   while (1) {
         /* L� um registro */
      fread(&registro, sizeof(registro), 1, streamReg);

         /* Se ocorreu erro de leitura ou o final  */
         /* do arquivo foi atingido encerra o la�o */
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
 * InfoTabela(): Apresenta informa��es gerais sobre a tabela
 *
 * Par�metros:
 *     tamTabela (entrada) - tamanho da tabela (i.e., o n�mero de
 *                           coletores prim�rios)
 *    *streamTab (entrada) - stream associado ao arquivo que
 *                           cont�m os coletores
 *    *streamRegs (entrada) - stream associado ao arquivo que
 *                            cont�m os registros
 *
 * Retorno: Nada
 *
 ****/
void InfoTabela( int tamTabela, FILE *streamTab,
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
          (int) sizeof(tColetorDEst));
   printf( "\n\t>>> Espaco desperdicado em cada coletor: %d",
           TB - (int) sizeof(tColetorDEst) );

   printf("\n\n\t\t>>>  Informacoes sobre a tabela  <<<\n");

      /* Calcula o n�mero de registros no arquivo original */
   nRegs = NumeroDeRegistros(streamRegs, sizeof(registro));

   printf("\n\t>>> Numero de registros no arquivo original: %d",
          nRegs);

      /* Calcula o n�mero de registros na tabela */
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
 * ExibeColetor(): Escreve o conte�do de um coletor num stream
 *
 * Par�metros:
 *      *coletor (entrada) - o coletor
 *      stream (entrada) - o stream no qual ocorrer� a escrita
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
              nChaves = 0,
              nColetoresPrimarios = 0,
              nColetoresExcedentes = 0,
              comExcedentes = 0,
              semExcedentes = 0;
   tColetorDEst   coletor;
   char      *nomeArq;
   FILE      *streamCol; /* */

      /* O limite de arquivos criados por se��o � 99 */
   if (++ordem > 99) {
      printf("\n\t\a>>> Ja' criei arquivos demais <<<\n");
      return;
   }

      /* Aloca espa�o para o nome do arquivo de n�s: */
      /* 1 (terminal) + 2 (ordem) + 4 (".txt") = 7   */
   nomeArq = malloc(strlen(nomeIni) + 7);
   ASSEGURA( nomeArq,
             "Nao foi possivel criar nome de arquivo de nos" );

      /* Cria o nome completo do arquivo */
   sprintf(nomeArq, "%s%.2d%s", nomeIni, ordem, ".txt");

      /* Tenta criar o aquivo */
   ASSEGURA( streamCol = fopen(nomeArq, "w"),
             "Nao foi possivel criar arquivo de nos" );

      /* A leitura deve come�ar no in�cio   */
      /* do arquivo que cont�m os coletores */
   rewind(streamTab);

      /* L� cada coletor no arquivo que cont�m a tabela e       */
      /* escreve as informa��es sobre eles num arquivo de texto */
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
 * main(): Testa opera��es com tabela de dispers�o est�tica
 *
 * Par�metros: nenhum
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
   int          op,  /* Op��o do usu�rio  */
                nOpcoes = sizeof(opcoes)/sizeof(opcoes[0]),
                sizeofLong = (int) sizeof(long),
                tamTabela; /* N�mero de coletores prim�rios */
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

      /* Cria o arquivo que conter� a tabela */
      /* e checa se abertura foi OK          */
   streamTab = AbreArquivo(NOME_ARQ_TAB, "w+b");

      /* Constr�i a tabela */
   tamTabela = ConstroiTabela(streamReg, streamTab);

   MedidaDeTempo("Tabela criada com sucesso");

   InfoTabela(tamTabela, streamTab, streamReg);

   while (1) { /* La�o principal do programa */
      ApresentaMenu(opcoes, nOpcoes);

      op = LeOpcao("123456");

         /* Verifica se o usu�rio deseja sair do programa */
      if (op == '6')
         break; /* Encerra o la�o */

      if (op <= '3') { /* Estas op��es precisam de uma chave */
         printf("\n\t>>> Digite a chave (codigo aluno/curso): ");
         umaChave = LeNatural();
      }

      switch (op) {
         case '1': /* Procura o registro que */
                   /* possui uma dada chave  */

               /* Tenta encontrar o registro  */
               /* que cont�m a chave de busca */
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
                   /* chave dada ainda n�o existe   */
               /* Recebe do usu�rio os dados */
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
                   /* num arquivo de texto (depura��o)    */
            CriaArquivoDeChavesEmColetores( streamTab,
                                            NOME_ARQ_CHAVES_DEBUG );

            break;

         case '5': /* Apresenta os registros num arquivo */
                   /* de texto para depura��o             */
            EscreveRegistrosDebug(streamReg, NOME_ARQ_DEBUG);
               printf( "\n\t>>> Os registros foram escritos no "
                       "arquivo: \"%s\"\n", NOME_ARQ_DEBUG );

            break;
      } /* switch */
   } /* while */

      /* Fecha o arquivo que cont�m os coletores */
   FechaArquivo(streamTab, NOME_ARQ_TAB);

      /* Fecha o arquivo de registros */
   FechaArquivo(streamReg, NOME_ARQ_BIN);

   printf("\nBye.\n\n");

   return 0;
}

/******************* Exemplo de Execu��o ******************



**********************************************************/

