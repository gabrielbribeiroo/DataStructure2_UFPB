/****
 *
 * Arquivo ArvoreBM.c
 *
 * Autor: Ulysses de Oliveira
 *
 * Data de Cria��o: 07/08/2012
 * �ltima modifica��o: 25/05/2018
 *
 * Descri��o: Implementa��o das fun��es que efetuam opera��es de
 *            busca e inser��o usando �rvore B+ armazenada em
 *            arquivo
 *
 ****/

/*********************** Includes *************************/

#include <stdio.h>     /* Fun��es de E/S                 */
#include "ArvoreBM.h"  /* Interface deste m�dulo         */
#include "Pilha.h"     /* Pilha usada para armazenar     */
                       /* caminhadas na �rvore           */
#include "Registros.h" /* Interface do m�dulo Registros  */
#include "ASSEGURA.h"  /* Macro ASSEGURA                 */
#include "Leitura.h"   /* Fun��es de leitura via teclado */

/**************** Compila��o Condidicional ****************/

#define DEPURACAO 1

/******************** Fun��es de Locais *******************/

/****
 *
 * IniciaNoBM(): Inicia o tipo do n� e o n�mero de chaves (folhas)
 *               ou filhos (n�s internos)
 *
 * Par�metros:
 *     tipoDoNo (entrada) - o tipo do n� (folha/interno) que ser�
 *                          iniciado
 *    *pNo (sa�da) - o n� que ser� iniciado
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

         /* Inicia todos o poss�veis filhos do n� */
      for (i = 0; i < G; ++i) {
         pNo->conteudo.noInterno.filhos[i] = POSICAO_NULA;
      }
   }
}

/****
 *
 * EscreveNoBM(): Escreve um n� de uma �rvore B+ em arquivo
 *
 * Par�metros:
 *     stream (entrada) - stream associado ao arquivo contendo a
 *                        �rvore no qual ser� feita a escrita
 *     pos (entrada) - posi��o no arquivo no qual ser� feita a
 *                     escrita
 *    *no (entrada) - n� que ser� escrito no arquivo
 *
 * Retorno: Nada
 *
 ****/
static void EscreveNoBM(FILE *stream, int pos, const tNoBM *no)
{
      /* Verifica se o stream � v�lido */
   ASSEGURA(stream, "Erro em EscreveNoBM(): stream e' NULL");

      /* Tenta mover o apontador de arquivo para o local */
      /* de escrita; se n�o conseguir aborta o programa  */
   MoveApontador(stream, sizeof(tNoBM)*pos, SEEK_SET);

      /* Efetua a escrita */
   fwrite(no, sizeof(tNoBM), 1, stream);

      /* Certifica-se que n�o houve erro */
   ASSEGURA( !ferror(stream), ERRO_FWRITE(EscreveNoBM) );
}

/****
 *
 * AcrescentaNoBM(): Acrescenta um n� de uma �rvore B+ ao final do
 *                   arquivo que cont�m a �rvore
 *
 * Par�metros:
 *     stream (entrada) - stream associado ao arquivo contendo a
 *                        �rvore na qual ser� feita a inser��o
 *    *no (entrada) - ponteiro para o n� que ser� escrito
 *
 * Retorno: Posi��o na qual foi feita a inser��o do n� no arquivo
 *
 ****/
static int AcrescentaNoBM(FILE *stream, const tNoBM *no)
{
   int posicao;

      /* Garante que stream � v�lido */
   ASSEGURA( stream,
             ERRO_STREAM_NULL(stream, AcrescentaNoBM) );

      /* Obt�m a posi��o de inser��o */
   posicao = NumeroDeRegistros(stream, sizeof(tNoBM));

      /* Tenta mover o apontador de posi��o */
      /* de arquivo para o local de escrita */
   MoveApontador(stream, 0, SEEK_END);

   fwrite(no, sizeof(tNoBM), 1, stream); /* Efetua a escrita */

      /* Certifica-se que n�o houve erro */
   ASSEGURA(!ferror(stream), ERRO_FWRITE(AcrescentaNoBM));

   return posicao;
}

/****
 *
 * BuscaEmNoBM(): Faz uma busca sequencial dentro de um
 *                n� (interno ou folha)
 *
 * Par�metros:
 *     chave (entrada) - a chave de busca
 *     no (entrada) - ponteiro para o n� no qual ser� feita a busca
 *    *encontrado (sa�da) - informar� se a chave foi encontrada
 *
 * Retorno: O �ndice da chave dentro do n� se ela for encontrada
 *          uma chave maior do que ou igual a chave de busca,
 *          ou o n�mero de chaves, se tal chave n�o for
 *          encontrada; i.e., se a chave de busca for maior
 *          do que qualquer chave do n�.
 *
 ****/
static int BuscaEmNoBM( tChave chave, const tNoBM *no,
                        int *encontrado )
{
   int i, nChaves;

      /* Ponteiro 'no' � v�lido? */
   ASSEGURA(no, "ERRO: No' e' NULL em BuscaEmNoBM()");

      /* Obt�m o n�mero de chaves do n� */
   nChaves = no->tipoDoNo == INTERNO
             ? no->conteudo.noInterno.nFilhos - 1
             : no->conteudo.noFolha.nChaves;

      /* Efetua a busca de acordo com o tipo do n� */
   if (no->tipoDoNo == INTERNO) {
      for (i = 0; i < nChaves; ++i)
         if (chave < no->conteudo.noInterno.chaves[i]) {
            *encontrado = 0;
            return i; /* Posi��o na qual a chave deveria estar */
         } else if ( chave == no->conteudo.noInterno.chaves[i] ) {
            *encontrado = 1;
            return i; /* Posi��o na qual a chave est� */
         }
   } else {
      for (i = 0; i < nChaves; ++i) {
            /* Verifica se a chave foi encontrada */
         if (chave < no->conteudo.noFolha.chaves[i].chave) {
            *encontrado = 0;
            return i;/* Posi��o na qual a chave deveria estar */
         } else if ( chave ==
                     no->conteudo.noFolha.chaves[i].chave ) {
            *encontrado = 1;
            return i; /* Posi��o na qual a chave est� */
         }
      }
   }

   /****                                              ****/
   /* Neste ponto, sabe-se que n�o foi encontrada uma    */
   /* chave no n� maior do que ou igual � chave de busca */
   /****                                              ****/

   *encontrado = 0;

   return nChaves;
}

/****
 *
 * EncontraCaminhoBM(): Armazena numa pilha o caminho da raiz at�
 *                      a folha na qual se encontra uma chave de
 *                      busca ou na qual ela deve ser inserida
 *
 * Par�metros:
 *     raiz (entrada) - ponteiro para a raiz da �rvore
 *                      (armazenada em mem�ria)
 *     streamArvore (entrada) - stream associado ao arquivo
 *                              que cont�m a �rvore
 *     chave (entrada) - a chave de busca
 *    *posicao (sa�da) - posi��o da chave no �ltimo n� do caminho
 *    *encontrado (sa�da) - informa se a chave foi encontrada
 *                          (1) ou n�o (0)
 *    *pilha (sa�da) - pilha que armazenar� o caminho de n�s
 *                     visitados
 *
 * Retorno: Nada
 *
 ****/
static void EncontraCaminhoBM( FILE *streamArvore, tChave chave,
                               const tNoBM *raiz, int *posicao,
                               int *encontrado, tPilha *pilha )
{
   tNoBM      no; /* N� usado para armazenar os n�s visitados */
   int        indiceNo; /* �ndice do n� visitado */
                        /* entre seus irm�os     */
   tNoCaminho noCaminho;  /* Elemento armazenado na pilha */
   int        posNo; /* Posi��o de um n� no arquivo */

      /* Certifica-se que a raiz n�o � NULL */
   ASSEGURA(raiz, "ERRO: Arvore vazia em EncontraCaminhoBM");

      /* Certifica-se que o stream n�o � NULL */
   ASSEGURA( streamArvore,
             ERRO_STREAM_NULL(streamArvore, EncontraCaminhoBM) );

   no = *raiz;   /* O primeiro n� visitado � a raiz da �rvore */
   posNo = 0; /* A raiz � sempre mantida no in�cio do arquivo */
   indiceNo = 0;    /* A raiz n�o tem irm�os */
   *encontrado = 0; /* A chave ainda n�o foi encontrada */

   CriaPilha(pilha); /* Inicia a pilha */

   while (no.tipoDoNo == INTERNO) {
         /***                                ***/
         /*** Cria o item que ser� empilhado ***/
         /***                                ***/
      noCaminho.no = no;          /* Armazena o n�,...           */
      noCaminho.endereco = posNo; /* sua posi��o no arquivo e... */
      noCaminho.pos = indiceNo;  /* seu �ndice entre seus irm�os */

         /* Empilha as informa��es sobre o n� corrente */
      Empilha(noCaminho, pilha);

         /* Encontra o �ndice do filho da pr�xima descida */
      indiceNo = BuscaEmNoBM(chave, &no, encontrado);

         /* Se a chave foi encontrada num n� interno, */
         /* a descida � pelo filho direito dele       */
      if (*encontrado)
         ++indiceNo;

         /* Desce at� o pr�ximo n� */
      posNo = no.conteudo.noInterno.filhos[indiceNo];

         /* L� o pr�ximo n� no arquivo */
      LeNoBM(streamArvore, posNo, &no);
   }

   /****                                                   ****/
   /* Neste ponto, sabe-se que 'no' � uma folha.              */
   /* Se a chave encontrar-se na �rvore ser� nessa folha.     */
   /* Caso contr�rio, � nessa folha que ela deve ser inserida */
   /****                                                   ****/

      /* Se a chave for encontrada, 'posicao' apontar� para o */
      /* �ndice da chave na folha. Caso contr�rio, apontar�   */
      /* para o �ndice na folha na qual a chave ser� inserida */
   *posicao = BuscaEmNoBM(chave, &no, encontrado);

      /* Falta acrescentar a folha ao caminho de n�s */
   noCaminho.no = no;          /* Armazena o n�,...            */
   noCaminho.endereco = posNo; /* ... seu endere�o e...        */
   noCaminho.pos = indiceNo;   /* seu �ndice entre seus irm�os */

   Empilha(noCaminho, pilha);
}

/****
 *
 * InsereEmNoInterno(): Insere uma chave e seu respectivo filho
 *                      � direita num n� interno incompleto
 *
 * Par�metros:
 *    *pNo (entrada/sa�da) - n� no qual ser� feita a inser��o
 *     pos (entrada) - posi��o no array de chaves do n� no qual
 *                     a chave ser� inserida
 *     chave (entrada) - a chave que ser� inserida
 *     pFilho (entrada) - posi��o do filho a ser inserido �
 *                        direita da nova chave
 *
 * Retorno: Nada
 *
 * Observa��o: A atualiza��o do n� modificado por esta fun��o
 *             no arquivo que cont�m a �rvore fica sob
 *             responsabilidade de quem chama esta fun��o.
 *
 ****/
static void InsereEmNoInterno( tNoBM *pNo, int pos,
                               tChave chave, int pFilho )
{
   int i;

      /* Verifica se o ponteiro pNo � v�lido */
   ASSEGURA(pNo,"ERRO: Tentativa de inserir chave em no' nulo");

      /* Verifica se pNo aponta para um n� interno */
   ASSEGURA( pNo->tipoDoNo == INTERNO,
             "ERRO: No' e' folha em InsereEmNoInterno()" );

      /* Verifica se a posi��o de inser��o � v�lida */
   ASSEGURA( pos >= 0 && pos < pNo->conteudo.noInterno.nFilhos,
             "ERRO: Posicao de insercao invalida "
             "em InsereEmNoInterno()" );

       /* Abre espa�o para a nova chave */
   for (i = pNo->conteudo.noInterno.nFilhos - 1; i > pos; --i) {
      pNo->conteudo.noInterno.filhos[i + 1] =
         pNo->conteudo.noInterno.filhos[i];
      pNo->conteudo.noInterno.chaves[i] =
         pNo->conteudo.noInterno.chaves[i - 1];
   }

      /* Insere a nova chave e seu filho � direita */
   pNo->conteudo.noInterno.chaves[pos] = chave;
   pNo->conteudo.noInterno.filhos[pos + 1] = pFilho;

   pNo->conteudo.noInterno.nFilhos++; /* A prole aumentou */
}

/****
 *
 * InsereEmFolhaBM(): Insere uma chave e seu respectivo �ndice
 *                    de registro numa folha incompleta
 *
 * Par�metros:
 *    *pNo (entrada/sa�da) - folha na qual ser� feita a inser��o
 *     pos (entrada) - posi��o no array de chaves do n� no qual
 *                     a chave ser� inserida
 *     chaveIndice (entrada) - par chave/�ndice que ser� inserido
 *
 * Retorno: Nada
 *
 * Observa��o: A atualiza��o da folha modificada por esta
 *             fun��o no arquivo que cont�m a �rvore fica
 *             sob responsabilidade de quem chama esta fun��o
 *
 ****/
static void InsereEmFolhaBM( tNoBM *pNo, int pos,
                             tChaveIndice chaveIndice )
{
   int i;

      /* Verifica se pNo aponta para uma folha */
   ASSEGURA( pNo && pNo->tipoDoNo == FOLHA,
             "Tentativa de inserir chave em folha invalida" );

      /* Verifica se a posi��o de inser��o � v�lida */
   ASSEGURA( pos >= 0 && pos <= pNo->conteudo.noFolha.nChaves,
             "ERRO: Posicao de insercao invalida "
             "em InsereEmFolhaBM()" );

      /* Abre espa�o para a nova chave */
   for (i = pNo->conteudo.noFolha.nChaves; i > pos; --i)
              /* Chave aqui � estrutura! */
      pNo->conteudo.noFolha.chaves[i] =
         pNo->conteudo.noFolha.chaves[i - 1];

      /* Insere a nova chave */
   pNo->conteudo.noFolha.chaves[pos] = chaveIndice;

      /* O n�mero de chaves aumentou */
   pNo->conteudo.noFolha.nChaves++;
}

/****
 *
 * CopiaChavesBM(): Copia chaves de um n� interno para outro
 *
 * Par�metros:
 *    *pNo1 (entrada) - n� que doar� as chaves que ser�o copiadas
 *    *pNo2 (sa�da) - n� que receber� as chaves copiadas
 *     indiceI (entrada) - �ndice da primeira chave a ser copiada
 *     indiceF (entrada) - �ndice da �ltima chave a ser copiada
 *
 * Retorno: Nada
 *
 * Observa��o: A atualiza��o dos n�s modificados por esta
 *             fun��o no arquivo que cont�m a �rvore fica
 *             sob responsabilidade de quem chama esta fun��o
 *
 ****/
static void CopiaChavesBM( const tNoBM *pNo1, tNoBM *pNo2,
                           int indiceI, int indiceF )
{
   int i,
       nChaves = indiceF - indiceI + 1; /* N�mero de chaves */
                                        /* a copiar         */

      /* Verifica se os dois n�s s�o internos */
   ASSEGURA(pNo1->tipoDoNo==INTERNO && pNo2->tipoDoNo==INTERNO,
            "ERRO: Um no' e' interno e o outro e' folha em "
            "CopiaChavesBM()");

      /* Verifica se o n�mero de chaves � v�lido */
   ASSEGURA( nChaves < G,
             "Erro: Excesso de chaves em CopiaChavesBM()" );

      /* Idem */
   ASSEGURA(nChaves > 0,
            "Erro: Nao ha chaves para copiar em CopiaChavesBM()");

      /* Copia chaves e filhos do n� apontado por */
      /* pNo1 para o n� apontado por pNo2         */
   for (i = 0; i < nChaves; ++i) {
      pNo2->conteudo.noInterno.chaves[i] =
              pNo1->conteudo.noInterno.chaves[indiceI + i];
      pNo2->conteudo.noInterno.filhos[i] =
              pNo1->conteudo.noInterno.filhos[indiceI + i];
   }

      /* Falta copiar o �ltimo filho */
   pNo2->conteudo.noInterno.filhos[nChaves] =
            pNo1->conteudo.noInterno.filhos[indiceI + nChaves];

      /* Atualiza o n�mero de chaves do n� que recebeu */
      /* a doa��o. A atualiza��o do doador (pNo1) �    */
      /* feita por quem chama esta fun��o.             */
   pNo2->conteudo.noInterno.nFilhos = nChaves + 1;
}

/****
 *
 * CopiaChavesIndicesBM(): Copia chaves e respectivos �ndices de
 *                         uma folha para outra
 *
 * Par�metros:
 *    *pNo1 (entrada) - folha que doar� as chaves que ser�o
 *                      copiadas
 *    *pNo2 (sa�da) - folha que receber� as chaves copiadas
 *     indiceI (entrada) - �ndice da primeira chave a ser copiada
 *     indiceF (entrada) - �ndice da �ltima chave a ser copiada
 *
 * Retorno: Nada
 *
 * Observa��o: A atualiza��o das folhas modificadas por esta
 *             fun��o no arquivo que cont�m a �rvore fica sob
 *             responsabilidade de quem chama esta fun��o
 *
 ****/
static void CopiaChavesIndicesBM( const tNoBM *pNo1, tNoBM *pNo2,
                                  int indiceI, int indiceF )
{
   int i,
       nChaves = indiceF - indiceI + 1; /* N�mero de chaves */
                                        /* a copiar         */

      /* Verifica se os dois n�s s�o folhas */
   ASSEGURA( pNo1->tipoDoNo == FOLHA && pNo2->tipoDoNo == FOLHA,
            "ERRO: Um no' e' interno e o outro e' folha em "
            "CopiaChavesIndicesBM()");

      /* Verifica se o n�mero de chaves � v�lido */
   ASSEGURA(nChaves <= F,
            "Erro: Excesso de chaves em CopiaChavesIndicesBM()");

      /* Idem */
   ASSEGURA(nChaves > 0, "Erro: Sem chaves para copiar em "
            "CopiaChavesIndicesBM()");

      /* Copia chaves e �ndices da folha */
      /* *pNo1 para a folha *pNo2        */
   for (i = 0; i < nChaves; ++i) {
      pNo2->conteudo.noFolha.chaves[i] =
              pNo1->conteudo.noFolha.chaves[indiceI + i];
   }

      /* Atualiza o n�mero de chaves do n� que recebeu a */
      /* doa��o. A atualiza��o do doador (pNo1) � feita  */
      /* por quem chama esta fun��o.                     */
   pNo2->conteudo.noFolha.nChaves = nChaves;
}

/****
 *
 * DivideNoInternoBM(): Insere uma chave num n� interno completo de
 *                      uma �rvore B+, criando um novo n� para a
 *                      metade superior das chaves e mantendo a
 *                      metade inferior das chaves no n� dividido
 *
 * Par�metros:
 *    *pNoDiv (entrada/sa�da) - n� que ser� dividido
 *     pos (entrada) - posi��o na qual a nova chave ser� inserida
 *     chave (entrada) - a chave que ser� inserida
 *     pFilhoDireita (entrada) - posi��o em arquivo do n� que
 *                               ser� o filho � direita da chave
 *                               inserida
 *    *pNoDireita (sa�da) - novo n� no qual ser�o colocadas as
 *                          chaves superiores do n� dividido
 *    *chaveQueSobe (sa�da) - a chave que subir� para um n� no
 *                            n�vel imediatamente superior
 *
 * Retorno: Nada
 *
 * Observa��es:
 *    1. Existe uma diferen�a fundamental entre esta fun��o e
 *       a fun��o DivideFolhaBM(): aqui, quando um n� � dividido,
 *       a chave que sobe � "movida" para um n� interno no
 *       pr�ximo n�vel acima, enquanto que na fun��o
 *       DivideFolhaBM() a chave que sobe � "copiada" de uma
 *       folha para um n� interno.
 *    2. Se o grau da �rvore for par, a �rvore (i.e., cada n�
 *       interno) ter� tend�ncia � esquerda; i.e., o novo
 *       dividido ter� uma chave a mais do que o novo n�. N�o
 *       confunda isto com a tend�ncia das folhas que � � direita
 *    3. A atualiza��o dos n�s modificados por esta fun��o no
 *       arquivo que cont�m a �rvore fica sob responsabilidade
 *       de quem chama esta fun��o.
 *
 ****/
static void DivideNoInternoBM( tNoBM *pNoDiv, int pos,
                               tChave chave, int pFilhoDireita,
                               tNoBM *pNoDireita,
                               tChave *chaveQueSobe )
{
   static const int meio = (G - 1)/2; /* Evita recalcular este */
                                      /* valor a cada chamada  */
                                      /* desta fun��o          */

   ASSEGURA( pNoDiv->tipoDoNo == INTERNO, "Erro: Tentativa de "
             "dividir folha usando DivideNoInternoBM()" );

      /* Inicia o novo n� que receber� as chaves superiores */
   IniciaNoBM(INTERNO, pNoDireita);

      /* Efetua a inser��o com a devida divis�o de n�s */
   if (pos > meio) { /* Caso 1: A nova chave pertence */
                     /* ao novo n� da direita         */
         /* Copia metade menos uma chave  */
         /* do n� dividido para o novo n� */
      CopiaChavesBM(pNoDiv, pNoDireita, meio + 1, G - 2);

         /* Insere a nova chave no novo n� da direita */
      InsereEmNoInterno( pNoDireita, pos - meio - 1, chave,
                         pFilhoDireita );

         /* A chave que subir� � a �ltima chave do */
         /* n� que foi dividido depois da divis�o  */
      *chaveQueSobe = pNoDiv->conteudo.noInterno.chaves[meio];

         /* Ajusta o n�mero de n�s do n� dividido */
      pNoDiv->conteudo.noInterno.nFilhos = meio + 1;
   } else if (pos == meio) { /* Caso 2: A nova chave �     */
                             /* aquela que subir� um n�vel */
         /* Copia metade das chaves do */
         /* n� dividido para o novo n� */
      CopiaChavesBM(pNoDiv, pNoDireita, meio, G - 2);

         /* Ajusta o n�mero de n�s do n� dividido */
      pNoDiv->conteudo.noInterno.nFilhos = meio + 1;

         /* O filho direito da nova chave passa */
         /* a ser o primeiro filho do novo n�   */
      pNoDireita->conteudo.noInterno.filhos[0] = pFilhoDireita;

         /* A chave que subir� � a nova chave */
      *chaveQueSobe = chave;
   } else { /* Caso 3: A nova chave pertence */
            /* ao n� que ser� dividido       */
         /* Copia metade das chaves do */
         /* n� dividido para o novo n� */
      CopiaChavesBM(pNoDiv, pNoDireita, meio, G - 2);

         /* Ajusta o n�mero de n�s do n� dividido */
      pNoDiv->conteudo.noInterno.nFilhos = meio + 1;

         /* Insere a nova chave no novo n� da direita */
      InsereEmNoInterno(pNoDiv, pos, chave, pFilhoDireita);

         /* A chave que subir� � a �ltima chave do */
         /* n� que foi dividido depois da divis�o  */
      *chaveQueSobe = pNoDiv->conteudo.noInterno.chaves[meio];

         /* Corrige o n�mero de filhos do n� dividido, pois    */
         /* sua �ltima chave subir� para o pr�ximo n�vel acima */
      --pNoDiv->conteudo.noInterno.nFilhos;
   }
}

/****
 *
 * DivideFolhaBM(): Insere um par chave/�ndice numa folha e
 *                  divide este n�, criando um novo n� para a
 *                  metade superior das chaves e mantendo a
 *                  metade inferior das chaves no n� dividido
 *
 * Par�metros:
 *    *pNoDiv (entrada/sa�da) - folha que ser� dividida
 *     pos (entrada) - posi��o na qual a nova chave ser� inserida
 *     chaveIndice (entrada) - par chave/�ndice que ser� inserido
 *    *pNoDireita (sa�da) - novo n� no qual ser�o colocadas as
 *                          chaves superiores do n� dividido
 *    *chaveQueSobe (sa�da) - a chave que ser� copiada para o
 *                            n�-pai do n� dividido; o filho
 *                            direito desta chave ser� pNoDireita
 *
 * Retorno: Nada
 *
 * Observa��es:
 *    1. Existe uma diferen�a fundamental entre esta fun��o e
 *       a fun��o DivideNoInternoBM(): aqui, quando uma folha �
 *       dividida, a chave que sobe � "copiada" para um n�
 *       interno, enquanto que na fun��o DivideNoInternoBM() a
 *       chave que sobe � "movida" de um n� interno para outro
 *       um n�vel acima.
 *    2. Se o n�mero de chaves for par, as folhas ter�o tend�ncia
 *       � direita; i.e., o novo n� ter� uma chave a mais do que
 *       o n� dividido. N�o confunda isto com a tend�ncia da
 *       pr�pria �rvore (i.e., de cada n� interno) que � �
 *       esquerda.
 *    3. A atualiza��o dos n�s modificados por esta fun��o no
 *       arquivo que cont�m a �rvore fica sob responsabilidade
 *       de quem chama esta fun��o.
 *    4. O encadeamento das folhas fica sob responsabilidade
 *       de quem chama esta fun��o.
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

      /* Inicia o novo n� que receber� as chaves superiores */
   IniciaNoBM(FOLHA, pNoDireita);

      /* Divide o n� e efetua a inser��o no devido n� */
   if (pos > meio) { /* Caso 1: A nova chave pertencer� */
                     /* ao novo n� da direita           */
         /* Copia metade menos uma chave para o novo n� */
      CopiaChavesIndicesBM( pNoDiv, pNoDireita, meio + resto,
                            F - 1 );

         /* Ajusta o n�mero de n�s do   */
         /* n�mero de n� do n� dividido */
      pNoDiv->conteudo.noFolha.nChaves = meio + resto;

         /* Insere a nova chave no novo n� */
      InsereEmFolhaBM(pNoDireita, pos - meio - resto, chaveIndice);

         /* A chave que sobe para um n� interno */
         /* � a primeira chave da folha direita */
      *chaveQueSobe =
           pNoDireita->conteudo.noFolha.chaves[0].chave;
   } else { /* Caso 2: A nova chave pertencer� ao n� dividido */
         /* Copia metade das chaves para o novo n� */
      CopiaChavesIndicesBM( pNoDiv, pNoDireita, meio, F - 1 );

         /* Antes de inserir a chave no n� dividido */
         /* deve-se reduzir seu n�mero de chaves,   */
         /* pois neste instante ele est� completo   */
      pNoDiv->conteudo.noFolha.nChaves = meio;

         /* Insere a nova chave no novo n� */
      InsereEmFolhaBM(pNoDiv, pos, chaveIndice);

         /* A chave que sobe para um n� interno */
         /* � a primeira chave da folha direita */
      *chaveQueSobe = pNoDireita->conteudo.noFolha.chaves[0].chave;
   }
}

/****
 *
 * JuntaNosBM(): Combina um n� interno com outros n�s internos
 *               de uma �rvore B+ quando ele fica com grau
 *               abaixo do m�nimo
 *
 * Par�metros:
 *      pNoN (entrada/sa�da) - ponteiro para o n� que ser�
 *                             combinado com outros
 *      posNo (entrada) - posi��o do n� entre seus irm�os
 *      endNo (entrada) - posi��o do n� no arquivo
 *     *pilha (entrada/sa�da) - pilha que armazena os n�s
 *                              encontrados no caminho da raiz
 *                              at� o n� que ser� combinado
 *                              (sem inclu�-lo)
 *      stream (entrada) - stream associado ao arquivo que
 *                         cont�m a �rvore
 *
 * Retorno: Nada
 *
 * Observa��es:
 *    1. Esta fun��o � semelhante � fun��o JuntaNos() para �rvores B
 *
 *    2. Considerando as figuras apresentadas no texto, pNoN �
 *       um ponteiro para o n� N, pNoE � a posi��o no arquivo que
 *       cont�m a �rvore do n� E (aqui denominado noE) e pNoD � a
 *       posi��o no mesmo arquivo do n� D (aqui denominado noD).
 *
 *    3. Quando se faz refer�ncia a uma vari�vel que cont�m uma
 *       posi��o em arquivo, 'apontar para' n�o tem o mesmo
 *       significado dessa mesma express�o usada com ponteiros.
 *       I.e., aqui, 'apontar para' significa conter a posi��o
 *       de um n� no arquivo que o armazena.
 *
 *    4. V. figuras e algoritmo no texto. � muito dif�cil
 *       acompanhar esta fun��o sem examinar cuidadosamente
 *       essas figuras.
 *
 ****/
static void JuntaNosBM( tNoBM *pNoN, int posNo, int endNo,
                        tPilha* pilha, FILE *stream )
{
   tNoCaminho caminho; /* Armazenar� cada item desempilhado */
   tNoBM      noE, /* N� E das figuras do texto */
              noD; /* N� D das figuras do texto */
   int        pNoE = POSICAO_NULA,/* Posi��o em arquivo de noE */
              pNoD = POSICAO_NULA,/* Posi��o em arquivo de noD */
              i;
   tChave     sucessora; /* A chave sucessora � qual */
                         /* se refere o algoritmo    */

      /* Passo 1 do algoritmo */
   if (pNoN->conteudo.noInterno.nFilhos >= TG) {
         /* N� n�o precisa ser combinado com outro */
      return;
   }

   /*                                                     */
   /* Neste ponto, sabe-se que o n� apontado por pNoN tem */
   /* um n�mero de filhos menor do que o m�nimo desejado  */
   /*                                                     */

      /* A pilha n�o pode estar vazia aqui */
   ASSEGURA(!PilhaVazia(*pilha), "Pilha vazia em JuntaNosBM()");

      /* O item no topo da pilha cont�m informa��es */
      /* sobre o pai do n� apontado por pNoN        */
   caminho = Desempilha(pilha);

      /* Se o n� a ser combinado n�o � o primeiro */
      /* filho, ele tem um irm�o esquerdo         */
   if(posNo > 0)
         /* Faz pNoE apontar para o irm�o    */
         /* esquerdo do n� apontado por pNoN */
      pNoE = caminho.no.conteudo.noInterno.filhos[posNo - 1];

      /* Se o n� a ser combinado n�o � o �ltimo */
      /* filho, ele tem um irm�o direito        */
   if(posNo < caminho.no.conteudo.noInterno.nFilhos - 1)
         /* Faz pNoD apontar para o irm�o   */
         /* direito do n� apontado por pNoN */
      pNoD = caminho.no.conteudo.noInterno.filhos[posNo + 1];

      /* O n� a ser combinado deve ter pelo menos um  */
      /* irm�o mais pr�ximo ou, ent�o, ele deve ser a */
      /* raiz que n�o tem irm�os                      */
   ASSEGURA( pNoE != POSICAO_NULA || pNoD != POSICAO_NULA ||
             PilhaVazia(*pilha),
             "Erro em JuntaNosBM(): o no' nao possui irmao" );

      /* Passo 2: Verifique se o irm�o esquerdo (apontado */
      /* por pNoE) do n� apontado por pNoN tem um n�mero  */
      /* de chaves maior do que o m�nimo                  */

      /* Verifica se o n� apontado por pNoN tem irm�o esquerdo */
   if (pNoE != POSICAO_NULA) {
         /* L� o irm�o esquerdo noE */
      LeNoBM(stream, pNoE, &noE);

         /* Verifica se o irm�o esquerdo pode doar uma chave */
      if (noE.conteudo.noInterno.nFilhos > TG) {
            /* Passo 2.1: Insira a sucessora da �ltima */
            /* chave do n� noE no in�cio das chaves do */
            /* n� apontado por pNoN                    */

            /* Obt�m a sucessora que se encontra no n�-pai */
         sucessora = caminho.no.conteudo.noInterno.chaves[posNo-1];

             /* Abre espa�o para o novo filho */
         for (i = pNoN->conteudo.noInterno.nFilhos - 1; i >= 0; --i)
            pNoN->conteudo.noInterno.filhos[i + 1] =
               pNoN->conteudo.noInterno.filhos[i];

             /* Abre espa�o para a nova chave */
         for (i = pNoN->conteudo.noInterno.nFilhos - 2; i >= 0; --i)
            pNoN->conteudo.noInterno.chaves[i + 1] =
               pNoN->conteudo.noInterno.chaves[i];

            /* Coloca a sucessora em seu devido lugar */
         pNoN->conteudo.noInterno.chaves[0] = sucessora;

            /* Passo 2.2: Fa�a o filho esquerdo da primeira */
            /* chave de pNoN apontar para o filho direito   */
            /* da �ltima chave de noE                       */
         pNoN->conteudo.noInterno.filhos[0] =
            noE.conteudo.noInterno.filhos
               [noE.conteudo.noInterno.nFilhos - 1];

            /* Passo 2.3: Substitua a sucessora da �ltima */
            /* chave do n� noE por essa �ltima chave      */
         caminho.no.conteudo.noInterno.chaves[posNo - 1] =
             noE.conteudo.noInterno.chaves[
                     noE.conteudo.noInterno.nFilhos - 2 ];

            /* O n�-pai que foi alterado � uma c�pia daquele */
            /* que se encontra na �rvore. Logo, � preciso    */
            /* atualizar o n� correspondente na �rvore.      */
         EscreveNoBM(stream, caminho.endereco, &caminho.no);

            /* Passo 2.4: Decremente o n�mero de filhos de noE */
         noE.conteudo.noInterno.nFilhos--;

            /* O n� noE precisa ser atualizado em arquivo */
         EscreveNoBM(stream, pNoE, &noE);

            /* Passo 2.5: Incremente o n�mero de */
            /* filhos do n� apontado por pNoN    */
         pNoN->conteudo.noInterno.nFilhos++;

            /* O n� apontado por pNoN precisa */
            /* ser atualizado em arquivo      */
         EscreveNoBM(stream, endNo, pNoN);

            /* Passo 2.6: Encerre */
         return;
      }
   }

      /* Passo 3: Verifique se o irm�o direito (apontado */
      /* por pNoD) do n� apontado por pNoN tem um n�mero */
      /* de chaves maior do que o m�nimo                 */
   if (pNoD != POSICAO_NULA) {
         /* L� o irm�o direito noD */
      LeNoBM(stream, pNoD, &noD);

         /* Verifica se o irm�o direito */
         /* pode fazer doa��o de chaves */
      if (noD.conteudo.noInterno.nFilhos > TG) {
            /* Passo 3.1: Acrescente a sucessora da */
            /* �ltima chave do n� apontado por pNoN */
            /* ao final das chaves desse n�         */

            /* Obt�m a sucessora que se encontra no n�-pai */
         sucessora = caminho.no.conteudo.noInterno.chaves[posNo];

            /* Coloca a sucessora em seu devido lugar */
         pNoN->conteudo.noInterno.chaves
            [pNoN->conteudo.noInterno.nFilhos - 1] = sucessora;

            /* Passo 3.2: Fa�a o filho direito da �ltima    */
            /* chave de pNoN apontar para o filho esquerdo  */
            /* da primeira chave de noD. Note que o n�mero  */
            /* de filhos de pNoN ainda n�o foi incrementado */
         pNoN->conteudo.noInterno.filhos
            [pNoN->conteudo.noInterno.nFilhos] =
            noD.conteudo.noInterno.filhos[0];

            /* Passo 3.3: Substitua a sucessora da �ltima chave */
            /* do n� pNoN pela primeira chave do n� noD         */
         caminho.no.conteudo.noInterno.chaves[posNo] =
               noD.conteudo.noInterno.chaves[0];

             /* Passo 3.4: Desloque cada chave e filho */
             /* do n� noD uma posi��o para tr�s        */
         for (i = 1; i <= noD.conteudo.noInterno.nFilhos - 1; ++i)
            noD.conteudo.noInterno.filhos[i - 1] =
               noD.conteudo.noInterno.filhos[i];

         for (i = 1; i <= noD.conteudo.noInterno.nFilhos - 2; ++i)
            noD.conteudo.noInterno.chaves[i - 1] =
               noD.conteudo.noInterno.chaves[i];

            /* O n�-pai que foi alterado � uma c�pia daquele */
            /* que se encontra na �rvore. Logo � preciso     */
            /* atualizar o n� correspondente na �rvore.      */
         EscreveNoBM(stream, caminho.endereco, &caminho.no);

            /* Passo 3.5: Decremente o n�mero */
            /* de filhos do n� noD            */
         noD.conteudo.noInterno.nFilhos--;

            /* Passo 3.6: Incremente o n�mero de */
            /* filhos do n� apontado por pNoN    */
         pNoN->conteudo.noInterno.nFilhos++;

            /* O n� noD precisa ser atualizado em arquivo */
         EscreveNoBM(stream, pNoD, &noD);

            /* O n� apontado por pNoN precisa */
            /* ser atualizado em arquivo      */
         EscreveNoBM(stream, endNo, pNoN);

            /* Passo 3.7: Encerre */
         return;
      }
   }

   /*                                                     */
   /* Passo 4: Se nenhum irm�o vizinho do n� apontado por */
   /* pNoN tiver um n�mero de n�s maior do que o m�nimo   */
   /*                                                     */

      /* Passo 4.1: Verifique se o n� apontado */
      /* por pNoN tem irm�o direito (pNoD) com */
      /* n�mero de filhos igual ao m�nimo      */
   if (pNoD != POSICAO_NULA) {
         /* L� o irm�o direito noD */
      LeNoBM(stream, pNoD, &noD);

         /* Garante que o n�mero de filhos de noD � igual ao   */
         /* m�nimo. Se esse n�mero fosse maior do que o m�nimo */
         /* ter-se-�a o Caso 3. Se esse n�mero fosse menor do  */
         /* que o m�nimo, n�o se teria uma �rvore B+.          */
      ASSEGURA( noD.conteudo.noInterno.nFilhos == TG,
                "Numero de chaves em no' direito nao e' minimo" );

         /* Passo 4.1.1: Insira a chave sucessora (S) da  */
         /* �ltima chave do n� apontado por pNoN ao final */
         /* das chaves do n� apontado por pNoN            */

         /* Obt�m a sucessora */
      sucessora = caminho.no.conteudo.noInterno.chaves[posNo];

         /* Coloca a sucessora em seu devido lugar */
      pNoN->conteudo.noInterno.chaves
         [pNoN->conteudo.noInterno.nFilhos - 1] = sucessora;

         /* Passo 4.1.2: Fa�a o filho direito da �ltima  */
         /* chave de pNoN apontar para  o filho esquerdo */
         /* da primeira chave de noD. Note que o n�mero  */
         /* de filhos de pNoN ainda n�o foi incrementado */
      pNoN->conteudo.noInterno.filhos
         [pNoN->conteudo.noInterno.nFilhos] =
         noD.conteudo.noInterno.filhos[0];

         /* O n�mero de chaves e filhos do */
         /* n� apontado por pNoN aumentou  */
      ++pNoN->conteudo.noInterno.nFilhos;

         /* Passo 4.1.3: Desloque cada chave do pai do n�    */
         /* apontado por pNoN e seu filho direito , a partir */
         /* da chave sucessora, para uma posi��o anterior    */
      for ( i = posNo + 1;
            i < caminho.no.conteudo.noInterno.nFilhos - 1; ++i )
         caminho.no.conteudo.noInterno.filhos[i] =
            caminho.no.conteudo.noInterno.filhos[i + 1];

      for ( i = posNo;
            i < caminho.no.conteudo.noInterno.nFilhos - 2; ++i )
         caminho.no.conteudo.noInterno.chaves[i] =
            caminho.no.conteudo.noInterno.chaves[i + 1];

         /* Passo 4.1.4: Decremente o n�mero de filhos */
         /* do pai do n� apontado por pNoN             */
      --caminho.no.conteudo.noInterno.nFilhos;

         /* � preciso atualizar o n� */
         /* correspondente na �rvore */
      EscreveNoBM(stream, caminho.endereco, &caminho.no);

         /* Passo 4.1.5: Acrescente todas as chaves e filhos */
         /* direitos correspondentes do n� noD ao final das  */
         /* chaves e filhos do n� apontado por pNoN          */
      for ( i = 0; i < noD.conteudo.noInterno.nFilhos - 1; ++i)
         pNoN->conteudo.noInterno.filhos
             [pNoN->conteudo.noInterno.nFilhos + i] =
               noD.conteudo.noInterno.filhos[i + 1];

      for ( i = 0; i <= noD.conteudo.noInterno.nFilhos - 2; ++i)
         pNoN->conteudo.noInterno.chaves
             [pNoN->conteudo.noInterno.nFilhos + i - 1] =
               noD.conteudo.noInterno.chaves[i];

         /* Passo 4.1.6: Atualize o n�mero de */
         /* filhos do n� apontado por pNoN    */
      pNoN->conteudo.noInterno.nFilhos +=
          noD.conteudo.noInterno.nFilhos;

         /* Passo 4.1.7: Libere o n� noD. N�o h� como liberar */
         /* um n� no interior de um arquivo. Ent�o, a melhor  */
         /* op��o � indicar que esse � um n� vazio.           */
      noD.tipoDoNo = NO_VAZIO;

         /* O n� noD precisa ser atualizado em arquivo */
      EscreveNoBM(stream, pNoD, &noD);

         /* O n� apontado por pNoN precisa */
         /* ser atualizado em arquivo      */
      EscreveNoBM(stream, endNo, pNoN);

         /* Passo 4.1.8: Ajuste o pai do n�       */
         /* apontado por pNoN usando JuntaNosBM() */
      JuntaNosBM( &caminho.no, caminho.pos, caminho.endereco,
                  pilha, stream );

      return;
   }

      /* Passo 4.2: Verifique se o n� apontado */
      /* por pNoN tem irm�o esquerdo (noE) com */
      /* n�mero de filhos igual ao m�nimo      */
   if (pNoE != POSICAO_NULA) {
         /* L� o irm�o esquerdo noE */
      LeNoBM(stream, pNoE, &noE);

         /* Garante que o n�mero de filhos de noE � igual ao   */
         /* m�nimo. Se esse n�mero fosse maior do que o m�nimo */
         /* ter-se-�a o Caso 2. Se esse n�mero fosse menor do  */
         /* que o m�nimo, n�o se teria uma �rvore B+.          */
      ASSEGURA( noE.conteudo.noInterno.nFilhos == TG,
                "Numero de chaves em no' esquerdo nao e' minimo" );

         /* Passo 4.2.1: Insira ao final das chaves de noE */
         /* a chave sucessora da �ltima chave de noE       */

         /* Obt�m a sucessora */
      sucessora = caminho.no.conteudo.noInterno.chaves[posNo - 1];

         /* Coloca a sucessora em seu devido lugar */
      noE.conteudo.noInterno.chaves
         [noE.conteudo.noInterno.nFilhos - 1] = sucessora;

         /* Passo 4.2.2: Fa�a o filho direito da �ltima  */
         /* chave de noE apontar para  o filho esquerdo  */
         /* da primeira chave de pNoN. Note que ainda    */
         /* n�o houve incremento no n�mero de n�s de noE */
      noE.conteudo.noInterno.filhos
         [noE.conteudo.noInterno.nFilhos] =
         pNoN->conteudo.noInterno.filhos[0];

         /* O n�mero de chaves e filhos do n� noE aumentou */
      ++noE.conteudo.noInterno.nFilhos;

         /* Passo 4.2.3: Desloque cada chave do pai do n�   */
         /* apontado por pNoN e seu filho direito, a partir */
         /* da chave sucessora, para uma posi��o anterior   */
      for ( i = posNo;
            i < caminho.no.conteudo.noInterno.nFilhos - 1; ++i)
         caminho.no.conteudo.noInterno.filhos[i] =
            caminho.no.conteudo.noInterno.filhos[i + 1];

      for ( i = posNo - 1;
            i < caminho.no.conteudo.noInterno.nFilhos - 2; ++i)
         caminho.no.conteudo.noInterno.chaves[i] =
            caminho.no.conteudo.noInterno.chaves[i + 1];

         /* Passo 4.2.4: Decremente o n�mero de filhos */
         /* do pai do n� apontado por pNoN             */
      --caminho.no.conteudo.noInterno.nFilhos;

         /* � preciso atualizar o n� correspondente na �rvore */
      EscreveNoBM(stream, caminho.endereco, &caminho.no);

         /* Passo 4.2.5: Acrescente todas as chaves e filhos */
         /* direitos correspondentes do n� apontado por pNoN */
         /* ao final das chaves de noE                       */
      for (i = 0; i < pNoN->conteudo.noInterno.nFilhos - 1; ++i)
         noE.conteudo.noInterno.filhos
            [noE.conteudo.noInterno.nFilhos + i] =
            pNoN->conteudo.noInterno.filhos[i + 1];

      for (i = 0; i <= pNoN->conteudo.noInterno.nFilhos - 2; ++i)
         noE.conteudo.noInterno.chaves
            [noE.conteudo.noInterno.nFilhos + i - 1] =
            pNoN->conteudo.noInterno.chaves[i];

         /* Passo 4.2.6: Atualize o n�mero de filhos de noE */
      noE.conteudo.noInterno.nFilhos +=
         pNoN->conteudo.noInterno.nFilhos;

         /* Passo 4.2.7: Libere o n� apontado por pNoN. */
         /* V. coment�rio acima sobre libera��o de n�s. */
      pNoN->tipoDoNo = NO_VAZIO;

         /* O n� noE precisa ser atualizado em arquivo */
      EscreveNoBM(stream, pNoE, &noE);

         /* O n� apontado por pNoN precisa */
         /* ser atualizado em arquivo      */
      EscreveNoBM(stream, endNo, pNoN);

         /* Passo 4.2.8: Ajuste o pai do n�     */
         /* apontado por pNoE usando JuntaNosBM() */
      JuntaNosBM( &caminho.no, caminho.pos, caminho.endereco,
                  pilha, stream );
   }
}

/****
 *
 * AtualizaIndice(): Atualiza os n�s internos de uma �rvore B+
 *                   ap�s uma remo��o de chave
 *
 * Par�metros:
 *       streamArvore (entrada) - stream associado ao arquivo
 *                                que armazena a �rvore
 *      *pilha (entrada/sa�da) - pilha contendo o caminho de n�
 *                               internos at� a folha (sem inclu�-
 *                               la) na qual ocorreu uma remo��o
 *       removida (entrada) - chave que foi removida
 *       subst (entrada) - se n�o for NULL, aponta para uma chave
 *                         que substituir� a chave removida caso
 *                         ela seja encontrada num n� interno
 *       aRemover (entrada) - se n�o for NULL, aponta para uma
 *                            chave que ser� removida do �ndice
 *                            juntamente com seu filho direto
 *
 * Retorno: Nada
 *
 * Descri��o do algoritmo: v. texto
 *
 ****/
static void AtualizaIndice( FILE *streamArvore, tPilha *pilha,
                            tChave removida, tChave *subst,
                            tChave *aRemover )
{
   int        i, j,
              nChaves; /* N�mero de chaves de um n� */
   tNoCaminho caminho; /* Um elemento da pilha que    */
                       /* representa o caminho de n�s */
   tPilha     pilhaAux; /* Pilha auxiliar */

      /* Se houver chave a remover e a substituir no     */
      /* �ndice, ser� necess�rio usar uma pilha auxiliar */
   CriaPilha(&pilhaAux);

      /***                                 ***/
      /* A primeira parte desta fun��o lida  */
      /* com substitui��o de chave no �ndice */
      /***                                 ***/

   while (subst && !PilhaVazia(*pilha)) {
      caminho = Desempilha(pilha);

      ASSEGURA( caminho.no.tipoDoNo == INTERNO,
                "Em AtualizaIndice(): No' deveria ser interno" );

         /* Obt�m o n�mero de chaves do n� corrente */
      nChaves = caminho.no.conteudo.noInterno.nFilhos - 1;

         /* Procura pela chave a ser substitu�da no n� corrente */
      for (i = 0; i < nChaves; ++i) {
         if ( removida ==
                  caminho.no.conteudo.noInterno.chaves[i] ) {
               /* A chave removida foi encontrada no */
               /* n� corrente e ser� substitu�da     */
            caminho.no.conteudo.noInterno.chaves[i] = *subst;

               /* Atualiza n� em arquivo */
            EscreveNoBM(streamArvore, caminho.endereco, &caminho.no);

            subst = NULL; /* S� h� uma chave a substituir */

            break;
         }
      }

         /* Se haver� remo��o, � preciso empilhar */
         /* o n� na pilha auxiliar                */
      if (aRemover) {
         Empilha(caminho, &pilhaAux);
      }
   }

      /***                               ***/
      /* A segunda parte desta fun��o lida */
      /* com remo��o de chave no �ndice    */
      /***                               ***/

      /* Se n�o h� chave a remover no �ndice, o */
      /* trabalho desta fun��o est� encerrado   */
   if (!aRemover) {
      EsvaziaPilha(pilha); /* A pilha n�o ser� necess�ria */
      return; /* Servi�o conclu�do */
   } else { /* Restaura a pilha, se for o caso */
      while (!PilhaVazia(pilhaAux)) {
         Empilha(Desempilha(&pilhaAux), pilha);
      }
   }

      /***                            ***/
      /* Esta fun��o s� remove chave no */
      /* �ltimo n�vel de n�s internos   */
      /***                            ***/

      /* Neste ponto, a pilha n�o pode estar vazia */
   ASSEGURA(!PilhaVazia(*pilha),
            "A pilha nao deveria estar vazia em AtualizaIndice()");

      /* Obt�m informa��es sobre o n� interno do �ltimo */
      /* n�vel que se encontra no topo da pilha         */
   caminho = Desempilha(pilha);

      /* O n� desempilhado deve ser interno */
   ASSEGURA( caminho.no.tipoDoNo == INTERNO,
             "Erro em AtualizaIndice(): No' deveria ser interno" );

      /* Obt�m o n�mero de chaves do n� corrente */
   nChaves = caminho.no.conteudo.noInterno.nFilhos - 1;

      /* Procura pela chave a ser removida no n� corrente */
   for (i = 0; i < nChaves; ++i) {
      if ( *aRemover ==
               caminho.no.conteudo.noInterno.chaves[i] ) {
            /* A chave a ser removida foi encontrada. */
            /* Move cada chave uma posi��o para tr�s. */
         for (j = i; j < nChaves - 1; ++j)
            caminho.no.conteudo.noInterno.chaves[j] =
               caminho.no.conteudo.noInterno.chaves[j + 1];

            /* Faz o mesmo com cada filho direito */
         for ( j = i + 1;
               j < caminho.no.conteudo.noInterno.nFilhos - 1; ++j )
            caminho.no.conteudo.noInterno.filhos[j] =
               caminho.no.conteudo.noInterno.filhos[j + 1];

            /* O n�mero de filhos do n� foi reduzido */
         caminho.no.conteudo.noInterno.nFilhos--;

         aRemover = NULL; /* N�o h� mais chave a remover */

         break;
      }
   }

      /* A chave deve ter sido encontrada e removida */
   ASSEGURA( !aRemover,
             "Chave a remover nao foi encontrada em no' interno" );

      /* Atualiza n� em arquivo */
   EscreveNoBM( streamArvore, caminho.endereco, &caminho.no );

      /* Verifica se o n� ficou descompensado */
   if (caminho.no.conteudo.noInterno.nFilhos < TG) {
         /* Chama JuntaNosBM() para completar o servi�o */
      JuntaNosBM( &caminho.no, caminho.pos, caminho.endereco,
                  pilha, streamArvore );
   } else {
      EsvaziaPilha(pilha);
   }
}

/******************** Fun��es de Globais ******************/

/****
 *
 * LeNoBM(): L� em arquivo um n� de uma �rvore B+
 *
 * Par�metros:
 *     stream (entrada) - stream associado ao arquivo contendo
 *                        a �rvore na qual ser� feita a leitura
 *     pos (entrada) - posi��o no arquivo na qual ser� feita a
 *                     leitura
 *    *no (sa�da) - ponteiro para o n� que conter� o resultado
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
      /* de leitura; se n�o conseguir aborta o programa  */
   MoveApontador(stream, sizeof(tNoBM)*pos, SEEK_SET);

      /* Efetua a leitura */
   fread(no, sizeof(tNoBM), 1, stream);

      /* Certifica-se que n�o houve erro de leitura */
   ASSEGURA(!ferror(stream), ERRO_FREAD(LeNoBM));
}

/****
 *
 * BuscaBM(): Faz buscas numa �rvore B+
 *
 * Par�metros:
 *     chave (entrada) - a chave de busca
 *     raiz (entrada) - ponteiro para a raiz da �rvore na qual
 *                      ser� feita a busca
 *     stream (entrada) - stream representando o arquivo no qual
 *                        a �rvore reside
 *
 * Retorno: �ndice do registro no arquivo de registros se a
 *          chave for encontrada; POSICAO_NULA, se a chave
 *          n�o for encontrada.
 *
 ****/
int BuscaBM(FILE *stream, const tNoBM *raiz, tChave chave)
{
   int   i,
         encontrado = 0, /* Informa se a chave foi encontrada */
         posNo = 0; /* Posi��o no n� no arquivo */
   tNoBM no; /* Armazenar� cada n� visitado */

      /* Verifica se o stream � v�lido */
   ASSEGURA( stream, ERRO_OPEN(BuscaBM) );

   no = *raiz;

      /* Desce na �rvore at� encontrar uma folha */
   while (no.tipoDoNo == INTERNO) {
      i = BuscaEmNoBM(chave, &no, &encontrado);

         /* Desce at� o pr�ximo n� */
      posNo = encontrado ? no.conteudo.noInterno.filhos[i + 1]
                         : no.conteudo.noInterno.filhos[i];
      LeNoBM(stream, posNo, &no);
   }

      /* Chegou-se a uma folha. Se a chave se encontrar */
      /* na �rvore, ela deve estar nessa folha          */
   i = BuscaEmNoBM(chave, &no, &encontrado);

   return encontrado ? no.conteudo.noFolha.chaves[i].indice
                     : POSICAO_NULA;
}

/****
 *
 * InsereBM(): Faz busca e inser��o numa �rvore B+ armazenada
 *             em arquivo
 *
 * Par�metros:
 *    *raiz (entrada/sa�da) - ponteiro para a raiz da �rvore
 *                              (armazenada em mem�ria)
 *     streamArvore (entrada) - stream associado ao arquivo que
 *                              cont�m a �rvore
 *    *chaveIndice (entrada/sa�da) - par chave/�ndice contendo
 *                                   a chave a ser inserida
 *
 * Retorno: 1, se houver inser��o; 0, em caso contr�rio.
 *
 * Descri��o do algoritmo: v. texto
 *
 * Observa��es: A chave � considerada prim�ria; i.e., chaves
 *              repetidas n�o s�o permitidas na �rvore.
 *
 ****/
int InsereBM( tNoBM *raiz, tChaveIndice *chaveIndice,
              FILE *streamArvore )
{
   int         encontrado, /* A chave foi encontrada? */
               indiceDaChave; /* �ndice da chave no n� no */
                              /* qual ela �, ou deveria   */
                              /* ser, encontrada          */
   tChave      chaveAInserir, /* A chave a ser inserida */
                              /* no n�vel corrente      */
               chaveQueSobe;  /* A chave a ser inserida */
                              /* um n�vel acima         */
   tNoBM       pNoDireito; /* N� � direita que conter� */
                           /* as chaves superiores de  */
                           /* um n� dividido           */
   int         pFilhoDireito; /* Posi��o no arquivo do n� que */
                              /* ser� filho � direita de uma  */
                              /* chave inserida               */
   tNoCaminho  noCaminho; /* Um elemento da pilha que    */
                          /* representa o caminho de n�s */
   tPilha      pilha; /* Pilha que armazenar� o   */
                      /* caminho de n�s visitados */

      /* Verifica a validade do stream */
   ASSEGURA( streamArvore,
             ERRO_STREAM_NULL(streamArvore, InsereBM) );

      /* Verifica se o ponteiro para a raiz � v�lido */
   ASSEGURA(raiz, "ERRO: A raiz � NULL em InsereBM\n");

      /* Verifica se a �rvore est� vazia */
   if (raiz->tipoDoNo == NO_VAZIO) { /* A �rvore ainda � virgem */
      IniciaNoBM(FOLHA, raiz); /* Inicia a raiz da �rvore */
      raiz->conteudo.noFolha.chaves[0] = *chaveIndice;
      raiz->conteudo.noFolha.nChaves = 1;

         /* Armazena a raiz da �rvore no arquivo, */
         /* no qual ela ocupa a posi��o 0         */
      EscreveNoBM(streamArvore, 0, raiz);

      return 1;
   }

      /* Encontra o caminho que conduz at� o local  */
      /* de inser��o ou onde a chave j� se encontra */
   EncontraCaminhoBM( streamArvore, chaveIndice->chave, raiz,
                      &indiceDaChave, &encontrado, &pilha );

      /* Verifica se a chave foi encontrada */
   if (encontrado) {
         /* A chave foi encontrada e n�o precisa inseri-la. */
         /* O conte�do da pilha n�o � mais necess�rio       */
      EsvaziaPilha(&pilha);

      return 0; /* A chave � prim�ria */
   }

      /* A chave n�o foi encontrada. Insere a chave */
      /* na folha que deve estar no topo da pilha   */

   noCaminho = Desempilha(&pilha);

   ASSEGURA( noCaminho.no.tipoDoNo == FOLHA,
             "Erro em InsereBM(): No' deveria ser folha" );

      /* Caso 1: H� espa�o para inser��o na folha */
   if (noCaminho.no.conteudo.noFolha.nChaves < F) {
      InsereEmFolhaBM(&noCaminho.no, indiceDaChave, *chaveIndice);

         /* Atualiza o n� no arquivo */
      EscreveNoBM(streamArvore, noCaminho.endereco, &noCaminho.no);

         /* Se a raiz da �rvore foi atualizada no */
         /* arquivo sua c�pia mantida em mem�ria  */
         /* tamb�m precisa ser atualizada         */
      if (!noCaminho.endereco)
            /* A posi��o da raiz no arquivo � zero */
         *raiz = noCaminho.no;

         /* O conte�do da pilha n�o � mais necess�rio */
      EsvaziaPilha(&pilha);

      return 1;
   }

      /* Caso 2: N�o h� espa�o para inser��o na folha */

   DivideFolhaBM( &noCaminho.no, indiceDaChave, *chaveIndice,
                  &pNoDireito, &chaveQueSobe );

      /* Faz o encadeamento de 'pNoDireito' com a folha  */
      /* para qual aponta correntemente a folha dividida */
   pNoDireito.conteudo.noFolha.proximaFolha =
        noCaminho.no.conteudo.noFolha.proximaFolha;

   /****                                                     ****/
   /* Armazena pNoDireito no arquivo. A fun��o AcrescentaNoBM() */
   /* retorna a posi��o na qual o n� foi armazenado no arquivo. */
   /* 'pFilhoDireito' armazena essa posi��o.                    */
   /****                                                     ****/

      /* O filho � direita da chave que subir� para */
      /* o n�vel superior ser� a nova folha criada  */
   pFilhoDireito = AcrescentaNoBM(streamArvore, &pNoDireito);

      /* Faz o encadeamento da folha   */
      /* dividida com sua pr�xima irm� */
   noCaminho.no.conteudo.noFolha.proximaFolha = pFilhoDireito;

      /* Atualiza a folha dividida no arquivo */
   EscreveNoBM(streamArvore, noCaminho.endereco, &noCaminho.no);

      /* Se a raiz da �rvore foi atualizada no arquivo sua   */
      /* c�pia mantida em mem�ria tamb�m deve ser atualizada */
   if (!noCaminho.endereco)
      *raiz = noCaminho.no;

      /* A seguir, percorre-se o caminho inverso at� */
      /* encontrar a raiz ou um n� incompleto.       */

   indiceDaChave = noCaminho.pos;
   chaveAInserir = chaveQueSobe;

   while (!PilhaVazia(pilha)) {
      noCaminho = Desempilha(&pilha);

      ASSEGURA( noCaminho.no.tipoDoNo == INTERNO,
                "Erro em InsereBM(): No' deveria ser interno" );

         /* Verifica se o n� est� completo */
      if (noCaminho.no.conteudo.noInterno.nFilhos < G) {
            /* D� para inserir neste n� */
         InsereEmNoInterno( &noCaminho.no, indiceDaChave,
                            chaveAInserir, pFilhoDireito );

            /* Atualiza o n� no arquivo */
         EscreveNoBM( streamArvore, noCaminho.endereco,
                      &noCaminho.no );

            /* Se a raiz da �rvore foi atualizada no */
            /* arquivo sua c�pia mantida em mem�ria  */
            /* tamb�m deve ser atualizada            */
         if (!noCaminho.endereco)
               /* A raiz ocupa a posi��o zero no arquivo */
            *raiz = noCaminho.no;

            /* A pilha n�o � mais necess�ria */
         EsvaziaPilha(&pilha);

         return 1;
      }

          /* O n� corrente � completo e precisa ser dividido */
      DivideNoInternoBM( &noCaminho.no, indiceDaChave,
                         chaveAInserir, pFilhoDireito,
                         &pNoDireito, &chaveQueSobe );

         /* Atualiza o n� dividido no arquivo */
      EscreveNoBM(streamArvore, noCaminho.endereco, &noCaminho.no);

         /* Se a raiz da �rvore foi atualizada no */
         /* arquivo sua c�pia mantida em mem�ria  */
         /* tamb�m deve ser atualizada            */
      if (!noCaminho.endereco)
         *raiz = noCaminho.no;

         /* O filho � direita da chave que subir� para o   */
         /* n�vel superior ser� o novo n� criado no n�vel  */
         /* corrente. Armazena este n� no arquivo e guarda */
         /* sua posi��o neste arquivo.                     */
      pFilhoDireito = AcrescentaNoBM(streamArvore, &pNoDireito);

      indiceDaChave = noCaminho.pos;
      chaveAInserir = chaveQueSobe;
   }

   /****                                                   ****/
   /* Neste ponto, sabe-se que o �ltimo n� desempilhado � uma */
   /* raiz completa, que j� foi dividida no la�o while ou     */
   /* antes se a raiz era tamb�m uma folha. Resta criar uma   */
   /* nova raiz contendo a chave que subiu at� ela tendo como */
   /* filho esquerdo o n� dividido e � direita o novo n�      */
   /****                                                   ****/

   IniciaNoBM(INTERNO, raiz); /* Inicia a nova raiz */

      /* Copia a chave */
   raiz->conteudo.noInterno.chaves[0] = chaveQueSobe;

      /* O filho da esquerda da nova raiz � a antiga raiz que */
      /* precisa ser armazenada em outra posi��o no arquivo   */
      /* porque a raiz deve sempre ocupar a primeira posi��o  */
   raiz->conteudo.noInterno.filhos[0] =
        AcrescentaNoBM(streamArvore, &noCaminho.no);

      /* O filho � direita da raiz j� foi inserido antes */
      /* no arquivo e sua posi��o neste arquivo est�     */
      /* guardada em 'pFilhoDireito'                     */
   raiz->conteudo.noInterno.filhos[1] = pFilhoDireito;

      /* A nova raiz tem dois filhos */
   raiz->conteudo.noInterno.nFilhos = 2;

      /* Armazena a raiz da �rvore no arquivo, */
      /* no qual ela ocupa a posi��o 0         */
   EscreveNoBM(streamArvore, 0, raiz);

   /****                                   ****/
   /* Neste ponto, a c�pia da raiz armazenada */
   /* em mem�ria j� est� atualizada           */
   /****                                   ****/

   return 1;
}

/****
 *
 * RemoveBM(): Remove a chave especificada de uma �rvore
 *             B+ armazenada em arquivo
 *
 * Par�metros:
 *       streamArvore (entrada) - stream associado ao arquivo
 *                                que armazena a �rvore
 *      *raiz (entrada/sa�da) - raiz da �rvore
 *       chave (entrada) - a chave que ser� removida
 *
 * Retorno: 1, se a remo��o for bem sucedida; 0, em caso contr�rio
 *
 * Descri��o do algoritmo: v. texto
 *
 ****/
int RemoveBM(FILE *streamArvore, tNoBM *raiz, tChave chave)
{
   int        i,
              encontrado, /* A chave foi encontrada? */
              indiceDaChave, /* �ndice da chave na follha na  */
                             /* qual ela est� ou deveria estar */
              nChaves, /* N�mero de chaves de uma folha */
              posFolha, /* Posi��o da folha entre suas irm�s */
              endFolha, /* Posi��o da folha no arquivo */
              endIrma; /* Posi��o de uma irm� da folha no arquivo */
   tPilha     pilha; /* Pilha que armazenar� o caminho */
                     /* de n�s visitados               */
   tNoCaminho caminho; /* Um elemento da pilha que    */
                       /* representa o caminho de n�s */
   tChave     substituta, /* Chave que substituir� a  */
                          /* chave removida no �ndice */
              movida, /* Chave movida de uma folha para outra */
              aRemover; /* Chave que ser� removida no n�-pai */
   tNoBM      irma, /* Irm� da folha na qual ocorreu a remo��o */
              folha; /* Folha na qual ocorre a remo��o */

      /* Se a �rvore for vazia, n�o h� chave para remover */
   if (!raiz)
      return 0; /* N�o h� chave para remover */

      /* Encontra o caminho da raiz at� o n� */
      /* que cont�m a chave a ser removida   */
   EncontraCaminhoBM( streamArvore, chave, raiz, &indiceDaChave,
                      &encontrado, &pilha );

      /* Se a chave n�o for encontrada, n�o h� mais nada a  */
      /* fazer. Mas, antes de retornar, esvazia-se a pilha. */
   if (!encontrado) {
      EsvaziaPilha(&pilha);
      return 0; /* N�o h� chave para remover */
   }

   /***                                           ***/
   /** A chave foi encontrada e informa��es sobre  **/
   /** a folha que a cont�m est�o no topo da pilha **/
   /***                                           ***/

      /* Neste ponto, a pilha n�o pode estar vazia */
   ASSEGURA(!PilhaVazia(pilha),
            "A pilha nao deveria estar vazia em RemoveBM()");

      /* No topo da pilha, encontra-se a folha */
      /* na qual a remo��o ser� efetuada       */
   caminho = Desempilha(&pilha);

      /* Garante que o elemento no topo da pilha era um folha */
   ASSEGURA( caminho.no.tipoDoNo == FOLHA,
             "ERRO: No' deveria ser folha em RemoveBM()" );

      /* Facilita a legibilidade */
   nChaves = caminho.no.conteudo.noFolha.nChaves;

      /* Move as chaves uma posi��o para tr�s */
   for ( i = indiceDaChave; i < nChaves - 1; ++i )
      caminho.no.conteudo.noFolha.chaves[i] =
         caminho.no.conteudo.noFolha.chaves[i + 1];

      /* O n�mero de chaves da folha foi reduzido */
   nChaves = --caminho.no.conteudo.noFolha.nChaves;

      /* Atualiza a folha no arquivo */
   EscreveNoBM(streamArvore, caminho.endereco, &caminho.no);

      /* Verifica se a folha � a raiz da �rvore */
   if (PilhaVazia(pilha)) {
      *raiz = caminho.no; /* Atualiza a raiz que se encontra */
                          /* em mem�ria principal            */
      return 1; /* Servi�o completo */
   }

      /* Se a chave removida era a menor chave da folha e o n�mero */
      /* de chaves na folha continuar acima do m�nimo requerido,   */
      /* resta apenas substituir a chave removida no �ndice        */
   if (!indiceDaChave && nChaves >= TF) {
      substituta = caminho.no.conteudo.noFolha.chaves[0].chave;
      AtualizaIndice(streamArvore, &pilha, chave, &substituta, NULL);
      return 1;
   } else if (nChaves >= TF) {
         /* A chave removida n�o era a primeira da folha e o */
         /* n�mero de chaves nela permanece dentro do limite */
      EsvaziaPilha(&pilha);
      return 1; /* N�o resta mais nada a fazer */
   }

      /***                                                  ***/
      /* Neste ponto, sabe-se que o n�mero de chaves na folha */
      /* ficou abaixo do m�nimo requerido. Agora existem duas */
      /* op��es: (1) redistribui��o de chaves ou (2) fus�o de */
      /* folhas.                                              */
      /***                                                  ***/

      /* Guarda a folha na qual ocorreu a remo��o */
   folha = caminho.no;

      /* Guarda a posi��o da folha no arquivo */
   endFolha = caminho.endereco;

      /* Guarda a posi��o da folha entre suas irm�s */
   posFolha = caminho.pos;

      /* Obt�m o n� interno que se encontra */
      /* no topo da pilha sem desempilh�-lo */
   caminho = ElementoTopo(&pilha);

      /* Garante que o n� no topo da pilha � interno */
   ASSEGURA( caminho.no.tipoDoNo == INTERNO,
             "ERRO: No' deveria ser interno em RemoveBM()" );

      /*************************************/
      /* Op��o 1: redistribui��o de chaves */
      /* =======                           */
      /*************************************/

      /* Verifica se a folha possui irm� � esquerda */
   if (posFolha) {
         /* Obt�m a posi��o da irm� esquerda no arquivo */
      endIrma = caminho.no.conteudo.noInterno.filhos[posFolha - 1];

         /* L� a irm� esquerda da folha */
      LeNoBM( streamArvore, endIrma, &irma );

         /* O n� lido deve ser uma folha */
      ASSEGURA( irma.tipoDoNo == FOLHA,
                "ERRO: No' deveria ser folha em RemoveBM()" );

         /* Verifica se a irm� pode ceder uma chave */
      if (irma.conteudo.noInterno.nFilhos > TF) {
            /*                                           */
            /* Move a �ltima chave da irm� esquerda para */
            /* o in�cio da folha que ficou deficiente    */
            /*                                           */

            /* Primeiro, abre espa�o para a nova chave */
         for ( i = folha.conteudo.noFolha.nChaves - 1; i >= 0;
               i-- )
            folha.conteudo.noFolha.chaves[i + 1] =
                            folha.conteudo.noFolha.chaves[i];

            /* Obt�m a chave que ser� substituta */
         substituta = irma.conteudo.noFolha.chaves[
                      irma.conteudo.noFolha.nChaves - 1 ].chave;

            /* Copia a chave */
         folha.conteudo.noFolha.chaves[0] =
            irma.conteudo.noFolha.chaves
            [irma.conteudo.noFolha.nChaves - 1];

            /* O n�mero de chaves da folha aumentou e  */
            /* o n�mero de chaves de sua irm� diminuiu */
         folha.conteudo.noFolha.nChaves++;
         irma.conteudo.noFolha.nChaves--;

            /* Atualiza a folha e sua irm� no arquivo */
         EscreveNoBM(streamArvore, endFolha, &folha);
         EscreveNoBM(streamArvore, endIrma, &irma);

            /* Atualiza os n�s internos */
         AtualizaIndice( streamArvore, &pilha, chave, &substituta,
                         NULL );

         return 1;
      }
   } else {  /* A folha deve possuir irm� � direita */
      ASSEGURA(posFolha < caminho.no.conteudo.noInterno.nFilhos - 1,
               "ERRO: A folha deveria possuir irma direita");

         /* Obt�m a posi��o da folha no arquivo */
      endIrma = caminho.no.conteudo.noInterno.filhos[posFolha + 1];

         /* L� a irm� direita da folha */
      LeNoBM( streamArvore, endIrma, &irma );

         /* O n� lido deve ser uma folha */
      ASSEGURA( irma.tipoDoNo == FOLHA,
                "ERRO: No' deveria ser folha em RemoveBM()" );

         /* Verifica se a irm� pode ceder uma chave */
      if (irma.conteudo.noInterno.nFilhos > TF) {
            /* Obt�m a chave a ser movida */
         movida = irma.conteudo.noFolha.chaves[0].chave;

            /* Move a primeira chave da irm� direita para */
            /* o final da folha que ficou deficiente      */
         folha.conteudo.noFolha.chaves
              [folha.conteudo.noFolha.nChaves] =
              irma.conteudo.noFolha.chaves[0];

            /* Remove a chave movida da irm� */
         for (i = 0; i > irma.conteudo.noFolha.nChaves - 1; ++i)
            irma.conteudo.noFolha.chaves[i] =
                    irma.conteudo.noFolha.chaves[i + 1];

            /* A irm� � uma folha direita. Portanto, */
            /* sua primeira chave aparece no n� pai, */
            /* que precisar� ser atualizado          */
         for ( i = 0; i < caminho.no.conteudo.noInterno.nFilhos - 1;
               ++i ) {
            if (caminho.no.conteudo.noInterno.chaves[i] == movida) {
               caminho.no.conteudo.noInterno.chaves[i] =
                 irma.conteudo.noFolha.chaves[0].chave;
            }
         }

            /* A chave movida deve ter sido */
            /* encontrada no n� interno     */
         ASSEGURA( i < caminho.no.conteudo.noInterno.nFilhos,
                   "Chave a ser substituida nao foi encontrada");

            /* Atualiza o n� interno no arquivo */
         EscreveNoBM(streamArvore, caminho.endereco, &caminho.no);

            /* O n�mero de chaves da folha aumentou e  */
            /* o n�mero de chaves de sua irm� diminuiu */
         folha.conteudo.noFolha.nChaves++;
         irma.conteudo.noFolha.nChaves--;

            /* Atualiza a folha e sua irm� no arquivo */
         EscreveNoBM(streamArvore, endFolha, &folha);
         EscreveNoBM(streamArvore, endIrma, &irma);

            /* Verifica se a chave removida aparece no �ndice */
         if (!indiceDaChave) {
               /* A chave substituta � a primeira chave da folha */
            substituta = folha.conteudo.noFolha.chaves[0].chave;

               /* O elemento que se encontra no topo da pilha */
               /* foi alterado e � preciso desempilh�-lo e    */
               /* empilhar o elemento alterado                */
            ASSEGURA( !PilhaVazia(pilha),
                      "ERRO: Pilha vazia em RemoveBM()" );
            (void) Desempilha(&pilha);
            Empilha(caminho, &pilha);

               /* Atualiza os n�s internos */
            AtualizaIndice( streamArvore, &pilha, chave,
                            &substituta, NULL );
         } else { /* A chave n�o aparece no �ndice */
            EsvaziaPilha(&pilha);
            return 1; /* N�o resta mais nada a fazer */
         }

         return 1;
      }
   }

      /****************************/
      /* Op��o 2: fus�o de folhas */
      /* =======                  */
      /****************************/

      /* Verifica se a folha possui irm� � esquerda */
   if (posFolha) {
         /* Obt�m a posi��o da folha no arquivo */
      endIrma = caminho.no.conteudo.noInterno.filhos[posFolha - 1];

         /* L� a irm� esquerda da folha */
      LeNoBM( streamArvore, endIrma, &irma );

         /* O n� lido deve ser uma folha */
      ASSEGURA( irma.tipoDoNo == FOLHA,
                "ERRO: No' deveria ser folha em RemoveBM()" );

         /* A irm� deve estar no limite m�nimo de chave */
      ASSEGURA( irma.conteudo.noFolha.nChaves == TF,
                "Folha nao tem numero minimo de chaves");
            /*                                         */
            /* Move as chaves da folha na qual ocorreu */
            /* a remo��o para sua irm� esquerda        */
            /*                                         */

         /* Se a chave removida n�o era a primeira da folha, */
         /* a chave a ser removida no n� pai ser� a primeira */
         /* chave da folha na qual ocorreu a remo��o         */
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

         /* O n�mero de chaves da folha ficou nulo  */
         /* o n�mero de chaves de sua irm� aumentou */
      folha.tipoDoNo = NO_VAZIO;
      irma.conteudo.noFolha.nChaves +=
          folha.conteudo.noFolha.nChaves;

         /* Preserva o encadeamento das folhas */
      irma.conteudo.noFolha.proximaFolha =
          folha.conteudo.noFolha.proximaFolha;

         /* Atualiza a folha e sua irm� no arquivo */
      EscreveNoBM(streamArvore, endFolha, &folha);
      EscreveNoBM(streamArvore, endIrma, &irma);

         /* Chama AtualizaIndice() para completar o servi�o */
      AtualizaIndice(streamArvore, &pilha, chave, NULL, &aRemover);
   } else {  /* A folha deve possuir irm� � direita */
      ASSEGURA(posFolha < caminho.no.conteudo.noInterno.nFilhos - 1,
               "ERRO: A folha deveria possuir irma direita");

         /* Obt�m a posi��o da irm� no arquivo */
      endIrma = caminho.no.conteudo.noInterno.filhos[posFolha + 1];

         /* L� a irm� direita da folha */
      LeNoBM( streamArvore, endIrma, &irma );

         /* O n� lido deve ser uma folha */
      ASSEGURA( irma.tipoDoNo == FOLHA,
                "ERRO: No' deveria ser folha em RemoveBM()" );

         /* A irm� deve estar no limite m�nimo de chave */
      ASSEGURA( irma.conteudo.noFolha.nChaves == TF,
                "Folha nao tem numero minimo de chaves");
            /*                                       */
            /* Move as chaves da irm� direita para a */
            /* folha na qual ocorreu a remo��o       */
            /*                                       */

         /* Se a chave removida era a primeira da folha, */
         /* ela dever� ser substitu�da no �ndice         */
      if (!indiceDaChave) {
         substituta = folha.conteudo.noFolha.chaves[0].chave;
      }

         /* Chave que ser� removida no n�-pai */
      aRemover = irma.conteudo.noFolha.chaves[0].chave;

         /* Copia as chaves */
      for (i = 0; i < TF; ++i) {
         folha.conteudo.noFolha.chaves[ i +
              folha.conteudo.noFolha.nChaves ] =
              irma.conteudo.noFolha.chaves[i];
      }

         /* O n�mero de chaves da irm� ficou nulo  */
         /* e o n�mero de chaves da folha aumentou */
      irma.tipoDoNo = NO_VAZIO;
      folha.conteudo.noFolha.nChaves += TF;

         /* Preserva o encadeamento das folhas */
      folha.conteudo.noFolha.proximaFolha =
           irma.conteudo.noFolha.proximaFolha;

         /* Atualiza a folha e sua irm� no arquivo */
      EscreveNoBM(streamArvore, endFolha, &folha);
      EscreveNoBM(streamArvore, endIrma, &irma);

         /* Chama AtualizaIndice() para completar o servi�o */
      if (indiceDaChave) {
         AtualizaIndice( streamArvore, &pilha, chave, &substituta,
                         &aRemover );
      } else {
         AtualizaIndice( streamArvore, &pilha, chave, NULL,
                         &aRemover );
      }
   }

   return 1; /* Remo��o bem sucedida */
}

/****
 *
 * MenorChaveBM(): Encontra a menor chave de uma �rvore B+
 *
 * Par�metros:
 *     stream (entrada) - stream associado ao arquivo que
 *                        cont�m a �rvore
 *
 * Retorno: A menor chave ou POSICAO_NULA se a �rvore estiver vazia
 *
 ****/
int MenorChaveBM(FILE *stream)
{
   tNoBM umNo;

   ASSEGURA( stream,
             ERRO_STREAM_NULL(stream, MenorChaveBM) );

   rewind(stream);

      /* O primeiro n� � a raiz */
   fread(&umNo, sizeof(umNo), 1, stream);

   ASSEGURA( !feof(stream) & !ferror(stream),
             ERRO_FREAD(MenorChaveBM) );

   if (umNo.tipoDoNo == NO_VAZIO)
      return POSICAO_NULA;

      /* Desce at� encontrar a folha mais � esquerda */
   while (umNo.tipoDoNo == INTERNO)
         /* Desce pelo filho mais � esquerda */
      LeNoBM(stream, umNo.conteudo.noInterno.filhos[0], &umNo);

   ASSEGURA( umNo.tipoDoNo == FOLHA,
             ERRO_INCONSISTENCIA(MenorChaveBM) );

      /* A menor chave da �rvore � a primeira chave */
      /* do n� mais � esquerda da �rvore.           */
   return umNo.conteudo.noFolha.chaves[0].chave;
}

/****
 *
 * MaiorChaveBM(): Encontra a maior chave de uma �rvore B+
 *
 * Par�metros:
 *     stream (entrada) - stream associado ao arquivo que
 *                        cont�m a �rvore
 *
 * Retorno: A maior chave ou POSICAO_NULA se a �rvore estiver vazia
 *
 ****/
int MaiorChaveBM(FILE *stream)
{
   tNoBM umNo;

   ASSEGURA( stream,
             ERRO_STREAM_NULL(stream, MaiorChaveBM) );

   rewind(stream);

      /* O primeiro n� � a raiz */
   fread(&umNo, sizeof(umNo), 1, stream);

   ASSEGURA( !feof(stream) & !ferror(stream),
             ERRO_FREAD(MaiorChaveBM) );

   if (umNo.tipoDoNo == NO_VAZIO)
      return POSICAO_NULA;

      /* Desce at� encontrar a folha mais � direita */
   while (umNo.tipoDoNo == INTERNO)
         /* Desce pelo filho mais � direita */
      LeNoBM( stream, umNo.conteudo.noInterno.filhos
                   [umNo.conteudo.noInterno.nFilhos - 1], &umNo );

      /* O �ltimo n� lido deve ser uma folha */
   ASSEGURA( umNo.tipoDoNo == FOLHA,
             ERRO_INCONSISTENCIA(MaiorChaveBM) );

      /* A maior chave da �rvore � a �ltima chave */
      /* da folha mais � direita da �rvore        */
   return umNo.conteudo.noFolha.chaves
          [umNo.conteudo.noFolha.nChaves-1].chave;
}

/****
 *
 * EscreveChavesOrdenadas(): Visita todas as folhas de uma
 *                           �rvore B+ imprimindo todas as
 *                           chaves da �rvore em ordem crescente
 *
 * Par�metros:
 *     arvore (entrada) - ponteiro para a raiz da �rvore
 *    *streamArv (entrada) - stream associado ao arquivo que
 *                           cont�m a �rvore
 *     streamChaves (entrada) - stream associado ao arquivo no
 *                              qual ser�o armazenadas as chaves
 *                              visitadas
 *
 * Retorno: 1, se tudo ocorrer bem; 0, em caso contr�rio.
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

      /* Desce sempre pelo filho mais � esquerda */
      /* at� encontrar a primeira folha          */
   while (umNo.tipoDoNo == INTERNO)
      LeNoBM( streamArv, umNo.conteudo.noInterno.filhos[0],
            &umNo );

   ASSEGURA( umNo.tipoDoNo == FOLHA,
             ERRO_INCONSISTENCIA(EscreveChavesOrdenadas) );

      /* Chegou-se � folha mais � esquerda. Como as folhas   */
      /* formam uma lista encadeada e essa folha encabe�a    */
      /* a lista, deve-se percorr�-la imprimindo suas chaves */
   do {
         /* Escreve cada chave da folha */
      for (i = 0; i < umNo.conteudo.noFolha.nChaves; ++i) {
            /* Escreve a chave corrente */
         fprintf( streamChaves, "%ld",
                  umNo.conteudo.noFolha.chaves[i].chave );

         fputc('\n', streamChaves); /* Salta linha */
      }

         /* Passa para a pr�xima folha */
      LeNoBM( streamArv, umNo.conteudo.noFolha.proximaFolha,
             &umNo );
   } while (umNo.conteudo.noFolha.proximaFolha != POSICAO_NULA);
}

/****
 *
 * BuscaIntervaloBM(): Coleta num arquivo todas as chaves que
 *                     est�o entre duas chaves dadas de uma
 *                     �rvore B+
 *
 * Par�metros:
 *     chave1 (entrada) - primeira chave que define o intervalo
 *     chave2 (entrada) - segunda chave que define o intervalo
 *     arvore (entrada) - ponteiro para a raiz da �rvore na qual
 *                        ser� feita a busca
 *    *streamArv (entrada) - stream associado ao arquivo que
 *                           cont�m a �rvore
 *     streamChaves (entrada) - stream associado ao arquivo que
 *                              conter� as chaves resultantes
 *                              da busca
 *
 * Retorno: 1, se a coleta de chaves for bem sucedida;
 *          0, em caso contr�rio
 *
 * Observa��es:
 *    1. N�o faz diferen�a qual das duas chaves � menor do
 *       que a outra, mas as chaves devem ser diferentes
 *       para que haja um intervalo.
 *    2. Numa aplica��o pr�tica (e n�o meramente did�tica),
 *       faria mais sentido coletar os registros (e n�o apenas
 *       as chaves). Mas, implementar uma extens�o desta fun��o
 *       para que ela seja mais pragm�tica � considerado trivial
 *
 ****/
int BuscaIntervaloBM( tChave chave1, tChave chave2,
                      const tNoBM* arvore,
                      FILE *streamArv, FILE *streamChaves )
{
   tChave menorChave, /* Menor chave */
          maiorChave; /* Maior chave */
   tNoBM  no; /* Armazenar� cada n� visitado */
   int    i,
          encontrado = 0, /* Usada para chamar BuscaEmNoBM() */
          posNo = 0; /* Posi��o do n� no arquivo */

      /* Verifica se streamArv � v�lido */
   ASSEGURA( streamArv,
             ERRO_STREAM_NULL(streamArv, BuscaIntervaloBM) );

      /* Verifica se streamChaves � v�lido */
   ASSEGURA( streamChaves,
             ERRO_STREAM_NULL(streamChaves, BuscaIntervaloBM) );

      /* Verifica se o ponteiro para a raiz � v�lido */
   ASSEGURA( arvore,
             "Erro: A arvore NULL em BuscaIntervaloBM");

      /* Verifica se a �rvore � vazia */
   if (arvore->tipoDoNo == NO_VAZIO)
      return 0; /* A �rvore � vazia */

      /* Se as chaves forem iguais, n�o h� intervalo */
   if (chave1 == chave2)
      return 0; /* N�o h� intervalo */

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

      /* A busca come�a na raiz da �rvore */
   no = *arvore;

      /* Desce na �rvore at� encontrar uma folha */
   while (no.tipoDoNo == INTERNO) {
      i = BuscaEmNoBM(menorChave, &no, &encontrado);

         /* Desce at� o pr�ximo n� */
      posNo = encontrado ? no.conteudo.noInterno.filhos[i + 1]
                         : no.conteudo.noInterno.filhos[i];

      LeNoBM(streamArv, posNo, &no);
   }

      /* Chegou-se a uma folha. Encontra, nessa */
      /* folha, a posi��o da primeira chave que */
      /* � maior do que ou igual a menor chave  */
   i = BuscaEmNoBM(menorChave, &no, &encontrado);

      /* Armazena em arquivo as chaves encontradas a  */
      /* partir dessa posi��o na folha encontrada at� */
      /* encontrar a �ltima chave da �ltima folha ou  */
      /* uma chave maior do que 'maiorChave'          */

      /* A coleta de chaves na primeira folha encontrada    */
      /* � diferente daquela nas demais folhas da sequ�ncia */
      /* porque come�a com a chave na posi��o i             */
   for ( ; i < no.conteudo.noFolha.nChaves &&
         no.conteudo.noFolha.chaves[i].chave <= maiorChave; ++i ){
         /* Escreve a chave corrente */
      fprintf( streamChaves, "%ld\n",
               no.conteudo.noFolha.chaves[i].chave );
   }

      /* Se a �ltima chave escrita foi a �ltima chave da */
      /* folha, � preciso ajustar o �ndice dessa chave   */
   if (i == no.conteudo.noFolha.nChaves)
      --i;

   if (no.conteudo.noFolha.chaves[i].chave > maiorChave)
         /* Foi encontrada uma chave maior do que a maior */
         /* chave do intervalo e nada mais resta a fazer  */
      return 1;

      /* Escreve cada chave de cada folha seguinte que    */
      /* n�o seja maior do que a maior chave do intervalo */
   do {
         /* Passa para a pr�xima folha */
      posNo = no.conteudo.noFolha.proximaFolha;

         /* L� a folha  */
      LeNoBM(streamArv, posNo, &no);

         /* Escreve cada chave que n�o � maior */
         /* do que a maior chave do intervalo  */
      for (i = 0; i < no.conteudo.noFolha.nChaves &&
               no.conteudo.noFolha.chaves[i].chave <= maiorChave;
                  ++i) {
            /* Escreve a chave corrente */
         fprintf( streamChaves, "%ld\n",
                  no.conteudo.noFolha.chaves[i].chave );
      }

         /* Checa se � preciso ajustar o �ndice da �ltima chave */
      if (i == no.conteudo.noFolha.nChaves)
         --i;
   } while ( posNo != POSICAO_NULA &&
             no.conteudo.noFolha.chaves[i].chave <= maiorChave );

   return 1;
}

/****
 *
 * ProfundidadeBM(): Calcula a profundidade de uma �rvore B+
 *
 * Par�metros:
 *      stream (entrada) - stream associado ao arquivo que cont�m
 *                         a �rvore
 *
 * Retorno: A referida profundidade
 *
 ****/
int ProfundidadeBM(FILE *stream)
{
   tNoBM umNo;
   int   pos, nNiveis = 0;

   rewind(stream);

      /* O primeiro n� � a raiz */
   fread(&umNo, sizeof(umNo), 1, stream);

   ASSEGURA( !feof(stream) & !ferror(stream),
             ERRO_FREAD(ProfundidadeBM) );

      /* Se a �rvore estiver vazia, sua profundidade � zero */
   if (umNo.tipoDoNo == NO_VAZIO)
      return 0;

   ++nNiveis; /* O primeiro n�vel j� foi encontrado */

      /* Desce at� encontrar a folha mais � esquerda */
   while (umNo.tipoDoNo == INTERNO) {
         /* Desce pelo filho mais � esquerda */
      pos = umNo.conteudo.noInterno.filhos[0]*sizeof(umNo);
      if (fseek(stream, pos, SEEK_SET)) {
         return 0;
      }

      fread(&umNo, sizeof(umNo), 1, stream);

      if (feof(stream) || ferror(stream))
         return 0;

      ++nNiveis; /* Conta-se um n�vel adiantado */
   }

      /* Deve-se ter chegado a uma folha */
   ASSEGURA( umNo.tipoDoNo == FOLHA,
             ERRO_INCONSISTENCIA(ProfundidadeBM) );

   return nNiveis;
}

/****
 *
 * NumeroDeChavesBM(): Calcula o n�mero de chaves armazenadas nas
 *                     folhas de uma �rvore B+
 *
 * Par�metros:
 *    *streamArv (entrada) - stream associado ao arquivo que
 *                           cont�m a �rvore
 *
 * Retorno: O n�mero de chaves
 *
 ****/
int NumeroDeChavesBM(FILE *streamArv)
{
   int   numChaves = 0;
   tNoBM umNo;

      /* Verifica se streamArv � v�lido */
   ASSEGURA( streamArv,
             ERRO_STREAM_NULL(streamArv, NumeroDeChavesBM) );

      /* Tenta mover o apontador de posi��o */
      /* do arquivo para o seu in�cio       */
   MoveApontador(streamArv, 0, SEEK_SET);

      /* Acessa sequencialmente cada n�. Se ele */
      /* for folha, conta seu n�mero de chaves  */
   while (1) {
         /* Tenta ler um n� */
      fread(&umNo, sizeof(&umNo), 1, streamArv);

         /* Verifica se ocorreu erro ou o */
         /* final de arquivo foi atingido */
      if (feof(streamArv) || ferror(streamArv))
         break;

         /* Se o n� for folha, acrescenta seu */
         /* n�mero de chaves � contagem       */
      if (umNo.tipoDoNo == FOLHA)
         numChaves += umNo.conteudo.noFolha.nChaves;
   }

      /* Verifica se ocorreu erro de leitura */
   ASSEGURA(!ferror(streamArv), ERRO_FREAD(NumeroDeChavesBM));

   return numChaves;
}

/****
 *
 * NumeroDeFolhasBM(): calcula o n�mero de folhas de uma �rvore B+
 *
 * Par�metros:
 *    streamArv (entrada) - stream associado ao arquivo que
 *                          cont�m a �rvore
 *
 * Retorno: O n�mero de folhas da �rvore B+
 *
 * Observa��o: Esta � uma opera��o bastante custosa, pois envolve
 *             in�meros acessos ao meio de armazenamento externo
 *
 ****/
int NumeroDeFolhasBM(FILE *streamArv)
{
   int   numFolhas = 0;
   tNoBM umNo;

      /* Verifica se streamArv � v�lido */
   ASSEGURA( streamArv,
             ERRO_STREAM_NULL(streamArv, NumeroDeFolhasBM) );

      /* Tenta mover o apontador de posi��o */
      /* do arquivo para o seu in�cio       */
   MoveApontador(streamArv, 0, SEEK_SET);

      /* Acessa sequencialmente cada n�. Se */
      /* ele for folha, leva-o em conta     */
   while (1) {
         /* Tenta ler um n� */
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
 * NumeroDeNosInternosBM(): Calcula o n�mero de n�s internos de
 *                          uma �rvore B+
 *
 * Par�metros:
 *    *streamArv (entrada) - stream associado ao arquivo que
 *                           cont�m a �rvore
 *
 * Retorno: N�mero de n�s internos da �rvore
 *
 ****/
int NumeroDeNosInternosBM(FILE *streamArv)
{
   int   numNos = 0;
   tNoBM umNo;

      /* Verifica se streamArv � v�lido */
   ASSEGURA( streamArv,
             ERRO_STREAM_NULL(streamArv, NumeroDeNosInternosBM) );

      /* Tenta mover o apontador de posi��o */
      /* do arquivo para o seu in�cio       */
   MoveApontador(streamArv, 0, SEEK_SET);

      /* Acessa sequencialmente cada n�. Se  */
      /* ele for n� interno, leva-o em conta */
   while (1) {
         /* Tenta ler um n� */
      fread(&umNo, sizeof(&umNo), 1, streamArv);

         /* Verifica se ocorreu erro ou o */
         /* final de arquivo foi atingido */
      if (feof(streamArv) || ferror(streamArv))
         break;

      if (umNo.tipoDoNo == INTERNO)
         ++numNos; /* Mais um n� interno encontrado */
   }

      /* Verifica se ocorreu erro de leitura */
   ASSEGURA(!ferror(streamArv), ERRO_FREAD(NumeroDeNosInternosBM));

   return numNos;
}

/****
 *
 * NumeroDeNosVaziosBM(): Calcula o n�mero de n�s vazios de uma
 *                        �rvore B+
 *
 * Par�metros:
 *    *streamArv (entrada) - stream associado ao arquivo que
 *                           cont�m a �rvore
 *
 * Retorno: N�mero de n�s vazios da �rvore
 *
 ****/
int NumeroDeNosVaziosBM(FILE *streamArv)
{
   int   numNos = 0;
   tNoBM umNo;

      /* Verifica se streamArv � v�lido */
   ASSEGURA( streamArv,
             ERRO_STREAM_NULL(streamArv, NumeroDeNosInternosBM) );

      /* Tenta mover o apontador de posi��o */
      /* do arquivo para o seu in�cio       */
   MoveApontador(streamArv, 0, SEEK_SET);

      /* Acessa sequencialmente cada n�. Se  */
      /* ele for n� interno, leva-o em conta */
   while (1) {
         /* Tenta ler um n� */
      fread(&umNo, sizeof(&umNo), 1, streamArv);

         /* Verifica se ocorreu erro ou o */
         /* final de arquivo foi atingido */
      if (feof(streamArv) || ferror(streamArv))
         break;

      if (umNo.tipoDoNo == NO_VAZIO)
         ++numNos; /* Mais um n� vazio encontrado */
   }

      /* Verifica se ocorreu erro de leitura */
   ASSEGURA(!ferror(streamArv), ERRO_FREAD(NumeroDeNosInternosBM));

   return numNos;
}

/****
 *
 * NumeroDeNosBM(): Calcula o n�mero total de n�s (internos e
 *                  folhas) de uma �rvore B+
 *
 * Par�metros:
 *      stream (entrada) - stream associado ao arquivo
 *                         contendo a �rvore
 *
 * Retorno: O n�mero de n�s da �rvore
 *
 ****/
int NumeroDeNosBM(FILE *stream)
{
   return NumeroDeNosInternosBM(stream) + NumeroDeFolhasBM(stream);
}
