/****
 *
 * Título: Gera DNA
 *
 * Autor: Ulysses de Oliveira
 *
 * Data de Criação: 16/04/2015
 * Última modificação: 08/10/2017
 *
 * Descrição: Cria um arquivo de texto contendo uma sequência de
 *            DNA gerada aleatóriamente
 *
 * Entrada: Tamanho da sequência de DNA
 *
 * Saída: Arquivo contendo a sequência de DNA gerada
 *
 ****/

/*********************** Includes *************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/***************** Constantes Simbólicas ******************/

#define N_ACIDOS 4
#define NOME_ARQ "DNA_Rand.txt"

/****************** Definições de Tipos *******************/

typedef enum {A, G, C, T} tAcido;

/***************** Definições de Funções ******************/

/****
 *
 * LimpaBuffer(): Limpa o buffer associado ao meio de
 *                entrada padrão (stdin)
 *
 * Parâmetros: Nenhum
 *
 * Retorno: O número de caracteres descartados, com
 *          exceção do caractere '\n'.
 *
 ****/
static int LimpaBuffer(void)
{
   int carLido, /* Armazena cada caractere lido */
       nCarLidos = 0; /* Conta o número de */
                      /* caracteres lidos  */

      /* Lê e descarta cada caractere lido até    */
      /* encontrar '\n' ou getchar() retornar EOF */
   do {
      carLido = getchar(); /* Lê um caractere */
      ++nCarLidos; /* Mais um caractere foi lido */
   } while ((carLido != '\n') && (carLido != EOF));

      /* O último caractere lido foi '\n' ou  */
      /* EOF e não deve ser considerado sobra */
   return nCarLidos - 1;
}

/****
 *
 * LeNaturalPositivo(): Lê um número natural positivo via
 *                      teclado e deixa o buffer intacto
 *
 * Parâmetros: Nenhum
 *
 * Retorno: O número natural lido
 *
 ****/
int LeNaturalPositivo(void)
{
   int num, /* O número lido */
       teste, /* Valor retornado por scanf() */
       nResto = 0; /* Número de caracteres */
                   /* excedentes           */

   /* Desvia para cá se o valor for inválido */
inicio:
      /* Tenta ler um valor válido */
   teste = scanf("%d", &num);

      /* Se não ocorreu erro de leitura ou de final */
      /* de arquivo, há caracteres remanescentes    */
      /* que precisam ser removidos                 */
   if (teste != EOF) {
      nResto = LimpaBuffer();
   }

      /* Enquanto o valor retornado por scanf()  */
      /* não indicar que um valor válido foi     */
      /* lido continua tentando obter esse valor */
   while(teste != 1 || num < 1) {
      printf("\a\n\t>>> O valor digitado e' invalido. "
                      "Tente novamente\n\t> " );
      goto inicio; /* Não causa dano algum */
   }

      /* Repreende o usuário se ele digitou demais */
   if (nResto == 1) {
      printf("\t>>> Um caractere foi descartado\n");
   } else if (nResto > 1) {
      printf( "\t>>> %d caracteres foram descartados\n",
              nResto );
   }

      /* O valor retornado certamente é válido */
   return num;
}

/****
 *
 * main(): Cria um arquivo de texto contendo uma sequência de DNA
 *         gerada aleatóriamente
 *
 * Parâmetros: Nenhum
 *
 * Retorno: Zero
 *
 ****/
 int main(void)
{
   int   i,
         tamanho; /* Tamanho da sequencia (e do arquivo) */
   FILE *stream;  /* Stream associado ao arquivo */

   srand(time(NULL)); /* Alimenta o gerador de números aleatórios */

   printf("\nIntroduza o tamanho da sequencia: ");
   tamanho = LeNaturalPositivo();

   stream = fopen(NOME_ARQ, "w"); /* Tenta abrir o arquivo */

   if (!stream) { /* Verifica se o arquivo foi aberto */
      printf("\nArquivo nao pode ser criado");
      return 1;
   }

      /* Gera e escreve a sequência no arquivo */
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
