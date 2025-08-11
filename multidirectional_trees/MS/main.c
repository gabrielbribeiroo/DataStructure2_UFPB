/****
 *
 * Título do Programa: Busca Multidirecional em Memória Secundária
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
 *     ArvoreMulti2.h e - interface e implementação de operações
 *     ArvoreMulti2.c     operações sobre árvores multidirecionais
 *                        armazenadas em arquivo
 *     Registros.h e - interface e implementação de operações
 *     Registros.c     sobre registros armazenados em arquivos
 *     main.c (este arquivo) - contém main() e outras funções
 *                             que testam as operações sobre
 *                             árvores de busca multidirecionais
 *                             armazenadas em arquivo
 *
 ****/

/*********************** Includes *************************/

#include <stdio.h>       /* Funções de Entrada/Saída    */
#include <string.h>      /* Funções strXXX() e memXXX() */
#include <time.h>        /* Medição de tempo            */
#include "ArvoreMulti2.h" /* Funções que lidam com árvores */
                          /* multimensionais               */
#include "Registros.h"   /* Funções de processamento */
                         /* de registros             */
#include "leitura.h"     /* Funções de leitura via teclado */
#include "ASSEGURA.h"    /* Macro ASSEGURA e outras macros */
                         /* de tratamento de exceção       */

/***************** Constantes Simbólicas ******************/

   /* Nome do arquivo de registros */
//#define NOME_ARQ_REG "I:\\Dados\\CensoMEC\\CensoMEC2012.bin"
#define NOME_ARQ_REG "MiniCensoMEC2012.bin"

   /* Nome do arquivo que contém a árvore */
//#define NOME_ARQ_ARV "I:\\Dados\\CensoMEC\\Arvore.dat"
#define NOME_ARQ_ARV "ArvoreMulti.dat"

   /* Nome do arquivo que contém as chaves ordenadas */
//#define NOME_ARQ_CHAVES  "I:\\Dados\\CensoMEC\\ChavesOrdenadas.txt"
#define NOME_ARQ_CHAVES  "ChavesOrdenadas.txt"

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
 * TamanhoNoSemPreenchimento(): Calcula o tamnho de um nó
 *                              sem o preenchimentode estrutura
 *                              efetuado pelo compilador
 *
 * Parâmetros: Nenhum
 *
 * Retorno: O tamnho de um nó sem preenchimento
 *
 * Observação: O propósito desta função é apenas depuração e
 *             melhor entendimento do programa
 *
 ****/
static int TamanhoNoSemPreenchimento(void)
{
   int tn, /* Tamanho de um nó sem preenchimento */
       ti = sizeof(int), /* Largura do tipo inteiro */
       tc = 2*sizeof(int); /* Tamanho de um par chave/índice */

   tn = ti + (G - 1)*tc + G*ti;

   return tn;
}

/****
 *
 * ApresentaEstatisticas(): Apresenta informações sobre o banco
 *                          de dados gerenciado pelo programa
 *
 * Parâmetros:
 *       arvore (entrada) - stream que representa a árvore
 *       registros (entrada) - stream que representa os registros
 *
 * Retorno: Nada
 *
 * Observação: O propósito desta função é apenas depuração e
 *             melhor entendimento do programa
 *
 ****/
void ApresentaEstatisticas( FILE *arvore, FILE *registros )
{
   tRegistro    umRegistro;
   int          nRegistros;
   int          nChaves = 0,
                prof; /* Profundidade da árvore de busca */
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

      /* Calcula a altura da árvore */
   prof = AlturaMultiMS(arvore, 0);

   printf("\n\t>>> Profundidade da arvore: %d", prof);

   ASSEGURA( nChaves == nRegistros,
             "\n\nThis program is fucking wrong!!!" );

      /* Encontra a posição do registro */
      /* que contém a menor chave       */
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

      /* Encontra a posição do registro */
      /* que contém a maior chave       */
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
 * ConstroiArvore(): Lê arquivo de dados binário contendo
 *                    registros e constrói uma árvore
 *                    multidirecional de busca em arquivo
 *
 * Parâmetros:
 *      streamReg (entrada) - stream associado ao arquivo que
 *                            contém os registros
 *      streamArvore (entrada) - stream associado ao arquivo
 *                               que conterá a árvore
 *
 * Retorno: O número de chaves repetidas que não foram inseridas
 *
 ****/
void ConstroiArvore( FILE *streamReg, FILE *streamArvore )
{
   tChaveIndice chaveEIndice;
   tRegistro    reg;
   int          pos = 0,
                arvore = POSICAO_NULA; /* Posição da raiz da */
                                       /* árvore em arquivo  */

      /* Verifica se o arquivo de registros não é NULL */
   ASSEGURA( streamReg,
             ERRO_STREAM_NULL(streamReg, ConstroiArvore) );

      /* Verifica se o arquivo da árvore não é NULL */
   ASSEGURA( streamReg,
             ERRO_STREAM_NULL(streamArvore, ConstroiArvore) );

      /* Tenta mover o apontador de posição    */
      /* para o início do arquivo de registros */
   MoveApontador(streamReg, 0, SEEK_SET);

      /* Lê cada registro e armazena a chave e    */
      /* respectivo índice do registro no arquivo */
      /* da árvore multidirecional de busca       */
   while (1) {
         /* Lê um registro */
      fread(&reg, sizeof(reg), 1, streamReg);

         /* Se ocorreu erro ou o final de arquivo */
         /* foi atingido, encerra o laço          */
      if (ferror(streamReg) || feof(streamReg))
         break;

         /* Cria o conteúdo do par chave/índice */
      chaveEIndice.chave = reg.codAlunoCurso;
      chaveEIndice.indice = pos++;

         /* Tenta armazenar o par chave/índice na árvore. */
         /* Se isso for impossível, a chave é repetida.   */
      ASSEGURA( InsereMultiMS(&arvore, &chaveEIndice,
                             streamArvore),
                "Insercao nao foi possivel" );
   }

      /* Certifica-se que não ocorreu erro de leitura */
   ASSEGURA(!ferror(streamReg), ERRO_FREAD(ConstroiArvore));
}

/****
 *
 * IniciaArvoreMulti(): Verifica se a árvore de busca já existe.
 *                      Se ela já existir e o usuário desejar
 *                      reconstrui-la ou se ela não existir,
 *                      ela será construída.
 *
 * Parâmetros:
 *     nomeArqRegistros (entrada) - nome do arquivo de registros
 *     nomeArqArvore (entrada) - nome do arquivo que armazena a
 *                               árvore
 *     *streamRegistros (entrada/saída) - stream associado ao
 *                                        arquivo de registros
 *     *streamArvore (entrada/saída) - stream associado ao
 *                                     arquivo que armazena
 *                                     a árvore
 *
 * Retorno: O número de chaves que não foram inseridas na árvore
 *
 ****/
void IniciaArvoreMulti( const char *nomeArqRegistros,
                       const char *nomeArqArvore,
                       FILE **streamRegistros,
                       FILE **streamArvore )
{
   int op; /* Opção do usuário */

      /* Tenta abrir o arquivo de registros */
   *streamRegistros = AbreArquivo(nomeArqRegistros, "r+b");

      /* Verifica se o arquivo que contém a árvore já existe */
   if (ArquivoExiste(nomeArqArvore)) {
         /* Árvore já existe. Se o usuário */
         /* desejar, ela será reconstruída */

      printf( "\n\t>>> Arvore ja existe. "
              "Deseja substitui-la (s/n)? " );

      op = LeOpcao("sSnN");

      if ( op == 'n' || op == 'N') { /* Usuário não deseja   */
                                     /* reconstruir a árvore */
            /* Tenta abrir o arquivo que contém a árvore */
         *streamArvore = AbreArquivo(nomeArqArvore, "r+b");

         return; /* Serviço completo */
      }
   }

         /*                                        */
         /* Neste ponto, ou a árvore ainda não     */
         /* existe ou usuário deseja reconstrui-la */
         /*                                        */

   *streamArvore = AbreArquivo(nomeArqArvore, "w+b");
   ASSEGURA( *streamArvore, ERRO_OPEN(IniciaArvoreMulti) );

   ConstroiArvore(*streamRegistros, *streamArvore);
}

/********************* Função main() **********************/

/****
 *
 * main(): Testa funções de busca multidirecional com a árvore
 *         de busca multidirecional armazenada em arquivo
 *
 * Parâmetros: nenhum
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
                nRegs = 0; /* Número de registros no arquivo */
   long         umaChave; /* Uma chave de busca */
   tChaveIndice chaveEIndice;
   FILE        *streamReg, /* Stream associado ao  */
                           /* arquivo de registros */
               *streamArv, /* Stream associado ao       */
                           /* arquivo contendo a árvore */
               *streamChaves, /* Stream associado a um     */
                              /* arquivo que armazena as   */
                              /* chaves em ordem crescente */
               *streamArvCp;  /* Cópia de 'streamArv'      */

   MedidaDeTempo();
   printf("\nCriando arvore...");

   IniciaArvoreMulti( NOME_ARQ_REG, NOME_ARQ_ARV,
                      &streamReg, &streamArv );

   printf("\n\t>>> Arvore criada com sucesso");

      /* Calcula o número de registros */
   nRegs = NumeroDeRegistros(streamReg, sizeof(umRegistro));

   MedidaDeTempo();

   MedidaDeTempo();
   printf("\nChecando a consistencia do programa...");

   ApresentaEstatisticas(streamArv, streamReg);

   printf("\n\n...consistencia OK");
   MedidaDeTempo();

   while (1) { /* Laço principal do programa */
      ApresentaMenu(opcoes, nOpcoes);

      op = LeOpcao("123456");

      if (op == '6') {
         break;
      }

      if (op >= '2') { /* Estas opções precisam de uma chave */
         printf("\n\t>>> Digite o codigo aluno/curso: ");
         umaChave = LeNatural();
      }

      switch (op) {
         case '1':   /* Imprime as chaves em ordem crescente */
            streamChaves = AbreArquivo(NOME_ARQ_CHAVES, "w");

               /* Faz um caminhamento in-ordem na árvore */
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

               /* Tenta encontrar a posição do registro */
               /* que contém a chave de busca           */
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
               /* Sabe-se que a raiz é o primeiro registro */
               /* do arquivo que contém a árvore           */
            posRaiz = 0;

               /* Tenta inserir o par chave/índice na árvore. */
               /* Se inserção for bem-sucedida, insere o      */
               /* respectivo registro no arquivo de registros */
            chaveEIndice.chave = umaChave;
            chaveEIndice.indice = nRegs;

            if (!InsereMultiMS( &posRaiz, &chaveEIndice,
                               streamArv) )
               printf("\n\t>>> A chave ja' existe na arvore");
            else {
                  /* A chave foi inserida na árvore. Resta    */
                  /* inserir o respectivo registro no arquivo */
               pos = chaveEIndice.indice;

                  /* A chave já foi armazenada na árvore. */
                  /* Portanto, a gravação do respectivo   */
                  /* registro no arquivo não pode falhar. */
               InsereRegistroEmArquivo(chaveEIndice.chave, streamReg);

               nRegs++; /* O número de registros aumentou */

                  /* Garante que os buffers associados */
                  /* aos arquivos são descarregados    */
               fflush(streamArv);
               fflush(streamReg);

               printf("\n\t>>> Insercao bem-sucedida");
            }

            break;

         case '4': /* Altera um registro         */
                   /* (mas NAO altera sua chave) */

            /*                                   */
            /* ATENÇÃO: A abordagem adotada aqui */
            /* não é amigável ao usuário         */
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
               if (!streamArvCp) { /* Árvore ficou vazia */
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

/******************* Exemplo de Execução ******************

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

