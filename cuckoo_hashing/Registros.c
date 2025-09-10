/****
 *
 * Título: Convertendo um Arquivo de Registros de Texto para Binário
 *
 * Autor: Ulysses de Oliveira
 *
 * Data de Criação: 21/05/2012
 * Última modificação: 21/05/2012
 *
 * Descrição: Lê cada linha de um arquivo de texto, converte-a numa
 *            estrutura do tipo tAluno e armazena-a num arquivo
 *            binário
 *
 * Entrada: Arquivo cujo nome é especificado pela constante
 *          NOME_ARQUIVO_TEXTO
 *
 * Saída: Arquivo cujo nome é especificado pela constante
 *        NOME_ARQUIVO_BIN
 *
 ****/

/*********************** Includes *************************/

#include <stdio.h>  /* Funções de Entrada e Saída   */
#include <string.h> /* Processamento de strings     */
#include <stdlib.h> /* strtod() e alocação dinâmica */
#include "Registros.h" /* Interface deste módulo    */
#include "Leitura.h"  /* Leitura via teclado        */

/********************* Funções Locais *********************/

/****
 *
 * ExibeArrayDeChars(): Exibe na tela um array de elementos
 *                      do tipo char que não é string
 *
 * Parâmetros:
 *      *nome (entrada) - string que será exibido antes do array
 *       ar (entrada) - endereço do array
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
 * LeIntNaoNegativo(): Lê um string e tenta convertê-lo
 *                     em número inteiro
 *
 * Parâmetros: Nenhum
 *
 * Retorno: O valor lido e convertido se a conversão
 *          for possível; caso contrário, -1
 *
 * Observação: Diferentemente do que ocorre com as funções
 *             do módulo 'leitura', esta função não insiste
 *             com o usuário
 *
 ****/
static int LeIntNaoNegativo(void)
{
   char  ar[MAX_NOME + 1];
   char *final;
   int   numero, resto;

      /* MAX_NOME é uma péssima escolha neste caso, mas  */
      /* este programa já tem constante simbólica demais */
   resto = LeString(ar, MAX_NOME + 1);

      /* Se o usuário digitou apenas [ENTER] ou digitou */
      /* demais, retorna -1 informando que o usuário    */
      /* não quis digitar o valor esperado              */
   if (!*ar || resto) {
      return -1;
   }

      /* Tenta efetuar a conversão em long int */
   numero = (int)strtol(ar, &final, 10);

      /* Se o string não foi totalmente convertido, */
      /* retorna -1 informando que o usuário não    */
      /* quis digitar o valor esperado              */
   if (*final) {
      return -1;
   }

      /* Se o número convertido for negativo, quem chamou */
      /* esta função deverá saber como lidar com o caso   */
   return numero;
}

/********************* Funções Globais ********************/

/****
 *
 * ExibeRegistro(): Exibe na tela um registro do tipo tRegistroCEP
 *
 * Parâmetros: *registro (entrada) - registro que será exibido
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
 * LeRegistro(): Lê os dados de um CEP
 *
 * Parâmetros: *regCEP (saída) - estrutura que receberá os
 *                               dados introduzidos pelo
 *                               usuário
 * Retorno: O parâmetro
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
 * SubstituiRegistro(): Lê os dados de um registro que, se forem
 *                      válidos, substituirão valores antigos
 *
 * Parâmetros:
 *     *novo (saída) - estrutura que armazenará os novos
 *                     dados introduzidos pelo usuário
 *     *antigo (entrada) - estrutura contendo os dados antigos
 *
 * Retorno: O primeiro parâmetro
 *
 * Observação: Esta função não está rigorosamente bem
 *             implementada pois não faz parte do tema
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

      /* Lê o valor de UF. Qualquer string que não seja  */
      /* vazio é considerado válido, mas não deveria ser */
      /* assim (v. Observação). O mesmo vale para os     */
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
 *                     bytes são lidos
 *
 * Parâmetros:
 *    stream (entrada) - stream binário associado ao arquivo cujo
 *                       tamanho será calculado
 *
 * Retorno: -1, se ocorrer algum erro.
 *          O tamanho do arquivo, em caso contrário.
 *
 * Observações:
 *     1. O arquivo deve estar aberto em modo binário que permite
 *        leitura
 *     2. A posição atual do indicador de posição do arquivo é
 *        preservada
 *
 ****/
int TamanhoDeArquivo(FILE *stream)
{
   int posicaoAtual, tamanho = 0;

      /* Guarda a posição atual do indicador de posição */
      /* do arquivo para que ele possa ser restaurado   */
   posicaoAtual = ftell(stream);

      /* Verifica se ftell() foi bem sucedida */
   if (posicaoAtual < 0) {
      return -1; /* A chamada de ftell() falhou */
   }

      /* Assegura leitura a partir do início do arquivo */
   if ( fseek(stream, 0, SEEK_SET) ) {
         /* O movimento do indicador de posição não foi   */
         /* possível. Neste caso, não precisa restaurá-lo */
      return -1;
   }

      /* O laço encerra quando o final do arquivo for */
      /* atingido ou se ocorrer erro de leitura       */
   while (1){
         /* Lê e descarta o próximo byte */
      fgetc(stream);

         /* Checa se o arquivo já foi inteiramente lido */
      if (feof(stream)) {
         break; /* Encerra laço */
      }

         /* Verifica se ocorreu erro de leitura */
      if (ferror(stream)) {
            /* Restaura a posição original do indicador */
            /* de posição do arquivo  antes de retornar */
         fseek(stream, posicaoAtual, SEEK_SET);
         return -1; /* Ocorreu erro de leitura */
      }

      ++tamanho; /* Mais um byte foi lido */
   }

      /* Restaura a posição original do indicador de posição */
   if ( fseek(stream, posicaoAtual, SEEK_SET) ) {
      return -1; /* O movimento não foi possível */
   }

   return tamanho;
}

/****
 *
 * NumeroDeRegistros(): Calcula o número de registros (i.e.,
 *                      partições de tamanho especificado) de um
 *                      arquivo binário
 *
 * Parâmetros:
 *    stream (entrada) - stream binário associado ao arquivo cujo
 *                       tamanho será calculado
 *    tamRegistro (entrada) - tamanho de cada partição do arquivo
 *
 * Retorno: O número de partições do arquivo, se não ocorrer
 *          nenhum erro, ou -1, em caso contrário.
 *
 * Observação: O arquivo deve estar aberto em modo binário que
 *             permite leitura
 *
 ****/
int NumeroDeRegistros( FILE *stream, int tamRegistro )
{
   int    nBytes = 0; /* Número de bytes do arquivo */

      /* Calcula o número de bytes do arquivo */
   nBytes = TamanhoDeArquivo(stream);

      /* Verifica se ocorreu erro na contagem de bytes */
   if (nBytes < 0) {
      return -1; /* Erro na contagem de bytes */
   }

      /* O número de registros é o número de bytes do arquivo */
      /* dividido pelo número de bytes de cada registro       */
   return nBytes/tamRegistro;
}
