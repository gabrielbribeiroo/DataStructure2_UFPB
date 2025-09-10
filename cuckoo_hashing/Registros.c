/****
 *
 * T�tulo: Convertendo um Arquivo de Registros de Texto para Bin�rio
 *
 * Autor: Ulysses de Oliveira
 *
 * Data de Cria��o: 21/05/2012
 * �ltima modifica��o: 21/05/2012
 *
 * Descri��o: L� cada linha de um arquivo de texto, converte-a numa
 *            estrutura do tipo tAluno e armazena-a num arquivo
 *            bin�rio
 *
 * Entrada: Arquivo cujo nome � especificado pela constante
 *          NOME_ARQUIVO_TEXTO
 *
 * Sa�da: Arquivo cujo nome � especificado pela constante
 *        NOME_ARQUIVO_BIN
 *
 ****/

/*********************** Includes *************************/

#include <stdio.h>  /* Fun��es de Entrada e Sa�da   */
#include <string.h> /* Processamento de strings     */
#include <stdlib.h> /* strtod() e aloca��o din�mica */
#include "Registros.h" /* Interface deste m�dulo    */
#include "Leitura.h"  /* Leitura via teclado        */

/********************* Fun��es Locais *********************/

/****
 *
 * ExibeArrayDeChars(): Exibe na tela um array de elementos
 *                      do tipo char que n�o � string
 *
 * Par�metros:
 *      *nome (entrada) - string que ser� exibido antes do array
 *       ar (entrada) - endere�o do array
 *       tam (entrada) - tamanho do array
 *
 * Retorno: Nada
 *
 ****/
static void ExibeArrayDeChars(const char *nome, const char *ar,
                              int tam)
{
   putchar('\n');

   if (*nome) {
      printf("%s: ", nome);
   }

   for (int i = 0; i < tam && *ar; ++i) {
      putchar(ar[i]);
   }
}

/****
 *
 * LeIntNaoNegativo(): L� um string e tenta convert�-lo
 *                     em n�mero inteiro
 *
 * Par�metros: Nenhum
 *
 * Retorno: O valor lido e convertido se a convers�o
 *          for poss�vel; caso contr�rio, -1
 *
 * Observa��o: Diferentemente do que ocorre com as fun��es
 *             do m�dulo 'leitura', esta fun��o n�o insiste
 *             com o usu�rio
 *
 ****/
static int LeIntNaoNegativo(void)
{
   char  ar[MAX_NOME + 1];
   char *final;
   int   numero, resto;

      /* MAX_NOME � uma p�ssima escolha neste caso, mas  */
      /* este programa j� tem constante simb�lica demais */
   resto = LeString(ar, MAX_NOME + 1);

      /* Se o usu�rio digitou apenas [ENTER] ou digitou */
      /* demais, retorna -1 informando que o usu�rio    */
      /* n�o quis digitar o valor esperado              */
   if (!*ar || resto) {
      return -1;
   }

      /* Tenta efetuar a convers�o em long int */
   numero = (int)strtol(ar, &final, 10);

      /* Se o string n�o foi totalmente convertido, */
      /* retorna -1 informando que o usu�rio n�o    */
      /* quis digitar o valor esperado              */
   if (*final) {
      return -1;
   }

      /* Se o n�mero convertido for negativo, quem chamou */
      /* esta fun��o dever� saber como lidar com o caso   */
   return numero;
}

/********************* Fun��es Globais ********************/

/****
 *
 * ExibeRegistro(): Exibe na tela um registro do tipo tRegistroCEP
 *
 * Par�metros: *registro (entrada) - registro que ser� exibido
 *
 * Retorno: Nada
 *
 ****/
void ExibeRegistro(const tRegistroCEP *registro)
{
   printf("\nNumero: %d", registro->numero);

   ExibeArrayDeChars("UF", registro->UF, TAM_UF);

   printf("\nNumero de localidade: %d",
          registro->localidadeNumero);

   ExibeArrayDeChars("Nome abreviado", registro->nomeAbr,
                     MAX_NOME);

   ExibeArrayDeChars("Nome", registro->nome, MAX_NOME);

   printf("\nBairro inicio: %d", registro->bairroInicio);

   printf("\nBairro fim: %d", registro->bairroFim);

   ExibeArrayDeChars("CEP", registro->CEP, TAM_CEP);

   ExibeArrayDeChars("Complemento", registro->complemento,
                     MAX_COMP);

   ExibeArrayDeChars("Tipo de logradouro",
                     registro->tipoLogradouro, MAX_TIPO_LOG);

   printf("\nStatus de logradouro: %c",
          registro->statusTipoLogradouro);

   ExibeArrayDeChars("Nome sem acento:",
                     registro->nomeSemAcento, MAX_NOME);

   ExibeArrayDeChars("Chave DNE", registro->chaveDNE, TAM_DNE);
}

/****
 *
 * LeRegistro(): L� os dados de um CEP
 *
 * Par�metros: *regCEP (sa�da) - estrutura que receber� os
 *                               dados introduzidos pelo
 *                               usu�rio
 * Retorno: O par�metro
 *
 ****/
tRegistroCEP *LeRegistro(tRegistroCEP *regCEP)
{
   char ar[MAX_COMP + 1];

   printf("\n\t>>> UF: ");
   LeString(ar, TAM_UF + 1);
   strncpy(regCEP->UF, ar, TAM_UF);

   printf("\n\t>>> Numero de localidade: ");
   regCEP->localidadeNumero = LeNatural();

   printf("\n\t>>> Nome abreviado: ");
   LeString(ar, MAX_NOME + 1);
   strncpy(regCEP->nomeAbr, ar, MAX_NOME);

   printf("\n\t>>> Nome: ");
   LeString(ar, MAX_NOME + 1);
   strncpy(regCEP->nome, ar, MAX_NOME);

   printf("\n\t>>> Bairro inicio: ");
   regCEP->bairroInicio = LeNatural();

   printf("\n\t>>> Bairro fim: ");
   regCEP->bairroFim = LeNatural();

   printf("\n\t>>> CEP: ");
   LeString(ar, TAM_CEP + 1);
   strncpy(regCEP->CEP, ar, TAM_CEP);

   printf("\n\t>>> Complemento: ");
   LeString(ar, MAX_COMP + 1);
   strncpy(regCEP->complemento, ar, MAX_COMP);

   printf("\n\t>>> Tipo de logradouro: ");
   LeString(ar, MAX_TIPO_LOG + 1);
   strncpy(regCEP->tipoLogradouro, ar, MAX_TIPO_LOG);

   printf("\n\t>>> Status tipo de logradouro: ");
   regCEP->statusTipoLogradouro = LeOpcao("SsNn");

   printf("\n\t>>> Nome sem acento: ");
   LeString(ar, MAX_NOME + 1);
   strncpy(regCEP->nomeSemAcento, ar, MAX_NOME);

   printf("\n\t>>> Chave DNE: ");
   LeString(ar, TAM_DNE + 1);
   strncpy(regCEP->chaveDNE, ar, TAM_DNE);

   return regCEP;
}

/****
 *
 * SubstituiRegistro(): L� os dados de um registro que, se forem
 *                      v�lidos, substituir�o valores antigos
 *
 * Par�metros:
 *     *novo (sa�da) - estrutura que armazenar� os novos
 *                     dados introduzidos pelo usu�rio
 *     *antigo (entrada) - estrutura contendo os dados antigos
 *
 * Retorno: O primeiro par�metro
 *
 * Observa��o: Esta fun��o n�o est� rigorosamente bem
 *             implementada pois n�o faz parte do tema
 *             central deste exemplo
 *
 ****/
tRegistroCEP *SubstituiRegistro( tRegistroCEP *novo,
                              const tRegistroCEP *antigo )
{
   char ar[MAX_COMP + 1];
   int  num;

   printf("\n>>> ATENCAO: Dados invalidos nao substituem "
          "dados antigos <<<\n");

      /* L� o valor de UF. Qualquer string que n�o seja  */
      /* vazio � considerado v�lido, mas n�o deveria ser */
      /* assim (v. Observa��o). O mesmo vale para os     */
      /* demais strings.                                 */
   printf("\n\t>>> UF: ");
   LeString(ar, TAM_UF + 1);
   strncpy(novo->UF, *ar ? ar : antigo->UF, TAM_UF);

   printf("\n\t>>> Numero de localidade: ");
   num = LeIntNaoNegativo();
   novo->localidadeNumero = num >= 0 ?
                            num :
                            antigo->localidadeNumero;

   printf("\n\t>>> Nome abreviado: ");
   LeString(ar, MAX_NOME + 1);
   strncpy(novo->nomeAbr, *ar ? ar : antigo->nomeAbr, MAX_NOME);

   printf("\n\t>>> Nome: ");
   LeString(ar, MAX_NOME + 1);
   strncpy(novo->nome, *ar ? ar : antigo->nome, MAX_NOME);

   printf("\n\t>>> Bairro inicio: ");
   num = LeIntNaoNegativo();
   novo->bairroInicio = num >= 0 ?
                            num :
                            antigo->bairroInicio;

   printf("\n\t>>> Bairro fim: ");
   num = LeIntNaoNegativo();
   novo->bairroFim = num >= 0 ?
                            num :
                            antigo->bairroFim;

   printf("\n\t>>> CEP: ");
   LeString(ar, TAM_CEP + 1);
   strncpy(novo->CEP, *ar ? ar : antigo->CEP, TAM_CEP);

   printf("\n\t>>> Complemento: ");
   LeString(ar, MAX_COMP + 1);
   strncpy(novo->complemento, *ar ? ar : antigo->complemento,
           MAX_COMP);

   printf("\n\t>>> Tipo de logradouro: ");
   LeString(ar, MAX_TIPO_LOG + 1);
   strncpy(novo->tipoLogradouro,
           *ar ? ar : antigo->tipoLogradouro,
           MAX_TIPO_LOG);

   printf("\n\t>>> Status tipo de logradouro: ");
   num = LeCaractere();

   if (num == 's' || num == 'S') {
      novo->statusTipoLogradouro = 'S';
   } else if (num == 'n' || num == 'N') {
      novo->statusTipoLogradouro = 'N';
   } else {
      novo->statusTipoLogradouro = antigo->statusTipoLogradouro;
   }

   printf("\n\t>>> Nome sem acento: ");
   LeString(ar, MAX_NOME + 1);
   strncpy(novo->nomeSemAcento,
           *ar ? ar : antigo->nomeSemAcento, MAX_NOME);

   printf("\n\t>>> Chave DNE: ");
   LeString(ar, TAM_DNE + 1);
   strncpy(novo->chaveDNE, *ar ? ar : antigo->chaveDNE,
           TAM_DNE);

   return novo;
}

/****
 *
 * TamanhoDeArquivo(): Calcula o tamanho de um arquivo lendo cada
 *                     byte que o constitui e contando quantos
 *                     bytes s�o lidos
 *
 * Par�metros:
 *    stream (entrada) - stream bin�rio associado ao arquivo cujo
 *                       tamanho ser� calculado
 *
 * Retorno: -1, se ocorrer algum erro.
 *          O tamanho do arquivo, em caso contr�rio.
 *
 * Observa��es:
 *     1. O arquivo deve estar aberto em modo bin�rio que permite
 *        leitura
 *     2. A posi��o atual do indicador de posi��o do arquivo �
 *        preservada
 *
 ****/
int TamanhoDeArquivo(FILE *stream)
{
   int posicaoAtual, tamanho = 0;

      /* Guarda a posi��o atual do indicador de posi��o */
      /* do arquivo para que ele possa ser restaurado   */
   posicaoAtual = ftell(stream);

      /* Verifica se ftell() foi bem sucedida */
   if (posicaoAtual < 0) {
      return -1; /* A chamada de ftell() falhou */
   }

      /* Assegura leitura a partir do in�cio do arquivo */
   if ( fseek(stream, 0, SEEK_SET) ) {
         /* O movimento do indicador de posi��o n�o foi   */
         /* poss�vel. Neste caso, n�o precisa restaur�-lo */
      return -1;
   }

      /* O la�o encerra quando o final do arquivo for */
      /* atingido ou se ocorrer erro de leitura       */
   while (1){
         /* L� e descarta o pr�ximo byte */
      fgetc(stream);

         /* Checa se o arquivo j� foi inteiramente lido */
      if (feof(stream)) {
         break; /* Encerra la�o */
      }

         /* Verifica se ocorreu erro de leitura */
      if (ferror(stream)) {
            /* Restaura a posi��o original do indicador */
            /* de posi��o do arquivo  antes de retornar */
         fseek(stream, posicaoAtual, SEEK_SET);
         return -1; /* Ocorreu erro de leitura */
      }

      ++tamanho; /* Mais um byte foi lido */
   }

      /* Restaura a posi��o original do indicador de posi��o */
   if ( fseek(stream, posicaoAtual, SEEK_SET) ) {
      return -1; /* O movimento n�o foi poss�vel */
   }

   return tamanho;
}

/****
 *
 * NumeroDeRegistros(): Calcula o n�mero de registros (i.e.,
 *                      parti��es de tamanho especificado) de um
 *                      arquivo bin�rio
 *
 * Par�metros:
 *    stream (entrada) - stream bin�rio associado ao arquivo cujo
 *                       tamanho ser� calculado
 *    tamRegistro (entrada) - tamanho de cada parti��o do arquivo
 *
 * Retorno: O n�mero de parti��es do arquivo, se n�o ocorrer
 *          nenhum erro, ou -1, em caso contr�rio.
 *
 * Observa��o: O arquivo deve estar aberto em modo bin�rio que
 *             permite leitura
 *
 ****/
int NumeroDeRegistros( FILE *stream, int tamRegistro )
{
   int    nBytes = 0; /* N�mero de bytes do arquivo */

      /* Calcula o n�mero de bytes do arquivo */
   nBytes = TamanhoDeArquivo(stream);

      /* Verifica se ocorreu erro na contagem de bytes */
   if (nBytes < 0) {
      return -1; /* Erro na contagem de bytes */
   }

      /* O n�mero de registros � o n�mero de bytes do arquivo */
      /* dividido pelo n�mero de bytes de cada registro       */
   return nBytes/tamRegistro;
}
