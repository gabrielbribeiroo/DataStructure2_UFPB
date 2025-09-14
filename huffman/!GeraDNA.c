/****
 *
 * T�tulo: Gera DNA
 *
 * Autor: Ulysses de Oliveira
 *
 * Data de Cria��o: 16/04/2015
 * �ltima modifica��o: 08/10/2017
 *
 * Descri��o: Cria um arquivo de texto contendo uma sequ�ncia de
 *            DNA gerada aleat�riamente
 *
 * Entrada: Tamanho da sequ�ncia de DNA
 *
 * Sa�da: Arquivo contendo a sequ�ncia de DNA gerada
 *
 ****/

/*********************** Includes *************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/***************** Constantes Simb�licas ******************/

#define N_ACIDOS 4
#define NOME_ARQ "DNA_Rand.txt"

/****************** Defini��es de Tipos *******************/

typedef enum {A, G, C, T} tAcido;

/***************** Defini��es de Fun��es ******************/

/****
 *
 * LimpaBuffer(): Limpa o buffer associado ao meio de
 *                entrada padr�o (stdin)
 *
 * Par�metros: Nenhum
 *
 * Retorno: O n�mero de caracteres descartados, com
 *          exce��o do caractere '\n'.
 *
 ****/
static int LimpaBuffer(void)
{
   int carLido, /* Armazena cada caractere lido */
       nCarLidos = 0; /* Conta o n�mero de */
                      /* caracteres lidos  */

      /* L� e descarta cada caractere lido at�    */
      /* encontrar '\n' ou getchar() retornar EOF */
   do {
      carLido = getchar(); /* L� um caractere */
      ++nCarLidos; /* Mais um caractere foi lido */
   } while ((carLido != '\n') && (carLido != EOF));

      /* O �ltimo caractere lido foi '\n' ou  */
      /* EOF e n�o deve ser considerado sobra */
   return nCarLidos - 1;
}

/****
 *
 * LeNaturalPositivo(): L� um n�mero natural positivo via
 *                      teclado e deixa o buffer intacto
 *
 * Par�metros: Nenhum
 *
 * Retorno: O n�mero natural lido
 *
 ****/
int LeNaturalPositivo(void)
{
   int num, /* O n�mero lido */
       teste, /* Valor retornado por scanf() */
       nResto = 0; /* N�mero de caracteres */
                   /* excedentes           */

   /* Desvia para c� se o valor for inv�lido */
inicio:
      /* Tenta ler um valor v�lido */
   teste = scanf("%d", &num);

      /* Se n�o ocorreu erro de leitura ou de final */
      /* de arquivo, h� caracteres remanescentes    */
      /* que precisam ser removidos                 */
   if (teste != EOF) {
      nResto = LimpaBuffer();
   }

      /* Enquanto o valor retornado por scanf()  */
      /* n�o indicar que um valor v�lido foi     */
      /* lido continua tentando obter esse valor */
   while(teste != 1 || num < 1) {
      printf("\a\n\t>>> O valor digitado e' invalido. "
                      "Tente novamente\n\t> " );
      goto inicio; /* N�o causa dano algum */
   }

      /* Repreende o usu�rio se ele digitou demais */
   if (nResto == 1) {
      printf("\t>>> Um caractere foi descartado\n");
   } else if (nResto > 1) {
      printf( "\t>>> %d caracteres foram descartados\n",
              nResto );
   }

      /* O valor retornado certamente � v�lido */
   return num;
}

/****
 *
 * main(): Cria um arquivo de texto contendo uma sequ�ncia de DNA
 *         gerada aleat�riamente
 *
 * Par�metros: Nenhum
 *
 * Retorno: Zero
 *
 ****/
 int main(void)
{
   int   i,
         tamanho; /* Tamanho da sequencia (e do arquivo) */
   FILE *stream;  /* Stream associado ao arquivo */

   srand(time(NULL)); /* Alimenta o gerador de n�meros aleat�rios */

   printf("\nIntroduza o tamanho da sequencia: ");
   tamanho = LeNaturalPositivo();

   stream = fopen(NOME_ARQ, "w"); /* Tenta abrir o arquivo */

   if (!stream) { /* Verifica se o arquivo foi aberto */
      printf("\nArquivo nao pode ser criado");
      return 1;
   }

      /* Gera e escreve a sequ�ncia no arquivo */
   for (i = 0; i < tamanho; ++i) {
      switch ((tAcido) (rand()%N_ACIDOS)) {
         case A:
            fputc('A', stream);
            break;

         case G:
            fputc('G', stream);
            break;

         case C:
            fputc('C', stream);
            break;

         case T:
            fputc('T', stream);
            break;

         default:
            printf("\nErro desconhecido");;
            return 1;
      }
   }

   fclose(stream);

   return 0;
}
