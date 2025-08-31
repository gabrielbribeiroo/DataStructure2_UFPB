/****
 *
 * Arquivo ArvoreBM.c
 *
 * Autor: Ulysses de Oliveira
 *
 * Data de Criação: 07/08/2012
 * Última modificação: 25/05/2018
 *
 * Descrição: Implementação das funções que efetuam operações de
 *            busca e inserção usando árvore B+ armazenada em
 *            arquivo
 *
 ****/

/*********************** Includes *************************/

#include <stdio.h>     /* Funções de E/S                 */
#include "ArvoreBM.h"  /* Interface deste módulo         */
#include "Pilha.h"     /* Pilha usada para armazenar     */
                       /* caminhadas na árvore           */
#include "Registros.h" /* Interface do módulo Registros  */
#include "ASSEGURA.h"  /* Macro ASSEGURA                 */
#include "Leitura.h"   /* Funções de leitura via teclado */

/**************** Compilação Condidicional ****************/

#define DEPURACAO 1

/******************** Funções de Locais *******************/

/****
 *
 * IniciaNoBM(): Inicia o tipo do nó e o número de chaves (folhas)
 *               ou filhos (nós internos)
 *
 * Parâmetros:
 *     tipoDoNo (entrada) - o tipo do nó (folha/interno) que será
 *                          iniciado
 *    *pNo (saída) - o nó que será iniciado
 *
 * Retorno: Nada
 *
 ****/
static void IniciaNoBM(tTipoDoNo tipoDoNo, tNoBM *pNo)
{
   int i;

   ASSEGURA( tipoDoNo == FOLHA || tipoDoNo == INTERNO,
             "Tipo de no nao e' folha nem interno em IniciaNoBM" );

   pNo->tipoDoNo = tipoDoNo;

   if (tipoDoNo == FOLHA) {
      pNo->conteudo.noFolha.nChaves = 0;
      pNo->conteudo.noFolha.proximaFolha = POSICAO_NULA;
   } else {
      pNo->conteudo.noInterno.nFilhos = 0;

         /* Inicia todos o possíveis filhos do nó */
      for (i = 0; i < G; ++i) {
         pNo->conteudo.noInterno.filhos[i] = POSICAO_NULA;
      }
   }
}

/****
 *
 * EscreveNoBM(): Escreve um nó de uma árvore B+ em arquivo
 *
 * Parâmetros:
 *     stream (entrada) - stream associado ao arquivo contendo a
 *                        árvore no qual será feita a escrita
 *     pos (entrada) - posição no arquivo no qual será feita a
 *                     escrita
 *    *no (entrada) - nó que será escrito no arquivo
 *
 * Retorno: Nada
 *
 ****/
static void EscreveNoBM(FILE *stream, int pos, const tNoBM *no)
{
      /* Verifica se o stream é válido */
   ASSEGURA(stream, "Erro em EscreveNoBM(): stream e' NULL");

      /* Tenta mover o apontador de arquivo para o local */
      /* de escrita; se não conseguir aborta o programa  */
   MoveApontador(stream, sizeof(tNoBM)*pos, SEEK_SET);

      /* Efetua a escrita */
   fwrite(no, sizeof(tNoBM), 1, stream);

      /* Certifica-se que não houve erro */
   ASSEGURA( !ferror(stream), ERRO_FWRITE(EscreveNoBM) );
}

/****
 *
 * AcrescentaNoBM(): Acrescenta um nó de uma árvore B+ ao final do
 *                   arquivo que contém a árvore
 *
 * Parâmetros:
 *     stream (entrada) - stream associado ao arquivo contendo a
 *                        árvore na qual será feita a inserção
 *    *no (entrada) - ponteiro para o nó que será escrito
 *
 * Retorno: Posição na qual foi feita a inserção do nó no arquivo
 *
 ****/
static int AcrescentaNoBM(FILE *stream, const tNoBM *no)
{
   int posicao;

      /* Garante que stream é válido */
   ASSEGURA( stream,
             ERRO_STREAM_NULL(stream, AcrescentaNoBM) );

      /* Obtém a posição de inserção */
   posicao = NumeroDeRegistros(stream, sizeof(tNoBM));

      /* Tenta mover o apontador de posição */
      /* de arquivo para o local de escrita */
   MoveApontador(stream, 0, SEEK_END);

   fwrite(no, sizeof(tNoBM), 1, stream); /* Efetua a escrita */

      /* Certifica-se que não houve erro */
   ASSEGURA(!ferror(stream), ERRO_FWRITE(AcrescentaNoBM));

   return posicao;
}

/****
 *
 * BuscaEmNoBM(): Faz uma busca sequencial dentro de um
 *                nó (interno ou folha)
 *
 * Parâmetros:
 *     chave (entrada) - a chave de busca
 *     no (entrada) - ponteiro para o nó no qual será feita a busca
 *    *encontrado (saída) - informará se a chave foi encontrada
 *
 * Retorno: O índice da chave dentro do nó se ela for encontrada
 *          uma chave maior do que ou igual a chave de busca,
 *          ou o número de chaves, se tal chave não for
 *          encontrada; i.e., se a chave de busca for maior
 *          do que qualquer chave do nó.
 *
 ****/
static int BuscaEmNoBM( tChave chave, const tNoBM *no,
                        int *encontrado )
{
   int i, nChaves;

      /* Ponteiro 'no' é válido? */
   ASSEGURA(no, "ERRO: No' e' NULL em BuscaEmNoBM()");

      /* Obtém o número de chaves do nó */
   nChaves = no->tipoDoNo == INTERNO
             ? no->conteudo.noInterno.nFilhos - 1
             : no->conteudo.noFolha.nChaves;

      /* Efetua a busca de acordo com o tipo do nó */
   if (no->tipoDoNo == INTERNO) {
      for (i = 0; i < nChaves; ++i)
         if (chave < no->conteudo.noInterno.chaves[i]) {
            *encontrado = 0;
            return i; /* Posição na qual a chave deveria estar */
         } else if ( chave == no->conteudo.noInterno.chaves[i] ) {
            *encontrado = 1;
            return i; /* Posição na qual a chave está */
         }
   } else {
      for (i = 0; i < nChaves; ++i) {
            /* Verifica se a chave foi encontrada */
         if (chave < no->conteudo.noFolha.chaves[i].chave) {
            *encontrado = 0;
            return i;/* Posição na qual a chave deveria estar */
         } else if ( chave ==
                     no->conteudo.noFolha.chaves[i].chave ) {
            *encontrado = 1;
            return i; /* Posição na qual a chave está */
         }
      }
   }

   /****                                              ****/
   /* Neste ponto, sabe-se que não foi encontrada uma    */
   /* chave no nó maior do que ou igual à chave de busca */
   /****                                              ****/

   *encontrado = 0;

   return nChaves;
}

/****
 *
 * EncontraCaminhoBM(): Armazena numa pilha o caminho da raiz até
 *                      a folha na qual se encontra uma chave de
 *                      busca ou na qual ela deve ser inserida
 *
 * Parâmetros:
 *     raiz (entrada) - ponteiro para a raiz da árvore
 *                      (armazenada em memória)
 *     streamArvore (entrada) - stream associado ao arquivo
 *                              que contém a árvore
 *     chave (entrada) - a chave de busca
 *    *posicao (saída) - posição da chave no último nó do caminho
 *    *encontrado (saída) - informa se a chave foi encontrada
 *                          (1) ou não (0)
 *    *pilha (saída) - pilha que armazenará o caminho de nós
 *                     visitados
 *
 * Retorno: Nada
 *
 ****/
static void EncontraCaminhoBM( FILE *streamArvore, tChave chave,
                               const tNoBM *raiz, int *posicao,
                               int *encontrado, tPilha *pilha )
{
   tNoBM      no; /* Nó usado para armazenar os nós visitados */
   int        indiceNo; /* Índice do nó visitado */
                        /* entre seus irmãos     */
   tNoCaminho noCaminho;  /* Elemento armazenado na pilha */
   int        posNo; /* Posição de um nó no arquivo */

      /* Certifica-se que a raiz não é NULL */
   ASSEGURA(raiz, "ERRO: Arvore vazia em EncontraCaminhoBM");

      /* Certifica-se que o stream não é NULL */
   ASSEGURA( streamArvore,
             ERRO_STREAM_NULL(streamArvore, EncontraCaminhoBM) );

   no = *raiz;   /* O primeiro nó visitado é a raiz da árvore */
   posNo = 0; /* A raiz é sempre mantida no início do arquivo */
   indiceNo = 0;    /* A raiz não tem irmãos */
   *encontrado = 0; /* A chave ainda não foi encontrada */

   CriaPilha(pilha); /* Inicia a pilha */

   while (no.tipoDoNo == INTERNO) {
         /***                                ***/
         /*** Cria o item que será empilhado ***/
         /***                                ***/
      noCaminho.no = no;          /* Armazena o nó,...           */
      noCaminho.endereco = posNo; /* sua posição no arquivo e... */
      noCaminho.pos = indiceNo;  /* seu índice entre seus irmãos */

         /* Empilha as informações sobre o nó corrente */
      Empilha(noCaminho, pilha);

         /* Encontra o índice do filho da próxima descida */
      indiceNo = BuscaEmNoBM(chave, &no, encontrado);

         /* Se a chave foi encontrada num nó interno, */
         /* a descida é pelo filho direito dele       */
      if (*encontrado)
         ++indiceNo;

         /* Desce até o próximo nó */
      posNo = no.conteudo.noInterno.filhos[indiceNo];

         /* Lê o próximo nó no arquivo */
      LeNoBM(streamArvore, posNo, &no);
   }

   /****                                                   ****/
   /* Neste ponto, sabe-se que 'no' é uma folha.              */
   /* Se a chave encontrar-se na árvore será nessa folha.     */
   /* Caso contrário, é nessa folha que ela deve ser inserida */
   /****                                                   ****/

      /* Se a chave for encontrada, 'posicao' apontará para o */
      /* índice da chave na folha. Caso contrário, apontará   */
      /* para o índice na folha na qual a chave será inserida */
   *posicao = BuscaEmNoBM(chave, &no, encontrado);

      /* Falta acrescentar a folha ao caminho de nós */
   noCaminho.no = no;          /* Armazena o nó,...            */
   noCaminho.endereco = posNo; /* ... seu endereço e...        */
   noCaminho.pos = indiceNo;   /* seu índice entre seus irmãos */

   Empilha(noCaminho, pilha);
}

/****
 *
 * InsereEmNoInterno(): Insere uma chave e seu respectivo filho
 *                      à direita num nó interno incompleto
 *
 * Parâmetros:
 *    *pNo (entrada/saída) - nó no qual será feita a inserção
 *     pos (entrada) - posição no array de chaves do nó no qual
 *                     a chave será inserida
 *     chave (entrada) - a chave que será inserida
 *     pFilho (entrada) - posição do filho a ser inserido à
 *                        direita da nova chave
 *
 * Retorno: Nada
 *
 * Observação: A atualização do nó modificado por esta função
 *             no arquivo que contém a árvore fica sob
 *             responsabilidade de quem chama esta função.
 *
 ****/
static void InsereEmNoInterno( tNoBM *pNo, int pos,
                               tChave chave, int pFilho )
{
   int i;

      /* Verifica se o ponteiro pNo é válido */
   ASSEGURA(pNo,"ERRO: Tentativa de inserir chave em no' nulo");

      /* Verifica se pNo aponta para um nó interno */
   ASSEGURA( pNo->tipoDoNo == INTERNO,
             "ERRO: No' e' folha em InsereEmNoInterno()" );

      /* Verifica se a posição de inserção é válida */
   ASSEGURA( pos >= 0 && pos < pNo->conteudo.noInterno.nFilhos,
             "ERRO: Posicao de insercao invalida "
             "em InsereEmNoInterno()" );

       /* Abre espaço para a nova chave */
   for (i = pNo->conteudo.noInterno.nFilhos - 1; i > pos; --i) {
      pNo->conteudo.noInterno.filhos[i + 1] =
         pNo->conteudo.noInterno.filhos[i];
      pNo->conteudo.noInterno.chaves[i] =
         pNo->conteudo.noInterno.chaves[i - 1];
   }

      /* Insere a nova chave e seu filho à direita */
   pNo->conteudo.noInterno.chaves[pos] = chave;
   pNo->conteudo.noInterno.filhos[pos + 1] = pFilho;

   pNo->conteudo.noInterno.nFilhos++; /* A prole aumentou */
}

/****
 *
 * InsereEmFolhaBM(): Insere uma chave e seu respectivo índice
 *                    de registro numa folha incompleta
 *
 * Parâmetros:
 *    *pNo (entrada/saída) - folha na qual será feita a inserção
 *     pos (entrada) - posição no array de chaves do nó no qual
 *                     a chave será inserida
 *     chaveIndice (entrada) - par chave/índice que será inserido
 *
 * Retorno: Nada
 *
 * Observação: A atualização da folha modificada por esta
 *             função no arquivo que contém a árvore fica
 *             sob responsabilidade de quem chama esta função
 *
 ****/
static void InsereEmFolhaBM( tNoBM *pNo, int pos,
                             tChaveIndice chaveIndice )
{
   int i;

      /* Verifica se pNo aponta para uma folha */
   ASSEGURA( pNo && pNo->tipoDoNo == FOLHA,
             "Tentativa de inserir chave em folha invalida" );

      /* Verifica se a posição de inserção é válida */
   ASSEGURA( pos >= 0 && pos <= pNo->conteudo.noFolha.nChaves,
             "ERRO: Posicao de insercao invalida "
             "em InsereEmFolhaBM()" );

      /* Abre espaço para a nova chave */
   for (i = pNo->conteudo.noFolha.nChaves; i > pos; --i)
              /* Chave aqui é estrutura! */
      pNo->conteudo.noFolha.chaves[i] =
         pNo->conteudo.noFolha.chaves[i - 1];

      /* Insere a nova chave */
   pNo->conteudo.noFolha.chaves[pos] = chaveIndice;

      /* O número de chaves aumentou */
   pNo->conteudo.noFolha.nChaves++;
}

/****
 *
 * CopiaChavesBM(): Copia chaves de um nó interno para outro
 *
 * Parâmetros:
 *    *pNo1 (entrada) - nó que doará as chaves que serão copiadas
 *    *pNo2 (saída) - nó que receberá as chaves copiadas
 *     indiceI (entrada) - índice da primeira chave a ser copiada
 *     indiceF (entrada) - índice da última chave a ser copiada
 *
 * Retorno: Nada
 *
 * Observação: A atualização dos nós modificados por esta
 *             função no arquivo que contém a árvore fica
 *             sob responsabilidade de quem chama esta função
 *
 ****/
static void CopiaChavesBM( const tNoBM *pNo1, tNoBM *pNo2,
                           int indiceI, int indiceF )
{
   int i,
       nChaves = indiceF - indiceI + 1; /* Número de chaves */
                                        /* a copiar         */

      /* Verifica se os dois nós são internos */
   ASSEGURA(pNo1->tipoDoNo==INTERNO && pNo2->tipoDoNo==INTERNO,
            "ERRO: Um no' e' interno e o outro e' folha em "
            "CopiaChavesBM()");

      /* Verifica se o número de chaves é válido */
   ASSEGURA( nChaves < G,
             "Erro: Excesso de chaves em CopiaChavesBM()" );

      /* Idem */
   ASSEGURA(nChaves > 0,
            "Erro: Nao ha chaves para copiar em CopiaChavesBM()");

      /* Copia chaves e filhos do nó apontado por */
      /* pNo1 para o nó apontado por pNo2         */
   for (i = 0; i < nChaves; ++i) {
      pNo2->conteudo.noInterno.chaves[i] =
              pNo1->conteudo.noInterno.chaves[indiceI + i];
      pNo2->conteudo.noInterno.filhos[i] =
              pNo1->conteudo.noInterno.filhos[indiceI + i];
   }

      /* Falta copiar o último filho */
   pNo2->conteudo.noInterno.filhos[nChaves] =
            pNo1->conteudo.noInterno.filhos[indiceI + nChaves];

      /* Atualiza o número de chaves do nó que recebeu */
      /* a doação. A atualização do doador (pNo1) é    */
      /* feita por quem chama esta função.             */
   pNo2->conteudo.noInterno.nFilhos = nChaves + 1;
}

/****
 *
 * CopiaChavesIndicesBM(): Copia chaves e respectivos índices de
 *                         uma folha para outra
 *
 * Parâmetros:
 *    *pNo1 (entrada) - folha que doará as chaves que serão
 *                      copiadas
 *    *pNo2 (saída) - folha que receberá as chaves copiadas
 *     indiceI (entrada) - índice da primeira chave a ser copiada
 *     indiceF (entrada) - índice da última chave a ser copiada
 *
 * Retorno: Nada
 *
 * Observação: A atualização das folhas modificadas por esta
 *             função no arquivo que contém a árvore fica sob
 *             responsabilidade de quem chama esta função
 *
 ****/
static void CopiaChavesIndicesBM( const tNoBM *pNo1, tNoBM *pNo2,
                                  int indiceI, int indiceF )
{
   int i,
       nChaves = indiceF - indiceI + 1; /* Número de chaves */
                                        /* a copiar         */

      /* Verifica se os dois nós são folhas */
   ASSEGURA( pNo1->tipoDoNo == FOLHA && pNo2->tipoDoNo == FOLHA,
            "ERRO: Um no' e' interno e o outro e' folha em "
            "CopiaChavesIndicesBM()");

      /* Verifica se o número de chaves é válido */
   ASSEGURA(nChaves <= F,
            "Erro: Excesso de chaves em CopiaChavesIndicesBM()");

      /* Idem */
   ASSEGURA(nChaves > 0, "Erro: Sem chaves para copiar em "
            "CopiaChavesIndicesBM()");

      /* Copia chaves e índices da folha */
      /* *pNo1 para a folha *pNo2        */
   for (i = 0; i < nChaves; ++i) {
      pNo2->conteudo.noFolha.chaves[i] =
              pNo1->conteudo.noFolha.chaves[indiceI + i];
   }

      /* Atualiza o número de chaves do nó que recebeu a */
      /* doação. A atualização do doador (pNo1) é feita  */
      /* por quem chama esta função.                     */
   pNo2->conteudo.noFolha.nChaves = nChaves;
}

/****
 *
 * DivideNoInternoBM(): Insere uma chave num nó interno completo de
 *                      uma árvore B+, criando um novo nó para a
 *                      metade superior das chaves e mantendo a
 *                      metade inferior das chaves no nó dividido
 *
 * Parâmetros:
 *    *pNoDiv (entrada/saída) - nó que será dividido
 *     pos (entrada) - posição na qual a nova chave será inserida
 *     chave (entrada) - a chave que será inserida
 *     pFilhoDireita (entrada) - posição em arquivo do nó que
 *                               será o filho à direita da chave
 *                               inserida
 *    *pNoDireita (saída) - novo nó no qual serão colocadas as
 *                          chaves superiores do nó dividido
 *    *chaveQueSobe (saída) - a chave que subirá para um nó no
 *                            nível imediatamente superior
 *
 * Retorno: Nada
 *
 * Observações:
 *    1. Existe uma diferença fundamental entre esta função e
 *       a função DivideFolhaBM(): aqui, quando um nó é dividido,
 *       a chave que sobe é "movida" para um nó interno no
 *       próximo nível acima, enquanto que na função
 *       DivideFolhaBM() a chave que sobe é "copiada" de uma
 *       folha para um nó interno.
 *    2. Se o grau da árvore for par, a árvore (i.e., cada nó
 *       interno) terá tendência à esquerda; i.e., o novo
 *       dividido terá uma chave a mais do que o novo nó. Não
 *       confunda isto com a tendência das folhas que é à direita
 *    3. A atualização dos nós modificados por esta função no
 *       arquivo que contém a árvore fica sob responsabilidade
 *       de quem chama esta função.
 *
 ****/
static void DivideNoInternoBM( tNoBM *pNoDiv, int pos,
                               tChave chave, int pFilhoDireita,
                               tNoBM *pNoDireita,
                               tChave *chaveQueSobe )
{
   static const int meio = (G - 1)/2; /* Evita recalcular este */
                                      /* valor a cada chamada  */
                                      /* desta função          */

   ASSEGURA( pNoDiv->tipoDoNo == INTERNO, "Erro: Tentativa de "
             "dividir folha usando DivideNoInternoBM()" );

      /* Inicia o novo nó que receberá as chaves superiores */
   IniciaNoBM(INTERNO, pNoDireita);

      /* Efetua a inserção com a devida divisão de nós */
   if (pos > meio) { /* Caso 1: A nova chave pertence */
                     /* ao novo nó da direita         */
         /* Copia metade menos uma chave  */
         /* do nó dividido para o novo nó */
      CopiaChavesBM(pNoDiv, pNoDireita, meio + 1, G - 2);

         /* Insere a nova chave no novo nó da direita */
      InsereEmNoInterno( pNoDireita, pos - meio - 1, chave,
                         pFilhoDireita );

         /* A chave que subirá é a última chave do */
         /* nó que foi dividido depois da divisão  */
      *chaveQueSobe = pNoDiv->conteudo.noInterno.chaves[meio];

         /* Ajusta o número de nós do nó dividido */
      pNoDiv->conteudo.noInterno.nFilhos = meio + 1;
   } else if (pos == meio) { /* Caso 2: A nova chave é     */
                             /* aquela que subirá um nível */
         /* Copia metade das chaves do */
         /* nó dividido para o novo nó */
      CopiaChavesBM(pNoDiv, pNoDireita, meio, G - 2);

         /* Ajusta o número de nós do nó dividido */
      pNoDiv->conteudo.noInterno.nFilhos = meio + 1;

         /* O filho direito da nova chave passa */
         /* a ser o primeiro filho do novo nó   */
      pNoDireita->conteudo.noInterno.filhos[0] = pFilhoDireita;

         /* A chave que subirá é a nova chave */
      *chaveQueSobe = chave;
   } else { /* Caso 3: A nova chave pertence */
            /* ao nó que será dividido       */
         /* Copia metade das chaves do */
         /* nó dividido para o novo nó */
      CopiaChavesBM(pNoDiv, pNoDireita, meio, G - 2);

         /* Ajusta o número de nós do nó dividido */
      pNoDiv->conteudo.noInterno.nFilhos = meio + 1;

         /* Insere a nova chave no novo nó da direita */
      InsereEmNoInterno(pNoDiv, pos, chave, pFilhoDireita);

         /* A chave que subirá é a última chave do */
         /* nó que foi dividido depois da divisão  */
      *chaveQueSobe = pNoDiv->conteudo.noInterno.chaves[meio];

         /* Corrige o número de filhos do nó dividido, pois    */
         /* sua última chave subirá para o próximo nível acima */
      --pNoDiv->conteudo.noInterno.nFilhos;
   }
}

/****
 *
 * DivideFolhaBM(): Insere um par chave/índice numa folha e
 *                  divide este nó, criando um novo nó para a
 *                  metade superior das chaves e mantendo a
 *                  metade inferior das chaves no nó dividido
 *
 * Parâmetros:
 *    *pNoDiv (entrada/saída) - folha que será dividida
 *     pos (entrada) - posição na qual a nova chave será inserida
 *     chaveIndice (entrada) - par chave/índice que será inserido
 *    *pNoDireita (saída) - novo nó no qual serão colocadas as
 *                          chaves superiores do nó dividido
 *    *chaveQueSobe (saída) - a chave que será copiada para o
 *                            nó-pai do nó dividido; o filho
 *                            direito desta chave será pNoDireita
 *
 * Retorno: Nada
 *
 * Observações:
 *    1. Existe uma diferença fundamental entre esta função e
 *       a função DivideNoInternoBM(): aqui, quando uma folha é
 *       dividida, a chave que sobe é "copiada" para um nó
 *       interno, enquanto que na função DivideNoInternoBM() a
 *       chave que sobe é "movida" de um nó interno para outro
 *       um nível acima.
 *    2. Se o número de chaves for par, as folhas terão tendência
 *       à direita; i.e., o novo nó terá uma chave a mais do que
 *       o nó dividido. Não confunda isto com a tendência da
 *       própria árvore (i.e., de cada nó interno) que é à
 *       esquerda.
 *    3. A atualização dos nós modificados por esta função no
 *       arquivo que contém a árvore fica sob responsabilidade
 *       de quem chama esta função.
 *    4. O encadeamento das folhas fica sob responsabilidade
 *       de quem chama esta função.
 *
 ****/
static void DivideFolhaBM( tNoBM *pNoDiv, int pos,
                           tChaveIndice chaveIndice,
                           tNoBM *pNoDireita,
                           tChave *chaveQueSobe )
{
   static const int meio = F/2;  /* Evita recalcular esses */
   static const int resto = F%2; /* valores a cada chamada */

      /* Certifica-se que pNoDiv aponta para uma folha */
   ASSEGURA( pNoDiv->tipoDoNo == FOLHA, "Erro: Tentativa de "
             "dividir um no' interno com DivideFolhaBM()" );

      /* Inicia o novo nó que receberá as chaves superiores */
   IniciaNoBM(FOLHA, pNoDireita);

      /* Divide o nó e efetua a inserção no devido nó */
   if (pos > meio) { /* Caso 1: A nova chave pertencerá */
                     /* ao novo nó da direita           */
         /* Copia metade menos uma chave para o novo nó */
      CopiaChavesIndicesBM( pNoDiv, pNoDireita, meio + resto,
                            F - 1 );

         /* Ajusta o número de nós do   */
         /* número de nó do nó dividido */
      pNoDiv->conteudo.noFolha.nChaves = meio + resto;

         /* Insere a nova chave no novo nó */
      InsereEmFolhaBM(pNoDireita, pos - meio - resto, chaveIndice);

         /* A chave que sobe para um nó interno */
         /* é a primeira chave da folha direita */
      *chaveQueSobe =
           pNoDireita->conteudo.noFolha.chaves[0].chave;
   } else { /* Caso 2: A nova chave pertencerá ao nó dividido */
         /* Copia metade das chaves para o novo nó */
      CopiaChavesIndicesBM( pNoDiv, pNoDireita, meio, F - 1 );

         /* Antes de inserir a chave no nó dividido */
         /* deve-se reduzir seu número de chaves,   */
         /* pois neste instante ele está completo   */
      pNoDiv->conteudo.noFolha.nChaves = meio;

         /* Insere a nova chave no novo nó */
      InsereEmFolhaBM(pNoDiv, pos, chaveIndice);

         /* A chave que sobe para um nó interno */
         /* é a primeira chave da folha direita */
      *chaveQueSobe = pNoDireita->conteudo.noFolha.chaves[0].chave;
   }
}

/****
 *
 * JuntaNosBM(): Combina um nó interno com outros nós internos
 *               de uma árvore B+ quando ele fica com grau
 *               abaixo do mínimo
 *
 * Parâmetros:
 *      pNoN (entrada/saída) - ponteiro para o nó que será
 *                             combinado com outros
 *      posNo (entrada) - posição do nó entre seus irmãos
 *      endNo (entrada) - posição do nó no arquivo
 *     *pilha (entrada/saída) - pilha que armazena os nós
 *                              encontrados no caminho da raiz
 *                              até o nó que será combinado
 *                              (sem incluí-lo)
 *      stream (entrada) - stream associado ao arquivo que
 *                         contém a árvore
 *
 * Retorno: Nada
 *
 * Observações:
 *    1. Esta função é semelhante à função JuntaNos() para árvores B
 *
 *    2. Considerando as figuras apresentadas no texto, pNoN é
 *       um ponteiro para o nó N, pNoE é a posição no arquivo que
 *       contém a árvore do nó E (aqui denominado noE) e pNoD é a
 *       posição no mesmo arquivo do nó D (aqui denominado noD).
 *
 *    3. Quando se faz referência a uma variável que contém uma
 *       posição em arquivo, 'apontar para' não tem o mesmo
 *       significado dessa mesma expressão usada com ponteiros.
 *       I.e., aqui, 'apontar para' significa conter a posição
 *       de um nó no arquivo que o armazena.
 *
 *    4. V. figuras e algoritmo no texto. É muito difícil
 *       acompanhar esta função sem examinar cuidadosamente
 *       essas figuras.
 *
 ****/
static void JuntaNosBM( tNoBM *pNoN, int posNo, int endNo,
                        tPilha* pilha, FILE *stream )
{
   tNoCaminho caminho; /* Armazenará cada item desempilhado */
   tNoBM      noE, /* Nó E das figuras do texto */
              noD; /* Nó D das figuras do texto */
   int        pNoE = POSICAO_NULA,/* Posição em arquivo de noE */
              pNoD = POSICAO_NULA,/* Posição em arquivo de noD */
              i;
   tChave     sucessora; /* A chave sucessora à qual */
                         /* se refere o algoritmo    */

      /* Passo 1 do algoritmo */
   if (pNoN->conteudo.noInterno.nFilhos >= TG) {
         /* Nó não precisa ser combinado com outro */
      return;
   }

   /*                                                     */
   /* Neste ponto, sabe-se que o nó apontado por pNoN tem */
   /* um número de filhos menor do que o mínimo desejado  */
   /*                                                     */

      /* A pilha não pode estar vazia aqui */
   ASSEGURA(!PilhaVazia(*pilha), "Pilha vazia em JuntaNosBM()");

      /* O item no topo da pilha contém informações */
      /* sobre o pai do nó apontado por pNoN        */
   caminho = Desempilha(pilha);

      /* Se o nó a ser combinado não é o primeiro */
      /* filho, ele tem um irmão esquerdo         */
   if(posNo > 0)
         /* Faz pNoE apontar para o irmão    */
         /* esquerdo do nó apontado por pNoN */
      pNoE = caminho.no.conteudo.noInterno.filhos[posNo - 1];

      /* Se o nó a ser combinado não é o último */
      /* filho, ele tem um irmão direito        */
   if(posNo < caminho.no.conteudo.noInterno.nFilhos - 1)
         /* Faz pNoD apontar para o irmão   */
         /* direito do nó apontado por pNoN */
      pNoD = caminho.no.conteudo.noInterno.filhos[posNo + 1];

      /* O nó a ser combinado deve ter pelo menos um  */
      /* irmão mais próximo ou, então, ele deve ser a */
      /* raiz que não tem irmãos                      */
   ASSEGURA( pNoE != POSICAO_NULA || pNoD != POSICAO_NULA ||
             PilhaVazia(*pilha),
             "Erro em JuntaNosBM(): o no' nao possui irmao" );

      /* Passo 2: Verifique se o irmão esquerdo (apontado */
      /* por pNoE) do nó apontado por pNoN tem um número  */
      /* de chaves maior do que o mínimo                  */

      /* Verifica se o nó apontado por pNoN tem irmão esquerdo */
   if (pNoE != POSICAO_NULA) {
         /* Lê o irmão esquerdo noE */
      LeNoBM(stream, pNoE, &noE);

         /* Verifica se o irmão esquerdo pode doar uma chave */
      if (noE.conteudo.noInterno.nFilhos > TG) {
            /* Passo 2.1: Insira a sucessora da última */
            /* chave do nó noE no início das chaves do */
            /* nó apontado por pNoN                    */

            /* Obtém a sucessora que se encontra no nó-pai */
         sucessora = caminho.no.conteudo.noInterno.chaves[posNo-1];

             /* Abre espaço para o novo filho */
         for (i = pNoN->conteudo.noInterno.nFilhos - 1; i >= 0; --i)
            pNoN->conteudo.noInterno.filhos[i + 1] =
               pNoN->conteudo.noInterno.filhos[i];

             /* Abre espaço para a nova chave */
         for (i = pNoN->conteudo.noInterno.nFilhos - 2; i >= 0; --i)
            pNoN->conteudo.noInterno.chaves[i + 1] =
               pNoN->conteudo.noInterno.chaves[i];

            /* Coloca a sucessora em seu devido lugar */
         pNoN->conteudo.noInterno.chaves[0] = sucessora;

            /* Passo 2.2: Faça o filho esquerdo da primeira */
            /* chave de pNoN apontar para o filho direito   */
            /* da última chave de noE                       */
         pNoN->conteudo.noInterno.filhos[0] =
            noE.conteudo.noInterno.filhos
               [noE.conteudo.noInterno.nFilhos - 1];

            /* Passo 2.3: Substitua a sucessora da última */
            /* chave do nó noE por essa última chave      */
         caminho.no.conteudo.noInterno.chaves[posNo - 1] =
             noE.conteudo.noInterno.chaves[
                     noE.conteudo.noInterno.nFilhos - 2 ];

            /* O nó-pai que foi alterado é uma cópia daquele */
            /* que se encontra na árvore. Logo, é preciso    */
            /* atualizar o nó correspondente na árvore.      */
         EscreveNoBM(stream, caminho.endereco, &caminho.no);

            /* Passo 2.4: Decremente o número de filhos de noE */
         noE.conteudo.noInterno.nFilhos--;

            /* O nó noE precisa ser atualizado em arquivo */
         EscreveNoBM(stream, pNoE, &noE);

            /* Passo 2.5: Incremente o número de */
            /* filhos do nó apontado por pNoN    */
         pNoN->conteudo.noInterno.nFilhos++;

            /* O nó apontado por pNoN precisa */
            /* ser atualizado em arquivo      */
         EscreveNoBM(stream, endNo, pNoN);

            /* Passo 2.6: Encerre */
         return;
      }
   }

      /* Passo 3: Verifique se o irmão direito (apontado */
      /* por pNoD) do nó apontado por pNoN tem um número */
      /* de chaves maior do que o mínimo                 */
   if (pNoD != POSICAO_NULA) {
         /* Lê o irmão direito noD */
      LeNoBM(stream, pNoD, &noD);

         /* Verifica se o irmão direito */
         /* pode fazer doação de chaves */
      if (noD.conteudo.noInterno.nFilhos > TG) {
            /* Passo 3.1: Acrescente a sucessora da */
            /* última chave do nó apontado por pNoN */
            /* ao final das chaves desse nó         */

            /* Obtém a sucessora que se encontra no nó-pai */
         sucessora = caminho.no.conteudo.noInterno.chaves[posNo];

            /* Coloca a sucessora em seu devido lugar */
         pNoN->conteudo.noInterno.chaves
            [pNoN->conteudo.noInterno.nFilhos - 1] = sucessora;

            /* Passo 3.2: Faça o filho direito da última    */
            /* chave de pNoN apontar para o filho esquerdo  */
            /* da primeira chave de noD. Note que o número  */
            /* de filhos de pNoN ainda não foi incrementado */
         pNoN->conteudo.noInterno.filhos
            [pNoN->conteudo.noInterno.nFilhos] =
            noD.conteudo.noInterno.filhos[0];

            /* Passo 3.3: Substitua a sucessora da última chave */
            /* do nó pNoN pela primeira chave do nó noD         */
         caminho.no.conteudo.noInterno.chaves[posNo] =
               noD.conteudo.noInterno.chaves[0];

             /* Passo 3.4: Desloque cada chave e filho */
             /* do nó noD uma posição para trás        */
         for (i = 1; i <= noD.conteudo.noInterno.nFilhos - 1; ++i)
            noD.conteudo.noInterno.filhos[i - 1] =
               noD.conteudo.noInterno.filhos[i];

         for (i = 1; i <= noD.conteudo.noInterno.nFilhos - 2; ++i)
            noD.conteudo.noInterno.chaves[i - 1] =
               noD.conteudo.noInterno.chaves[i];

            /* O nó-pai que foi alterado é uma cópia daquele */
            /* que se encontra na árvore. Logo é preciso     */
            /* atualizar o nó correspondente na árvore.      */
         EscreveNoBM(stream, caminho.endereco, &caminho.no);

            /* Passo 3.5: Decremente o número */
            /* de filhos do nó noD            */
         noD.conteudo.noInterno.nFilhos--;

            /* Passo 3.6: Incremente o número de */
            /* filhos do nó apontado por pNoN    */
         pNoN->conteudo.noInterno.nFilhos++;

            /* O nó noD precisa ser atualizado em arquivo */
         EscreveNoBM(stream, pNoD, &noD);

            /* O nó apontado por pNoN precisa */
            /* ser atualizado em arquivo      */
         EscreveNoBM(stream, endNo, pNoN);

            /* Passo 3.7: Encerre */
         return;
      }
   }

   /*                                                     */
   /* Passo 4: Se nenhum irmão vizinho do nó apontado por */
   /* pNoN tiver um número de nós maior do que o mínimo   */
   /*                                                     */

      /* Passo 4.1: Verifique se o nó apontado */
      /* por pNoN tem irmão direito (pNoD) com */
      /* número de filhos igual ao mínimo      */
   if (pNoD != POSICAO_NULA) {
         /* Lê o irmão direito noD */
      LeNoBM(stream, pNoD, &noD);

         /* Garante que o número de filhos de noD é igual ao   */
         /* mínimo. Se esse número fosse maior do que o mínimo */
         /* ter-se-ía o Caso 3. Se esse número fosse menor do  */
         /* que o mínimo, não se teria uma árvore B+.          */
      ASSEGURA( noD.conteudo.noInterno.nFilhos == TG,
                "Numero de chaves em no' direito nao e' minimo" );

         /* Passo 4.1.1: Insira a chave sucessora (S) da  */
         /* última chave do nó apontado por pNoN ao final */
         /* das chaves do nó apontado por pNoN            */

         /* Obtém a sucessora */
      sucessora = caminho.no.conteudo.noInterno.chaves[posNo];

         /* Coloca a sucessora em seu devido lugar */
      pNoN->conteudo.noInterno.chaves
         [pNoN->conteudo.noInterno.nFilhos - 1] = sucessora;

         /* Passo 4.1.2: Faça o filho direito da última  */
         /* chave de pNoN apontar para  o filho esquerdo */
         /* da primeira chave de noD. Note que o número  */
         /* de filhos de pNoN ainda não foi incrementado */
      pNoN->conteudo.noInterno.filhos
         [pNoN->conteudo.noInterno.nFilhos] =
         noD.conteudo.noInterno.filhos[0];

         /* O número de chaves e filhos do */
         /* nó apontado por pNoN aumentou  */
      ++pNoN->conteudo.noInterno.nFilhos;

         /* Passo 4.1.3: Desloque cada chave do pai do nó    */
         /* apontado por pNoN e seu filho direito , a partir */
         /* da chave sucessora, para uma posição anterior    */
      for ( i = posNo + 1;
            i < caminho.no.conteudo.noInterno.nFilhos - 1; ++i )
         caminho.no.conteudo.noInterno.filhos[i] =
            caminho.no.conteudo.noInterno.filhos[i + 1];

      for ( i = posNo;
            i < caminho.no.conteudo.noInterno.nFilhos - 2; ++i )
         caminho.no.conteudo.noInterno.chaves[i] =
            caminho.no.conteudo.noInterno.chaves[i + 1];

         /* Passo 4.1.4: Decremente o número de filhos */
         /* do pai do nó apontado por pNoN             */
      --caminho.no.conteudo.noInterno.nFilhos;

         /* É preciso atualizar o nó */
         /* correspondente na árvore */
      EscreveNoBM(stream, caminho.endereco, &caminho.no);

         /* Passo 4.1.5: Acrescente todas as chaves e filhos */
         /* direitos correspondentes do nó noD ao final das  */
         /* chaves e filhos do nó apontado por pNoN          */
      for ( i = 0; i < noD.conteudo.noInterno.nFilhos - 1; ++i)
         pNoN->conteudo.noInterno.filhos
             [pNoN->conteudo.noInterno.nFilhos + i] =
               noD.conteudo.noInterno.filhos[i + 1];

      for ( i = 0; i <= noD.conteudo.noInterno.nFilhos - 2; ++i)
         pNoN->conteudo.noInterno.chaves
             [pNoN->conteudo.noInterno.nFilhos + i - 1] =
               noD.conteudo.noInterno.chaves[i];

         /* Passo 4.1.6: Atualize o número de */
         /* filhos do nó apontado por pNoN    */
      pNoN->conteudo.noInterno.nFilhos +=
          noD.conteudo.noInterno.nFilhos;

         /* Passo 4.1.7: Libere o nó noD. Não há como liberar */
         /* um nó no interior de um arquivo. Então, a melhor  */
         /* opção é indicar que esse é um nó vazio.           */
      noD.tipoDoNo = NO_VAZIO;

         /* O nó noD precisa ser atualizado em arquivo */
      EscreveNoBM(stream, pNoD, &noD);

         /* O nó apontado por pNoN precisa */
         /* ser atualizado em arquivo      */
      EscreveNoBM(stream, endNo, pNoN);

         /* Passo 4.1.8: Ajuste o pai do nó       */
         /* apontado por pNoN usando JuntaNosBM() */
      JuntaNosBM( &caminho.no, caminho.pos, caminho.endereco,
                  pilha, stream );

      return;
   }

      /* Passo 4.2: Verifique se o nó apontado */
      /* por pNoN tem irmão esquerdo (noE) com */
      /* número de filhos igual ao mínimo      */
   if (pNoE != POSICAO_NULA) {
         /* Lê o irmão esquerdo noE */
      LeNoBM(stream, pNoE, &noE);

         /* Garante que o número de filhos de noE é igual ao   */
         /* mínimo. Se esse número fosse maior do que o mínimo */
         /* ter-se-ía o Caso 2. Se esse número fosse menor do  */
         /* que o mínimo, não se teria uma árvore B+.          */
      ASSEGURA( noE.conteudo.noInterno.nFilhos == TG,
                "Numero de chaves em no' esquerdo nao e' minimo" );

         /* Passo 4.2.1: Insira ao final das chaves de noE */
         /* a chave sucessora da última chave de noE       */

         /* Obtém a sucessora */
      sucessora = caminho.no.conteudo.noInterno.chaves[posNo - 1];

         /* Coloca a sucessora em seu devido lugar */
      noE.conteudo.noInterno.chaves
         [noE.conteudo.noInterno.nFilhos - 1] = sucessora;

         /* Passo 4.2.2: Faça o filho direito da última  */
         /* chave de noE apontar para  o filho esquerdo  */
         /* da primeira chave de pNoN. Note que ainda    */
         /* não houve incremento no número de nós de noE */
      noE.conteudo.noInterno.filhos
         [noE.conteudo.noInterno.nFilhos] =
         pNoN->conteudo.noInterno.filhos[0];

         /* O número de chaves e filhos do nó noE aumentou */
      ++noE.conteudo.noInterno.nFilhos;

         /* Passo 4.2.3: Desloque cada chave do pai do nó   */
         /* apontado por pNoN e seu filho direito, a partir */
         /* da chave sucessora, para uma posição anterior   */
      for ( i = posNo;
            i < caminho.no.conteudo.noInterno.nFilhos - 1; ++i)
         caminho.no.conteudo.noInterno.filhos[i] =
            caminho.no.conteudo.noInterno.filhos[i + 1];

      for ( i = posNo - 1;
            i < caminho.no.conteudo.noInterno.nFilhos - 2; ++i)
         caminho.no.conteudo.noInterno.chaves[i] =
            caminho.no.conteudo.noInterno.chaves[i + 1];

         /* Passo 4.2.4: Decremente o número de filhos */
         /* do pai do nó apontado por pNoN             */
      --caminho.no.conteudo.noInterno.nFilhos;

         /* É preciso atualizar o nó correspondente na árvore */
      EscreveNoBM(stream, caminho.endereco, &caminho.no);

         /* Passo 4.2.5: Acrescente todas as chaves e filhos */
         /* direitos correspondentes do nó apontado por pNoN */
         /* ao final das chaves de noE                       */
      for (i = 0; i < pNoN->conteudo.noInterno.nFilhos - 1; ++i)
         noE.conteudo.noInterno.filhos
            [noE.conteudo.noInterno.nFilhos + i] =
            pNoN->conteudo.noInterno.filhos[i + 1];

      for (i = 0; i <= pNoN->conteudo.noInterno.nFilhos - 2; ++i)
         noE.conteudo.noInterno.chaves
            [noE.conteudo.noInterno.nFilhos + i - 1] =
            pNoN->conteudo.noInterno.chaves[i];

         /* Passo 4.2.6: Atualize o número de filhos de noE */
      noE.conteudo.noInterno.nFilhos +=
         pNoN->conteudo.noInterno.nFilhos;

         /* Passo 4.2.7: Libere o nó apontado por pNoN. */
         /* V. comentário acima sobre liberação de nós. */
      pNoN->tipoDoNo = NO_VAZIO;

         /* O nó noE precisa ser atualizado em arquivo */
      EscreveNoBM(stream, pNoE, &noE);

         /* O nó apontado por pNoN precisa */
         /* ser atualizado em arquivo      */
      EscreveNoBM(stream, endNo, pNoN);

         /* Passo 4.2.8: Ajuste o pai do nó     */
         /* apontado por pNoE usando JuntaNosBM() */
      JuntaNosBM( &caminho.no, caminho.pos, caminho.endereco,
                  pilha, stream );
   }
}

/****
 *
 * AtualizaIndice(): Atualiza os nós internos de uma árvore B+
 *                   após uma remoção de chave
 *
 * Parâmetros:
 *       streamArvore (entrada) - stream associado ao arquivo
 *                                que armazena a árvore
 *      *pilha (entrada/saída) - pilha contendo o caminho de nó
 *                               internos até a folha (sem incluí-
 *                               la) na qual ocorreu uma remoção
 *       removida (entrada) - chave que foi removida
 *       subst (entrada) - se não for NULL, aponta para uma chave
 *                         que substituirá a chave removida caso
 *                         ela seja encontrada num nó interno
 *       aRemover (entrada) - se não for NULL, aponta para uma
 *                            chave que será removida do índice
 *                            juntamente com seu filho direto
 *
 * Retorno: Nada
 *
 * Descrição do algoritmo: v. texto
 *
 ****/
static void AtualizaIndice( FILE *streamArvore, tPilha *pilha,
                            tChave removida, tChave *subst,
                            tChave *aRemover )
{
   int        i, j,
              nChaves; /* Número de chaves de um nó */
   tNoCaminho caminho; /* Um elemento da pilha que    */
                       /* representa o caminho de nós */
   tPilha     pilhaAux; /* Pilha auxiliar */

      /* Se houver chave a remover e a substituir no     */
      /* índice, será necessário usar uma pilha auxiliar */
   CriaPilha(&pilhaAux);

      /***                                 ***/
      /* A primeira parte desta função lida  */
      /* com substituição de chave no índice */
      /***                                 ***/

   while (subst && !PilhaVazia(*pilha)) {
      caminho = Desempilha(pilha);

      ASSEGURA( caminho.no.tipoDoNo == INTERNO,
                "Em AtualizaIndice(): No' deveria ser interno" );

         /* Obtém o número de chaves do nó corrente */
      nChaves = caminho.no.conteudo.noInterno.nFilhos - 1;

         /* Procura pela chave a ser substituída no nó corrente */
      for (i = 0; i < nChaves; ++i) {
         if ( removida ==
                  caminho.no.conteudo.noInterno.chaves[i] ) {
               /* A chave removida foi encontrada no */
               /* nó corrente e será substituída     */
            caminho.no.conteudo.noInterno.chaves[i] = *subst;

               /* Atualiza nó em arquivo */
            EscreveNoBM(streamArvore, caminho.endereco, &caminho.no);

            subst = NULL; /* Só há uma chave a substituir */

            break;
         }
      }

         /* Se haverá remoção, é preciso empilhar */
         /* o nó na pilha auxiliar                */
      if (aRemover) {
         Empilha(caminho, &pilhaAux);
      }
   }

      /***                               ***/
      /* A segunda parte desta função lida */
      /* com remoção de chave no índice    */
      /***                               ***/

      /* Se não há chave a remover no índice, o */
      /* trabalho desta função está encerrado   */
   if (!aRemover) {
      EsvaziaPilha(pilha); /* A pilha não será necessária */
      return; /* Serviço concluído */
   } else { /* Restaura a pilha, se for o caso */
      while (!PilhaVazia(pilhaAux)) {
         Empilha(Desempilha(&pilhaAux), pilha);
      }
   }

      /***                            ***/
      /* Esta função só remove chave no */
      /* último nível de nós internos   */
      /***                            ***/

      /* Neste ponto, a pilha não pode estar vazia */
   ASSEGURA(!PilhaVazia(*pilha),
            "A pilha nao deveria estar vazia em AtualizaIndice()");

      /* Obtém informações sobre o nó interno do último */
      /* nível que se encontra no topo da pilha         */
   caminho = Desempilha(pilha);

      /* O nó desempilhado deve ser interno */
   ASSEGURA( caminho.no.tipoDoNo == INTERNO,
             "Erro em AtualizaIndice(): No' deveria ser interno" );

      /* Obtém o número de chaves do nó corrente */
   nChaves = caminho.no.conteudo.noInterno.nFilhos - 1;

      /* Procura pela chave a ser removida no nó corrente */
   for (i = 0; i < nChaves; ++i) {
      if ( *aRemover ==
               caminho.no.conteudo.noInterno.chaves[i] ) {
            /* A chave a ser removida foi encontrada. */
            /* Move cada chave uma posição para trás. */
         for (j = i; j < nChaves - 1; ++j)
            caminho.no.conteudo.noInterno.chaves[j] =
               caminho.no.conteudo.noInterno.chaves[j + 1];

            /* Faz o mesmo com cada filho direito */
         for ( j = i + 1;
               j < caminho.no.conteudo.noInterno.nFilhos - 1; ++j )
            caminho.no.conteudo.noInterno.filhos[j] =
               caminho.no.conteudo.noInterno.filhos[j + 1];

            /* O número de filhos do nó foi reduzido */
         caminho.no.conteudo.noInterno.nFilhos--;

         aRemover = NULL; /* Não há mais chave a remover */

         break;
      }
   }

      /* A chave deve ter sido encontrada e removida */
   ASSEGURA( !aRemover,
             "Chave a remover nao foi encontrada em no' interno" );

      /* Atualiza nó em arquivo */
   EscreveNoBM( streamArvore, caminho.endereco, &caminho.no );

      /* Verifica se o nó ficou descompensado */
   if (caminho.no.conteudo.noInterno.nFilhos < TG) {
         /* Chama JuntaNosBM() para completar o serviço */
      JuntaNosBM( &caminho.no, caminho.pos, caminho.endereco,
                  pilha, streamArvore );
   } else {
      EsvaziaPilha(pilha);
   }
}

/******************** Funções de Globais ******************/

/****
 *
 * LeNoBM(): Lê em arquivo um nó de uma árvore B+
 *
 * Parâmetros:
 *     stream (entrada) - stream associado ao arquivo contendo
 *                        a árvore na qual será feita a leitura
 *     pos (entrada) - posição no arquivo na qual será feita a
 *                     leitura
 *    *no (saída) - ponteiro para o nó que conterá o resultado
 *                  da leitura
 *
 * Retorno: Nada
 *
 ****/
void LeNoBM(FILE *stream, int pos, tNoBM *no)
{

      /* Verifica a validade do stream */
   ASSEGURA(stream, ERRO_STREAM_NULL(stream, LeNoBM));

      /* Tenta mover o apontador de arquivo para o local */
      /* de leitura; se não conseguir aborta o programa  */
   MoveApontador(stream, sizeof(tNoBM)*pos, SEEK_SET);

      /* Efetua a leitura */
   fread(no, sizeof(tNoBM), 1, stream);

      /* Certifica-se que não houve erro de leitura */
   ASSEGURA(!ferror(stream), ERRO_FREAD(LeNoBM));
}

/****
 *
 * BuscaBM(): Faz buscas numa árvore B+
 *
 * Parâmetros:
 *     chave (entrada) - a chave de busca
 *     raiz (entrada) - ponteiro para a raiz da árvore na qual
 *                      será feita a busca
 *     stream (entrada) - stream representando o arquivo no qual
 *                        a árvore reside
 *
 * Retorno: Índice do registro no arquivo de registros se a
 *          chave for encontrada; POSICAO_NULA, se a chave
 *          não for encontrada.
 *
 ****/
int BuscaBM(FILE *stream, const tNoBM *raiz, tChave chave)
{
   int   i,
         encontrado = 0, /* Informa se a chave foi encontrada */
         posNo = 0; /* Posição no nó no arquivo */
   tNoBM no; /* Armazenará cada nó visitado */

      /* Verifica se o stream é válido */
   ASSEGURA( stream, ERRO_OPEN(BuscaBM) );

   no = *raiz;

      /* Desce na árvore até encontrar uma folha */
   while (no.tipoDoNo == INTERNO) {
      i = BuscaEmNoBM(chave, &no, &encontrado);

         /* Desce até o próximo nó */
      posNo = encontrado ? no.conteudo.noInterno.filhos[i + 1]
                         : no.conteudo.noInterno.filhos[i];
      LeNoBM(stream, posNo, &no);
   }

      /* Chegou-se a uma folha. Se a chave se encontrar */
      /* na árvore, ela deve estar nessa folha          */
   i = BuscaEmNoBM(chave, &no, &encontrado);

   return encontrado ? no.conteudo.noFolha.chaves[i].indice
                     : POSICAO_NULA;
}

/****
 *
 * InsereBM(): Faz busca e inserção numa árvore B+ armazenada
 *             em arquivo
 *
 * Parâmetros:
 *    *raiz (entrada/saída) - ponteiro para a raiz da árvore
 *                              (armazenada em memória)
 *     streamArvore (entrada) - stream associado ao arquivo que
 *                              contém a árvore
 *    *chaveIndice (entrada/saída) - par chave/índice contendo
 *                                   a chave a ser inserida
 *
 * Retorno: 1, se houver inserção; 0, em caso contrário.
 *
 * Descrição do algoritmo: v. texto
 *
 * Observações: A chave é considerada primária; i.e., chaves
 *              repetidas não são permitidas na árvore.
 *
 ****/
int InsereBM( tNoBM *raiz, tChaveIndice *chaveIndice,
              FILE *streamArvore )
{
   int         encontrado, /* A chave foi encontrada? */
               indiceDaChave; /* Índice da chave no nó no */
                              /* qual ela é, ou deveria   */
                              /* ser, encontrada          */
   tChave      chaveAInserir, /* A chave a ser inserida */
                              /* no nível corrente      */
               chaveQueSobe;  /* A chave a ser inserida */
                              /* um nível acima         */
   tNoBM       pNoDireito; /* Nó à direita que conterá */
                           /* as chaves superiores de  */
                           /* um nó dividido           */
   int         pFilhoDireito; /* Posição no arquivo do nó que */
                              /* será filho à direita de uma  */
                              /* chave inserida               */
   tNoCaminho  noCaminho; /* Um elemento da pilha que    */
                          /* representa o caminho de nós */
   tPilha      pilha; /* Pilha que armazenará o   */
                      /* caminho de nós visitados */

      /* Verifica a validade do stream */
   ASSEGURA( streamArvore,
             ERRO_STREAM_NULL(streamArvore, InsereBM) );

      /* Verifica se o ponteiro para a raiz é válido */
   ASSEGURA(raiz, "ERRO: A raiz é NULL em InsereBM\n");

      /* Verifica se a árvore está vazia */
   if (raiz->tipoDoNo == NO_VAZIO) { /* A árvore ainda é virgem */
      IniciaNoBM(FOLHA, raiz); /* Inicia a raiz da árvore */
      raiz->conteudo.noFolha.chaves[0] = *chaveIndice;
      raiz->conteudo.noFolha.nChaves = 1;

         /* Armazena a raiz da árvore no arquivo, */
         /* no qual ela ocupa a posição 0         */
      EscreveNoBM(streamArvore, 0, raiz);

      return 1;
   }

      /* Encontra o caminho que conduz até o local  */
      /* de inserção ou onde a chave já se encontra */
   EncontraCaminhoBM( streamArvore, chaveIndice->chave, raiz,
                      &indiceDaChave, &encontrado, &pilha );

      /* Verifica se a chave foi encontrada */
   if (encontrado) {
         /* A chave foi encontrada e não precisa inseri-la. */
         /* O conteúdo da pilha não é mais necessário       */
      EsvaziaPilha(&pilha);

      return 0; /* A chave é primária */
   }

      /* A chave não foi encontrada. Insere a chave */
      /* na folha que deve estar no topo da pilha   */

   noCaminho = Desempilha(&pilha);

   ASSEGURA( noCaminho.no.tipoDoNo == FOLHA,
             "Erro em InsereBM(): No' deveria ser folha" );

      /* Caso 1: Há espaço para inserção na folha */
   if (noCaminho.no.conteudo.noFolha.nChaves < F) {
      InsereEmFolhaBM(&noCaminho.no, indiceDaChave, *chaveIndice);

         /* Atualiza o nó no arquivo */
      EscreveNoBM(streamArvore, noCaminho.endereco, &noCaminho.no);

         /* Se a raiz da árvore foi atualizada no */
         /* arquivo sua cópia mantida em memória  */
         /* também precisa ser atualizada         */
      if (!noCaminho.endereco)
            /* A posição da raiz no arquivo é zero */
         *raiz = noCaminho.no;

         /* O conteúdo da pilha não é mais necessário */
      EsvaziaPilha(&pilha);

      return 1;
   }

      /* Caso 2: Não há espaço para inserção na folha */

   DivideFolhaBM( &noCaminho.no, indiceDaChave, *chaveIndice,
                  &pNoDireito, &chaveQueSobe );

      /* Faz o encadeamento de 'pNoDireito' com a folha  */
      /* para qual aponta correntemente a folha dividida */
   pNoDireito.conteudo.noFolha.proximaFolha =
        noCaminho.no.conteudo.noFolha.proximaFolha;

   /****                                                     ****/
   /* Armazena pNoDireito no arquivo. A função AcrescentaNoBM() */
   /* retorna a posição na qual o nó foi armazenado no arquivo. */
   /* 'pFilhoDireito' armazena essa posição.                    */
   /****                                                     ****/

      /* O filho à direita da chave que subirá para */
      /* o nível superior será a nova folha criada  */
   pFilhoDireito = AcrescentaNoBM(streamArvore, &pNoDireito);

      /* Faz o encadeamento da folha   */
      /* dividida com sua próxima irmã */
   noCaminho.no.conteudo.noFolha.proximaFolha = pFilhoDireito;

      /* Atualiza a folha dividida no arquivo */
   EscreveNoBM(streamArvore, noCaminho.endereco, &noCaminho.no);

      /* Se a raiz da árvore foi atualizada no arquivo sua   */
      /* cópia mantida em memória também deve ser atualizada */
   if (!noCaminho.endereco)
      *raiz = noCaminho.no;

      /* A seguir, percorre-se o caminho inverso até */
      /* encontrar a raiz ou um nó incompleto.       */

   indiceDaChave = noCaminho.pos;
   chaveAInserir = chaveQueSobe;

   while (!PilhaVazia(pilha)) {
      noCaminho = Desempilha(&pilha);

      ASSEGURA( noCaminho.no.tipoDoNo == INTERNO,
                "Erro em InsereBM(): No' deveria ser interno" );

         /* Verifica se o nó está completo */
      if (noCaminho.no.conteudo.noInterno.nFilhos < G) {
            /* Dá para inserir neste nó */
         InsereEmNoInterno( &noCaminho.no, indiceDaChave,
                            chaveAInserir, pFilhoDireito );

            /* Atualiza o nó no arquivo */
         EscreveNoBM( streamArvore, noCaminho.endereco,
                      &noCaminho.no );

            /* Se a raiz da árvore foi atualizada no */
            /* arquivo sua cópia mantida em memória  */
            /* também deve ser atualizada            */
         if (!noCaminho.endereco)
               /* A raiz ocupa a posição zero no arquivo */
            *raiz = noCaminho.no;

            /* A pilha não é mais necessária */
         EsvaziaPilha(&pilha);

         return 1;
      }

          /* O nó corrente é completo e precisa ser dividido */
      DivideNoInternoBM( &noCaminho.no, indiceDaChave,
                         chaveAInserir, pFilhoDireito,
                         &pNoDireito, &chaveQueSobe );

         /* Atualiza o nó dividido no arquivo */
      EscreveNoBM(streamArvore, noCaminho.endereco, &noCaminho.no);

         /* Se a raiz da árvore foi atualizada no */
         /* arquivo sua cópia mantida em memória  */
         /* também deve ser atualizada            */
      if (!noCaminho.endereco)
         *raiz = noCaminho.no;

         /* O filho à direita da chave que subirá para o   */
         /* nível superior será o novo nó criado no nível  */
         /* corrente. Armazena este nó no arquivo e guarda */
         /* sua posição neste arquivo.                     */
      pFilhoDireito = AcrescentaNoBM(streamArvore, &pNoDireito);

      indiceDaChave = noCaminho.pos;
      chaveAInserir = chaveQueSobe;
   }

   /****                                                   ****/
   /* Neste ponto, sabe-se que o último nó desempilhado é uma */
   /* raiz completa, que já foi dividida no laço while ou     */
   /* antes se a raiz era também uma folha. Resta criar uma   */
   /* nova raiz contendo a chave que subiu até ela tendo como */
   /* filho esquerdo o nó dividido e à direita o novo nó      */
   /****                                                   ****/

   IniciaNoBM(INTERNO, raiz); /* Inicia a nova raiz */

      /* Copia a chave */
   raiz->conteudo.noInterno.chaves[0] = chaveQueSobe;

      /* O filho da esquerda da nova raiz é a antiga raiz que */
      /* precisa ser armazenada em outra posição no arquivo   */
      /* porque a raiz deve sempre ocupar a primeira posição  */
   raiz->conteudo.noInterno.filhos[0] =
        AcrescentaNoBM(streamArvore, &noCaminho.no);

      /* O filho à direita da raiz já foi inserido antes */
      /* no arquivo e sua posição neste arquivo está     */
      /* guardada em 'pFilhoDireito'                     */
   raiz->conteudo.noInterno.filhos[1] = pFilhoDireito;

      /* A nova raiz tem dois filhos */
   raiz->conteudo.noInterno.nFilhos = 2;

      /* Armazena a raiz da árvore no arquivo, */
      /* no qual ela ocupa a posição 0         */
   EscreveNoBM(streamArvore, 0, raiz);

   /****                                   ****/
   /* Neste ponto, a cópia da raiz armazenada */
   /* em memória já está atualizada           */
   /****                                   ****/

   return 1;
}

/****
 *
 * RemoveBM(): Remove a chave especificada de uma árvore
 *             B+ armazenada em arquivo
 *
 * Parâmetros:
 *       streamArvore (entrada) - stream associado ao arquivo
 *                                que armazena a árvore
 *      *raiz (entrada/saída) - raiz da árvore
 *       chave (entrada) - a chave que será removida
 *
 * Retorno: 1, se a remoção for bem sucedida; 0, em caso contrário
 *
 * Descrição do algoritmo: v. texto
 *
 ****/
int RemoveBM(FILE *streamArvore, tNoBM *raiz, tChave chave)
{
   int        i,
              encontrado, /* A chave foi encontrada? */
              indiceDaChave, /* Índice da chave na follha na  */
                             /* qual ela está ou deveria estar */
              nChaves, /* Número de chaves de uma folha */
              posFolha, /* Posição da folha entre suas irmãs */
              endFolha, /* Posição da folha no arquivo */
              endIrma; /* Posição de uma irmã da folha no arquivo */
   tPilha     pilha; /* Pilha que armazenará o caminho */
                     /* de nós visitados               */
   tNoCaminho caminho; /* Um elemento da pilha que    */
                       /* representa o caminho de nós */
   tChave     substituta, /* Chave que substituirá a  */
                          /* chave removida no índice */
              movida, /* Chave movida de uma folha para outra */
              aRemover; /* Chave que será removida no nó-pai */
   tNoBM      irma, /* Irmã da folha na qual ocorreu a remoção */
              folha; /* Folha na qual ocorre a remoção */

      /* Se a árvore for vazia, não há chave para remover */
   if (!raiz)
      return 0; /* Não há chave para remover */

      /* Encontra o caminho da raiz até o nó */
      /* que contém a chave a ser removida   */
   EncontraCaminhoBM( streamArvore, chave, raiz, &indiceDaChave,
                      &encontrado, &pilha );

      /* Se a chave não for encontrada, não há mais nada a  */
      /* fazer. Mas, antes de retornar, esvazia-se a pilha. */
   if (!encontrado) {
      EsvaziaPilha(&pilha);
      return 0; /* Não há chave para remover */
   }

   /***                                           ***/
   /** A chave foi encontrada e informações sobre  **/
   /** a folha que a contém estão no topo da pilha **/
   /***                                           ***/

      /* Neste ponto, a pilha não pode estar vazia */
   ASSEGURA(!PilhaVazia(pilha),
            "A pilha nao deveria estar vazia em RemoveBM()");

      /* No topo da pilha, encontra-se a folha */
      /* na qual a remoção será efetuada       */
   caminho = Desempilha(&pilha);

      /* Garante que o elemento no topo da pilha era um folha */
   ASSEGURA( caminho.no.tipoDoNo == FOLHA,
             "ERRO: No' deveria ser folha em RemoveBM()" );

      /* Facilita a legibilidade */
   nChaves = caminho.no.conteudo.noFolha.nChaves;

      /* Move as chaves uma posição para trás */
   for ( i = indiceDaChave; i < nChaves - 1; ++i )
      caminho.no.conteudo.noFolha.chaves[i] =
         caminho.no.conteudo.noFolha.chaves[i + 1];

      /* O número de chaves da folha foi reduzido */
   nChaves = --caminho.no.conteudo.noFolha.nChaves;

      /* Atualiza a folha no arquivo */
   EscreveNoBM(streamArvore, caminho.endereco, &caminho.no);

      /* Verifica se a folha é a raiz da árvore */
   if (PilhaVazia(pilha)) {
      *raiz = caminho.no; /* Atualiza a raiz que se encontra */
                          /* em memória principal            */
      return 1; /* Serviço completo */
   }

      /* Se a chave removida era a menor chave da folha e o número */
      /* de chaves na folha continuar acima do mínimo requerido,   */
      /* resta apenas substituir a chave removida no índice        */
   if (!indiceDaChave && nChaves >= TF) {
      substituta = caminho.no.conteudo.noFolha.chaves[0].chave;
      AtualizaIndice(streamArvore, &pilha, chave, &substituta, NULL);
      return 1;
   } else if (nChaves >= TF) {
         /* A chave removida não era a primeira da folha e o */
         /* número de chaves nela permanece dentro do limite */
      EsvaziaPilha(&pilha);
      return 1; /* Não resta mais nada a fazer */
   }

      /***                                                  ***/
      /* Neste ponto, sabe-se que o número de chaves na folha */
      /* ficou abaixo do mínimo requerido. Agora existem duas */
      /* opções: (1) redistribuição de chaves ou (2) fusão de */
      /* folhas.                                              */
      /***                                                  ***/

      /* Guarda a folha na qual ocorreu a remoção */
   folha = caminho.no;

      /* Guarda a posição da folha no arquivo */
   endFolha = caminho.endereco;

      /* Guarda a posição da folha entre suas irmãs */
   posFolha = caminho.pos;

      /* Obtém o nó interno que se encontra */
      /* no topo da pilha sem desempilhá-lo */
   caminho = ElementoTopo(&pilha);

      /* Garante que o nó no topo da pilha é interno */
   ASSEGURA( caminho.no.tipoDoNo == INTERNO,
             "ERRO: No' deveria ser interno em RemoveBM()" );

      /*************************************/
      /* Opção 1: redistribuição de chaves */
      /* =======                           */
      /*************************************/

      /* Verifica se a folha possui irmã à esquerda */
   if (posFolha) {
         /* Obtém a posição da irmã esquerda no arquivo */
      endIrma = caminho.no.conteudo.noInterno.filhos[posFolha - 1];

         /* Lê a irmã esquerda da folha */
      LeNoBM( streamArvore, endIrma, &irma );

         /* O nó lido deve ser uma folha */
      ASSEGURA( irma.tipoDoNo == FOLHA,
                "ERRO: No' deveria ser folha em RemoveBM()" );

         /* Verifica se a irmã pode ceder uma chave */
      if (irma.conteudo.noInterno.nFilhos > TF) {
            /*                                           */
            /* Move a última chave da irmã esquerda para */
            /* o início da folha que ficou deficiente    */
            /*                                           */

            /* Primeiro, abre espaço para a nova chave */
         for ( i = folha.conteudo.noFolha.nChaves - 1; i >= 0;
               i-- )
            folha.conteudo.noFolha.chaves[i + 1] =
                            folha.conteudo.noFolha.chaves[i];

            /* Obtém a chave que será substituta */
         substituta = irma.conteudo.noFolha.chaves[
                      irma.conteudo.noFolha.nChaves - 1 ].chave;

            /* Copia a chave */
         folha.conteudo.noFolha.chaves[0] =
            irma.conteudo.noFolha.chaves
            [irma.conteudo.noFolha.nChaves - 1];

            /* O número de chaves da folha aumentou e  */
            /* o número de chaves de sua irmã diminuiu */
         folha.conteudo.noFolha.nChaves++;
         irma.conteudo.noFolha.nChaves--;

            /* Atualiza a folha e sua irmã no arquivo */
         EscreveNoBM(streamArvore, endFolha, &folha);
         EscreveNoBM(streamArvore, endIrma, &irma);

            /* Atualiza os nós internos */
         AtualizaIndice( streamArvore, &pilha, chave, &substituta,
                         NULL );

         return 1;
      }
   } else {  /* A folha deve possuir irmã à direita */
      ASSEGURA(posFolha < caminho.no.conteudo.noInterno.nFilhos - 1,
               "ERRO: A folha deveria possuir irma direita");

         /* Obtém a posição da folha no arquivo */
      endIrma = caminho.no.conteudo.noInterno.filhos[posFolha + 1];

         /* Lê a irmã direita da folha */
      LeNoBM( streamArvore, endIrma, &irma );

         /* O nó lido deve ser uma folha */
      ASSEGURA( irma.tipoDoNo == FOLHA,
                "ERRO: No' deveria ser folha em RemoveBM()" );

         /* Verifica se a irmã pode ceder uma chave */
      if (irma.conteudo.noInterno.nFilhos > TF) {
            /* Obtém a chave a ser movida */
         movida = irma.conteudo.noFolha.chaves[0].chave;

            /* Move a primeira chave da irmã direita para */
            /* o final da folha que ficou deficiente      */
         folha.conteudo.noFolha.chaves
              [folha.conteudo.noFolha.nChaves] =
              irma.conteudo.noFolha.chaves[0];

            /* Remove a chave movida da irmã */
         for (i = 0; i > irma.conteudo.noFolha.nChaves - 1; ++i)
            irma.conteudo.noFolha.chaves[i] =
                    irma.conteudo.noFolha.chaves[i + 1];

            /* A irmã é uma folha direita. Portanto, */
            /* sua primeira chave aparece no nó pai, */
            /* que precisará ser atualizado          */
         for ( i = 0; i < caminho.no.conteudo.noInterno.nFilhos - 1;
               ++i ) {
            if (caminho.no.conteudo.noInterno.chaves[i] == movida) {
               caminho.no.conteudo.noInterno.chaves[i] =
                 irma.conteudo.noFolha.chaves[0].chave;
            }
         }

            /* A chave movida deve ter sido */
            /* encontrada no nó interno     */
         ASSEGURA( i < caminho.no.conteudo.noInterno.nFilhos,
                   "Chave a ser substituida nao foi encontrada");

            /* Atualiza o nó interno no arquivo */
         EscreveNoBM(streamArvore, caminho.endereco, &caminho.no);

            /* O número de chaves da folha aumentou e  */
            /* o número de chaves de sua irmã diminuiu */
         folha.conteudo.noFolha.nChaves++;
         irma.conteudo.noFolha.nChaves--;

            /* Atualiza a folha e sua irmã no arquivo */
         EscreveNoBM(streamArvore, endFolha, &folha);
         EscreveNoBM(streamArvore, endIrma, &irma);

            /* Verifica se a chave removida aparece no índice */
         if (!indiceDaChave) {
               /* A chave substituta é a primeira chave da folha */
            substituta = folha.conteudo.noFolha.chaves[0].chave;

               /* O elemento que se encontra no topo da pilha */
               /* foi alterado e é preciso desempilhá-lo e    */
               /* empilhar o elemento alterado                */
            ASSEGURA( !PilhaVazia(pilha),
                      "ERRO: Pilha vazia em RemoveBM()" );
            (void) Desempilha(&pilha);
            Empilha(caminho, &pilha);

               /* Atualiza os nós internos */
            AtualizaIndice( streamArvore, &pilha, chave,
                            &substituta, NULL );
         } else { /* A chave não aparece no índice */
            EsvaziaPilha(&pilha);
            return 1; /* Não resta mais nada a fazer */
         }

         return 1;
      }
   }

      /****************************/
      /* Opção 2: fusão de folhas */
      /* =======                  */
      /****************************/

      /* Verifica se a folha possui irmã à esquerda */
   if (posFolha) {
         /* Obtém a posição da folha no arquivo */
      endIrma = caminho.no.conteudo.noInterno.filhos[posFolha - 1];

         /* Lê a irmã esquerda da folha */
      LeNoBM( streamArvore, endIrma, &irma );

         /* O nó lido deve ser uma folha */
      ASSEGURA( irma.tipoDoNo == FOLHA,
                "ERRO: No' deveria ser folha em RemoveBM()" );

         /* A irmã deve estar no limite mínimo de chave */
      ASSEGURA( irma.conteudo.noFolha.nChaves == TF,
                "Folha nao tem numero minimo de chaves");
            /*                                         */
            /* Move as chaves da folha na qual ocorreu */
            /* a remoção para sua irmã esquerda        */
            /*                                         */

         /* Se a chave removida não era a primeira da folha, */
         /* a chave a ser removida no nó pai será a primeira */
         /* chave da folha na qual ocorreu a remoção         */
      if (indiceDaChave) {
         aRemover = folha.conteudo.noFolha.chaves[0].chave;
      } else {
         aRemover = chave;
      }

         /* Copia as chaves */
      for (i = 0; i < folha.conteudo.noFolha.nChaves; ++i) {
         irma.conteudo.noFolha.chaves[i + TF] =
              folha.conteudo.noFolha.chaves[i];
      }

         /* O número de chaves da folha ficou nulo  */
         /* o número de chaves de sua irmã aumentou */
      folha.tipoDoNo = NO_VAZIO;
      irma.conteudo.noFolha.nChaves +=
          folha.conteudo.noFolha.nChaves;

         /* Preserva o encadeamento das folhas */
      irma.conteudo.noFolha.proximaFolha =
          folha.conteudo.noFolha.proximaFolha;

         /* Atualiza a folha e sua irmã no arquivo */
      EscreveNoBM(streamArvore, endFolha, &folha);
      EscreveNoBM(streamArvore, endIrma, &irma);

         /* Chama AtualizaIndice() para completar o serviço */
      AtualizaIndice(streamArvore, &pilha, chave, NULL, &aRemover);
   } else {  /* A folha deve possuir irmã à direita */
      ASSEGURA(posFolha < caminho.no.conteudo.noInterno.nFilhos - 1,
               "ERRO: A folha deveria possuir irma direita");

         /* Obtém a posição da irmã no arquivo */
      endIrma = caminho.no.conteudo.noInterno.filhos[posFolha + 1];

         /* Lê a irmã direita da folha */
      LeNoBM( streamArvore, endIrma, &irma );

         /* O nó lido deve ser uma folha */
      ASSEGURA( irma.tipoDoNo == FOLHA,
                "ERRO: No' deveria ser folha em RemoveBM()" );

         /* A irmã deve estar no limite mínimo de chave */
      ASSEGURA( irma.conteudo.noFolha.nChaves == TF,
                "Folha nao tem numero minimo de chaves");
            /*                                       */
            /* Move as chaves da irmã direita para a */
            /* folha na qual ocorreu a remoção       */
            /*                                       */

         /* Se a chave removida era a primeira da folha, */
         /* ela deverá ser substituída no índice         */
      if (!indiceDaChave) {
         substituta = folha.conteudo.noFolha.chaves[0].chave;
      }

         /* Chave que será removida no nó-pai */
      aRemover = irma.conteudo.noFolha.chaves[0].chave;

         /* Copia as chaves */
      for (i = 0; i < TF; ++i) {
         folha.conteudo.noFolha.chaves[ i +
              folha.conteudo.noFolha.nChaves ] =
              irma.conteudo.noFolha.chaves[i];
      }

         /* O número de chaves da irmã ficou nulo  */
         /* e o número de chaves da folha aumentou */
      irma.tipoDoNo = NO_VAZIO;
      folha.conteudo.noFolha.nChaves += TF;

         /* Preserva o encadeamento das folhas */
      folha.conteudo.noFolha.proximaFolha =
           irma.conteudo.noFolha.proximaFolha;

         /* Atualiza a folha e sua irmã no arquivo */
      EscreveNoBM(streamArvore, endFolha, &folha);
      EscreveNoBM(streamArvore, endIrma, &irma);

         /* Chama AtualizaIndice() para completar o serviço */
      if (indiceDaChave) {
         AtualizaIndice( streamArvore, &pilha, chave, &substituta,
                         &aRemover );
      } else {
         AtualizaIndice( streamArvore, &pilha, chave, NULL,
                         &aRemover );
      }
   }

   return 1; /* Remoção bem sucedida */
}

/****
 *
 * MenorChaveBM(): Encontra a menor chave de uma árvore B+
 *
 * Parâmetros:
 *     stream (entrada) - stream associado ao arquivo que
 *                        contém a árvore
 *
 * Retorno: A menor chave ou POSICAO_NULA se a árvore estiver vazia
 *
 ****/
int MenorChaveBM(FILE *stream)
{
   tNoBM umNo;

   ASSEGURA( stream,
             ERRO_STREAM_NULL(stream, MenorChaveBM) );

   rewind(stream);

      /* O primeiro nó é a raiz */
   fread(&umNo, sizeof(umNo), 1, stream);

   ASSEGURA( !feof(stream) & !ferror(stream),
             ERRO_FREAD(MenorChaveBM) );

   if (umNo.tipoDoNo == NO_VAZIO)
      return POSICAO_NULA;

      /* Desce até encontrar a folha mais à esquerda */
   while (umNo.tipoDoNo == INTERNO)
         /* Desce pelo filho mais à esquerda */
      LeNoBM(stream, umNo.conteudo.noInterno.filhos[0], &umNo);

   ASSEGURA( umNo.tipoDoNo == FOLHA,
             ERRO_INCONSISTENCIA(MenorChaveBM) );

      /* A menor chave da árvore é a primeira chave */
      /* do nó mais à esquerda da árvore.           */
   return umNo.conteudo.noFolha.chaves[0].chave;
}

/****
 *
 * MaiorChaveBM(): Encontra a maior chave de uma árvore B+
 *
 * Parâmetros:
 *     stream (entrada) - stream associado ao arquivo que
 *                        contém a árvore
 *
 * Retorno: A maior chave ou POSICAO_NULA se a árvore estiver vazia
 *
 ****/
int MaiorChaveBM(FILE *stream)
{
   tNoBM umNo;

   ASSEGURA( stream,
             ERRO_STREAM_NULL(stream, MaiorChaveBM) );

   rewind(stream);

      /* O primeiro nó é a raiz */
   fread(&umNo, sizeof(umNo), 1, stream);

   ASSEGURA( !feof(stream) & !ferror(stream),
             ERRO_FREAD(MaiorChaveBM) );

   if (umNo.tipoDoNo == NO_VAZIO)
      return POSICAO_NULA;

      /* Desce até encontrar a folha mais à direita */
   while (umNo.tipoDoNo == INTERNO)
         /* Desce pelo filho mais à direita */
      LeNoBM( stream, umNo.conteudo.noInterno.filhos
                   [umNo.conteudo.noInterno.nFilhos - 1], &umNo );

      /* O último nó lido deve ser uma folha */
   ASSEGURA( umNo.tipoDoNo == FOLHA,
             ERRO_INCONSISTENCIA(MaiorChaveBM) );

      /* A maior chave da árvore é a última chave */
      /* da folha mais à direita da árvore        */
   return umNo.conteudo.noFolha.chaves
          [umNo.conteudo.noFolha.nChaves-1].chave;
}

/****
 *
 * EscreveChavesOrdenadas(): Visita todas as folhas de uma
 *                           árvore B+ imprimindo todas as
 *                           chaves da árvore em ordem crescente
 *
 * Parâmetros:
 *     arvore (entrada) - ponteiro para a raiz da árvore
 *    *streamArv (entrada) - stream associado ao arquivo que
 *                           contém a árvore
 *     streamChaves (entrada) - stream associado ao arquivo no
 *                              qual serão armazenadas as chaves
 *                              visitadas
 *
 * Retorno: 1, se tudo ocorrer bem; 0, em caso contrário.
 *
 ****/
void EscreveChavesOrdenadas( const tNoBM* arvore, FILE *streamArv,
                             FILE *streamChaves )
{
   int   i;
   tNoBM umNo;

      /*  */
   ASSEGURA( streamArv,
             ERRO_STREAM_NULL(streamArv, EscreveChavesOrdenadas) );

      /*  */
   ASSEGURA( streamChaves,
             ERRO_STREAM_NULL( streamChaves,
                              EscreveChavesOrdenadas) );

   ASSEGURA( arvore,
             "Erro: A arvore NULL em EscreveChavesOrdenadas");

   ASSEGURA( arvore->tipoDoNo != NO_VAZIO,
             "Erro: A arvore vazia em EscreveChavesOrdenadas");

   umNo = *arvore;

      /* Desce sempre pelo filho mais à esquerda */
      /* até encontrar a primeira folha          */
   while (umNo.tipoDoNo == INTERNO)
      LeNoBM( streamArv, umNo.conteudo.noInterno.filhos[0],
            &umNo );

   ASSEGURA( umNo.tipoDoNo == FOLHA,
             ERRO_INCONSISTENCIA(EscreveChavesOrdenadas) );

      /* Chegou-se à folha mais à esquerda. Como as folhas   */
      /* formam uma lista encadeada e essa folha encabeça    */
      /* a lista, deve-se percorrê-la imprimindo suas chaves */
   do {
         /* Escreve cada chave da folha */
      for (i = 0; i < umNo.conteudo.noFolha.nChaves; ++i) {
            /* Escreve a chave corrente */
         fprintf( streamChaves, "%ld",
                  umNo.conteudo.noFolha.chaves[i].chave );

         fputc('\n', streamChaves); /* Salta linha */
      }

         /* Passa para a próxima folha */
      LeNoBM( streamArv, umNo.conteudo.noFolha.proximaFolha,
             &umNo );
   } while (umNo.conteudo.noFolha.proximaFolha != POSICAO_NULA);
}

/****
 *
 * BuscaIntervaloBM(): Coleta num arquivo todas as chaves que
 *                     estão entre duas chaves dadas de uma
 *                     árvore B+
 *
 * Parâmetros:
 *     chave1 (entrada) - primeira chave que define o intervalo
 *     chave2 (entrada) - segunda chave que define o intervalo
 *     arvore (entrada) - ponteiro para a raiz da árvore na qual
 *                        será feita a busca
 *    *streamArv (entrada) - stream associado ao arquivo que
 *                           contém a árvore
 *     streamChaves (entrada) - stream associado ao arquivo que
 *                              conterá as chaves resultantes
 *                              da busca
 *
 * Retorno: 1, se a coleta de chaves for bem sucedida;
 *          0, em caso contrário
 *
 * Observações:
 *    1. Não faz diferença qual das duas chaves é menor do
 *       que a outra, mas as chaves devem ser diferentes
 *       para que haja um intervalo.
 *    2. Numa aplicação prática (e não meramente didática),
 *       faria mais sentido coletar os registros (e não apenas
 *       as chaves). Mas, implementar uma extensão desta função
 *       para que ela seja mais pragmática é considerado trivial
 *
 ****/
int BuscaIntervaloBM( tChave chave1, tChave chave2,
                      const tNoBM* arvore,
                      FILE *streamArv, FILE *streamChaves )
{
   tChave menorChave, /* Menor chave */
          maiorChave; /* Maior chave */
   tNoBM  no; /* Armazenará cada nó visitado */
   int    i,
          encontrado = 0, /* Usada para chamar BuscaEmNoBM() */
          posNo = 0; /* Posição do nó no arquivo */

      /* Verifica se streamArv é válido */
   ASSEGURA( streamArv,
             ERRO_STREAM_NULL(streamArv, BuscaIntervaloBM) );

      /* Verifica se streamChaves é válido */
   ASSEGURA( streamChaves,
             ERRO_STREAM_NULL(streamChaves, BuscaIntervaloBM) );

      /* Verifica se o ponteiro para a raiz é válido */
   ASSEGURA( arvore,
             "Erro: A arvore NULL em BuscaIntervaloBM");

      /* Verifica se a árvore é vazia */
   if (arvore->tipoDoNo == NO_VAZIO)
      return 0; /* A árvore é vazia */

      /* Se as chaves forem iguais, não há intervalo */
   if (chave1 == chave2)
      return 0; /* Não há intervalo */

   /****                                               ****/
   /* O trecho a seguir permite que as chaves que definem */
   /* o intervalo sejam introduzidas em qualquer ordem    */
   /****                                               ****/

   if (chave1 < chave2) { /* chave1 < chave2 */
      menorChave = chave1;
      maiorChave = chave2;
   } else { /* chave1 >= chave2 */
      menorChave = chave2;
      maiorChave = chave1;
   }

      /* A busca começa na raiz da árvore */
   no = *arvore;

      /* Desce na árvore até encontrar uma folha */
   while (no.tipoDoNo == INTERNO) {
      i = BuscaEmNoBM(menorChave, &no, &encontrado);

         /* Desce até o próximo nó */
      posNo = encontrado ? no.conteudo.noInterno.filhos[i + 1]
                         : no.conteudo.noInterno.filhos[i];

      LeNoBM(streamArv, posNo, &no);
   }

      /* Chegou-se a uma folha. Encontra, nessa */
      /* folha, a posição da primeira chave que */
      /* é maior do que ou igual a menor chave  */
   i = BuscaEmNoBM(menorChave, &no, &encontrado);

      /* Armazena em arquivo as chaves encontradas a  */
      /* partir dessa posição na folha encontrada até */
      /* encontrar a última chave da última folha ou  */
      /* uma chave maior do que 'maiorChave'          */

      /* A coleta de chaves na primeira folha encontrada    */
      /* é diferente daquela nas demais folhas da sequência */
      /* porque começa com a chave na posição i             */
   for ( ; i < no.conteudo.noFolha.nChaves &&
         no.conteudo.noFolha.chaves[i].chave <= maiorChave; ++i ){
         /* Escreve a chave corrente */
      fprintf( streamChaves, "%ld\n",
               no.conteudo.noFolha.chaves[i].chave );
   }

      /* Se a última chave escrita foi a última chave da */
      /* folha, é preciso ajustar o índice dessa chave   */
   if (i == no.conteudo.noFolha.nChaves)
      --i;

   if (no.conteudo.noFolha.chaves[i].chave > maiorChave)
         /* Foi encontrada uma chave maior do que a maior */
         /* chave do intervalo e nada mais resta a fazer  */
      return 1;

      /* Escreve cada chave de cada folha seguinte que    */
      /* não seja maior do que a maior chave do intervalo */
   do {
         /* Passa para a próxima folha */
      posNo = no.conteudo.noFolha.proximaFolha;

         /* Lê a folha  */
      LeNoBM(streamArv, posNo, &no);

         /* Escreve cada chave que não é maior */
         /* do que a maior chave do intervalo  */
      for (i = 0; i < no.conteudo.noFolha.nChaves &&
               no.conteudo.noFolha.chaves[i].chave <= maiorChave;
                  ++i) {
            /* Escreve a chave corrente */
         fprintf( streamChaves, "%ld\n",
                  no.conteudo.noFolha.chaves[i].chave );
      }

         /* Checa se é preciso ajustar o índice da última chave */
      if (i == no.conteudo.noFolha.nChaves)
         --i;
   } while ( posNo != POSICAO_NULA &&
             no.conteudo.noFolha.chaves[i].chave <= maiorChave );

   return 1;
}

/****
 *
 * ProfundidadeBM(): Calcula a profundidade de uma árvore B+
 *
 * Parâmetros:
 *      stream (entrada) - stream associado ao arquivo que contém
 *                         a árvore
 *
 * Retorno: A referida profundidade
 *
 ****/
int ProfundidadeBM(FILE *stream)
{
   tNoBM umNo;
   int   pos, nNiveis = 0;

   rewind(stream);

      /* O primeiro nó é a raiz */
   fread(&umNo, sizeof(umNo), 1, stream);

   ASSEGURA( !feof(stream) & !ferror(stream),
             ERRO_FREAD(ProfundidadeBM) );

      /* Se a árvore estiver vazia, sua profundidade é zero */
   if (umNo.tipoDoNo == NO_VAZIO)
      return 0;

   ++nNiveis; /* O primeiro nível já foi encontrado */

      /* Desce até encontrar a folha mais à esquerda */
   while (umNo.tipoDoNo == INTERNO) {
         /* Desce pelo filho mais à esquerda */
      pos = umNo.conteudo.noInterno.filhos[0]*sizeof(umNo);
      if (fseek(stream, pos, SEEK_SET)) {
         return 0;
      }

      fread(&umNo, sizeof(umNo), 1, stream);

      if (feof(stream) || ferror(stream))
         return 0;

      ++nNiveis; /* Conta-se um nível adiantado */
   }

      /* Deve-se ter chegado a uma folha */
   ASSEGURA( umNo.tipoDoNo == FOLHA,
             ERRO_INCONSISTENCIA(ProfundidadeBM) );

   return nNiveis;
}

/****
 *
 * NumeroDeChavesBM(): Calcula o número de chaves armazenadas nas
 *                     folhas de uma árvore B+
 *
 * Parâmetros:
 *    *streamArv (entrada) - stream associado ao arquivo que
 *                           contém a árvore
 *
 * Retorno: O número de chaves
 *
 ****/
int NumeroDeChavesBM(FILE *streamArv)
{
   int   numChaves = 0;
   tNoBM umNo;

      /* Verifica se streamArv é válido */
   ASSEGURA( streamArv,
             ERRO_STREAM_NULL(streamArv, NumeroDeChavesBM) );

      /* Tenta mover o apontador de posição */
      /* do arquivo para o seu início       */
   MoveApontador(streamArv, 0, SEEK_SET);

      /* Acessa sequencialmente cada nó. Se ele */
      /* for folha, conta seu número de chaves  */
   while (1) {
         /* Tenta ler um nó */
      fread(&umNo, sizeof(&umNo), 1, streamArv);

         /* Verifica se ocorreu erro ou o */
         /* final de arquivo foi atingido */
      if (feof(streamArv) || ferror(streamArv))
         break;

         /* Se o nó for folha, acrescenta seu */
         /* número de chaves à contagem       */
      if (umNo.tipoDoNo == FOLHA)
         numChaves += umNo.conteudo.noFolha.nChaves;
   }

      /* Verifica se ocorreu erro de leitura */
   ASSEGURA(!ferror(streamArv), ERRO_FREAD(NumeroDeChavesBM));

   return numChaves;
}

/****
 *
 * NumeroDeFolhasBM(): calcula o número de folhas de uma árvore B+
 *
 * Parâmetros:
 *    streamArv (entrada) - stream associado ao arquivo que
 *                          contém a árvore
 *
 * Retorno: O número de folhas da árvore B+
 *
 * Observação: Esta é uma operação bastante custosa, pois envolve
 *             inúmeros acessos ao meio de armazenamento externo
 *
 ****/
int NumeroDeFolhasBM(FILE *streamArv)
{
   int   numFolhas = 0;
   tNoBM umNo;

      /* Verifica se streamArv é válido */
   ASSEGURA( streamArv,
             ERRO_STREAM_NULL(streamArv, NumeroDeFolhasBM) );

      /* Tenta mover o apontador de posição */
      /* do arquivo para o seu início       */
   MoveApontador(streamArv, 0, SEEK_SET);

      /* Acessa sequencialmente cada nó. Se */
      /* ele for folha, leva-o em conta     */
   while (1) {
         /* Tenta ler um nó */
      fread(&umNo, sizeof(&umNo), 1, streamArv);

         /* Verifica se ocorreu erro ou o */
         /* final de arquivo foi atingido */
      if (feof(streamArv) || ferror(streamArv))
         break;

      if (umNo.tipoDoNo == FOLHA)
         ++numFolhas; /* Mais uma folha encontrada */
   }

      /* Verifica se ocorreu erro de leitura */
   ASSEGURA(!ferror(streamArv), ERRO_FREAD(NumeroDeFolhasBM));

   return numFolhas;
}

/****
 *
 * NumeroDeNosInternosBM(): Calcula o número de nós internos de
 *                          uma árvore B+
 *
 * Parâmetros:
 *    *streamArv (entrada) - stream associado ao arquivo que
 *                           contém a árvore
 *
 * Retorno: Número de nós internos da árvore
 *
 ****/
int NumeroDeNosInternosBM(FILE *streamArv)
{
   int   numNos = 0;
   tNoBM umNo;

      /* Verifica se streamArv é válido */
   ASSEGURA( streamArv,
             ERRO_STREAM_NULL(streamArv, NumeroDeNosInternosBM) );

      /* Tenta mover o apontador de posição */
      /* do arquivo para o seu início       */
   MoveApontador(streamArv, 0, SEEK_SET);

      /* Acessa sequencialmente cada nó. Se  */
      /* ele for nó interno, leva-o em conta */
   while (1) {
         /* Tenta ler um nó */
      fread(&umNo, sizeof(&umNo), 1, streamArv);

         /* Verifica se ocorreu erro ou o */
         /* final de arquivo foi atingido */
      if (feof(streamArv) || ferror(streamArv))
         break;

      if (umNo.tipoDoNo == INTERNO)
         ++numNos; /* Mais um nó interno encontrado */
   }

      /* Verifica se ocorreu erro de leitura */
   ASSEGURA(!ferror(streamArv), ERRO_FREAD(NumeroDeNosInternosBM));

   return numNos;
}

/****
 *
 * NumeroDeNosVaziosBM(): Calcula o número de nós vazios de uma
 *                        árvore B+
 *
 * Parâmetros:
 *    *streamArv (entrada) - stream associado ao arquivo que
 *                           contém a árvore
 *
 * Retorno: Número de nós vazios da árvore
 *
 ****/
int NumeroDeNosVaziosBM(FILE *streamArv)
{
   int   numNos = 0;
   tNoBM umNo;

      /* Verifica se streamArv é válido */
   ASSEGURA( streamArv,
             ERRO_STREAM_NULL(streamArv, NumeroDeNosInternosBM) );

      /* Tenta mover o apontador de posição */
      /* do arquivo para o seu início       */
   MoveApontador(streamArv, 0, SEEK_SET);

      /* Acessa sequencialmente cada nó. Se  */
      /* ele for nó interno, leva-o em conta */
   while (1) {
         /* Tenta ler um nó */
      fread(&umNo, sizeof(&umNo), 1, streamArv);

         /* Verifica se ocorreu erro ou o */
         /* final de arquivo foi atingido */
      if (feof(streamArv) || ferror(streamArv))
         break;

      if (umNo.tipoDoNo == NO_VAZIO)
         ++numNos; /* Mais um nó vazio encontrado */
   }

      /* Verifica se ocorreu erro de leitura */
   ASSEGURA(!ferror(streamArv), ERRO_FREAD(NumeroDeNosInternosBM));

   return numNos;
}

/****
 *
 * NumeroDeNosBM(): Calcula o número total de nós (internos e
 *                  folhas) de uma árvore B+
 *
 * Parâmetros:
 *      stream (entrada) - stream associado ao arquivo
 *                         contendo a árvore
 *
 * Retorno: O número de nós da árvore
 *
 ****/
int NumeroDeNosBM(FILE *stream)
{
   return NumeroDeNosInternosBM(stream) + NumeroDeFolhasBM(stream);
}
