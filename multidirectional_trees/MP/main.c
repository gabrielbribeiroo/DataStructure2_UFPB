/****
 *
 * Título do Programa: Busca Multidirecional
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
#include "ArvoreMulti.h" /* Funções que lidam com árvores */
                         /* multimensionais               */
#include "Registros.h"   /* Funções de processamento */
                         /* de registros             */
#include "leitura.h"     /* Funções de leitura via teclado */
#include "ASSEGURA.h"    /* Macro ASSEGURA e outras macros */
                         /* de tratamento de exceção       */

/***************** Constantes Simbólicas ******************/

   /* Nome do arquivo de registros */
#define NOME_ARQ_REG     "MiniCensoMEC2012.bin"

   /* Nome do arquivo que contém as chaves ordenadas */
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

   printf("\nEscolha uma das opcoes a seguir:\n");

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
 * Retorno: O tamanho de um nó sem preenchimento
 *
 * Observação: O propósito desta função é apenas depuração e
 *             melhor entendimento do programa
 *
 ****/
static int TamanhoNoSemPreenchimento()
{
   int tamInt = sizeof(int),
       tamChaves = (G - 1)*sizeof(tChaveIndice),
       tamFilhos = G*sizeof(struct no *);

   return tamInt + tamChaves + tamFilhos;
}

/****
 *
 * ApresentaEstatisticas(): Apresenta informações sobre o banco
 *                          de dados gerenciado pelo programa
 *
 * Parâmetros:
 *       arvore - stream que representa a árvore
 *       registros - stream que representa os registros
 *
 * Retorno: Nada
 *
 * Observação: O propósito desta função é apenas depuração e
 *             melhor entendimento do programa
 *
 ****/
void ApresentaEstatisticas(tArvoreMulti arvore, FILE *registros)
{
   tRegistro     umRegistro;
   int           nRegistros;
   tChaveIndice *chaveIndice;
   int           nChaves,
                 prof; /* Profundidade da árvore de busca */

   printf( "\n\n>>> Estatisticas do Banco de Dados <<<\n" );

   printf("\n\t>>> Grau da arvore: %d", G);
   printf( "\n\t>>> Tamanho de um no' COM preenchimento: %d",
           (int) sizeof(tNoMulti) );
   printf( "\n\t>>> Tamanho de um no' SEM preenchimento: %d",
           TamanhoNoSemPreenchimento() );

   printf( "\n\t>>> Numero de nos da arvore: %d\n",
           NumeroDeNosMulti(arvore) );

   nRegistros = NumeroDeRegistros(registros, sizeof(umRegistro));

   printf( "\n\t>>> Numero de registros no arquivo: %d\n",
           nRegistros );

   nChaves = NumeroDeChavesMulti(arvore);
   printf("\n\t>>> Numero de chaves da arvore: %d", nChaves);

      /* Calcula a altura da árvore */
   prof = AlturaMulti(arvore);

   printf("\n\t>>> Profundidade da arvore: %d", prof);

   ASSEGURA( nChaves == nRegistros,
             "\n\nThis program is fucking wrong!!!" );

      /* Encontra a posição do registro */
      /* que contém a menor chave       */
   chaveIndice = MenorChaveMulti(arvore);

   if (!chaveIndice)
      printf("\n\t>>> A menor chave nao foi encontrada");
   else if (LeRegistroEmArquivo(&umRegistro, registros, chaveIndice->indice)){
      printf("\n\n\t>>> Registro com a menor chave:\n\n");
      ExibeRegistro(&umRegistro);
   } else
      printf( "\n\t>>> Ocorreu um erro ao tentar "
             "recuperar o registro com a menor chave" );

      /* Encontra a posição do registro */
      /* que contém a maior chave       */
   chaveIndice = MaiorChaveMulti(arvore);

   if (!chaveIndice)
      printf("\n\t>>> A maior chave nao foi encontrada");
   else if (LeRegistroEmArquivo(&umRegistro, registros, chaveIndice->indice)){
      printf("\n\n\t>>> Registro com a maior chave:\n\n");
      ExibeRegistro(&umRegistro);
   } else
      printf( "\n\t>>> Ocorreu um erro ao tentar "
              "recuperar o registro com a maior chave" );
}

/****
 *
 * ConstroiArvore(): Lê arquivo de dados (binário) contendo
 *                   registros e constrói uma árvore
 *                   multidirecional de busca na memória
 *                   principal
 *
 * Parâmetros:
 *      stream (entrada) - stream associado ao arquivo
 *                         de registros
 *
 * Retorno: Ponteiro para a árvore criada
 *
 ****/
tArvoreMulti ConstroiArvore( FILE *stream )
{
   int          pos = 0;
   tChaveIndice chaveEIndice;
   tRegistro    reg;
   tArvoreMulti arvore = NULL;

   while ( fread(&reg, sizeof(reg), 1, stream) ) {
         /* Cria o conteúdo do par chave/índice */
      chaveEIndice.chave = reg.codAlunoCurso;
      chaveEIndice.indice = pos++;

         /* Tenta armazenar o par chave/índice */
         /* em algum nó da árvore              */
      if (!InsereMulti(&arvore, &chaveEIndice))
         return NULL; /* Se o armazenamento não foi      */
                      /* possível, não dá para continuar */
   }

   return arvore;
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
   tRegistro    umRegistro;
   const char *opcoes[] = { "Apresenta chaves em ordem",
                            "Consulta dados de um aluno",
                            "Insere um aluno",
                            "Altera dados de um aluno",
                            "Remove um aluno",
                            "Encerra o programa"
                          };
   int          op, pos,
                nOpcoes = sizeof(opcoes)/sizeof(opcoes[0]),
                nRegs = 0, /* Número de registros no arquivo */
                umaChave; /* Uma chave de busca */
   tChaveIndice chaveEIndice;
   FILE        *streamReg, /* Stream associado ao  */
                           /* arquivo de registros */
               *streamChaves; /* Stream associado a um     */
                              /* arquivo que armazena as   */
                              /* chaves em ordem crescente */
//   tListaRemocoes listaDeRemocoes;
   tArvoreMulti arvore = NULL;

   streamReg = fopen(NOME_ARQ_REG, "r+b");

   ASSEGURA(streamReg, "Arquivo de registros nao foi aberto");

   MedidaDeTempo();
   printf("\nCriando arvore...");

   arvore = ConstroiArvore(streamReg);

   printf("\n\n\t>>> Arvore criada com sucesso");

   MedidaDeTempo();

      /* Calcula o número de registros */
   nRegs = NumeroDeRegistros(streamReg, sizeof(umRegistro));

#if DEPURACAO == 0

   MedidaDeTempo();
   printf("\nChecando a consistencia do programa...");

   ApresentaEstatisticas(arvore, streamReg);

   printf("\n\n...consistencia OK");
   MedidaDeTempo();

#endif

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
            CaminhamentoInfixoMulti(arvore, streamChaves);
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
            pos = BuscaMulti(arvore, umaChave);

            if (pos < 0)
               printf("\n\t>>> A chave nao foi encontrada\n");
            else if (LeRegistroEmArquivo(&umRegistro, streamReg, pos)) {
               putchar('\n');
               ExibeRegistro(&umRegistro);
               putchar('\n');
            } else
               printf("\n\t>>> Ocorreu um erro ao tentar"
                      " recuperar registro\n");

            break;

         case '3': /* Insere um novo registro, se a */
                   /* chave dada ainda nao existe   */
               /* Tenta inserir o par chave/índice na árvore. */
               /* Se inserção for bem-sucedida, insere o      */
               /* respectivo registro no arquivo de registros */
            chaveEIndice.chave = umaChave;
            chaveEIndice.indice = nRegs;

            if ( !InsereMulti( &arvore, &chaveEIndice) )
               printf("\n\t>>> A chave ja' existe na arvore");
            else {
                  /* A chave foi inserida na árvore.  Resta   */
                  /* inserir o respectivo registro no arquivo */
               pos = chaveEIndice.indice;

                  /* A chave já foi armazenada na árvore. */
                  /* Portanto, a gravação do respectivo   */
                  /* registro no arquivo não pode falhar. */
               InsereRegistroEmArquivo(chaveEIndice.chave, streamReg);

               nRegs++; /* O número de registros aumentou */
            }

            break;

         case '4': /* Altera um registro         */
                   /* (mas NAO altera sua chave) */

            /*                                   */
            /* ATENÇÃO: A abordagem adotada aqui */
            /* não é amigável ao usuário         */
            /*                                   */

               /* Tenta encontrar a chave */
            pos = BuscaMulti(arvore, umaChave);

            if (pos < 0)
               printf("\n\t>>> A chave não foi encontrada\n");
            else {
               ASSEGURA(LeRegistroEmArquivo(&umRegistro, streamReg,
                                            pos),
                        "Impossivel ler registro em arquivo");

               printf("\n\n+++  Dados correntes do registro  +++\n");
               ExibeRegistro(&umRegistro);

               printf("\n\n>>> Introduza os novos dados do registro\n");
               LeRegistro(&umRegistro);

               ASSEGURA( !fseek(streamReg, pos*sizeof(umRegistro),
                                SEEK_SET),
                         "Impossivel apontador de posicao de arquivo" );

               ASSEGURA( !fwrite(&umRegistro, sizeof(umRegistro), 1,
                                 streamReg),
                         "Impossivel gravar registro em arquivo" );
            }

            break;

         case '5': /* Remove um registro */
            if (RemoveChaveMulti(&arvore, umaChave) )
               printf( "\n\t>>> Remocao bem sucedida.");
            else
               printf( "\n\t>>> A remocao NAO foi bem "
                      "sucedida.");

            break;
      } /* switch */
   } /* while */

      /* Fecha o arquivo de registros */
   FechaArquivo(streamReg, NOME_ARQ_REG);

   printf("\nObrigado por usar este programa. Bye.\n");

   return 0;
}

/******************* Exemplo de Execução ******************

Criando arvore...

        >>> Arvore criada com sucesso

        *** Tempo gasto na operacao:  2.00 segundos ***

Checando a consistencia do programa...

>>> Estatisticas do Banco de Dados <<<

        >>> Grau da arvore: 200
        >>> Tamanho de um no' COM preenchimento: 3992
        >>> Tamanho de um no' SEM preenchimento: 3192
        >>> Numero de nos da arvore: 55089

        >>> Numero de registros no arquivo: 673580

        >>> Numero de chaves da arvore: 673580
        >>> Profundidade da arvore: 34

        >>> Registro com a menor chave:

Numero: 623305
UF: SP
Numero de localidade: 9668
Nome abreviado: da S?
Nome: Pra?a da S? - at? 999 - lado ?mpar
Bairro inicio: 29400
Bairro fim: 0
aluno: 1001000
Complemento: - at? 999 - lado ?mpar
Tipo de logradouro: Pra?a
Status de logradouro: S
Nome sem acento:: da Se
Chave DNE: UGCNUGAL8XLAGUAL

        >>> Registro com a maior chave:

Numero: 400636
UF: RS
Numero de localidade: 7953
Nome abreviado: Principal
Nome: Rua Principal
Bairro inicio: 39035
Bairro fim: 0
aluno: 99099000
Complemento:
Tipo de logradouro: Rua
Status de logradouro: S
Nome sem acento:: Principal
Chave DNE: V5BKUGBK5VLAGUAL

...consistencia OK

        *** Tempo gasto na operacao:  0.00 segundos ***

Escolha uma das opcoes a seguir:

        [1] Apresenta chaves em ordem
        [2] Estatisticas do banco de dados
        [3] Consulta dados de um aluno
        [4] Insere um aluno
        [5] Altera dados de um aluno
        [6] Remove um aluno
        [7] Encerra o programa

        >>> 3

Digite um(a) aluno com exatamente 8 digitos:
        > 58038510

Numero: 236144
UF: PB
Numero de localidade: 4964
Nome abreviado: Juvenal Mßrio da Silva
Nome: Rua Juvenal Mßrio da Silva - at? 623/624
Bairro inicio: 6999
Bairro fim: 0
aluno: 58038510
Complemento: - at? 623/624
Tipo de logradouro: Rua
Status de logradouro: S
Nome sem acento:: Juvenal Mario da Silva
Chave DNE: 4CDMUGGR7YSJ8XLA
Escolha uma das opcoes a seguir:

        [1] Apresenta chaves em ordem
        [2] Estatisticas do banco de dados
        [3] Consulta dados de um aluno
        [4] Insere um aluno
        [5] Altera dados de um aluno
        [6] Remove um aluno
        [7] Encerra o programa

        >>> 7

Obrigado por usar este programa. Bye.

**********************************************************/