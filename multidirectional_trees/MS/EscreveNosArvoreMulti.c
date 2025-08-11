/****
 *
 * Descri��o: Apresenta os n�s de uma �rvore multidirecional
 *            de busca num arquivo de texto
 *
 * Observa��o: Este programa � usado com os prop�sitos �nicos
 *             de depura��o e ganho de melhor comprens�o do
 *             programa de �rvores B. Ele n�o cont�m tratamento
 *             de exce��o ou coment�rios adequados. Ou pior,
 *             muitos coment�rios n�o correspondem aquilo que
 *             � comentado.
 *
 * Autor: Ulysses de Oliveira
 *
 * Data de Cria��o: 15/03/2017
 * �ltima modifica��o: 15/03/2017
 *
 * Entrada: Arquivo cujo nome � especificado pela constante
 *          NOME_ARQ_REG
 *
 * Sa�da: Arquivo cujo nome � especificado pela constante
 *        NOME_ARQ_TXT
 *
 ****/

#include <stdio.h>
#include <stdlib.h>

/************ Defini��es de Tipos e Constantes ************/

typedef int tChave;

   /* Posi��o nula (inv�lida) em arquivo */
#define POSICAO_NULA -1

   /* Tipo do par chave/�ndice armazenado nos n�s */
typedef struct  {
          tChave chave;  /* Chave do registro              */
          int    indice; /* Posi��o do registro no arquivo */
} tChaveIndice;

#define TB   4096 /* Tamanho do bloco lido/escrito  */
#define TCI   sizeof(tChaveIndice)  /* Tamanho de um    */
                                   /* par chave/�ndice */
#define TI   sizeof(int) /* Tamanho de um filho e do inteiro */
                         /* que representa o grau do n�      */

   /* C�lculo do grau da �rvore */
//#define G  ((TB + TCI - TI)/(TCI + TI) - 1)
#define G 5

   /* Tipo de um n� da �rvore de busca      */
   /* multidirecional armazenada em arquivo */
typedef struct {
           int          nFilhos; /* O n�mero de filhos do n� */
           tChaveIndice chaves[G - 1];/* array contendo chaves  */
                                      /* e �ndices de registros */
           int          filhos[G];/* array contendo as posi��es */
                                  /*  dos filhos no arquivo */
                                  /* que cont�m a �rvore    */
        } tNoMulti2, *tArvore;

   /* Nome do arquivo que cont�m a �rvore */
#define NOME_ARQ_REG "ArvoreMulti.dat"

   /* Nome do arquivo de texto contendo registros */
#define NOME_ARQ_TXT "ArvoreMultiTeste.txt"

/************************* Fun��es ************************/

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
 * main(): Cria um arquivo de texto contendo informa��es
 *         sobre uma �rvore B
 *
 * Par�metros: Nenhum
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
