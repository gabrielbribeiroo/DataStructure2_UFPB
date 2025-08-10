/****
 *
 * Arquivo ArvoreMulti2.c
 *
 * Autor: Ulysses de Oliveira
 *
 * Data de Cria��o: 25/04/2004
 * �ltima modifica��o: 21/07/2008
 *
 * Descri��o: Implementa��o das fun��es que executam opera��es usando
 *            �rvores de busca multidirecional armazenada em arquivo
 *
 ****/

/*********************** Includes *************************/

#include <string.h>       /* Fun��o strxxx() e memxxx() */
#include "ArvoreMulti2.h" /* Interface deste m�dulo     */
#include "Registros.h"   /* Fun��es de processamento    */
                         /* de registros                */
#include "leitura.h"  /* Fun��es de leitura via teclado */
#include "ASSEGURA.h" /* Macro ASSEGURA                 */

/******************** Fun��es de Locais *******************/

/****
 *
 * IniciaNoMultiMS(): Inicia um n� de uma �rvore multidirecional
 *                    armazenada em arquivo
 *
 * Par�metros:
 *      chaveIndice (entrada) - um par chave/�ndice a ser armazenado no n�
 *      pNo (sa�da) - endere�o do n� criado
 *
 * Retorno: Nada
 *
 * Observa��o: O primeiro par�metro poder� ser NULL. Nesse caso,
 *             ser� criado um n� vazio (i.e., sem nenhuma chave)
 *
 ****/
static void IniciaNoMultiMS(const tChaveIndice *chaveIndice, tNoMultiMS *pNo)
{
   int i;

   if (chaveIndice)
         /* A chave ser� a primeira do n� */
      pNo->chaves[0] = *chaveIndice;

      /* O n�mero de filhos num n� de uma �rvore     */
      /* multidirecional � igual ao n�mero de chaves */
      /* mais um, a n�o ser que n�o haja chave       */
   pNo->nFilhos = chaveIndice ? 2 : 0;

      /* Inicia todos os poss�veis filhos do n� */
   for (i = 0; i < G; ++i)
         /* Um novo n� ainda n�o possui filhos */
      pNo->filhos[i] = POSICAO_NULA;
}

/****
 *
 * LeNoMultiMS(): Faz a leitura de um n� de uma �rvore
 *         multidirecional armazenada em arquivo
 *
 * Par�metros:
 *      stream (entrada) - stream associado ao arquivo contendo a �rvore onde ser�
 *                         feita a leitura
 *      pos (entrada) - posi��o no arquivo onde ser� feita a leitura
 *     *no (sa�da) - ponteiro para o n� que conter� o resultado da leitura
 *
 * Retorno: Nada
 *
 ****/
static void LeNoMultiMS(FILE *stream, int pos, tNoMultiMS *no)
{
   ASSEGURA(stream, ERRO_STREAM_NULL(stream, LeNoMultiMS));
   ASSEGURA(pos >= 0, ERRO_POSICAO(LeNoMultiMS));

      /* Tenta mover o apontador de arquivo para o local  */
      /* de leitura; se n�o conseguir, aborta o programa  */
   MoveApontador(stream, sizeof(tNoMultiMS)*pos, SEEK_SET);

      /* Tenta ler o n� */
   fread(no, sizeof(tNoMultiMS), 1, stream);

      /* Verifica se ocorreu erro de leitura */
   ASSEGURA( !ferror(stream), ERRO_FREAD(LeNoMultiMS) );
}

/****
 *
 * EscreveNoMultiMS(): Atualiza o conte�do de um n� no arquivo que cont�m a �rvore
 *
 * Par�metros:
 *      stream (entrada) - stream associado ao arquivo
 *                         que cont�m a �rvore
 *      pos (entrada) - posi��o do n� no arquivo
 *                      que cont�m a �rvore
 *     *pNo (entrada) - n� que ser� atualizado
 *
 * Retorno: Nada
 *
 ****/
static void EscreveNoMultiMS(FILE *stream, int pos, const tNoMultiMS *pNo)
{
      /* Tenta mover o apontador de posi��o     */
      /* para o local onde o n� est� armazenado */
   MoveApontador(stream, sizeof(*pNo)*pos, SEEK_SET);

      /* Escreve o n� no arquivo */
    fwrite(pNo, sizeof(*pNo), 1, stream);

      /* Se ocorreu erro, aborta o programa */
   ASSEGURA(!ferror(stream), ERRO_FWRITE(EscreveNoMultiMS));
}

/****
 *
 * BuscaEmNoMultiMS(): Tenta encontrar dentro de um n� uma chave que
 *                     seja maior do que ou igual a uma dada chave
 *
 * Par�metros:
 *      chave (entrada) - a chave
 *      no (entrada) - ponteiro para o n� onde ser� feita a busca
 *
 * Retorno: O �ndice da chave dentro do n� se for encontrada
 *          uma chave maior do que a chave de busca; o n�mero
 *          de chaves, se tal chave n�o for encontrada
 *
 ****/
static int BuscaEmNoMultiMS(tChave chave, const tNoMultiMS *no)
{
   int i,
       nChaves = no->nFilhos - 1; /* N�mero de chaves do n� */

      /* Procura no n� uma chave que seja maior */
      /* do que ou igual � chave de busca       */
   for (i = 0; i < nChaves; ++i)
         /* Verifica se tal chave foi encontrada */
      if (chave <= no->chaves[i].chave)
         return i;   /* A chave foi encontrada */

      /* A chave de busca � maior do que qualquer chave do n� */
   return nChaves; /* A chave N�O foi encontrada */
}

/****
 *
 * EncontraNoMultiMS(): Procura uma chave na �rvore e retorna a
 *                posi��o no arquivo do n� que cont�m a chave,
 *                se ela for encontrada. Se a chave n�o for
 *                encontrada, o n� cuja posi��o � retornada �
 *                aquele que conter� a chave se ela for inserida
 *
 * Par�metros:
 *      stream (entrada) - stream associado ao arquivo que
 *                         cont�m a �rvore
 *      chave (entrada) - a chave de busca
 *     *posicaoDaChaveNoNo (sa�da) - conter� o �ndice da
 *                                   chave no n�
 *     *encontrado (sa�da) - indicar� se a chave foi encontrada
 *
 * Retorno: Posi��o no arquivo do n� contendo a chave de busca,
 *          se ela for encontrada, ou do pai do n� onde ela
 *          deveria estar se ela n�o for encontrada
 *
 ****/
static int EncontraNoMultiMS( FILE *stream, tChave chave,
                              int *posicaoDaChaveNoNo, int *encontrado )
{
   int        p,  /* p conter� a posi��o desejada    */
              q,  /* q conter� a posi��o do pai de p */
              posChave;
   tNoMultiMS umNo;

      /* Verifica se o stream � NULL */
   ASSEGURA(stream, ERRO_STREAM_NULL(stream, EncontraNoMultiMS));

      /* Tenta mover o apontador de arquivo para seu in�cio */
   MoveApontador(stream, 0, SEEK_SET);

   p = 0; /* Come�a busca na raiz da �rvore */
   q = POSICAO_NULA; /* A raiz n�o tem pai */

   while (p != POSICAO_NULA) {
         /* L� um n� da �rvore na posi��o p do arquivo */
      LeNoMultiMS(stream, p, &umNo);

         /* Tenta encontrar a chave no n� corrente */
      posChave = BuscaEmNoMultiMS(chave, &umNo);

         /* Verifica se a chave foi encontrada */
      if((posChave < umNo.nFilhos - 1) && chave == umNo.chaves[posChave].chave) {
         *encontrado = 1;  /* A chave foi encontrada */
         *posicaoDaChaveNoNo = posChave;

         return p; /* Retorna a posi��o do n� no arquivo que cont�m a �rvore */
      }

         /* A chave ainda n�o foi encontrada; desce um n�vel na �rvore.  */
         /* Antes que p passe a referir-se a um filho, guarda seu valor. */
      q = p;
      p = umNo.filhos[posChave];
   }

   *encontrado = 0; /* A chave n�o foi encontrada */


   *posicaoDaChaveNoNo = posChave; /* Posi��o onde a chave deve ser inserida */

   return q;  /* Retorna o pai do n� p */
}

/****
 *
 * InsereEmFolhaMultiMS(): Insere uma chave numa folha incompleta de uma
 *                   �rvore multidirecional armazenada em arquivo
 *
 * Par�metros:
 *      stream (entrada) - stream associado ao arquivo que cont�m a �rvore
 *      posicaoNo (entrada) - posi��o no arquivo do n� no
 *                            qual ser� feita a inser��o
 *      posicaoDaChave (entrada) - posi��o da nova chave no
 *                                 array de chaves do n�
 *      chaveEIndice (entrada) - a chave e seu respectivo
 *                               �ndice que ser�o inseridos
 *
 * Retorno: Nada
 *
 ****/
static void InsereEmFolhaMultiMS( FILE *stream, int posicaoNo,
                                  int posicaoDaChave, tChaveIndice chaveEIndice )
{
   int        i;
   tNoMultiMS umNo;

      /* Verifica se o stream � v�lido */
   ASSEGURA(stream, ERRO_STREAM_NULL(stream, InsereEmFolhaMultiMS));

      /* L� o n�-folha no arquivo que cont�m a �rvore */
   LeNoMultiMS(stream, posicaoNo, &umNo);

      /* O n�mero de filhos do n� deve ser estritamente menor do que o grau */
      /* da �rvore. Se n�o o for, o  n� n�o � uma folha, deve haver algo    */
      /* errado e o programa ser� abortado.                                 */
   ASSEGURA( umNo.nFilhos < G, "ERRO: Tentativa de inserir em folha completa" );

      /* Abre espa�o para a nova chave */
   for (i = umNo.nFilhos - 1; i > posicaoDaChave; --i)
      umNo.chaves[i] = umNo.chaves[i - 1];

   umNo.chaves[posicaoDaChave] = chaveEIndice; /* Armazena a nova chave */

   ++umNo.nFilhos; /* O n�mero de filhos do n� � acrescido de um */

   EscreveNoMultiMS(stream, posicaoNo, &umNo); /* Atualiza n� no arquivo */
}

/****
 *
 * MaiorInteiro(): Encontra o maior elemento de um array de inteiros do tipo int
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

   for (i = 1; i < nElementos; ++i)
      if (maior < ar[i])
         maior = ar[i];

   return maior;
}

/****
 *
 * CompactaNoMultiMS(): Compacta um n� de uma �rvore multidirecional de
 *                busca armazenada em arquivo ap�s uma opera��o de
 *                remo��o de chave
 *
 * Par�metros:
 *      pNo (entrada) - ponteiro para o n� que ser� compactado
 *      posicao (entrada) - posi��o da chave removida
 *
 * Retorno: Nada
 *
 ****/
static void CompactaNoMultiMS(tNoMultiMS *pNo, int posicao)
{
   int i;

   ASSEGURA(pNo->filhos[posicao] == POSICAO_NULA ||
            pNo->filhos[posicao + 1] == POSICAO_NULA,
            "Nenhum dos filhos da chave removida e' nulo");

      /* Primeiro, move as chaves */
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

   --pNo->nFilhos; /* O n�mero de filhos (e de chaves) diminuiu */
}

/******************** Fun��es de Globais ******************/

/****
 *
 * BuscaMultiMS(): Faz uma busca numa �rvore multidirecional
 *               armazenada em arquivo
 *
 * Par�metros:
 *      stream (entrada) - stream associado ao arquivo no
 *                         qual a �rvore reside
 *      chave (entrada) - a chave de busca
 *
 * Retorno: �ndice do registro no arquivo de registros
 *          se a chave for encontrada; -1, se a chave
 *          n�o for encontrada.
 *
 ****/
int BuscaMultiMS(FILE *stream, tChave chave)
{
   tNoMultiMS no;   /* Armazenar� cada n� lido no arquivo */
   int        posNo = 0, /* Posi��o do n� no arquivo; a busca    */
                         /* come�a na raiz que ocupa a posi��o 0 */
              i; /* �ndice de uma chave num n� */

      /* Desce na �rvore at� encontrar a chave de busca ou uma posi��o nula */
   while (posNo != POSICAO_NULA) {
      LeNoMultiMS(stream, posNo, &no); /* L� o n� na posi��o indicada */

         /* Tenta encontrar a chave no n� rec�m-recuperado */
      i = BuscaEmNoMultiMS(chave, &no);

         /* Verifica se a chave foi encontrada. A primeira    */
         /* compara��o abaixo � necess�ria para evitar acesso */
         /* a uma posi��o inv�lida no array chaves[]          */
      if (i < no.nFilhos - 1 && chave == no.chaves[i].chave)
         return no.chaves[i].indice; /* A chave foi encontrada */

      posNo = no.filhos[i]; /* Desce mais um n�vel na �rvore */
   }

   return POSICAO_NULA;  /* A chave n�o foi encontrada */
}

/****
 *
 * InsereMultiMS(): Insere um par chave/�ndice numa �rvore
 *                 multidirecional descendente de busca
 *                 armazenada em arquivo
 *
 * Par�metros:
 *     *posicaoRaiz (entrada/sa�da) - posi��o no arquivo da
 *                                    raiz da �rvore
 *      chaveEIndice (entrada) - ponteiro para um par
 *                               chave/�ndice que ser� inserido
 *     *indiceDaChave (sa�da) - �ndice da chave no n� onde ela
 *                              se encontra ou onde ela ser�
 *                              inserida
 *      streamArvore (entrada) - stream associado ao arquivo
 *                               que cont�m a �rvore
 *
 * Retorno: 1, se tudo houver inser��o; 0, se n�o houver
 *
 * Observa��o: A chave � considerada prim�ria; i.e., chaves
 *             repetidas n�o s�o permitidas na �rvore.
 *
 ****/
int InsereMultiMS( int *posicaoRaiz, tChaveIndice *chaveEIndice,
                   FILE *streamArvore )
{
   tNoMultiMS no, novoNo;
   int        pNo, pNovoNo,
              encontrado,    /* Indicar� se a chave foi encontrada */
              indiceDaChave; /* �ndice da chave no n� onde ela */
                             /* for encontrada ou inserida     */

      /* O stream que representa o arquivo que */
      /* cont�m a �rvore n�o pode ser NULL     */
   ASSEGURA( streamArvore, ERRO_STREAM_NULL(streamArvore, InsereMultiMS) );

   if (*posicaoRaiz == POSICAO_NULA) {
         /* A �rvore ainda n�o foi criada */
      IniciaNoMultiMS(chaveEIndice, &no);
      *posicaoRaiz = 0;

         /* Armazena a raiz da �rvore na posi��o 0 do arquivo   */
      EscreveNoMultiMS(streamArvore, 0, &no);

      return 1;
   }
         /* Tenta encontrar o n� que cont�m a chave */
   pNo = EncontraNoMultiMS( streamArvore, chaveEIndice->chave, &indiceDaChave,
                            &encontrado );

      /* Se a chave foi encontrada, n�o h� mais nada */
      /* a fazer, pois ela � considerada prim�ria    */
   if (encontrado)
      return 0; /* N�o houve inser��o */

   /*                                                     */
   /* Neste ponto, sabe-se que a chave n�o foi encontrada */
   /*                                                     */

      /* L� no arquivo o n� cuja posi��o foi retornada pela fun��o EncontraNoMultiMS() */
   LeNoMultiMS(streamArvore, pNo, &no);

      /* Verifica se h� espa�o para inser��o nesse n� */
   if (no.nFilhos < G) {
      /* H� espa�o para inser��o nesse n� */

         /* Usando esse esquema de inser��o, se o n� tem espa�o sobrando ele */
         /* � uma folha. Ent�o,  tenta-se inserir a chave no n� folha cuja   */
         /* posi��o � indicada por 'pNo'.                                    */
      InsereEmFolhaMultiMS( streamArvore, pNo, indiceDaChave, *chaveEIndice );

      return 1;  /* Servi�o completo */
   }

      /*                                           */
      /* Neste ponto, sabe-se que n�o h� nenhum n� */
      /* na �rvore com espa�o para conter a chave. */
      /*                                           */

      /* � necess�rio criar um novo n� para colocar a chave */
   IniciaNoMultiMS(chaveEIndice, &novoNo);

      /* O novo n� ser� armazenado ao final do arquivo; */
      /* portanto sua posi��o no arquivo ser� igual ao  */
      /* n�mero de registros (n�s) do arquivo (NB: a    */
      /* indexa��o do arquivo come�a em zero; por isso, */
      /* n�o se acrescenta 1 a esse valor).             */
   pNovoNo = NumeroDeRegistros(streamArvore, sizeof(tNoMultiMS));

      /* O n� encontrado n�o tinha espa�o para inser��o */
      /* da chave. Portanto o �ndice da chave retornado */
      /* pela fun��o EncontraNoMultiMS() � o �ndice do novo   */
      /* n� que ser� filho do n� encontrado             */
   no.filhos[indiceDaChave] = pNovoNo;

      /* O n� encontrado foi alterado; � preciso */
      /* atualiz�-lo no arquivo                  */
   EscreveNoMultiMS(streamArvore, pNo, &no);

   /****                                      ****/
   /**** Insere o novo n� ao final do arquivo ****/
   /****                                      ****/

      /* Primeiro, tenta-se mover o apontador do  */
      /* arquivo para o final do arquivo; se isso */
      /* n�o for poss�vel, aborta-se o programa   */
   MoveApontador(streamArvore, 0, SEEK_END);

      /* Agora tenta-se escrever o n� no arquivo; se */
      /* isso n�o for poss�vel, aborta-se o programa */
   fwrite(&novoNo, sizeof(tNoMultiMS), 1, streamArvore);
   ASSEGURA(!ferror(streamArvore), ERRO_FWRITE(InsereMultiMS));

   return 1;  /* Servi�o completo */
}

/****
 *
 * RemoveChaveMultiMS(): Remove a chave especificada de uma �rvore
 *                 multidirecional de busca armazenada em arquivo
 *
 * Par�metros:
 *      *streamArvore (entrada/sa�da) - stream associado ao
 *                                      arquivo que armazena
 *                                      a �rvore
 *      chave (entrada) - a chave que ser� removida
 *
 * Retorno: 1, se a remo��o for bem sucedida;
 *          0, em caso contr�rio
 *
 ****/
int RemoveChaveMultiMS(FILE **streamArvore, tChave chave)
{
   tChaveIndice sucessoraEIndice;
   tNoMultiMS    no, /* Armazenar� cada n� lido no arquivo */
                noPai; /* Armazenar� o pai de 'no' */
   int          i,
                posNo = 0, /* Posi��o no arquivo do n� que  */
                           /* cont�m a chave, se ela for    */
                           /* encontrada. A busca come�a na */
                           /*  raiz que est� na posi��o 0   */
                posNoPai = POSICAO_NULA, /* Posi��o de 'noPai' */
                iPai; /* �ndice do filho do pen�ltimo n�  */
                      /* visitado que cont�m a posi��o no */
                      /* arquivo de 'no'                  */

   ASSEGURA(*streamArvore,
            ERRO_STREAM_NULL(streamArvore, RemoveChaveMultiMS) );

      /* A busca inicia na raiz e prossegue at� que se */
      /* encontre a chave a ser removida ou um n� nulo */
   while (posNo != POSICAO_NULA) {
         /* L� o n� na posi��o indicada por 'posNo' */
      LeNoMultiMS(*streamArvore, posNo, &no);

         /* Tenta encontrar a chave no n� rec�m-recuperado */
      i = BuscaEmNoMultiMS(chave, &no);

          /* Verifica se a chave foi encontrada */
      if (i < no.nFilhos - 1 && chave == no.chaves[i].chave)
         break; /* A chave foi encontrada */

         /* Atualiza os valores de 'posNoPai', 'noPai' e   */
         /* 'iPai' antes de descer mais um n�vel na �rvore */
      posNoPai = posNo;
      noPai = no;
      iPai = i;

         /* Desce mais um n�vel na �rvore */
      posNo = no.filhos[i];
   }

   if (posNo == POSICAO_NULA) {
      return 0; /* Chave n�o foi encontrada */
   }

   /*********************************************************/
   /* Neste ponto sabe-se que 'no' cont�m a chave que ser�  */
   /* removida e que a posi��o dessa chave nesse n� � i.    */
   /* O �ndice do filho de 'noPai' que cont�m a posi��o em  */
   /* arquivo de 'no' � 'iPai'.                             */
   /*                                                       */
   /* O n� ser� logicamente removido, visto que ele n�o     */
   /* poder� mais ser acessado, mas continuar� ocupando es- */
   /* pa�o no arquivo at� que a �rvore seja reconstru�da.   */
   /*********************************************************/

      /* Verifica quantos filhos a chave a ser removida possui */
   if ( no.filhos[i] == POSICAO_NULA ||
        no.filhos[i + 1] == POSICAO_NULA ) {
      /* A chave tem, no m�ximo, um filho */

         /* Verifica se o n� s� tem uma chave */
      if(no.nFilhos == 2) {
         /*                                     */
         /* A �ltima chave do n� ser� removida. */
         /* Logo esse n� tamb�m ser� removido.  */
         /*                                     */

            /* Se o n� a ser removido n�o */
            /* tem pai, trata-se da raiz  */
         if (posNoPai == POSICAO_NULA) {
            *streamArvore = NULL; /* A �rvore acabou-se */
            return 1; /* Game over */
         }

            /* Verifica quantos filhos o n� */
            /* a ser removido possui        */
         if(no.filhos[i] != POSICAO_NULA) {
               /* O n� tem filho esquerdo. O pai desse filho */
               /* passar� a ser o pai do n� a ser removido   */
            noPai.filhos[iPai] = no.filhos[i];
            EscreveNoMultiMS(*streamArvore, posNoPai, &noPai);
         } else if(no.filhos[i + 1] != POSICAO_NULA) {
               /* O n� tem filho direito. O pai desse filho */
               /* passar� a ser o pai do n� a ser removido  */
            noPai.filhos[iPai] = no.filhos[i + 1];
            EscreveNoMultiMS(*streamArvore, posNoPai, &noPai);
         } else {
               /* O n� n�o tem nenhum filho. O pai do */
               /* n� a ser removido passa a ser nulo  */
            noPai.filhos[iPai] = POSICAO_NULA;
         }
      } else {  /* Havia mais de uma chave no n� */
            /* A fun��o CompactaNoMultiMS() completa a remo��o */
         CompactaNoMultiMS(&no, i);

            /* O n� precisa ser atualizado em arquivo */
         EscreveNoMultiMS(*streamArvore, posNo, &no);
      }

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
   sucessoraEIndice = MenorChaveMultiMS( *streamArvore,
                                   no.filhos[i + 1] );

      /* Agora chama-se esta fun��o recursivamente para */
      /* remover a chave sucessora. Haver� apenas uma   */
      /* chamada recursiva porque a chave sucessora     */
      /* possui, no m�ximo, um filho.                   */
   if (!RemoveChaveMultiMS(streamArvore, sucessoraEIndice.chave))
      return 0;

      /* Substitui a chave a ser removida por sua sucessora */
   no.chaves[i] = sucessoraEIndice;
   EscreveNoMultiMS(*streamArvore, posNo, &no);

   return 1;
}

/****
 *
 * CaminhamentoInfixoMultiMS(): Executa um caminhamento numa �rvore
 *                        de busca multidirecional escrevendo
 *                        num arquvio de texto todas chaves
 *                        em ordem crescente
 *
 * Par�metros:
 *      streamArvore (entrada) - stream associado ao arquivo
 *                               que armazena a �rvore
 *      posNo (entrada) - posi��o no arquivo que cont�m a
 *                        o n� inicial do caminhamento
 *      streamChaves (entrada)- stream associado ao arquivo
 *                              onde ser�o escritas as chaves
 *
 * Retorno: Nada
 *
 ****/
void CaminhamentoInfixoMultiMS( FILE *streamArvore, int posNo,
                         FILE *streamChaves )
{
   int  i;
   tNoMultiMS  umNo;

      /* Nenhum dos streams recebidos pode ser NULL */
   ASSEGURA(streamArvore,
            ERRO_STREAM_NULL(streamArvore, CaminhamentoInfixoMultiMS));
   ASSEGURA(streamChaves,
            ERRO_STREAM_NULL(streamChaves, CaminhamentoInfixoMultiMS));

      /* Visita cada n� da �rvore em in-ordem at� que a  */
      /* posi��o inicial do caminhamento assuma um valor */
      /* inv�lido                                        */
   if (posNo != POSICAO_NULA) {
         /* L� o n� na posi��o 'posNo' do */
         /* arquivo que cont�m a �rvore   */
      LeNoMultiMS(streamArvore, posNo, &umNo);

         /* Para cada n�, caminha-se recursivamente na */
         /* sub�rvore esquerda de cada chave e depois  */
         /* escreve-se essa chave no arquivo de texto  */
      for (i = 0; i < umNo.nFilhos-1; ++i) {
         CaminhamentoInfixoMultiMS( streamArvore, umNo.filhos[i],
                             streamChaves );
            /* Visita a chave de �ndice i */
         fprintf(streamChaves, "%d\n", umNo.chaves[i].chave);
      }

         /* Agora caminha-se recursivamente na */
         /* sub�rvore direita da �ltima chave  */
      CaminhamentoInfixoMultiMS( streamArvore,
                           umNo.filhos[umNo.nFilhos-1],
                           streamChaves );
   }
}

/****
 *
 * MenorChaveMultiMS(): Encontra a menor chave com a respectiva
 *                posi��o do registro correspondente numa
 *                �rvore multidirecional descendente de
 *                busca armazenada em arquivo
 *
 * Par�metros:
 *      streamArvore (entrada) - stream associado ao arquivo
 *                               que cont�m a �rvore
 *      raiz (entrada) - posi��o da raiz da �rvore no arquivo
 *                       que cont�m a �rvore
 *
 * Retorno: O par chave/�ndice que cont�m a menor chave da �rvore
 *
 ****/
tChaveIndice MenorChaveMultiMS(FILE *streamArvore, int raiz)
{
   tNoMultiMS umNo; /* Armazena um n� */
   int       pos;  /* Armazena a posi��o de um n� */

      /* Verifica se o stream que representa a �rvore � v�lido */
   ASSEGURA( streamArvore,
             ERRO_STREAM_NULL(streamArvore, MenorChaveMultiMS) );

      /* L� o n� que representa a raiz da �rvore */
   LeNoMultiMS(streamArvore, raiz, &umNo);

      /* Armazena em 'pos' o filho mais � esquerda da raiz */
   pos = umNo.filhos[0];

      /* Encontra o n� mais � esquerda na �rvore */
   while (pos != POSICAO_NULA) {
         /* L� o n� cuja posi��o no arquivo */
         /* � indicada por 'pos'            */
      LeNoMultiMS(streamArvore, pos, &umNo);

         /* Desce-se at� o filho mais � esquerda deste n� */
      pos = umNo.filhos[0];
   }

      /* Retorna o par chave/�ndice que tem a menor chave */
   return umNo.chaves[0];
}

/****
 *
 * MaiorChaveMultiMS(): Encontra a maior chave com a respectiva
 *                posi��o do registro correspondente numa
 *                �rvore de busca multidirecional
 *
 * Par�metros:
 *      streamArvore (entrada) - stream associado ao arquivo
 *                               que cont�m a �rvore
 *
 * Retorno: A posi��o do registro que cont�m a maior chave
 *          da �rvore
 *
 ****/
tChaveIndice MaiorChaveMultiMS(FILE *streamArvore, int raiz)
{
   tNoMultiMS umNo; /* Armazena um n� */
   int pos;  /* Armazena a posi��o de um n� */

      /* Verifica se o stream que representa a �rvore � v�lido */
   ASSEGURA( streamArvore,
             ERRO_STREAM_NULL(streamArvore, MaiorChaveMultiMS) );

      /* L� o n� que representa a raiz da �rvore */
   LeNoMultiMS(streamArvore, raiz, &umNo);

      /* Armazena em 'pos' o filho mais � direita da raiz */
   pos = umNo.filhos[umNo.nFilhos-1];

      /* Encontra o n� mais � direita na �rvore */
   while (pos != POSICAO_NULA) {
         /* L� o n� cuja posi��o no arquivo */
         /* � indicada por 'pos'            */
      LeNoMultiMS(streamArvore, pos, &umNo);

         /* Passa para o filho mais � direita deste n� */
      pos = umNo.filhos[umNo.nFilhos-1];
   }

      /* Retorna o par chave/�ndice que tem a maior chave */
   return umNo.chaves[umNo.nFilhos-2];
}

/****
 *
 * NumeroDeChavesMultiMS(): Calcula o n�mero de n�s de uma �rvore
 *                multidirecional armazenada em arquivo
 *
 * Par�metros:
 *      stream (entrada) - stream associado ao arquivo contendo a
 *                         �rvore
 *
 * Retorno: O n�mero de n�s da �rvore
 *
 ****/
int NumeroDeChavesMultiMS(FILE *stream)
{
   int       nNos = 0;
   tNoMultiMS umNo;

      /* O stream que cont�m a �rvore n�o pode ser NULL */
   ASSEGURA(stream, ERRO_STREAM_NULL(stream, NumeroDeChavesMultiMS) );

      /* Tenta mover o apontador de posi��o */
      /* do arquivo para o seu in�cio       */
   MoveApontador(stream, 0, SEEK_SET);

      /* Acessa sequencialmente cada n� da �rvore */
      /* contando o n�mero de chaves em cada n�   */
   while (1) {
         /* L� um n� da �rvore */
      fread(&umNo, sizeof(umNo), 1, stream);

         /* Se ocorreu erro ou o final do arquivo */
         /* foi atingido, encerra o la�o          */
      if (ferror(stream) || feof(stream))
         break;

         /* Verifica se o n� n�o foi removido */
      if (umNo.nFilhos > 0)
         ++nNos;
   }

      /* Verifica se houve erro de leitura */
   ASSEGURA(!ferror(stream),ERRO_FREAD(NumeroDeChavesMultiMS));

   return nNos; /* Servi�o completo */
}

/****
 *
 * NumeroDeNosMultiMS(): Calcula o n�mero de chaves de uma
 *                   �rvore multidirecional de busca
 *
 * Par�metros:
 *     stream (entrada) - stream associado ao arquivo que
 *                        armazena a �rvore cujo n�mero de
 *                        chaves ser� determinado
 *     posNo (entrada) - a posi��o da raiz da �rvore no
 *                       arquivo que a cont�m
 *
 * Retorno: O referido n�mero de chaves
 *
 ****/
int NumeroDeNosMultiMS(FILE *stream, int posNo)
{
   int  nChaves = 0;
   tNoMultiMS  umNo;

      /* O stream que cont�m a �rvore n�o pode ser NULL */
   ASSEGURA(stream, ERRO_STREAM_NULL(stream, NumeroDeNosMultiMS) );

      /* Tenta mover o apontador de posi��o */
      /* do arquivo para o seu in�cio        */
   MoveApontador(stream, 0, SEEK_SET);

      /* Acessa sequencialmente cada n� da �rvore */
      /* contando o n�mero de chaves em cada n�   */
   while (1) {
      fread(&umNo, sizeof(umNo), 1, stream);

      if (ferror(stream) || feof(stream))
         break;

      nChaves += umNo.nFilhos - 1;
   }

   ASSEGURA(!ferror(stream), ERRO_FREAD(NumeroDeNosMultiMS));

   return nChaves; /* Servi�o completo */
}

/****
 *
 * AlturaMultiMS(): Calcula a altura de uma �rvore multidirecional descendente de busca
 *
 * Par�metros:
 *     stream (entrada) - stream associado ao arquivo que
 *                        armazena a �rvore cujo n�mero de
 *                        chaves ser� determinado
 *     posNo (entrada) - a posi��o da raiz da �rvore no arquivo que a cont�m
 *
 * Retorno: A referida altura
 *
 ****/
int AlturaMultiMS(FILE *stream, long posNo)
{
   int  i,
        alturas[G] = {0}; /* Array que armazenar� as alturas */
                      /* das sub�rvores de um n�          */
   tNoMultiMS  umNo; /* Armazenar� um n� da �rvore */

      /* O stream que cont�m a �rvore n�o pode ser NULL */
   ASSEGURA( stream, ERRO_STREAM_NULL(stream, AlturaMultiMS) );

      /* Se a �rvore n�o for vazia calcula sua altura */
   if (posNo != POSICAO_NULA) {
         /* L� o n� na posi��o 'posNo' do arquivo que cont�m a �rvore */
      LeNoMultiMS(stream, posNo, &umNo);

         /* Recursivamente, calcula-se a altura */
         /* da sub�rvore esquerda de cada chave */
      for (i = 0; i < umNo.nFilhos-1; ++i)
         alturas[i] = 1 + AlturaMultiMS(stream, umNo.filhos[i]);

         /* Agora calcula-se a altura da sub�rvore direita da �ltima chave */
      alturas[i] = 1 + AlturaMultiMS(stream, umNo.filhos[umNo.nFilhos-1]);

         /* A altura de uma �rvore � a maior        */
         /* altura encontrada entre suas sub�rvores */
      return MaiorInteiro(alturas, G);
   } else
      return 0; /* �rvore vazia tem altura zero */
}

