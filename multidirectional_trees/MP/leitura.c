/****
 *
 * T�tulo: LeituraFacil
 *
 * Autor: Gabriel Ribeiro
 *
 * Descri��o: Implementa��o da biblioteca LeituraFacil
 *
 ****/

/************************ Includes ************************/

#include <stdio.h>   /* Entrada e sa�da             */
#include <string.h>  /* Processamento de strings    */
#include <stdlib.h>  /* maloc()                     */
#include <ctype.h>   /* Classifica��o de caracteres */
#include "leitura.h" /* Interface da biblioteca     */
                     /* LeituraFacil                */

/******************** Fun��es de Locais *******************/

/****
 *
 * LimpaBuffer(): Limpa o buffer associado ao meio de
 *                entrada padr�o (stdin)
 *
 * Par�metros: Nenhum
 *
 * Retorno: O n�mero de caracteres descartados, com
 *          exce��o do caractere '\n'.
 *
 ****/
static int LimpaBuffer(void)
{
   int carLido, /* Armazena cada caractere lido */
       nCarLidos = 0; /* Conta o n�mero de */
                      /* caracteres lidos  */

      /* L� e descarta cada caractere lido at�    */
      /* encontrar '\n' ou getchar() retornar EOF */
   do {
      carLido = getchar(); /* L� um caractere */
      ++nCarLidos; /* Mais um caractere foi lido */
   } while ((carLido != '\n') && (carLido != EOF));

      /* O �ltimo caractere lido foi '\n' ou  */
      /* EOF e n�o deve ser considerado sobra */
   return nCarLidos - 1;
}

/******************** Fun��es de Globais ******************/

/****
 *
 * LeCaractere(): L� um caractere via teclado e remove os
 *                caracteres remanescentes no buffer
 *
 * Par�metros: Nenhum
 *
 * Retorno: O caractere lido
 *
 * Observa��o: Esta fun��o deixa o buffer limpo
 *
 ****/
int LeCaractere(void)
{
   int c, /* Armazenar� cada caractere lido */
       nResto = 0; /* N�mero de caracteres */
                   /* excedentes           */

   /* Volta para c� se ocorrer erro de leitura */
inicio:
   c = getchar(); /* L� o caractere digitado */

      /* Verifica se getchar() retornou EOF */
   if (c == EOF) {
      printf( "\a\n\t>>>Erro de leitura. "
              "Tente novamente\n\t> " );
      goto inicio; /* N�o faz mal algum */
   }

      /* Pelo menos '\n' se encontra no buffer */
   nResto = LimpaBuffer();

      /* Repreende o usu�rio se ele digitou demais */
   if (nResto == 1) {
      printf("\t>>> Um caractere foi descartado\n");
   } else if (nResto > 1) {
      printf( "\t>>> %d caracteres foram descartados\n",
              nResto );
   }

   return c; /* Retorna o caractere lido */
}

/****
 *
 * LeInteiro(): L� um n�mero inteiro via teclado
 *              e deixa o buffer intacto
 *
 * Par�metros: Nenhum
 *
 * Retorno: O n�mero inteiro lido
 *
 ****/
int LeInteiro(void)
{
   int num, /* O n�mero lido */
       teste, /* Valor retornado por scanf() */
       nResto = 0; /* N�mero de caracteres */
                   /* excedentes           */

   /* Desvia para c� se o valor for inv�lido */
inicio:
      /* Tenta ler um valor v�lido */
   teste = scanf("%d", &num);

      /* Se n�o ocorreu erro de leitura ou de final */
      /* de arquivo, h� caracteres remanescentes    */
      /* que precisam ser removidos                 */
   if (teste != EOF) {
      nResto = LimpaBuffer();
   }

      /* Enquanto o valor retornado por scanf()  */
      /* n�o indicar que um valor v�lido foi     */
      /* lido continua tentando obter esse valor */
   while(teste != 1) {
      printf("\a\n\t>>> O valor digitado e' invalido. "
                      "Tente novamente\n\t> " );
      goto inicio; /* N�o causa dano algum */
   }

      /* Repreende o usu�rio se ele digitou demais */
   if (nResto == 1) {
      printf("\t>>> Um caractere foi descartado\n");
   } else if (nResto > 1) {
      printf("\t>>> %d caracteres foram descartados\n",
             nResto);
   }

      /* O valor retornado certamente � v�lido */
   return num;
}

/****
 *
 * LeInteiroEntre(): L� um n�mero inteiro que se encontra
 *                   entre dois valores via teclado
 *                   e deixa o buffer intacto
 *
 * Par�metros: Nenhum
 *
 * Retorno: O n�mero inteiro lido
 *
 ****/
int LeInteiroEntre(int valor1, int valor2)
{
   int num, /* O n�mero lido */
       menorValor = valor1 <= valor2 ? valor1 : valor2,
       maiorValor = valor1 >= valor2 ? valor1 : valor2,
       teste;

   /* Desvia para c� se o valor for inv�lido */
inicio:
      /* Tenta ler um valor v�lido */
   num = LeInteiro();

   teste = num >= menorValor && num <= maiorValor;

      /* Enquanto o valor lido n�o for v�lido */
      /* continua tentando obter esse valor   */
   while(!teste) {
      printf("\a\n\t>>> O valor digitado e' invalido. "
                      "Tente novamente\n\t> " );
      goto inicio; /* N�o causa dano algum */
   }

      /* O valor retornado certamente � v�lido */
   return num;
}

/****
 *
 * LeNatural(): L� um n�mero natural via teclado
 *              e deixa o buffer intacto
 *
 * Par�metros: Nenhum
 *
 * Retorno: O n�mero natural lido
 *
 ****/
int LeNatural(void)
{
   int num, /* O n�mero lido */
       teste, /* Valor retornado por scanf() */
       nResto = 0; /* N�mero de caracteres */
                   /* excedentes           */

   /* Desvia para c� se o valor for inv�lido */
inicio:
      /* Tenta ler um valor v�lido */
   teste = scanf("%d", &num);

      /* Se n�o ocorreu erro de leitura ou de final */
      /* de arquivo, h� caracteres remanescentes    */
      /* que precisam ser removidos                 */
   if (teste != EOF) {
      nResto = LimpaBuffer();
   }

      /* Enquanto o valor retornado por scanf()  */
      /* n�o indicar que um valor v�lido foi     */
      /* lido continua tentando obter esse valor */
   while(teste != 1 || num < 0) {
      printf("\a\n\t>>> O valor digitado e' invalido. "
                      "Tente novamente\n\t> " );
      goto inicio; /* N�o causa dano algum */
   }

      /* Repreende o usu�rio se ele digitou demais */
   if (nResto == 1) {
      printf("\t>>> Um caractere foi descartado\n");
   } else if (nResto > 1) {
      printf( "\t>>> %d caracteres foram descartados\n",
              nResto );
   }

      /* O valor retornado certamente � v�lido */
   return num;
}

/****
 *
 * LeNaturalPositivo(): L� um n�mero natural positivo via
 *                      teclado e deixa o buffer intacto
 *
 * Par�metros: Nenhum
 *
 * Retorno: O n�mero natural lido
 *
 ****/
int LeNaturalPositivo(void)
{
   int num, /* O n�mero lido */
       teste, /* Valor retornado por scanf() */
       nResto = 0; /* N�mero de caracteres */
                   /* excedentes           */

   /* Desvia para c� se o valor for inv�lido */
inicio:
      /* Tenta ler um valor v�lido */
   teste = scanf("%d", &num);

      /* Se n�o ocorreu erro de leitura ou de final */
      /* de arquivo, h� caracteres remanescentes    */
      /* que precisam ser removidos                 */
   if (teste != EOF) {
      nResto = LimpaBuffer();
   }

      /* Enquanto o valor retornado por scanf()  */
      /* n�o indicar que um valor v�lido foi     */
      /* lido continua tentando obter esse valor */
   while(teste != 1 || num < 1) {
      printf("\a\n\t>>> O valor digitado e' invalido. "
                      "Tente novamente\n\t> " );
      goto inicio; /* N�o causa dano algum */
   }

      /* Repreende o usu�rio se ele digitou demais */
   if (nResto == 1) {
      printf("\t>>> Um caractere foi descartado\n");
   } else if (nResto > 1) {
      printf( "\t>>> %d caracteres foram descartados\n",
              nResto );
   }

      /* O valor retornado certamente � v�lido */
   return num;
}

/****
 *
 * LeReal(): L� um valor do tipo double via teclado
 *           deixando o buffer associado ao teclado
 *           sem caracteres remanescentes
 *
 * Par�metros: Nenhum
 *
 * Retorno: O valor lido e validado
 *
 ****/
double LeReal(void)
{
   int    teste, /* Armazena o retorno de scanf() */
          nResto = 0; /* N�mero de caracteres */
                      /* excedentes           */
   double valorLido; /* O valor digitado pelo usu�rio */

      /* O la�o encerra quando o usu�rio se comporta bem */
   while (1) {
         /* Tenta ler um valor do tipo double */
      teste = scanf("%lf", &valorLido);

         /* Se n�o ocorreu erro de leitura ou de final */
         /* de arquivo, h� caracteres remanescentes    */
         /* que precisam ser removidos                 */
      if (teste != EOF) {
         nResto = LimpaBuffer();
      }

         /* Se o valor retornado por scanf() */
         /* foi 1, a leitura foi OK          */
      if (teste == 1) {
         break; /* Leitura foi nota 10 */
      } else { /* Usu�rio foi mal comportado */
         printf( "\a\n\t>>> O valor digitado e' invalido. "
                 "Tente novamente\n\t> " );
      }
   }

      /* Repreende o usu�rio se ele digitou demais */
   if (nResto == 1) {
      printf("\t>>> Um caractere foi descartado\n");
   } else if (nResto > 1) {
      printf("\t>>> %d caracteres foram descartados\n",
             nResto);
   }

      /* Retorna um valor v�lido do tipo double */
   return valorLido;
}

/****
 *
 * LeString(): L� um string via teclado deixando o buffer
 *             zerado
 *
 * Par�metros:
 *      ar[] (sa�da): array que armazenar� o string lido
 *      nElementos (entrada): n�mero m�ximo de elementos que
 *                            ser�o armazenados no array,
 *                            incluindo '\0'
 *
 * Retorno: O n�mero de caracteres que o usu�rio digitou a
 *          mais, sem incluir '\n'
 *
 ****/
int LeString(char *ar, int nElementos)
{
   char *p; /* Usado para remover '\n' do string */
   int   nCarDeixados = 0; /* Conta o n�mero de     */
                           /* caracteres excedentes */
                           /* no buffer             */

      /* L� no m�ximo nElementos - 1 caracteres ou at�   */
      /* encontrar '\n'. Quando fgets() retorna NULL,    */
      /* ocorreu erro ou tentativa de leitura al�m do    */
      /* final do arquivo. Nesses casos, o la�o continua */
      /* efetuando novas tentativas de leitura.          */
   while ( fgets(ar, nElementos, stdin) == NULL )  {
      printf( "\a\n\t>>> %s", "Erro de leitura. "
              "Tente novamente\n\t> " );
   }

      /* Faz p apontar para o caractere que      */
      /* antecede o caractere terminal do string */
   p = strchr(ar, '\0') - 1;

      /* Se o caractere '\n' foi lido, remove-o do   */
      /* string. Caso contr�rio, remove-o do buffer. */
   if (*p == '\n') {
         /* Caractere '\n' faz parte do string */
      *p = '\0'; /* Sobrescreve '\n' com '\0' */
   } else { /* Usu�rio digitou caracteres demais */
      nCarDeixados = LimpaBuffer();
   }

      /* Retorna o n�mero de caracteres que o usu�rio */
      /* digitou al�m do esperado, sem incluir '\n'   */
   return nCarDeixados;
}

/****
 *
 * LeOpcao(): L�, via teclado, um caractere que deve fazer
 *            parte de um string para ser considerado v�lido
 *
 * Par�metros:
 *      opcoes (entrada) - string contendo os caracteres
 *                         v�lidos
 *
 * Retorno: Um caractere v�lido
 *
 * Observa��es:
 *      1. Esta fun��o deixa o buffer virgem novamente
 *      2. Esta fun��o � denominada 'LeOpcao' porque o
 *         caractere lido e validado � restrito a um
 *         conjunto de caracteres (op��es) representado
 *         pelo string recebido como par�metro
 *
 ****/
int LeOpcao(const char *opcoes)
{
   int op; /* Op��o (caractere) escolhida pelo usu�rio */

      /* Enquanto o usu�rio n�o escolher uma op��o */
      /* v�lida, o la�o a seguir n�o encerra       */
   while (1) {
         /* L� o caractere digitado */
      op = LeCaractere();

         /* Verifica se o caractere � v�lido */
      if (strchr(opcoes, op)) {
         break; /* � */
      } else { /* N�o � */
         printf( "\a\n\t>>> Opcao incorreta. "
                 "Tente novamente\n\t> " );
      }
   }

      /* Certamente, a op��o retornada � v�lida */
   return op;
}

/****
 *
 * LeNome(): L� um string via teclado e garante
 *           que ele s� cont�m letras e espa�os
 *           em branco
 *
 * Par�metros: nome (sa�da) - array que conter�
 *                            o string lido
 *             tam (entrada) - n�mero de elementos
 *                             do array
 *
 * Retorno: O n�mero de caracteres digitados al�m do limite
 *          permitido
 *
 * Observa��es:
 *      1. Caracteres digitados al�m do limite especificado
 *         ser�o ignorados
 *
 *      2. O �nico espa�o em branco permitido num nome � ' '
 *
 *      3. Esta fun��o n�o testa se o string digitado �
 *         constitu�do apenas por espa�os em branco
 *
 ****/
int LeNome(char *nome, int tam)
{
   int   remanescentes; /* N�mero de caracteres digitados */
                        /* al�m do permitido              */
   char *p;

   while (1) {
      printf( "\nDigite o nome (max = %d letras):\n\t> ",
              tam - 1 );
      remanescentes = LeString(nome, tam);

         /* Verifica se o string lido � vazio */
      if (!*nome) {
         printf("\a\n\t>> Um nome nao pode ser vazio <<\n");
         continue; /* Salta o restante do la�o */
      }

         /***************************************/
         /*                                     */
         /* Verifica se o string lido s� cont�m */
         /* letras e espa�os em branco          */
         /*                                     */
         /***************************************/

         /* O ponteiro p ser� usado para testar o string */
      p = nome;

         /* O la�o encerra quando p apontar para o */
         /* caractere terminal do string ou quando */
         /* for encontrado um caractere que nem �  */
         /* letra nem espa�o em branco             */
      while (*p) {
            /* Se for encontrado um caractere que */
            /* n�o � letra ou espa�o em branco    */
            /* encerra este la�o                  */
         if( !isalpha(*p) && *p != ' ' ) {
            break;
         }

         p++; /* Aponta para o pr�ximo caractere */
      }

         /* Se p apontar para o caractere terminal do */
         /* string, este foi aprovado no teste acima  */
      if (!*p) {
         break; /* String s� cont�m letras e espa�os */
      } else { /* String foi reprovado no teste */
         printf("\a\n\t>>> Um nome deve conter apenas <<<"
                "\n\t>>> letras e espacos em branco <<<\n");
      }
   }

   return remanescentes;
}

/****
 *
 * LeMatricula(): L� uma matr�cula ou similar via teclado
 *
 * Par�metros:
 *     nomeMatr (entrada) - nome do tipo de string que ser� lido
 *     matr[] (sa�da) - array que conter� a matr�cula lida
 *     tamArray (entrada) - n�mero de elementos do array
 *
 * Retorno: O endere�o do array apontado por matr
 *
 * Observa��o: Assume-se que o tamanho da matr�cula �
 *             igual ao tamanho do array matr[] que a
 *             armazenar� menos um.
 *
 ****/
char *LeMatricula( const char *nomeMatr, char *matr,
                   int tamArray )
{
   int   resto; /* Caracteres excedentes que */
                /* possam ter sido digitados */
   char *p;

      /* O la�o while encerra quando for */
      /* lida uma matricula v�lida       */
   while (1) {
         /* Apresenta prompt */
      printf( "\nDigite um(a) %s com exatamente %d "
              "digitos:\n\t> ", nomeMatr, tamArray - 1 );

         /* L� entrada do usu�rio */
      resto = LeString(matr, tamArray);

         /* Verifica se o usu�rio digitou */
         /* caracteres em excesso         */
      if (resto) {
         printf("\a\n\t>>> Excesso de caracteres <<<\n");
         continue; /* Faz nova tentativa */
      }

         /* Verifica se foram digitados menos caracteres */
         /* do que esperado. Note o uso do operador de   */
         /* convers�o expl�cita (int) para converter o   */
         /* valor retornado por strlen(), que � do tipo  */
         /* size_t, e, assim, evitar a compara��o de um  */
         /* valor sem sinal e outro com sinal.           */
      if ( (int)strlen(matr) != tamArray - 1 ) {
         printf("\a\n\t>>> Matricula incompleta <<<\n");
         continue; /* Faz nova tentativa */
      }

         /***************************************/
         /*                                     */
         /* Neste ponto, o tamanho da matr�cula */
         /* est� correto. Resta testar se ela   */
         /* s� cont�m d�gitos.                  */
         /*                                     */
         /***************************************/

         /* O ponteiro p ser� usado para testar o string */
      p = matr;

         /* O la�o encerra quando p apontar para o     */
         /* caractere terminal do string ou quando for */
         /* encontrado um caractere que n�o � d�gito   */
      while (*p) {
            /* Se for encontrado um caractere que */
            /* n�o � d�gito encerra este la�o     */
         if(!isdigit(*p)) {
            break; /* Encerra o la�o interno */
         }

         p++; /* Aponta para o pr�ximo caractere */
      }

         /* Se p apontar para o caractere terminal do */
         /* string, este foi aprovado no teste acima  */
      if (!*p) {
         break; /* Matr�cula v�lida */
      } else { /* Matr�cula inv�lida */
         printf("\a\n\t>>> \'%c\' nao e' digito <<<\n", *p);
      }
   }

   return matr;
}

/****
 *
 * LeNota(): L� um valor x do tipo double
 *           tal que: 0.0 <= x <= 10.0
 *
 * Par�metros: Nenhum
 *
 * Retorno: O valor lido
 *
 ****/
double LeNota(void)
{
   double valorLido;

      /* O la�o while encerra apenas quando */
      /* for lido um valor v�lido           */
   while (1) {

      valorLido = LeReal();

         /* Verifica se o valor lido est� */
         /* de acordo com o esperado      */
      if (valorLido >= 0.0 && valorLido <= 10.0) {
         break; /* O valor lido est� OK */
      } else {
         printf( "\n\t>>> Nota incorreta. "
                 "Tente novamente\n\t> " );
      }
   }

   return valorLido;
}

/****
 *
 * LeChaveLonga(): L� um valor x do tipo long long
 *
 * Par�metros: Nenhum
 *
 * Retorno: O valor lido
 *
 ****/
long long LeChaveLonga( const char *nomeChave,
                        int tamChave )
{
   char       *ar = malloc(tamChave + 1),
             *str, *final;
   long long  aChave;

   while (1) {
      str = LeMatricula( nomeChave, ar, tamChave + 1 );

      aChave = strtoll(str, &final, 10);

      if (!*final) {
         break;
      }

      printf("\n\a\t>>> Erro de conversao numerica. "
             "Tente novamente.");
   }

   free(ar);

   return aChave;
}