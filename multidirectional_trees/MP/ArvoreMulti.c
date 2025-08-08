/****
 *
 * Arquivo ArvoreMulti.c
 *
 * Autor: Gabriel Ribeiro
 *
 * Data de Cria��o: 20/04/2004
 * �ltima modifica��o: 20/07/2008
 *
 * Descri��o: Implementa��o das fun��es que executam opera��es
 *            usando �rvores multidirecionais de busca em
 *            mem�ria principal.
 *
 * NOTA: �rvores multidirecionais de busca devem ser
 *       implementadas em mem�ria secund�ria. Esta
 *       implementa��o � meramente did�tica!
 *
 ****/

/*********************** Includes *************************/

#include <stdlib.h> /* Fun��es de aloca��o din�mica e outras */
#include <stdio.h>  /* Fun��es de E/S */
#include <string.h>      /* Fun��es strXXX() e memXXX() */
#include "ArvoreMulti.h" /* Interface deste m�dulo      */
#include "Registros.h"   /* Fun��es de processamento    */
                         /* de registros                */
#include "leitura.h"     /* Fun��es de leitura via teclado */
#include "ASSEGURA.h"    /* Macro ASSEGURA e outras macros */
                         /* de tratamento de exce��o       */

/******************** Fun��es de Locais *******************/

/****
 *
 * BuscaEmNo(): Faz uma busca (sequencial) em um n� de uma
 *              �rvore multidirecional
 *
 * Par�metros:
 *      chave (entrada) - a chave de busca
 *      no (entrada) - ponteiro para o n� no qual ser� feita
 *                     a busca
 *
 * Retorno: O �ndice da primeira chave encontrada dentro do
 *          n� que � maior do que ou igual a chave de busca.
 *          Se tal chave n�o for encontrada, o n�mero de chaves
 *
 ****/
static int BuscaEmNo(tChave chave, const tNoMulti *no)
{
   int i,
       nChaves = no->nFilhos - 1; /* N�o � necess�rio, */
                                  /* mas facilita      */

   for (i = 0; i < nChaves; ++i)
      if ( chave <= no->chaves[i].chave )
            /* Encontrada uma chave maior do */
            /* que ou igual � chave de busca */
         return i;

      /* A chave de busca � maior do que */
      /* qualquer chave no n�            */
   return nChaves;
}

/****
 *
 * EncontraNo(): Procura uma chave na �rvore e retorna um
 *               ponteiro para o n� que cont�m a chave, se
 *               ela for encontrada, ou deveria cont�-la,
 *               se ela n�o for encontrada
 *
 * Par�metros:
 *      arvore (entrada) - ponteiro para a raiz da �rvore
 *      chave (entrada) - a chave de busca
 *     *posicao (sa�da) - posi��o da primeira chave no n� que
 *                        � maior do que ou igual a chave de
 *                        busca
 *     *encontrado (sa�da) - informar� se a chave foi
 *                           encontrada ou n�o
 *
 * Retorno: Ponteiro para o n� contendo a chave de busca, se
 *          ela for encontrada; ou onde ela estaria, se ela
 *          n�o for encontrada
 *
 ****/
static tNoMulti *EncontraNo( tArvoreMulti arvore, tChave chave,
                             int *posicao, int *encontrado )
{
   tNoMulti *p, *q;
   int       i;

   p = arvore; /* Ponteiro que desce a �rvore */
   q = NULL;   /* Ponteiro que desce atr�s de p */

   while (p) {
      i = BuscaEmNo(chave, p);
      q = p;

            /* A primeira compara��o � necess�ria */
            /* para evitar acesso a uma posi��o   */
            /* inv�lida no array chaves[]         */
      if( (i < p->nFilhos - 1) &&
          (chave == p->chaves[i].chave) ) {
         *encontrado = 1;
         *posicao = i;

            /* A chave foi encontrada */
         return p; /* N�o h� mais o que fazer */
      }

      p = p->filhos[i]; /* Desce mais um n�vel na �rvore */
   }

   /*                                             */
   /* A chave n�o foi encontrada. Ela deveria ser */
   /* inserida na posi��o i do n� apontado por q  */
   /*                                             */

   *encontrado = 0;
   *posicao = i;

   return q;
}

/****
 *
 * InsereEmFolha(): Insere uma chave na folha incompleta
 *                  especificada
 *
 * Par�metros:
 *      pNo (entrada/sa�da) - ponteiro para a folha onde
 *                            ser� feita a inser��o
 *      posicao (entrada) - posi��o da chave no array de
 *                          chaves do n�
 *      chaveEIndice (entrada) - a chave (e seu respectivo
 *                               �ndice) que ser� inserida
 *
 * Retorno: Nada
 *
 ****/
static void InsereEmFolha( tNoMulti *pNo, int posicao,
                           tChaveIndice chaveEIndice )
{
   int i;

   ASSEGURA( pNo->nFilhos < G,
             "Tentativa de insercao em no' e' completo" );

      /* Abre espa�o para a nova chave */
   for (i = pNo->nFilhos - 1; i > posicao; --i)
      pNo->chaves[i] = pNo->chaves[i - 1];

   pNo->chaves[posicao] = chaveEIndice; /* Insere a chave */

   ++pNo->nFilhos; /* Nasceu mais um filho */
}

/****
 *
 * ConstroiNo(): Cria um n� de uma �rvore multidirecional
 *               contendo uma �nica chave
 *
 * Par�metros:
 *     *chaveIndice (entrada) - um par chave/�ndice a ser
 *                              armazenado no n�
 *
 * Retorno: Ponteiro para a n� criado
 *
 ****/
static tNoMulti *ConstroiNo(const tChaveIndice *chaveIndice)
{
   tNoMulti *pNo;
   int       i;

   pNo = malloc(sizeof(tNoMulti));
   ASSEGURA(pNo, "Nao foi possivel alocar um no");

      /* A chave ser� a primeira do n� */
   pNo->chaves[0] = *chaveIndice;

      /* O n�mero de filhos num n� de uma �rvore  */
      /* multidirecional � sempre igual ao n�mero */
      /* de chaves mais um                        */
   pNo->nFilhos = 2;

      /* Inicia todos os poss�veis filhos do n� com NULL */
   for (i = 0; i < G; ++i)
      pNo->filhos[i] = NULL;

   return pNo;
}

/****
 *
 * CompactaNo(): Compacta um n� de uma �rvore multidirecional
 *               de busca ap�s uma opera��o de remo��o de chave
 *
 * Par�metros:
 *      pNo (entrada) - ponteiro para o n� que ser� compactado
 *      posicao (entrada) - posi��o da chave removida
 *
 * Retorno: Nada
 *
 ****/
static void CompactaNo(tNoMulti *pNo, int posicao)
{
   int i;

      /* A chave removida deve ter pelo menos um filho vazio */
   ASSEGURA(!pNo->filhos[posicao] || !pNo->filhos[posicao + 1],
            "Nenhum dos filhos da chave removida e' nulo");

      /* Primeiro, movem-se as chaves */
   for (i = posicao; i < pNo->nFilhos - 2; ++i)
      pNo->chaves[i] = pNo->chaves[i + 1];

      /* A movimenta��o de filhos depende de qual dos filhos  */
      /* da chave removida � vazio. Se o filho esquerdo n�o   */
      /* for vazio, a movimenta��o come�a uma posi��o adiante */
   if (pNo->filhos[posicao])
      posicao++;

      /* Move os filhos do n� */
   for (i = posicao; i < pNo->nFilhos - 1; ++i)
      pNo->filhos[i] = pNo->filhos[i + 1];

      /* O n�mero de filhos (e de chaves) diminuiu */
   --pNo->nFilhos;
}

/****
 *
 * MaiorInteiro(): Encontra o maior elemento de um array de
 *                 inteiros do tipo int
 *
 * Par�metros:
 *      ar (entrada) - o array
 *      nElementos (entrada) - n�mero de elementos do array
 *
 * Retorno: O maior valor encontrado em ar[]
 *
 ****/
static int MaiorInteiro(const int ar[], int nElementos)
{
   int i, maior = ar[0];

   for (i = 1; i < nElementos; ++i) {
      if (maior < ar[i]) {
         maior = ar[i];
      }
   }

   return maior;
}

/******************** Fun��es de Globais ******************/

/****
 *
 * BuscaMulti(): Faz uma busca numa �rvore multidirecional
 *
 * Par�metros:
 *      chave (entrada) - a chave de busca
 *      arvore (entrada) - ponteiro para a raiz da �rvore na
 *                         qual ser� feita a busca
 *
 * Retorno: �ndice do registro contendo a chave de busca,
 *          ou -1, se tal chave n�o for encontrada
 *
 ****/
int BuscaMulti(tArvoreMulti arvore, tChave chave)
{
   int i;

      /* Desce na �rvore realizando uma busca em um  */
      /* n� de cada n�vel. A busca prossegue at� que */
      /* a chave ou um filho nulo seja encontrado    */
   while (arvore) { /*  */
         /* Faz busca num n� no n�vel corrente */
      i = BuscaEmNo(chave, arvore);

         /* A primeira compara��o � necess�ria para evitar  */
         /* acesso a uma posi��o inv�lida no array chaves[] */
      if ( i < arvore->nFilhos - 1 &&
           (chave == arvore->chaves[i].chave) )
            /* A chave foi encontrada. Retorna */
            /* o �ndice do registro            */
         return arvore->chaves[i].indice;

         /* A busca continua no pr�ximo n�vel */
      arvore = arvore->filhos[i];
   }

   return -1; /* A chave n�o foi encontrada */
}

/****
 *
 * InsereMulti(): Insere uma chave numa �rvore de busca
 *                multidirecional armazenada em mem�ria principal
 *
 * Par�metros:
 *     *arvore (entrada/sa�da) - ponteiro para a �rvore
 *     *chaveEIndice (entrada/sa�da) - par chave/�ndice que ser�
 *                                     inserido
 *
 * Retorno: 1, se tudo ocorrer bem; 0, se ocorrer algum erro
 *
 * Observa��o: A chave � considerada prim�ria; i.e., chaves
 *             repetidas n�o s�o permitidas na �rvore.
 *
 ****/
int InsereMulti(tArvoreMulti *arvore, tChaveIndice *chaveEIndice)
{
   tNoMulti *pNo, *pNovoNo;
   int       encontrado, /* Indicar� se a chave */
                         /* foi encontrada      */
             indiceDaChave; /* �ndice da chave no n� onde ela */
                            /* � encontrada ou inserida       */

   if (!*arvore) { /* A �rvore ainda n�o existe */
         /* Constr�i a raiz da �rvore */
      *arvore = ConstroiNo(chaveEIndice);

      return 1;
   }

      /* Procura a chave na �rvore */
   pNo = EncontraNo( *arvore, chaveEIndice->chave,
                     &indiceDaChave, &encontrado );

      /* Se a chave foi encontrada, n�o h� mais nada */
      /* a fazer, pois ela � considerada prim�ria    */
   if (encontrado) {
      return 0;
   }

      /* Numa �rvore multidirecional descendente, */
      /* se um n� � incompleto, ele � uma folha   */
   if (pNo->nFilhos < G) {
         /* O n� � uma folha incompleta e n�o */
         /* � necess�rio criar um novo n�     */
      InsereEmFolha(pNo, indiceDaChave, *chaveEIndice);

      return 1; /* Nada mais a fazer */
   }

      /****************************************************/
      /* Neste ponto, resta lidar com o caso em que o n�  */
      /* no qual dever� ser realizada a inser��o � um n�  */
      /* completo. Nesse caso, � necess�rio criar um novo */
      /* n� na posi��o indicada por 'indiceDaChave' e     */
      /* inserir o par chave/�ndice nesse novo n�.        */
      /****************************************************/

      /* � preciso criar um novo n� com uma �nica chave */
   pNovoNo = ConstroiNo(chaveEIndice);
   pNo->filhos[indiceDaChave] = pNovoNo;

   return 1;
}

/****
 *
 * RemoveChaveMulti(): Remove uma chave especificada da �rvore e seu
 *                respectivo registro do arquivo
 *
 * Par�metros:
 *     *arvore (entrada/sa�da) - ponteiro para a raiz da �rvore
 *      chave (entrada) - a chave que ser� removida
 *
 * Retorno: 1, se a remo��o for bem sucedida;
 *          0, em caso contr�rio
 *
 ****/
int RemoveChaveMulti(tArvoreMulti *arvore, tChave chave)
{
   tNoMulti    *p, *q;
   int          i, iPai;
   tChaveIndice sucessoraEIndice, *pSucessoraEIndice;

   p = *arvore; /* Ponteiro usado para descer na �rvore   */
   q = NULL;    /* Ponteiro que segue um n�vel acima de p */

      /* A busca inicia na raiz e prossegue at� que se */
      /* encontre a chave a ser removida ou um n� nulo */
   while (p) {
         /* Faz busca num n� no n�vel corrente */
      i = BuscaEmNo(chave, p);

          /* Verifica se a chave foi encontrada */
      if ( i < p->nFilhos - 1 &&
           chave == p->chaves[i].chave )
         break; /* Chave foi encontrada */

      q = p;

         /* �ndice do filho do pen�ltimo n� */
         /* visitado que aponta para p      */
      iPai = i;

         /* A busca continua no pr�ximo n�vel */
      p = p->filhos[i];
   }

      /* Verifica se a chave foi encontrada */
   if (!p)
      return 0; /* A chave n�o foi encontrada */

   /*********************************************************/
   /* Neste ponto sabe-se que p aponta para o n� que cont�m */
   /* a chave que ser� removida e que a posi��o dessa chave */
   /* nesse n� � i. O ponteiro q est� apontando para o n�   */
   /* que tem p como filho na posi��o 'iPai'.               */
   /*********************************************************/

      /* Verifica quantos filhos a chave a ser removida possui */
   if (!p->filhos[i] || !p->filhos[i+1]) {
      /* A chave tem, no m�ximo, um filho */

         /* Verifica se o n� s� tem uma chave */
      if(p->nFilhos == 2) {
         /*                                     */
         /* A �ltima chave do n� ser� removida. */
         /* Logo esse n� tamb�m ser� removido.  */
         /*                                     */

            /* Se o n� a ser removido n�o */
            /* tem pai, trata-se da raiz  */
         if (!q) {
            free(p); /* Libera a raiz */
            *arvore = NULL; /* A �rvore acabou-se */
            return 1; /* Game over */
         }

            /* Verifica quantos filhos o n� */
            /* a ser removido possui        */
         if(p->filhos[i])
               /* O n� tem filho esquerdo. O pai desse filho */
               /* passar� a ser o pai do n� a ser removido   */
            q->filhos[iPai] = p->filhos[i];
         else if(p->filhos[i + 1])
               /* O n� tem filho direito. O pai desse filho */
               /* passar� a ser o pai do n� a ser removido  */
            q->filhos[iPai] = p->filhos[i + 1];
         else
               /* O n� n�o tem nenhum filho. O pai do */
               /* n� a ser removido passa a ser nulo  */
            q->filhos[iPai] = NULL;

         free(p); /* O n� j� pode ser liberado */
      } else /* H� mais de uma chave no n� */
            /* A fun��o CompactaNo() completa a remo��o */
         CompactaNo(p,i);

      return 1;
   }

   /***************************************************/
   /* Neste ponto, sabe-se que a chave a ser removida */
   /* possui dois filhos n�o vazios. Nesse caso,      */
   /* substitui-se a chave a ser removida por sua     */
   /* sucessora imediata e remove-se essa sucessora   */
   /***************************************************/

      /* A chave sucessora imediata da chave a ser removida */
      /* � a menor chave da sub�rvore cuja raiz � o filho   */
      /* direito da chave a ser removida                    */
   pSucessoraEIndice = MenorChaveMulti(p->filhos[i + 1]);

      /* Se a chave possui dois filhos, ela */
      /* deve ter uma chave sucessora       */
   ASSEGURA(pSucessoraEIndice,
            "RemoveChaveMulti(): Nao existe chave sucessora");

      /* Guarda a chave sucessora com respectivo */
      /* �ndice para posterior substitui��o da   */
      /* chave que ser� removida                 */
   sucessoraEIndice = *pSucessoraEIndice;

      /* Agora, chama-se esta fun��o recursivamente     */
      /* para remover a chave sucessora. Haver� apenas  */
      /* uma chamada recursiva porque a chave sucessora */
      /* possui, no m�ximo, um filho.                   */
   if (!RemoveChaveMulti(arvore, pSucessoraEIndice->chave))
      return 0;

      /* Substitui a chave a ser removida por sua sucessora */
   p->chaves[i] = sucessoraEIndice;

   return 1;
}

/****
 *
 * CaminhamentoInfixoMulti(): Executa um caminhamento em ordem
 *                       infixa numa �rvore de busca
 *                       multidirecional exibindo todas as
 *                       chaves da �rvore em ordem crescente
 *
 * Par�metros:
 *      arvore (entrada) - ponteiro para a raiz da �rvore
 *      stream (entrada) - stream associado ao arquivo onde ser�o
 *                         armazenadas as chaves visitadas
 *
 * Retorno: Nada
 *
 * Observa��o: Um caminhamento envolve executar uma opera��o
 *             sobre cada chave na �rvore numa determinada
 *             ordem. A ordem de caminhamento apresentada aqui
 *             � uma generaliza��o do caminhamento em ordem
 *             infixa para �rvores bin�rias e a opera��o
 *             executada sobre cada chave � simplesmente a
 *             exibi��o dela. Este caminhamento � o mais �til
 *             para �rvores de busca porque permite acessar
 *             as chaves em ordem crescente, mas n�o h� nenhuma
 *             dificuldade em generalizar outros caminhamentos
 *             para �rvores bin�rias nem o tipo de opera��o
 *             realizada sobre as chaves.
 *
 ****/
void CaminhamentoInfixoMulti(tArvoreMulti arvore, FILE *stream)
{
   int i;

      /* O caminhamento encerra quando */
      /* se encontra um n� nulo        */
   if (arvore) {
      for (i = 0; i < arvore->nFilhos-1; ++i) {
            /* Caminha no filho esquerdo */
            /* da chave de �ndice i      */
         CaminhamentoInfixoMulti(arvore->filhos[i], stream);

            /* Visita a chave de �ndice i */
         fprintf(stream, "%d\n", arvore->chaves[i].chave);
      }
         /* Caminha no filho mais � direita do n� */
      CaminhamentoInfixoMulti( arvore->filhos[arvore->nFilhos-1],
                           stream );
   }
}

/****
 *
 * MenorChaveMulti(): Encontra a menor chave de uma �rvore
 *               multidirecional, que � a primeira chave
 *               do n� mais � esquerda da �rvore
 *
 * Par�metros:
 *      arvore (entrada) - ponteiro para a raiz da �rvore
 *
 * Retorno: a menor chave da �rvore
 *
 ****/
tChaveIndice *MenorChaveMulti(tArvoreMulti arvore)
{
   if (!arvore)
      return NULL; /* Se n�o h� �rvore, n�o h� chave */

      /* Desce na �rvore sempre seguindo o */
      /* filho mais � esquerda de cada n�  */
   while (arvore->filhos[0])
      arvore = arvore->filhos[0];

   return &arvore->chaves[0];
}

/****
 *
 * MaiorChaveMulti(): Encontra a maior chave de uma �rvore
 *               multidirecional, que � a �ltima chave
 *               do n� mais � direita da �rvore
 *
 * Par�metros:
 *      arvore (entrada) - ponteiro para a raiz da �rvore
 *
 * Retorno: a maior chave da �rvore
 *
 ****/
tChaveIndice *MaiorChaveMulti(tArvoreMulti arvore)
{
   if (!arvore)
      return NULL; /* Se n�o h� �rvore, n�o h� chave */

   while (arvore->filhos[arvore->nFilhos-1])
         /* Vai descendo � direita */
      arvore = arvore->filhos[arvore->nFilhos-1];

   return &arvore->chaves[arvore->nFilhos-2];
}

/****
 *
 * AlturaMulti(): Calcula a altura de uma �rvore
 *                      multidirecional de busca
 *
 * Par�metros:
 *     arvore (entrada) - a �rvore cuja altura ser� calculada
 *
 * Retorno: A referida altura
 *
 ****/
int AlturaMulti(tArvoreMulti arvore)
{
   int  i,
        alturas[G] = {0}; /* Array que armazena as alturas */
                          /* das sub�rvores de um n�       */

   if (arvore != NULL) {
         /* Recursivamente, calcula-se a altura */
         /* da sub�rvore esquerda de cada chave       */
      for (i = 0; i < arvore->nFilhos - 1; ++i) {
         alturas[i] = 1 + AlturaMulti(arvore->filhos[i]);
      }

         /* Agora, calcula-se a altura da */
         /* sub�rvore direita da �ltima chave   */
      alturas[i] = 1 + AlturaMulti(arvore->filhos[arvore->nFilhos - 1]);

         /* A altura de uma �rvore � a maior        */
         /* altura encontrada entre suas sub�rvores */
      return MaiorInteiro(alturas, G);
   } else {
      return 0; /* �rvore vazia tem altura zero */
   }
}

/****
 *
 * NumeroDeChavesMulti(): Calcula o n�mero de chaves de uma
 *                   �rvore multidirecional de busca
 *
 * Par�metros:
 *     arvore (entrada) - a �rvore cujo n�mero de chaves
 *                        ser� determinado
 *
 * Retorno: O referido n�mero de chaves
 *
 ****/
int NumeroDeChavesMulti(tArvoreMulti arvore)
{
   int  i,
        nChaves = 0;

      /* Visita cada n� da �rvore em ordem infixa at� */
      /* que a posi��o inicial do caminhamento assuma */
      /* um valor inv�lido                            */
   if (arvore != NULL) {
         /* Conta as chaves deste n� */
      nChaves = nChaves + arvore->nFilhos - 1;

         /* Agora, visita-se recursivamente a sub�rvore */
         /* esquerda de cada chave contando-se o n�mero */
         /* de chaves da raiz de cada sub�rvore         */
      for (i = 0; i < arvore->nFilhos - 1; ++i) {
         nChaves = nChaves +
                   NumeroDeChavesMulti(arvore->filhos[i] );
      }

         /* Agora, contam-se as chaves da sub�rvore */
         /* direita da �ltima chave                 */
      nChaves = nChaves +
                NumeroDeChavesMulti(arvore->filhos[arvore->nFilhos - 1]);
   }

   return nChaves;
}

/****
 *
 * NumeroDeNosMulti(): faz um caminhamento numa �rvore de busca
 *                multidirecional contando o n�mero de n�s
 *                visitados
 *
 * Par�metros:
 *      arvore (entrada) - ponteiro para a raiz da �rvore
 *
 * Retorno: N�mero de n�s da �rvore
 *
 ****/
int NumeroDeNosMulti(tArvoreMulti arvore)
{
   int i, nNos = 0;

   if (arvore) {
      nNos++;

      for (i = 0; i < arvore->nFilhos; ++i)
         nNos = nNos + NumeroDeNosMulti(arvore->filhos[i]);
   }

   return nNos;
}