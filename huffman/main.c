/****
 *
 * T�tulo: Codifica��o de Huffman
 *
 * Se��o 10.5.1
 *
 * Autor: Ulysses de Oliveira
 *
 * Data de Cria��o: 16/04/2015
 * �ltima modifica��o: 08/10/2017
 *
 * Descri��o: Implementa a Codifica��o de Huffman
 *
 * Entrada: Nome de um arquivo a ser codificado
 *
 * Sa�da: Arquivo contendo a codifica��o
 *
 ****/

/*********************** Includes *************************/

#include <stdio.h>    /* Entrada e sa�da              */
#include <string.h>   /* Processamento de strings     */
#include <stdlib.h>   /* Aloca��o din�mica            */
#include <time.h>     /* time_t e time()              */
#include <ctype.h>    /* Classifica��o de caracteres  */
#include "ASSEGURA.h" /* Tratamento de exce��es       */
#include "Huffman.h"  /* Codifica��o de Huffman       */

/******************** Fun��es de Locais *******************/

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
void MedidaDeTempo(void)
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
 * NomeCodificacao(): Cria o nome de um arquivo de codifica��o
 *
 * Par�metros:
 *     nomeOriginal (entrada) - nome do arquivo original
 *
 * Retorno: Nome de um arquivo de codifica��o
 *
 ****/
char *NomeCodificacao(const char *nomeOriginal)
{
   size_t tamOriginal = strlen(nomeOriginal),
          tamNovo = tamOriginal + 5;
   char *nomeCod = malloc(tamNovo);

   ASSEGURA(nomeCod, "Nome de arquivo nao pode ser criado");

   return strcat(strcpy(nomeCod, nomeOriginal), ".huf");
}

/****
 *
 * NomeDecodificacao(): Cria o nome de um arquivo de decodifica��o
 *
 * Par�metros:
 *     nomeOriginal (entrada) - nome do arquivo original
 *
 * Retorno: Nome de um arquivo de decodifica��o
 *
 ****/
char *NomeDecodificacao(const char *nomeOriginal)
{
   size_t tamOriginal = strlen(nomeOriginal),
          tamNovo = tamOriginal - 4;
   char  *nomeNovo = malloc(tamNovo + 1);

   ASSEGURA(nomeNovo, "Nome de arquivo nao pode ser criado");

   strncpy(nomeNovo, nomeOriginal, tamNovo);

   nomeNovo[tamNovo] = '\0';

   return nomeNovo;
}

/********************* Fun��o main() **********************/

/****
 *
 * main(): Testa as fun��es da codifica��o can�nica de Huffman
 *
 * Par�metros:
 *     argc (entrada) - N�mero de argumentos de linha de comando
 *     argv (entrada) - Array de strings presentes na linha de
 *                      comando quando o programa o programa �
 *                      executado
 *
 * Retorno: Zero, se n�o ocorrer nenhum erro.
 *          Um valor diferente de zero em caso contr�rio.
 *
 * Observa��o: Se algum valor introduzido em linha de comando n�o
 *             puder ser convertido, a soma n�o ser� calculada.
 *
 ****/
int main(int argc, char *argv[])
{
   int     codifica, decodifica;
   char   *nomeArq;

      /* Deve haver pelo menos tr�s argumentos */
      /* de linha de comando                   */
   if (argc < 3) {
      printf( "\n\t>>> Este programa deve ser usado assim:"
              "\n\t>>> %s -c|-d <nome-de-arquivo>", argv[0]);
      return 1;
   }

   codifica = !strcmp(argv[1], "-c") || !strcmp(argv[1], "-C");
   decodifica = !strcmp(argv[1], "-d") || !strcmp(argv[1], "-D");

   if (codifica) {
         /* Cria o nome do arquivo que conter� a codifica��o */
      nomeArq = NomeCodificacao(argv[2]);

      MedidaDeTempo();
      printf("\n\t>>> Codificando o arquivo \"%s\"...", argv[2]);

         /* Cria a codifica��o de Huffman */
      CodificaHuff(argv[2], nomeArq);

      printf("\n...codificacao concluida com sucesso\n");
      MedidaDeTempo();

      printf( "\n\t>>> A codificacao foi gravada no arquivo "
              "\"%s\"\n\n", nomeArq );

         /* Nome do arquivo n�o � mais necess�rio */
      free(nomeArq);
   } else if (decodifica) {
         /* Cria o nome do arquivo que conter� a decodifica��o */
      nomeArq = NomeDecodificacao(argv[2]);

      MedidaDeTempo();
      printf("\n\t>>> Decodificando o arquivo \"%s\"...", argv[2]);

         /* DecodificaHuff o arquivo */
      DecodificaHuff(argv[2], nomeArq);

      printf("\n...decodificacao concluida com sucesso\n");
      MedidaDeTempo();

      printf( "\n\t>>> A decodificacao foi gravada no arquivo "
              "\"%s\"\n\n", nomeArq );

         /* Nome do arquivo n�o � mais necess�rio */
      free(nomeArq);
   } else {
      printf("\nOpcao desconhecida");
      return 1;
   }

   return 0;
}

/******************* Exemplo de Execu��o ******************

gdb --args Secao09_5_1 -c DNA.txt

**********************************************************/
