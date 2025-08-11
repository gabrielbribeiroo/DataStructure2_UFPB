/****
 *
 * T�tulo do Programa: Busca Multidirecional em Mem�ria Secund�ria
 *
 * Autor: Ulysses de Oliveira
 *
 * Data de Cria��o: 25/04/2004
 * �ltima modifica��o: 21/07/2008
 *
 * Descri��o Geral: Este programa demonstra v�rias
 *                  opera��es utilizando �rvores de busca
 *                  multidirecionais armazenadas em arquivo
 *
 * Entrada: Op��es escolhidas atrav�s de menu
 *
 * Sa�da: Resultados das opera��es escolhidas pelo usu�rio
 *        (v. exemplo de execu��o do programa no final
 *        deste arquivo)
 *
 * Arquivos:
 *     ASSERT.h - macros de tratamento de exce��o usadas pelo
 *                programa
 *     ArvoreMulti2.h e - interface e implementa��o de opera��es
 *     ArvoreMulti2.c     opera��es sobre �rvores multidirecionais
 *                        armazenadas em arquivo
 *     Registros.h e - interface e implementa��o de opera��es
 *     Registros.c     sobre registros armazenados em arquivos
 *     main.c (este arquivo) - cont�m main() e outras fun��es
 *                             que testam as opera��es sobre
 *                             �rvores de busca multidirecionais
 *                             armazenadas em arquivo
 *
 ****/

/*********************** Includes *************************/

#include <stdio.h>       /* Fun��es de Entrada/Sa�da    */
#include <string.h>      /* Fun��es strXXX() e memXXX() */
#include <time.h>        /* Medi��o de tempo            */
#include "ArvoreMulti2.h" /* Fun��es que lidam com �rvores */
                          /* multimensionais               */
#include "Registros.h"   /* Fun��es de processamento */
                         /* de registros             */
#include "leitura.h"     /* Fun��es de leitura via teclado */
#include "ASSEGURA.h"    /* Macro ASSEGURA e outras macros */
                         /* de tratamento de exce��o       */

/***************** Constantes Simb�licas ******************/

   /* Nome do arquivo de registros */
//#define NOME_ARQ_REG "I:\\Dados\\CensoMEC\\CensoMEC2012.bin"
#define NOME_ARQ_REG "MiniCensoMEC2012.bin"

   /* Nome do arquivo que cont�m a �rvore */
//#define NOME_ARQ_ARV "I:\\Dados\\CensoMEC\\Arvore.dat"
#define NOME_ARQ_ARV "ArvoreMulti.dat"

   /* Nome do arquivo que cont�m as chaves ordenadas */
//#define NOME_ARQ_CHAVES  "I:\\Dados\\CensoMEC\\ChavesOrdenadas.txt"
#define NOME_ARQ_CHAVES  "ChavesOrdenadas.txt"

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
static void MedidaDeTempo(void)
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

/****
 *
 * TamanhoNoSemPreenchimento(): Calcula o tamnho de um n�
 *                              sem o preenchimentode estrutura
 *                              efetuado pelo compilador
 *
 * Par�metros: Nenhum
 *
 * Retorno: O tamnho de um n� sem preenchimento
 *
 * Observa��o: O prop�sito desta fun��o � apenas depura��o e
 *             melhor entendimento do programa
 *
 ****/
static int TamanhoNoSemPreenchimento(void)
{
   int tn, /* Tamanho de um n� sem preenchimento */
       ti = sizeof(int), /* Largura do tipo inteiro */
       tc = 2*sizeof(int); /* Tamanho de um par chave/�ndice */

   tn = ti + (G - 1)*tc + G*ti;

   return tn;
}

/****
 *
 * ApresentaEstatisticas(): Apresenta informa��es sobre o banco
 *                          de dados gerenciado pelo programa
 *
 * Par�metros:
 *       arvore (entrada) - stream que representa a �rvore
 *       registros (entrada) - stream que representa os registros
 *
 * Retorno: Nada
 *
 * Observa��o: O prop�sito desta fun��o � apenas depura��o e
 *             melhor entendimento do programa
 *
 ****/
void ApresentaEstatisticas( FILE *arvore, FILE *registros )
{
   tRegistro    umRegistro;
   int          nRegistros;
   int          nChaves = 0,
                prof; /* Profundidade da �rvore de busca */
   tChaveIndice chaveIndice;

   printf( "\n\n>>> Estatisticas do Banco de Dados <<<\n" );

   printf("\n\t>>> Grau da arvore: %d", G);
   printf( "\n\t>>> Tamanho de um no' COM preenchimento: %d",
           (int) sizeof(tNoMultiMS) );
   printf( "\n\t>>> Tamanho de um no' SEM preenchimento: %d",
           TamanhoNoSemPreenchimento() );
   printf("\n\t>>> Tamanho de bloco: %d\n", TB);

   printf( "\n\t>>> Numero de nos da arvore: %d\n",
           NumeroDeChavesMultiMS(arvore) );

   nRegistros = NumeroDeRegistros(registros, sizeof(umRegistro));

   printf( "\n\t>>> Numero de registros no arquivo: %d\n",
           nRegistros );

   nChaves = NumeroDeNosMultiMS(arvore, 0);
   printf("\n\t>>> Numero de chaves da arvore: %d", nChaves);

      /* Calcula a altura da �rvore */
   prof = AlturaMultiMS(arvore, 0);

   printf("\n\t>>> Profundidade da arvore: %d", prof);

   ASSEGURA( nChaves == nRegistros,
             "\n\nThis program is fucking wrong!!!" );

      /* Encontra a posi��o do registro */
      /* que cont�m a menor chave       */
   chaveIndice = MenorChaveMultiMS(arvore, 0);

   if (chaveIndice.indice < 0)
      printf("\n\t>>> A menor chave nao foi encontrada");
   else if (LeRegistroEmArquivo(&umRegistro, registros,
                                chaveIndice.indice)){
      printf("\n\n\t>>> Registro com a menor chave:\n\n");
      ExibeRegistro(&umRegistro);
   } else
      printf( "\n\t>>> Ocorreu um erro ao tentar "
             "recuperar o registro com a menor chave" );

      /* Encontra a posi��o do registro */
      /* que cont�m a maior chave       */
   chaveIndice = MaiorChaveMultiMS(arvore, 0);

   if (chaveIndice.indice < 0)
      printf("\n\t>>> A maior chave nao foi encontrada");
   else if (LeRegistroEmArquivo(&umRegistro, registros,
                                chaveIndice.indice)){
      printf("\n\n\t>>> Registro com a maior chave:\n\n");
      ExibeRegistro(&umRegistro);
   } else
      printf( "\n\t>>> Ocorreu um erro ao tentar "
              "recuperar o registro com a maior chave" );
}

/****
 *
 * ConstroiArvore(): L� arquivo de dados bin�rio contendo
 *                    registros e constr�i uma �rvore
 *                    multidirecional de busca em arquivo
 *
 * Par�metros:
 *      streamReg (entrada) - stream associado ao arquivo que
 *                            cont�m os registros
 *      streamArvore (entrada) - stream associado ao arquivo
 *                               que conter� a �rvore
 *
 * Retorno: O n�mero de chaves repetidas que n�o foram inseridas
 *
 ****/
void ConstroiArvore( FILE *streamReg, FILE *streamArvore )
{
   tChaveIndice chaveEIndice;
   tRegistro    reg;
   int          pos = 0,
                arvore = POSICAO_NULA; /* Posi��o da raiz da */
                                       /* �rvore em arquivo  */

      /* Verifica se o arquivo de registros n�o � NULL */
   ASSEGURA( streamReg,
             ERRO_STREAM_NULL(streamReg, ConstroiArvore) );

      /* Verifica se o arquivo da �rvore n�o � NULL */
   ASSEGURA( streamReg,
             ERRO_STREAM_NULL(streamArvore, ConstroiArvore) );

      /* Tenta mover o apontador de posi��o    */
      /* para o in�cio do arquivo de registros */
   MoveApontador(streamReg, 0, SEEK_SET);

      /* L� cada registro e armazena a chave e    */
      /* respectivo �ndice do registro no arquivo */
      /* da �rvore multidirecional de busca       */
   while (1) {
         /* L� um registro */
      fread(&reg, sizeof(reg), 1, streamReg);

         /* Se ocorreu erro ou o final de arquivo */
         /* foi atingido, encerra o la�o          */
      if (ferror(streamReg) || feof(streamReg))
         break;

         /* Cria o conte�do do par chave/�ndice */
      chaveEIndice.chave = reg.codAlunoCurso;
      chaveEIndice.indice = pos++;

         /* Tenta armazenar o par chave/�ndice na �rvore. */
         /* Se isso for imposs�vel, a chave � repetida.   */
      ASSEGURA( InsereMultiMS(&arvore, &chaveEIndice,
                             streamArvore),
                "Insercao nao foi possivel" );
   }

      /* Certifica-se que n�o ocorreu erro de leitura */
   ASSEGURA(!ferror(streamReg), ERRO_FREAD(ConstroiArvore));
}

/****
 *
 * IniciaArvoreMulti(): Verifica se a �rvore de busca j� existe.
 *                      Se ela j� existir e o usu�rio desejar
 *                      reconstrui-la ou se ela n�o existir,
 *                      ela ser� constru�da.
 *
 * Par�metros:
 *     nomeArqRegistros (entrada) - nome do arquivo de registros
 *     nomeArqArvore (entrada) - nome do arquivo que armazena a
 *                               �rvore
 *     *streamRegistros (entrada/sa�da) - stream associado ao
 *                                        arquivo de registros
 *     *streamArvore (entrada/sa�da) - stream associado ao
 *                                     arquivo que armazena
 *                                     a �rvore
 *
 * Retorno: O n�mero de chaves que n�o foram inseridas na �rvore
 *
 ****/
void IniciaArvoreMulti( const char *nomeArqRegistros,
                       const char *nomeArqArvore,
                       FILE **streamRegistros,
                       FILE **streamArvore )
{
   int op; /* Op��o do usu�rio */

      /* Tenta abrir o arquivo de registros */
   *streamRegistros = AbreArquivo(nomeArqRegistros, "r+b");

      /* Verifica se o arquivo que cont�m a �rvore j� existe */
   if (ArquivoExiste(nomeArqArvore)) {
         /* �rvore j� existe. Se o usu�rio */
         /* desejar, ela ser� reconstru�da */

      printf( "\n\t>>> Arvore ja existe. "
              "Deseja substitui-la (s/n)? " );

      op = LeOpcao("sSnN");

      if ( op == 'n' || op == 'N') { /* Usu�rio n�o deseja   */
                                     /* reconstruir a �rvore */
            /* Tenta abrir o arquivo que cont�m a �rvore */
         *streamArvore = AbreArquivo(nomeArqArvore, "r+b");

         return; /* Servi�o completo */
      }
   }

         /*                                        */
         /* Neste ponto, ou a �rvore ainda n�o     */
         /* existe ou usu�rio deseja reconstrui-la */
         /*                                        */

   *streamArvore = AbreArquivo(nomeArqArvore, "w+b");
   ASSEGURA( *streamArvore, ERRO_OPEN(IniciaArvoreMulti) );

   ConstroiArvore(*streamRegistros, *streamArvore);
}

/********************* Fun��o main() **********************/

/****
 *
 * main(): Testa fun��es de busca multidirecional com a �rvore
 *         de busca multidirecional armazenada em arquivo
 *
 * Par�metros: nenhum
 *
 * Retorno: 0
 *
 ****/
int main(void)
{
   tRegistro   umRegistro;
   const char *opcoes[] = { "Apresenta chaves em ordem",  /* 1 */
                            "Consulta dados de um aluno", /* 2 */
                            "Insere um aluno",            /* 3 */
                            "Altera dados de um aluno",   /* 4 */
                            "Remove um aluno",            /* 5 */
                            "Encerra o programa"          /* 6 */
                          };
   int          op, pos, posRaiz,
                nOpcoes = sizeof(opcoes)/sizeof(opcoes[0]),
                nRegs = 0; /* N�mero de registros no arquivo */
   long         umaChave; /* Uma chave de busca */
   tChaveIndice chaveEIndice;
   FILE        *streamReg, /* Stream associado ao  */
                           /* arquivo de registros */
               *streamArv, /* Stream associado ao       */
                           /* arquivo contendo a �rvore */
               *streamChaves, /* Stream associado a um     */
                              /* arquivo que armazena as   */
                              /* chaves em ordem crescente */
               *streamArvCp;  /* C�pia de 'streamArv'      */

   MedidaDeTempo();
   printf("\nCriando arvore...");

   IniciaArvoreMulti( NOME_ARQ_REG, NOME_ARQ_ARV,
                      &streamReg, &streamArv );

   printf("\n\t>>> Arvore criada com sucesso");

      /* Calcula o n�mero de registros */
   nRegs = NumeroDeRegistros(streamReg, sizeof(umRegistro));

   MedidaDeTempo();

   MedidaDeTempo();
   printf("\nChecando a consistencia do programa...");

   ApresentaEstatisticas(streamArv, streamReg);

   printf("\n\n...consistencia OK");
   MedidaDeTempo();

   while (1) { /* La�o principal do programa */
      ApresentaMenu(opcoes, nOpcoes);

      op = LeOpcao("123456");

      if (op == '6') {
         break;
      }

      if (op >= '2') { /* Estas op��es precisam de uma chave */
         printf("\n\t>>> Digite o codigo aluno/curso: ");
         umaChave = LeNatural();
      }

      switch (op) {
         case '1':   /* Imprime as chaves em ordem crescente */
            streamChaves = AbreArquivo(NOME_ARQ_CHAVES, "w");

               /* Faz um caminhamento in-ordem na �rvore */
               /* e grava as chaves encontradas nesse    */
               /* processo num arquivo de texto          */
            CaminhamentoInfixoMultiMS(streamArv, 0, streamChaves);
            printf( "\nAs chaves foram gravados em ordem "
                    "crescente no arquivo %s\n",
                    NOME_ARQ_CHAVES );

               /* Fecha o arquivo de chaves ordenadas */
            FechaArquivo(streamChaves, NOME_ARQ_CHAVES);

            break;

         case '2': /* Procura o registro que */
                   /* possui uma dada chave  */

               /* Tenta encontrar a posi��o do registro */
               /* que cont�m a chave de busca           */
            pos = BuscaMultiMS(streamArv, umaChave);

            if (pos < 0)
               printf("\n\t>>> A chave nao foi encontrada");
            else if (LeRegistroEmArquivo(&umRegistro, streamReg, pos)) {
               printf("\nRegistro encontrado:\n\n");
               ExibeRegistro(&umRegistro);
               printf("\n");
            } else
               printf("\n\t>>> Ocorreu um erro ao tentar"
                      " recuperar registro");

            break;

         case '3': /* Insere um novo registro, se a */
                   /* chave dada ainda nao existe   */
               /* Sabe-se que a raiz � o primeiro registro */
               /* do arquivo que cont�m a �rvore           */
            posRaiz = 0;

               /* Tenta inserir o par chave/�ndice na �rvore. */
               /* Se inser��o for bem-sucedida, insere o      */
               /* respectivo registro no arquivo de registros */
            chaveEIndice.chave = umaChave;
            chaveEIndice.indice = nRegs;

            if (!InsereMultiMS( &posRaiz, &chaveEIndice,
                               streamArv) )
               printf("\n\t>>> A chave ja' existe na arvore");
            else {
                  /* A chave foi inserida na �rvore. Resta    */
                  /* inserir o respectivo registro no arquivo */
               pos = chaveEIndice.indice;

                  /* A chave j� foi armazenada na �rvore. */
                  /* Portanto, a grava��o do respectivo   */
                  /* registro no arquivo n�o pode falhar. */
               InsereRegistroEmArquivo(chaveEIndice.chave, streamReg);

               nRegs++; /* O n�mero de registros aumentou */

                  /* Garante que os buffers associados */
                  /* aos arquivos s�o descarregados    */
               fflush(streamArv);
               fflush(streamReg);

               printf("\n\t>>> Insercao bem-sucedida");
            }

            break;

         case '4': /* Altera um registro         */
                   /* (mas NAO altera sua chave) */

            /*                                   */
            /* ATEN��O: A abordagem adotada aqui */
            /* n�o � amig�vel ao usu�rio         */
            /*                                   */

               /* Tenta encontrar a chave */
            pos = BuscaMultiMS(streamArv, umaChave);

            if (pos < 0)
               printf("\n\t>>> A chave nao foi encontrada\n");
            else {
               ASSEGURA(LeRegistroEmArquivo(&umRegistro, streamReg,
                                            pos),
                        "Impossivel ler registro em arquivo");

               printf("\n\n+++  Dados correntes do registro  +++\n");
               ExibeRegistro(&umRegistro);

               printf("\n\n>>> Introduza os novos dados do registro\n");
               LeRegistro(&umRegistro);

               MoveApontador( streamReg, pos*sizeof(umRegistro),
                              SEEK_SET );

               ASSEGURA( !fwrite(&umRegistro, sizeof(umRegistro), 1,
                                 streamReg),
                         "Impossivel gravar registro em arquivo" );
            }

            break;

         case '5': /* Remove um registro */
            streamArvCp = streamArv;

            if (RemoveChaveMultiMS(&streamArvCp, umaChave)) {
               if (!streamArvCp) { /* �rvore ficou vazia */
                  posRaiz = POSICAO_NULA;
                  FechaArquivo(streamArv, NOME_ARQ_ARV);
                  streamArv = AbreArquivo(NOME_ARQ_ARV, "w+b");
               }

               printf( "\n\t>>> Remocao bem sucedida.\n");
            } else {
               printf( "\n\t>>> A remocao MAL sucedida.\n");
            }

            break;
      } /* switch */
   } /* while */

      /* Fecha os arquivos */
   FechaArquivo(streamReg, NOME_ARQ_REG);
   FechaArquivo(streamArv, NOME_ARQ_ARV);

   printf("\nObrigado por usar este programa. Bye.\n");

   return 0;
}

/******************* Exemplo de Execu��o ******************

Criando arvore...
        >>> Arvore ja existe. Deseja substitui-la (s/n)? s

        >>> Arvore criada com sucesso

        *** Tempo gasto na operacao: 287.00 segundos ***

Checando a consistencia do programa...

>>> Estatisticas do Banco de Dados <<<

        >>> Grau da arvore: 340
        >>> Tamanho de um no' COM preenchimento: 4076
        >>> Tamanho de um no' SEM preenchimento: 4076
        >>> Tamanho de bloco: 4096

        >>> Numero de nos da arvore: 1006463

        >>> Numero de registros no arquivo: 9565483

        >>> Numero de chaves da arvore: 9565483
        >>> Numero de chaves repetidas: 0
        >>> Profundidade da arvore: 7

        >>> Registro com a menor chave:
Codigo do aluno: 400010594975
Codigo do curso: 1
Nome da IES: FACULDADE DE CI?NCIAS SOCIAIS APLICADAS DE BELO HORIZONTE
Nome do curso: PEDAGOGIA
Sexo do aluno: Masculino
Idade do aluno: 46
UF de nascimento: MINAS GERAIS
Ano de ingresso: 2010

        >>> Registro com a maior chave:
Codigo do aluno: 400012124372
Codigo do curso: 21569970
Nome da IES: INSTITUTO FEDERAL DE EDUCA??O, CI?NCIA E TECNOLOGIA DE S?O PAULO
Nome do curso: MATEM?TICA
Sexo do aluno: Masculino
Idade do aluno: 55
UF de nascimento:
Ano de ingresso: 2011
...consistencia OK

        *** Tempo gasto na operacao:  7.00 segundos ***

>>> Escolha uma das opcoes a seguir:

        [1] Apresenta chaves em ordem
        [2] Consulta dados de um aluno
        [3] Insere um aluno
        [4] Altera dados de um aluno
        [5] Remove um aluno
        [6] Encerra o programa

        >>> 2

        >>> Digite o codigo aluno/curso: 8174201

Registro encontrado:

Codigo do aluno: 400008869798
Codigo do curso: 8174201
Nome da IES: CENTRO UNIVERSIT?RIO LEONARDO DA VINCI
Nome do curso: LOG?STICA
Sexo do aluno: Masculino
Idade do aluno: 26
UF de nascimento: AMAZONAS
Ano de ingresso: 2012

>>> Escolha uma das opcoes a seguir:

        [1] Apresenta chaves em ordem
        [2] Consulta dados de um aluno
        [3] Insere um aluno
        [4] Altera dados de um aluno
        [5] Remove um aluno
        [6] Encerra o programa

        >>> 6

Obrigado por usar este programa. Bye.

**********************************************************/

