/****
 *
 * Arquivo ArvoreMulti2.c
 *
 * Autor: Ulysses de Oliveira
 *
 * Data de Criação: 25/04/2004
 * Última modificação: 21/07/2008
 *
 * Descrição: Implementação das funções que executam operações usando
 *            árvores de busca multidirecional armazenada em arquivo
 *
 ****/

/*********************** Includes *************************/

#include <string.h>       /* Função strxxx() e memxxx() */
#include "ArvoreMulti2.h" /* Interface deste módulo     */
#include "Registros.h"   /* Funções de processamento    */
                         /* de registros                */
#include "leitura.h"  /* Funções de leitura via teclado */
#include "ASSEGURA.h" /* Macro ASSEGURA                 */

/******************** Funções de Locais *******************/

/****
 *
 * IniciaNoMultiMS(): Inicia um nó de uma árvore multidirecional
 *                    armazenada em arquivo
 *
 * Parâmetros:
 *      chaveIndice (entrada) - um par chave/índice a ser armazenado no nó
 *      pNo (saída) - endereço do nó criado
 *
 * Retorno: Nada
 *
 * Observação: O primeiro parâmetro poderá ser NULL. Nesse caso,
 *             será criado um nó vazio (i.e., sem nenhuma chave)
 *
 ****/
static void IniciaNoMultiMS(const tChaveIndice *chaveIndice, tNoMultiMS *pNo)
{
   int i;

   if (chaveIndice)
         /* A chave será a primeira do nó */
      pNo->chaves[0] = *chaveIndice;

      /* O número de filhos num nó de uma árvore     */
      /* multidirecional é igual ao número de chaves */
      /* mais um, a não ser que não haja chave       */
   pNo->nFilhos = chaveIndice ? 2 : 0;

      /* Inicia todos os possíveis filhos do nó */
   for (i = 0; i < G; ++i)
         /* Um novo nó ainda não possui filhos */
      pNo->filhos[i] = POSICAO_NULA;
}

/****
 *
 * LeNoMultiMS(): Faz a leitura de um nó de uma árvore
 *         multidirecional armazenada em arquivo
 *
 * Parâmetros:
 *      stream (entrada) - stream associado ao arquivo contendo a árvore onde será
 *                         feita a leitura
 *      pos (entrada) - posição no arquivo onde será feita a leitura
 *     *no (saída) - ponteiro para o nó que conterá o resultado da leitura
 *
 * Retorno: Nada
 *
 ****/
static void LeNoMultiMS(FILE *stream, int pos, tNoMultiMS *no)
{
   ASSEGURA(stream, ERRO_STREAM_NULL(stream, LeNoMultiMS));
   ASSEGURA(pos >= 0, ERRO_POSICAO(LeNoMultiMS));

      /* Tenta mover o apontador de arquivo para o local  */
      /* de leitura; se não conseguir, aborta o programa  */
   MoveApontador(stream, sizeof(tNoMultiMS)*pos, SEEK_SET);

      /* Tenta ler o nó */
   fread(no, sizeof(tNoMultiMS), 1, stream);

      /* Verifica se ocorreu erro de leitura */
   ASSEGURA( !ferror(stream), ERRO_FREAD(LeNoMultiMS) );
}

/****
 *
 * EscreveNoMultiMS(): Atualiza o conteúdo de um nó no arquivo que contém a árvore
 *
 * Parâmetros:
 *      stream (entrada) - stream associado ao arquivo
 *                         que contém a árvore
 *      pos (entrada) - posição do nó no arquivo
 *                      que contém a árvore
 *     *pNo (entrada) - nó que será atualizado
 *
 * Retorno: Nada
 *
 ****/
static void EscreveNoMultiMS(FILE *stream, int pos, const tNoMultiMS *pNo)
{
      /* Tenta mover o apontador de posição     */
      /* para o local onde o nó está armazenado */
   MoveApontador(stream, sizeof(*pNo)*pos, SEEK_SET);

      /* Escreve o nó no arquivo */
    fwrite(pNo, sizeof(*pNo), 1, stream);

      /* Se ocorreu erro, aborta o programa */
   ASSEGURA(!ferror(stream), ERRO_FWRITE(EscreveNoMultiMS));
}

/****
 *
 * BuscaEmNoMultiMS(): Tenta encontrar dentro de um nó uma chave que
 *                     seja maior do que ou igual a uma dada chave
 *
 * Parâmetros:
 *      chave (entrada) - a chave
 *      no (entrada) - ponteiro para o nó onde será feita a busca
 *
 * Retorno: O índice da chave dentro do nó se for encontrada
 *          uma chave maior do que a chave de busca; o número
 *          de chaves, se tal chave não for encontrada
 *
 ****/
static int BuscaEmNoMultiMS(tChave chave, const tNoMultiMS *no)
{
   int i,
       nChaves = no->nFilhos - 1; /* Número de chaves do nó */

      /* Procura no nó uma chave que seja maior */
      /* do que ou igual à chave de busca       */
   for (i = 0; i < nChaves; ++i)
         /* Verifica se tal chave foi encontrada */
      if (chave <= no->chaves[i].chave)
         return i;   /* A chave foi encontrada */

      /* A chave de busca é maior do que qualquer chave do nó */
   return nChaves; /* A chave NÃO foi encontrada */
}

/****
 *
 * EncontraNoMultiMS(): Procura uma chave na árvore e retorna a
 *                posição no arquivo do nó que contém a chave,
 *                se ela for encontrada. Se a chave não for
 *                encontrada, o nó cuja posição é retornada é
 *                aquele que conterá a chave se ela for inserida
 *
 * Parâmetros:
 *      stream (entrada) - stream associado ao arquivo que
 *                         contém a árvore
 *      chave (entrada) - a chave de busca
 *     *posicaoDaChaveNoNo (saída) - conterá o índice da
 *                                   chave no nó
 *     *encontrado (saída) - indicará se a chave foi encontrada
 *
 * Retorno: Posição no arquivo do nó contendo a chave de busca,
 *          se ela for encontrada, ou do pai do nó onde ela
 *          deveria estar se ela não for encontrada
 *
 ****/
static int EncontraNoMultiMS( FILE *stream, tChave chave,
                              int *posicaoDaChaveNoNo, int *encontrado )
{
   int        p,  /* p conterá a posição desejada    */
              q,  /* q conterá a posição do pai de p */
              posChave;
   tNoMultiMS umNo;

      /* Verifica se o stream é NULL */
   ASSEGURA(stream, ERRO_STREAM_NULL(stream, EncontraNoMultiMS));

      /* Tenta mover o apontador de arquivo para seu início */
   MoveApontador(stream, 0, SEEK_SET);

   p = 0; /* Começa busca na raiz da árvore */
   q = POSICAO_NULA; /* A raiz não tem pai */

   while (p != POSICAO_NULA) {
         /* Lê um nó da árvore na posição p do arquivo */
      LeNoMultiMS(stream, p, &umNo);

         /* Tenta encontrar a chave no nó corrente */
      posChave = BuscaEmNoMultiMS(chave, &umNo);

         /* Verifica se a chave foi encontrada */
      if((posChave < umNo.nFilhos - 1) && chave == umNo.chaves[posChave].chave) {
         *encontrado = 1;  /* A chave foi encontrada */
         *posicaoDaChaveNoNo = posChave;

         return p; /* Retorna a posição do nó no arquivo que contém a árvore */
      }

         /* A chave ainda não foi encontrada; desce um nível na árvore.  */
         /* Antes que p passe a referir-se a um filho, guarda seu valor. */
      q = p;
      p = umNo.filhos[posChave];
   }

   *encontrado = 0; /* A chave não foi encontrada */


   *posicaoDaChaveNoNo = posChave; /* Posição onde a chave deve ser inserida */

   return q;  /* Retorna o pai do nó p */
}

/****
 *
 * InsereEmFolhaMultiMS(): Insere uma chave numa folha incompleta de uma
 *                   árvore multidirecional armazenada em arquivo
 *
 * Parâmetros:
 *      stream (entrada) - stream associado ao arquivo que contém a árvore
 *      posicaoNo (entrada) - posição no arquivo do nó no
 *                            qual será feita a inserção
 *      posicaoDaChave (entrada) - posição da nova chave no
 *                                 array de chaves do nó
 *      chaveEIndice (entrada) - a chave e seu respectivo
 *                               índice que serão inseridos
 *
 * Retorno: Nada
 *
 ****/
static void InsereEmFolhaMultiMS( FILE *stream, int posicaoNo,
                                  int posicaoDaChave, tChaveIndice chaveEIndice )
{
   int        i;
   tNoMultiMS umNo;

      /* Verifica se o stream é válido */
   ASSEGURA(stream, ERRO_STREAM_NULL(stream, InsereEmFolhaMultiMS));

      /* Lê o nó-folha no arquivo que contém a árvore */
   LeNoMultiMS(stream, posicaoNo, &umNo);

      /* O número de filhos do nó deve ser estritamente menor do que o grau */
      /* da árvore. Se não o for, o  nó não é uma folha, deve haver algo    */
      /* errado e o programa será abortado.                                 */
   ASSEGURA( umNo.nFilhos < G, "ERRO: Tentativa de inserir em folha completa" );

      /* Abre espaço para a nova chave */
   for (i = umNo.nFilhos - 1; i > posicaoDaChave; --i)
      umNo.chaves[i] = umNo.chaves[i - 1];

   umNo.chaves[posicaoDaChave] = chaveEIndice; /* Armazena a nova chave */

   ++umNo.nFilhos; /* O número de filhos do nó é acrescido de um */

   EscreveNoMultiMS(stream, posicaoNo, &umNo); /* Atualiza nó no arquivo */
}

/****
 *
 * MaiorInteiro(): Encontra o maior elemento de um array de inteiros do tipo int
 *
 * Parâmetros:
 *      ar (entrada) - o array
 *      nElementos (entrada) - número de elementos do array
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
 * CompactaNoMultiMS(): Compacta um nó de uma árvore multidirecional de
 *                busca armazenada em arquivo após uma operação de
 *                remoção de chave
 *
 * Parâmetros:
 *      pNo (entrada) - ponteiro para o nó que será compactado
 *      posicao (entrada) - posição da chave removida
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

      /* A movimentação de filhos depende de qual dos filhos  */
      /* da chave removida é vazio. Se o filho esquerdo não   */
      /* for vazio, a movimentação começa uma posição adiante */
   if (pNo->filhos[posicao])
      posicao++;

      /* Move os filhos do nó */
   for (i = posicao; i < pNo->nFilhos - 1; ++i)
      pNo->filhos[i] = pNo->filhos[i + 1];

   --pNo->nFilhos; /* O número de filhos (e de chaves) diminuiu */
}

/******************** Funções de Globais ******************/

/****
 *
 * BuscaMultiMS(): Faz uma busca numa árvore multidirecional
 *               armazenada em arquivo
 *
 * Parâmetros:
 *      stream (entrada) - stream associado ao arquivo no
 *                         qual a árvore reside
 *      chave (entrada) - a chave de busca
 *
 * Retorno: Índice do registro no arquivo de registros
 *          se a chave for encontrada; -1, se a chave
 *          não for encontrada.
 *
 ****/
int BuscaMultiMS(FILE *stream, tChave chave)
{
   tNoMultiMS no;   /* Armazenará cada nó lido no arquivo */
   int        posNo = 0, /* Posição do nó no arquivo; a busca    */
                         /* começa na raiz que ocupa a posição 0 */
              i; /* Índice de uma chave num nó */

      /* Desce na árvore até encontrar a chave de busca ou uma posição nula */
   while (posNo != POSICAO_NULA) {
      LeNoMultiMS(stream, posNo, &no); /* Lê o nó na posição indicada */

         /* Tenta encontrar a chave no nó recém-recuperado */
      i = BuscaEmNoMultiMS(chave, &no);

         /* Verifica se a chave foi encontrada. A primeira    */
         /* comparação abaixo é necessária para evitar acesso */
         /* a uma posição inválida no array chaves[]          */
      if (i < no.nFilhos - 1 && chave == no.chaves[i].chave)
         return no.chaves[i].indice; /* A chave foi encontrada */

      posNo = no.filhos[i]; /* Desce mais um nível na árvore */
   }

   return POSICAO_NULA;  /* A chave não foi encontrada */
}

/****
 *
 * InsereMultiMS(): Insere um par chave/índice numa árvore
 *                 multidirecional descendente de busca
 *                 armazenada em arquivo
 *
 * Parâmetros:
 *     *posicaoRaiz (entrada/saída) - posição no arquivo da
 *                                    raiz da árvore
 *      chaveEIndice (entrada) - ponteiro para um par
 *                               chave/índice que será inserido
 *     *indiceDaChave (saída) - índice da chave no nó onde ela
 *                              se encontra ou onde ela será
 *                              inserida
 *      streamArvore (entrada) - stream associado ao arquivo
 *                               que contém a árvore
 *
 * Retorno: 1, se tudo houver inserção; 0, se não houver
 *
 * Observação: A chave é considerada primária; i.e., chaves
 *             repetidas não são permitidas na árvore.
 *
 ****/
int InsereMultiMS( int *posicaoRaiz, tChaveIndice *chaveEIndice,
                   FILE *streamArvore )
{
   tNoMultiMS no, novoNo;
   int        pNo, pNovoNo,
              encontrado,    /* Indicará se a chave foi encontrada */
              indiceDaChave; /* Índice da chave no nó onde ela */
                             /* for encontrada ou inserida     */

      /* O stream que representa o arquivo que */
      /* contém a árvore não pode ser NULL     */
   ASSEGURA( streamArvore, ERRO_STREAM_NULL(streamArvore, InsereMultiMS) );

   if (*posicaoRaiz == POSICAO_NULA) {
         /* A árvore ainda não foi criada */
      IniciaNoMultiMS(chaveEIndice, &no);
      *posicaoRaiz = 0;

         /* Armazena a raiz da árvore na posição 0 do arquivo   */
      EscreveNoMultiMS(streamArvore, 0, &no);

      return 1;
   }
         /* Tenta encontrar o nó que contém a chave */
   pNo = EncontraNoMultiMS( streamArvore, chaveEIndice->chave, &indiceDaChave,
                            &encontrado );

      /* Se a chave foi encontrada, não há mais nada */
      /* a fazer, pois ela é considerada primária    */
   if (encontrado)
      return 0; /* Não houve inserção */

   /*                                                     */
   /* Neste ponto, sabe-se que a chave não foi encontrada */
   /*                                                     */

      /* Lê no arquivo o nó cuja posição foi retornada pela função EncontraNoMultiMS() */
   LeNoMultiMS(streamArvore, pNo, &no);

      /* Verifica se há espaço para inserção nesse nó */
   if (no.nFilhos < G) {
      /* Há espaço para inserção nesse nó */

         /* Usando esse esquema de inserção, se o nó tem espaço sobrando ele */
         /* é uma folha. Então,  tenta-se inserir a chave no nó folha cuja   */
         /* posição é indicada por 'pNo'.                                    */
      InsereEmFolhaMultiMS( streamArvore, pNo, indiceDaChave, *chaveEIndice );

      return 1;  /* Serviço completo */
   }

      /*                                           */
      /* Neste ponto, sabe-se que não há nenhum nó */
      /* na árvore com espaço para conter a chave. */
      /*                                           */

      /* É necessário criar um novo nó para colocar a chave */
   IniciaNoMultiMS(chaveEIndice, &novoNo);

      /* O novo nó será armazenado ao final do arquivo; */
      /* portanto sua posição no arquivo será igual ao  */
      /* número de registros (nós) do arquivo (NB: a    */
      /* indexação do arquivo começa em zero; por isso, */
      /* não se acrescenta 1 a esse valor).             */
   pNovoNo = NumeroDeRegistros(streamArvore, sizeof(tNoMultiMS));

      /* O nó encontrado não tinha espaço para inserção */
      /* da chave. Portanto o índice da chave retornado */
      /* pela função EncontraNoMultiMS() é o índice do novo   */
      /* nó que será filho do nó encontrado             */
   no.filhos[indiceDaChave] = pNovoNo;

      /* O nó encontrado foi alterado; é preciso */
      /* atualizá-lo no arquivo                  */
   EscreveNoMultiMS(streamArvore, pNo, &no);

   /****                                      ****/
   /**** Insere o novo nó ao final do arquivo ****/
   /****                                      ****/

      /* Primeiro, tenta-se mover o apontador do  */
      /* arquivo para o final do arquivo; se isso */
      /* não for possível, aborta-se o programa   */
   MoveApontador(streamArvore, 0, SEEK_END);

      /* Agora tenta-se escrever o nó no arquivo; se */
      /* isso não for possível, aborta-se o programa */
   fwrite(&novoNo, sizeof(tNoMultiMS), 1, streamArvore);
   ASSEGURA(!ferror(streamArvore), ERRO_FWRITE(InsereMultiMS));

   return 1;  /* Serviço completo */
}

/****
 *
 * RemoveChaveMultiMS(): Remove a chave especificada de uma árvore
 *                 multidirecional de busca armazenada em arquivo
 *
 * Parâmetros:
 *      *streamArvore (entrada/saída) - stream associado ao
 *                                      arquivo que armazena
 *                                      a árvore
 *      chave (entrada) - a chave que será removida
 *
 * Retorno: 1, se a remoção for bem sucedida;
 *          0, em caso contrário
 *
 ****/
int RemoveChaveMultiMS(FILE **streamArvore, tChave chave)
{
   tChaveIndice sucessoraEIndice;
   tNoMultiMS    no, /* Armazenará cada nó lido no arquivo */
                noPai; /* Armazenará o pai de 'no' */
   int          i,
                posNo = 0, /* Posição no arquivo do nó que  */
                           /* contém a chave, se ela for    */
                           /* encontrada. A busca começa na */
                           /*  raiz que está na posição 0   */
                posNoPai = POSICAO_NULA, /* Posição de 'noPai' */
                iPai; /* Índice do filho do penúltimo nó  */
                      /* visitado que contém a posição no */
                      /* arquivo de 'no'                  */

   ASSEGURA(*streamArvore,
            ERRO_STREAM_NULL(streamArvore, RemoveChaveMultiMS) );

      /* A busca inicia na raiz e prossegue até que se */
      /* encontre a chave a ser removida ou um nó nulo */
   while (posNo != POSICAO_NULA) {
         /* Lê o nó na posição indicada por 'posNo' */
      LeNoMultiMS(*streamArvore, posNo, &no);

         /* Tenta encontrar a chave no nó recém-recuperado */
      i = BuscaEmNoMultiMS(chave, &no);

          /* Verifica se a chave foi encontrada */
      if (i < no.nFilhos - 1 && chave == no.chaves[i].chave)
         break; /* A chave foi encontrada */

         /* Atualiza os valores de 'posNoPai', 'noPai' e   */
         /* 'iPai' antes de descer mais um nível na árvore */
      posNoPai = posNo;
      noPai = no;
      iPai = i;

         /* Desce mais um nível na árvore */
      posNo = no.filhos[i];
   }

   if (posNo == POSICAO_NULA) {
      return 0; /* Chave não foi encontrada */
   }

   /*********************************************************/
   /* Neste ponto sabe-se que 'no' contém a chave que será  */
   /* removida e que a posição dessa chave nesse nó é i.    */
   /* O índice do filho de 'noPai' que contém a posição em  */
   /* arquivo de 'no' é 'iPai'.                             */
   /*                                                       */
   /* O nó será logicamente removido, visto que ele não     */
   /* poderá mais ser acessado, mas continuará ocupando es- */
   /* paço no arquivo até que a árvore seja reconstruída.   */
   /*********************************************************/

      /* Verifica quantos filhos a chave a ser removida possui */
   if ( no.filhos[i] == POSICAO_NULA ||
        no.filhos[i + 1] == POSICAO_NULA ) {
      /* A chave tem, no máximo, um filho */

         /* Verifica se o nó só tem uma chave */
      if(no.nFilhos == 2) {
         /*                                     */
         /* A última chave do nó será removida. */
         /* Logo esse nó também será removido.  */
         /*                                     */

            /* Se o nó a ser removido não */
            /* tem pai, trata-se da raiz  */
         if (posNoPai == POSICAO_NULA) {
            *streamArvore = NULL; /* A árvore acabou-se */
            return 1; /* Game over */
         }

            /* Verifica quantos filhos o nó */
            /* a ser removido possui        */
         if(no.filhos[i] != POSICAO_NULA) {
               /* O nó tem filho esquerdo. O pai desse filho */
               /* passará a ser o pai do nó a ser removido   */
            noPai.filhos[iPai] = no.filhos[i];
            EscreveNoMultiMS(*streamArvore, posNoPai, &noPai);
         } else if(no.filhos[i + 1] != POSICAO_NULA) {
               /* O nó tem filho direito. O pai desse filho */
               /* passará a ser o pai do nó a ser removido  */
            noPai.filhos[iPai] = no.filhos[i + 1];
            EscreveNoMultiMS(*streamArvore, posNoPai, &noPai);
         } else {
               /* O nó não tem nenhum filho. O pai do */
               /* nó a ser removido passa a ser nulo  */
            noPai.filhos[iPai] = POSICAO_NULA;
         }
      } else {  /* Havia mais de uma chave no nó */
            /* A função CompactaNoMultiMS() completa a remoção */
         CompactaNoMultiMS(&no, i);

            /* O nó precisa ser atualizado em arquivo */
         EscreveNoMultiMS(*streamArvore, posNo, &no);
      }

      return 1;
   }

   /***************************************************/
   /* Neste ponto, sabe-se que a chave a ser removida */
   /* possui dois filhos não vazios. Nesse caso,      */
   /* substitui-se a chave a ser removida por sua     */
   /* sucessora imediata e remove-se essa sucessora   */
   /***************************************************/

      /* A chave sucessora imediata da chave a ser removida */
      /* é a menor chave da subárvore cuja raiz é o filho   */
      /* direito da chave a ser removida                    */
   sucessoraEIndice = MenorChaveMultiMS( *streamArvore,
                                   no.filhos[i + 1] );

      /* Agora chama-se esta função recursivamente para */
      /* remover a chave sucessora. Haverá apenas uma   */
      /* chamada recursiva porque a chave sucessora     */
      /* possui, no máximo, um filho.                   */
   if (!RemoveChaveMultiMS(streamArvore, sucessoraEIndice.chave))
      return 0;

      /* Substitui a chave a ser removida por sua sucessora */
   no.chaves[i] = sucessoraEIndice;
   EscreveNoMultiMS(*streamArvore, posNo, &no);

   return 1;
}

/****
 *
 * CaminhamentoInfixoMultiMS(): Executa um caminhamento numa árvore
 *                        de busca multidirecional escrevendo
 *                        num arquvio de texto todas chaves
 *                        em ordem crescente
 *
 * Parâmetros:
 *      streamArvore (entrada) - stream associado ao arquivo
 *                               que armazena a árvore
 *      posNo (entrada) - posição no arquivo que contém a
 *                        o nó inicial do caminhamento
 *      streamChaves (entrada)- stream associado ao arquivo
 *                              onde serão escritas as chaves
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

      /* Visita cada nó da árvore em in-ordem até que a  */
      /* posição inicial do caminhamento assuma um valor */
      /* inválido                                        */
   if (posNo != POSICAO_NULA) {
         /* Lê o nó na posição 'posNo' do */
         /* arquivo que contém a árvore   */
      LeNoMultiMS(streamArvore, posNo, &umNo);

         /* Para cada nó, caminha-se recursivamente na */
         /* subárvore esquerda de cada chave e depois  */
         /* escreve-se essa chave no arquivo de texto  */
      for (i = 0; i < umNo.nFilhos-1; ++i) {
         CaminhamentoInfixoMultiMS( streamArvore, umNo.filhos[i],
                             streamChaves );
            /* Visita a chave de índice i */
         fprintf(streamChaves, "%d\n", umNo.chaves[i].chave);
      }

         /* Agora caminha-se recursivamente na */
         /* subárvore direita da última chave  */
      CaminhamentoInfixoMultiMS( streamArvore,
                           umNo.filhos[umNo.nFilhos-1],
                           streamChaves );
   }
}

/****
 *
 * MenorChaveMultiMS(): Encontra a menor chave com a respectiva
 *                posição do registro correspondente numa
 *                árvore multidirecional descendente de
 *                busca armazenada em arquivo
 *
 * Parâmetros:
 *      streamArvore (entrada) - stream associado ao arquivo
 *                               que contém a árvore
 *      raiz (entrada) - posição da raiz da árvore no arquivo
 *                       que contém a árvore
 *
 * Retorno: O par chave/índice que contém a menor chave da árvore
 *
 ****/
tChaveIndice MenorChaveMultiMS(FILE *streamArvore, int raiz)
{
   tNoMultiMS umNo; /* Armazena um nó */
   int       pos;  /* Armazena a posição de um nó */

      /* Verifica se o stream que representa a árvore é válido */
   ASSEGURA( streamArvore,
             ERRO_STREAM_NULL(streamArvore, MenorChaveMultiMS) );

      /* Lê o nó que representa a raiz da árvore */
   LeNoMultiMS(streamArvore, raiz, &umNo);

      /* Armazena em 'pos' o filho mais à esquerda da raiz */
   pos = umNo.filhos[0];

      /* Encontra o nó mais à esquerda na árvore */
   while (pos != POSICAO_NULA) {
         /* Lê o nó cuja posição no arquivo */
         /* é indicada por 'pos'            */
      LeNoMultiMS(streamArvore, pos, &umNo);

         /* Desce-se até o filho mais à esquerda deste nó */
      pos = umNo.filhos[0];
   }

      /* Retorna o par chave/índice que tem a menor chave */
   return umNo.chaves[0];
}

/****
 *
 * MaiorChaveMultiMS(): Encontra a maior chave com a respectiva
 *                posição do registro correspondente numa
 *                árvore de busca multidirecional
 *
 * Parâmetros:
 *      streamArvore (entrada) - stream associado ao arquivo
 *                               que contém a árvore
 *
 * Retorno: A posição do registro que contém a maior chave
 *          da árvore
 *
 ****/
tChaveIndice MaiorChaveMultiMS(FILE *streamArvore, int raiz)
{
   tNoMultiMS umNo; /* Armazena um nó */
   int pos;  /* Armazena a posição de um nó */

      /* Verifica se o stream que representa a árvore é válido */
   ASSEGURA( streamArvore,
             ERRO_STREAM_NULL(streamArvore, MaiorChaveMultiMS) );

      /* Lê o nó que representa a raiz da árvore */
   LeNoMultiMS(streamArvore, raiz, &umNo);

      /* Armazena em 'pos' o filho mais à direita da raiz */
   pos = umNo.filhos[umNo.nFilhos-1];

      /* Encontra o nó mais à direita na árvore */
   while (pos != POSICAO_NULA) {
         /* Lê o nó cuja posição no arquivo */
         /* é indicada por 'pos'            */
      LeNoMultiMS(streamArvore, pos, &umNo);

         /* Passa para o filho mais à direita deste nó */
      pos = umNo.filhos[umNo.nFilhos-1];
   }

      /* Retorna o par chave/índice que tem a maior chave */
   return umNo.chaves[umNo.nFilhos-2];
}

/****
 *
 * NumeroDeChavesMultiMS(): Calcula o número de nós de uma árvore
 *                multidirecional armazenada em arquivo
 *
 * Parâmetros:
 *      stream (entrada) - stream associado ao arquivo contendo a
 *                         árvore
 *
 * Retorno: O número de nós da árvore
 *
 ****/
int NumeroDeChavesMultiMS(FILE *stream)
{
   int       nNos = 0;
   tNoMultiMS umNo;

      /* O stream que contém a árvore não pode ser NULL */
   ASSEGURA(stream, ERRO_STREAM_NULL(stream, NumeroDeChavesMultiMS) );

      /* Tenta mover o apontador de posição */
      /* do arquivo para o seu início       */
   MoveApontador(stream, 0, SEEK_SET);

      /* Acessa sequencialmente cada nó da árvore */
      /* contando o número de chaves em cada nó   */
   while (1) {
         /* Lê um nó da árvore */
      fread(&umNo, sizeof(umNo), 1, stream);

         /* Se ocorreu erro ou o final do arquivo */
         /* foi atingido, encerra o laço          */
      if (ferror(stream) || feof(stream))
         break;

         /* Verifica se o nó não foi removido */
      if (umNo.nFilhos > 0)
         ++nNos;
   }

      /* Verifica se houve erro de leitura */
   ASSEGURA(!ferror(stream),ERRO_FREAD(NumeroDeChavesMultiMS));

   return nNos; /* Serviço completo */
}

/****
 *
 * NumeroDeNosMultiMS(): Calcula o número de chaves de uma
 *                   árvore multidirecional de busca
 *
 * Parâmetros:
 *     stream (entrada) - stream associado ao arquivo que
 *                        armazena a árvore cujo número de
 *                        chaves será determinado
 *     posNo (entrada) - a posição da raiz da árvore no
 *                       arquivo que a contém
 *
 * Retorno: O referido número de chaves
 *
 ****/
int NumeroDeNosMultiMS(FILE *stream, int posNo)
{
   int  nChaves = 0;
   tNoMultiMS  umNo;

      /* O stream que contém a árvore não pode ser NULL */
   ASSEGURA(stream, ERRO_STREAM_NULL(stream, NumeroDeNosMultiMS) );

      /* Tenta mover o apontador de posição */
      /* do arquivo para o seu início        */
   MoveApontador(stream, 0, SEEK_SET);

      /* Acessa sequencialmente cada nó da árvore */
      /* contando o número de chaves em cada nó   */
   while (1) {
      fread(&umNo, sizeof(umNo), 1, stream);

      if (ferror(stream) || feof(stream))
         break;

      nChaves += umNo.nFilhos - 1;
   }

   ASSEGURA(!ferror(stream), ERRO_FREAD(NumeroDeNosMultiMS));

   return nChaves; /* Serviço completo */
}

/****
 *
 * AlturaMultiMS(): Calcula a altura de uma árvore multidirecional descendente de busca
 *
 * Parâmetros:
 *     stream (entrada) - stream associado ao arquivo que
 *                        armazena a árvore cujo número de
 *                        chaves será determinado
 *     posNo (entrada) - a posição da raiz da árvore no arquivo que a contém
 *
 * Retorno: A referida altura
 *
 ****/
int AlturaMultiMS(FILE *stream, long posNo)
{
   int  i,
        alturas[G] = {0}; /* Array que armazenará as alturas */
                      /* das subárvores de um nó          */
   tNoMultiMS  umNo; /* Armazenará um nó da árvore */

      /* O stream que contém a árvore não pode ser NULL */
   ASSEGURA( stream, ERRO_STREAM_NULL(stream, AlturaMultiMS) );

      /* Se a árvore não for vazia calcula sua altura */
   if (posNo != POSICAO_NULA) {
         /* Lê o nó na posição 'posNo' do arquivo que contém a árvore */
      LeNoMultiMS(stream, posNo, &umNo);

         /* Recursivamente, calcula-se a altura */
         /* da subárvore esquerda de cada chave */
      for (i = 0; i < umNo.nFilhos-1; ++i)
         alturas[i] = 1 + AlturaMultiMS(stream, umNo.filhos[i]);

         /* Agora calcula-se a altura da subárvore direita da última chave */
      alturas[i] = 1 + AlturaMultiMS(stream, umNo.filhos[umNo.nFilhos-1]);

         /* A altura de uma árvore é a maior        */
         /* altura encontrada entre suas subárvores */
      return MaiorInteiro(alturas, G);
   } else
      return 0; /* Árvore vazia tem altura zero */
}

