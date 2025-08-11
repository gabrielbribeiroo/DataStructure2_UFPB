/****
 *
 * Descrição: Apresenta os nós de uma árvore multidirecional
 *            de busca num arquivo de texto
 *
 * Observação: Este programa é usado com os propósitos únicos
 *             de depuração e ganho de melhor comprensão do
 *             programa de árvores B. Ele não contém tratamento
 *             de exceção ou comentários adequados. Ou pior,
 *             muitos comentários não correspondem aquilo que
 *             é comentado.
 *
 * Autor: Ulysses de Oliveira
 *
 * Data de Criação: 15/03/2017
 * Última modificação: 15/03/2017
 *
 * Entrada: Arquivo cujo nome é especificado pela constante
 *          NOME_ARQ_REG
 *
 * Saída: Arquivo cujo nome é especificado pela constante
 *        NOME_ARQ_TXT
 *
 ****/

#include <stdio.h>
#include <stdlib.h>

/************ Definições de Tipos e Constantes ************/

typedef int tChave;

   /* Posição nula (inválida) em arquivo */
#define POSICAO_NULA -1

   /* Tipo do par chave/índice armazenado nos nós */
typedef struct  {
          tChave chave;  /* Chave do registro              */
          int    indice; /* Posição do registro no arquivo */
} tChaveIndice;

#define TB   4096 /* Tamanho do bloco lido/escrito  */
#define TCI   sizeof(tChaveIndice)  /* Tamanho de um    */
                                   /* par chave/índice */
#define TI   sizeof(int) /* Tamanho de um filho e do inteiro */
                         /* que representa o grau do nó      */

   /* Cálculo do grau da árvore */
//#define G  ((TB + TCI - TI)/(TCI + TI) - 1)
#define G 5

   /* Tipo de um nó da árvore de busca      */
   /* multidirecional armazenada em arquivo */
typedef struct {
           int          nFilhos; /* O número de filhos do nó */
           tChaveIndice chaves[G - 1];/* array contendo chaves  */
                                      /* e índices de registros */
           int          filhos[G];/* array contendo as posições */
                                  /*  dos filhos no arquivo */
                                  /* que contém a árvore    */
        } tNoMulti2, *tArvore;

   /* Nome do arquivo que contém a árvore */
#define NOME_ARQ_REG "ArvoreMulti.dat"

   /* Nome do arquivo de texto contendo registros */
#define NOME_ARQ_TXT "ArvoreMultiTeste.txt"

/************************* Funções ************************/

void EscreveArrayChavesIndices(const tChaveIndice ci[],
                               int n, FILE *stream)
{
   int i;

   fprintf(stream, "\nChaves e posicoes = { ");

   for (i = 0; i < n; ++i) {
      fprintf(stream, "(%d, %d) ", ci[i].chave, ci[i].indice);
   }

   fprintf(stream, "}");
}

void EscreveArrayFilhos(const int filhos[], int n, FILE *stream)
{
   int i;

   fprintf(stream, "\nFilhos = {%d", filhos[0]);

   for (i = 1; i < n; ++i) {
      fprintf(stream, ", %d", filhos[i]);
   }

   fprintf(stream, "}");
}

void ExibeNo(const tNoMulti2 *no, FILE *stream)
{
   fprintf(stream, "Numero de chaves: %d", no->nFilhos - 1);
   fprintf(stream, "\nNumero de filhos: %d", no->nFilhos);
   EscreveArrayChavesIndices(no->chaves, no->nFilhos - 1, stream);
   EscreveArrayFilhos(no->filhos, no->nFilhos, stream);
}

/****
 *
 * main(): Cria um arquivo de texto contendo informações
 *         sobre uma árvore B
 *
 * Parâmetros: Nenhum
 *
 * Retorno: Zero
 *
 ****/
int main(void)
{
   int   i = 0,
         nNos = 0;
   tNoMulti2   no;
   FILE *streamArv = fopen(NOME_ARQ_REG, "rb"),
        *streamTxt = fopen(NOME_ARQ_TXT, "w");

   while (1) {
      fread(&no, sizeof(no), 1, streamArv);

      if (feof(streamArv) || ferror(streamArv)) {
         break;
      }

      fprintf(streamTxt, "%d.\n\n", i++);

      ExibeNo(&no, streamTxt);

      ++nNos;

      fprintf(streamTxt, "\n\n----------\n\n");
   }

   fprintf(streamTxt, "\t>>> Grau da arvore (G): %d", G);
   fprintf(streamTxt, "\n\t>>> Numero total de nos: %d\n\n", nNos);

   fclose(streamArv);
   fclose(streamTxt);

   printf("\n\t>>> Resultado escrito no arquivo: \n\t\t\"%s\"\n",
          NOME_ARQ_TXT);

   return 0;
}
