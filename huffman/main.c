/****
 *
 * Título: Codificação de Huffman
 *
 * Seção 10.5.1
 *
 * Autor: Ulysses de Oliveira
 *
 * Data de Criação: 16/04/2015
 * Última modificação: 08/10/2017
 *
 * Descrição: Implementa a Codificação de Huffman
 *
 * Entrada: Nome de um arquivo a ser codificado
 *
 * Saída: Arquivo contendo a codificação
 *
 ****/

/*********************** Includes *************************/

#include <stdio.h>    /* Entrada e saída              */
#include <string.h>   /* Processamento de strings     */
#include <stdlib.h>   /* Alocação dinâmica            */
#include <time.h>     /* time_t e time()              */
#include <ctype.h>    /* Classificação de caracteres  */
#include "ASSEGURA.h" /* Tratamento de exceções       */
#include "Huffman.h"  /* Codificação de Huffman       */

/******************** Funções de Locais *******************/

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
void MedidaDeTempo(void)
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
 * NomeCodificacao(): Cria o nome de um arquivo de codificação
 *
 * Parâmetros:
 *     nomeOriginal (entrada) - nome do arquivo original
 *
 * Retorno: Nome de um arquivo de codificação
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
 * NomeDecodificacao(): Cria o nome de um arquivo de decodificação
 *
 * Parâmetros:
 *     nomeOriginal (entrada) - nome do arquivo original
 *
 * Retorno: Nome de um arquivo de decodificação
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

/********************* Função main() **********************/

/****
 *
 * main(): Testa as funções da codificação canônica de Huffman
 *
 * Parâmetros:
 *     argc (entrada) - Número de argumentos de linha de comando
 *     argv (entrada) - Array de strings presentes na linha de
 *                      comando quando o programa o programa é
 *                      executado
 *
 * Retorno: Zero, se não ocorrer nenhum erro.
 *          Um valor diferente de zero em caso contrário.
 *
 * Observação: Se algum valor introduzido em linha de comando não
 *             puder ser convertido, a soma não será calculada.
 *
 ****/
int main(int argc, char *argv[])
{
   int     codifica, decodifica;
   char   *nomeArq;

      /* Deve haver pelo menos três argumentos */
      /* de linha de comando                   */
   if (argc < 3) {
      printf( "\n\t>>> Este programa deve ser usado assim:"
              "\n\t>>> %s -c|-d <nome-de-arquivo>", argv[0]);
      return 1;
   }

   codifica = !strcmp(argv[1], "-c") || !strcmp(argv[1], "-C");
   decodifica = !strcmp(argv[1], "-d") || !strcmp(argv[1], "-D");

   if (codifica) {
         /* Cria o nome do arquivo que conterá a codificação */
      nomeArq = NomeCodificacao(argv[2]);

      MedidaDeTempo();
      printf("\n\t>>> Codificando o arquivo \"%s\"...", argv[2]);

         /* Cria a codificação de Huffman */
      CodificaHuff(argv[2], nomeArq);

      printf("\n...codificacao concluida com sucesso\n");
      MedidaDeTempo();

      printf( "\n\t>>> A codificacao foi gravada no arquivo "
              "\"%s\"\n\n", nomeArq );

         /* Nome do arquivo não é mais necessário */
      free(nomeArq);
   } else if (decodifica) {
         /* Cria o nome do arquivo que conterá a decodificação */
      nomeArq = NomeDecodificacao(argv[2]);

      MedidaDeTempo();
      printf("\n\t>>> Decodificando o arquivo \"%s\"...", argv[2]);

         /* DecodificaHuff o arquivo */
      DecodificaHuff(argv[2], nomeArq);

      printf("\n...decodificacao concluida com sucesso\n");
      MedidaDeTempo();

      printf( "\n\t>>> A decodificacao foi gravada no arquivo "
              "\"%s\"\n\n", nomeArq );

         /* Nome do arquivo não é mais necessário */
      free(nomeArq);
   } else {
      printf("\nOpcao desconhecida");
      return 1;
   }

   return 0;
}

/******************* Exemplo de Execução ******************

gdb --args Secao09_5_1 -c DNA.txt

**********************************************************/
