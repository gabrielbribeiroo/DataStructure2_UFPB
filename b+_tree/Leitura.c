/****
 *
 * Título: LeituraFacil
 *
 * Autor: Ulysses de Oliveira
 *
 * Data de Criação: 15/07/2012
 * Última modificação: 10/10/2013
 *
 * Descrição: Implementação da biblioteca LeituraFacil
 *
 ****/

/************************ Includes ************************/

#include <stdio.h>   /* Entrada e saída             */
#include <string.h>  /* Processamento de strings    */
#include <stdlib.h>  /* maloc()                     */
#include <ctype.h>   /* Classificação de caracteres */
#include "Leitura.h" /* Interface da biblioteca     */
                     /* LeituraFacil                */

/******************** Funções de Locais *******************/

/****
 *
 * LimpaBuffer(): Limpa o buffer associado ao meio de
 *                entrada padrão (stdin)
 *
 * Parâmetros: Nenhum
 *
 * Retorno: O número de caracteres descartados, com
 *          exceção do caractere '\n'.
 *
 ****/
static int LimpaBuffer(void)
{
   int carLido, /* Armazena cada caractere lido */
       nCarLidos = 0; /* Conta o número de */
                      /* caracteres lidos  */

      /* Lê e descarta cada caractere lido até    */
      /* encontrar '\n' ou getchar() retornar EOF */
   do {
      carLido = getchar(); /* Lê um caractere */
      ++nCarLidos; /* Mais um caractere foi lido */
   } while ((carLido != '\n') && (carLido != EOF));

      /* O último caractere lido foi '\n' ou  */
      /* EOF e não deve ser considerado sobra */
   return nCarLidos - 1;
}

/******************** Funções de Globais ******************/

/****
 *
 * LeCaractere(): Lê um caractere via teclado e remove os
 *                caracteres remanescentes no buffer
 *
 * Parâmetros: Nenhum
 *
 * Retorno: O caractere lido
 *
 * Observação: Esta função deixa o buffer limpo
 *
 ****/
int LeCaractere(void)
{
   int c, /* Armazenará cada caractere lido */
       nResto = 0; /* Número de caracteres */
                   /* excedentes           */

   /* Volta para cá se ocorrer erro de leitura */
inicio:
   c = getchar(); /* Lê o caractere digitado */

      /* Verifica se getchar() retornou EOF */
   if (c == EOF) {
      printf( "\a\n\t>>>Erro de leitura. "
              "Tente novamente\n\t> " );
      goto inicio; /* Não faz mal algum */
   }

      /* Pelo menos '\n' se encontra no buffer */
   nResto = LimpaBuffer();

      /* Repreende o usuário se ele digitou demais */
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
 * LeInteiro(): Lê um número inteiro via teclado
 *              e deixa o buffer intacto
 *
 * Parâmetros: Nenhum
 *
 * Retorno: O número inteiro lido
 *
 ****/
int LeInteiro(void)
{
   int num, /* O número lido */
       teste, /* Valor retornado por scanf() */
       nResto = 0; /* Número de caracteres */
                   /* excedentes           */

   /* Desvia para cá se o valor for inválido */
inicio:
      /* Tenta ler um valor válido */
   teste = scanf("%d", &num);

      /* Se não ocorreu erro de leitura ou de final */
      /* de arquivo, há caracteres remanescentes    */
      /* que precisam ser removidos                 */
   if (teste != EOF) {
      nResto = LimpaBuffer();
   }

      /* Enquanto o valor retornado por scanf()  */
      /* não indicar que um valor válido foi     */
      /* lido continua tentando obter esse valor */
   while(teste != 1) {
      printf("\a\n\t>>> O valor digitado e' invalido. "
                      "Tente novamente\n\t> " );
      goto inicio; /* Não causa dano algum */
   }

      /* Repreende o usuário se ele digitou demais */
   if (nResto == 1) {
      printf("\t>>> Um caractere foi descartado\n");
   } else if (nResto > 1) {
      printf("\t>>> %d caracteres foram descartados\n",
             nResto);
   }

      /* O valor retornado certamente é válido */
   return num;
}

/****
 *
 * LeInteiroEntre(): Lê um número inteiro que se encontra
 *                   entre dois valores via teclado
 *                   e deixa o buffer intacto
 *
 * Parâmetros: Nenhum
 *
 * Retorno: O número inteiro lido
 *
 ****/
int LeInteiroEntre(int valor1, int valor2)
{
   int num, /* O número lido */
       menorValor = valor1 <= valor2 ? valor1 : valor2,
       maiorValor = valor1 >= valor2 ? valor1 : valor2,
       teste;

   /* Desvia para cá se o valor for inválido */
inicio:
      /* Tenta ler um valor válido */
   num = LeInteiro();

   teste = num >= menorValor && num <= maiorValor;

      /* Enquanto o valor lido não for válido */
      /* continua tentando obter esse valor   */
   while(!teste) {
      printf("\a\n\t>>> O valor digitado e' invalido. "
                      "Tente novamente\n\t> " );
      goto inicio; /* Não causa dano algum */
   }

      /* O valor retornado certamente é válido */
   return num;
}

/****
 *
 * LeNatural(): Lê um número natural via teclado
 *              e deixa o buffer intacto
 *
 * Parâmetros: Nenhum
 *
 * Retorno: O número natural lido
 *
 ****/
int LeNatural(void)
{
   int num, /* O número lido */
       teste, /* Valor retornado por scanf() */
       nResto = 0; /* Número de caracteres */
                   /* excedentes           */

   /* Desvia para cá se o valor for inválido */
inicio:
      /* Tenta ler um valor válido */
   teste = scanf("%d", &num);

      /* Se não ocorreu erro de leitura ou de final */
      /* de arquivo, há caracteres remanescentes    */
      /* que precisam ser removidos                 */
   if (teste != EOF) {
      nResto = LimpaBuffer();
   }

      /* Enquanto o valor retornado por scanf()  */
      /* não indicar que um valor válido foi     */
      /* lido continua tentando obter esse valor */
   while(teste != 1 || num < 0) {
      printf("\a\n\t>>> O valor digitado e' invalido. "
                      "Tente novamente\n\t> " );
      goto inicio; /* Não causa dano algum */
   }

      /* Repreende o usuário se ele digitou demais */
   if (nResto == 1) {
      printf("\t>>> Um caractere foi descartado\n");
   } else if (nResto > 1) {
      printf( "\t>>> %d caracteres foram descartados\n",
              nResto );
   }

      /* O valor retornado certamente é válido */
   return num;
}

/****
 *
 * LeNaturalPositivo(): Lê um número natural positivo via
 *                      teclado e deixa o buffer intacto
 *
 * Parâmetros: Nenhum
 *
 * Retorno: O número natural lido
 *
 ****/
int LeNaturalPositivo(void)
{
   int num, /* O número lido */
       teste, /* Valor retornado por scanf() */
       nResto = 0; /* Número de caracteres */
                   /* excedentes           */

   /* Desvia para cá se o valor for inválido */
inicio:
      /* Tenta ler um valor válido */
   teste = scanf("%d", &num);

      /* Se não ocorreu erro de leitura ou de final */
      /* de arquivo, há caracteres remanescentes    */
      /* que precisam ser removidos                 */
   if (teste != EOF) {
      nResto = LimpaBuffer();
   }

      /* Enquanto o valor retornado por scanf()  */
      /* não indicar que um valor válido foi     */
      /* lido continua tentando obter esse valor */
   while(teste != 1 || num < 1) {
      printf("\a\n\t>>> O valor digitado e' invalido. "
                      "Tente novamente\n\t> " );
      goto inicio; /* Não causa dano algum */
   }

      /* Repreende o usuário se ele digitou demais */
   if (nResto == 1) {
      printf("\t>>> Um caractere foi descartado\n");
   } else if (nResto > 1) {
      printf( "\t>>> %d caracteres foram descartados\n",
              nResto );
   }

      /* O valor retornado certamente é válido */
   return num;
}

/****
 *
 * LeReal(): Lê um valor do tipo double via teclado
 *           deixando o buffer associado ao teclado
 *           sem caracteres remanescentes
 *
 * Parâmetros: Nenhum
 *
 * Retorno: O valor lido e validado
 *
 ****/
double LeReal(void)
{
   int    teste, /* Armazena o retorno de scanf() */
          nResto = 0; /* Número de caracteres */
                      /* excedentes           */
   double valorLido; /* O valor digitado pelo usuário */

      /* O laço encerra quando o usuário se comporta bem */
   while (1) {
         /* Tenta ler um valor do tipo double */
      teste = scanf("%lf", &valorLido);

         /* Se não ocorreu erro de leitura ou de final */
         /* de arquivo, há caracteres remanescentes    */
         /* que precisam ser removidos                 */
      if (teste != EOF) {
         nResto = LimpaBuffer();
      }

         /* Se o valor retornado por scanf() */
         /* foi 1, a leitura foi OK          */
      if (teste == 1) {
         break; /* Leitura foi nota 10 */
      } else { /* Usuário foi mal comportado */
         printf( "\a\n\t>>> O valor digitado e' invalido. "
                 "Tente novamente\n\t> " );
      }
   }

      /* Repreende o usuário se ele digitou demais */
   if (nResto == 1) {
      printf("\t>>> Um caractere foi descartado\n");
   } else if (nResto > 1) {
      printf("\t>>> %d caracteres foram descartados\n",
             nResto);
   }

      /* Retorna um valor válido do tipo double */
   return valorLido;
}

/****
 *
 * LeString(): Lê um string via teclado deixando o buffer
 *             zerado
 *
 * Parâmetros:
 *      ar[] (saída): array que armazenará o string lido
 *      nElementos (entrada): número máximo de elementos que
 *                            serão armazenados no array,
 *                            incluindo '\0'
 *
 * Retorno: O número de caracteres que o usuário digitou a
 *          mais, sem incluir '\n'
 *
 ****/
int LeString(char *ar, int nElementos)
{
   char *p; /* Usado para remover '\n' do string */
   int   nCarDeixados = 0; /* Conta o número de     */
                           /* caracteres excedentes */
                           /* no buffer             */

      /* Lê no máximo nElementos - 1 caracteres ou até   */
      /* encontrar '\n'. Quando fgets() retorna NULL,    */
      /* ocorreu erro ou tentativa de leitura além do    */
      /* final do arquivo. Nesses casos, o laço continua */
      /* efetuando novas tentativas de leitura.          */
   while ( fgets(ar, nElementos, stdin) == NULL )  {
      printf( "\a\n\t>>> %s", "Erro de leitura. "
              "Tente novamente\n\t> " );
   }

      /* Faz p apontar para o caractere que      */
      /* antecede o caractere terminal do string */
   p = strchr(ar, '\0') - 1;

      /* Se o caractere '\n' foi lido, remove-o do   */
      /* string. Caso contrário, remove-o do buffer. */
   if (*p == '\n') {
         /* Caractere '\n' faz parte do string */
      *p = '\0'; /* Sobrescreve '\n' com '\0' */
   } else { /* Usuário digitou caracteres demais */
      nCarDeixados = LimpaBuffer();
   }

      /* Retorna o número de caracteres que o usuário */
      /* digitou além do esperado, sem incluir '\n'   */
   return nCarDeixados;
}

/****
 *
 * LeOpcao(): Lê, via teclado, um caractere que deve fazer
 *            parte de um string para ser considerado válido
 *
 * Parâmetros:
 *      opcoes (entrada) - string contendo os caracteres
 *                         válidos
 *
 * Retorno: Um caractere válido
 *
 * Observações:
 *      1. Esta função deixa o buffer virgem novamente
 *      2. Esta função é denominada 'LeOpcao' porque o
 *         caractere lido e validado é restrito a um
 *         conjunto de caracteres (opções) representado
 *         pelo string recebido como parâmetro
 *
 ****/
int LeOpcao(const char *opcoes)
{
   int op; /* Opção (caractere) escolhida pelo usuário */

      /* Enquanto o usuário não escolher uma opção */
      /* válida, o laço a seguir não encerra       */
   while (1) {
         /* Lê o caractere digitado */
      op = LeCaractere();

         /* Verifica se o caractere é válido */
      if (strchr(opcoes, op)) {
         break; /* É */
      } else { /* Não é */
         printf( "\a\n\t>>> Opcao incorreta. "
                 "Tente novamente\n\t> " );
      }
   }

      /* Certamente, a opção retornada é válida */
   return op;
}

/****
 *
 * LeNome(): Lê um string via teclado e garante
 *           que ele só contém letras e espaços
 *           em branco
 *
 * Parâmetros: nome (saída) - array que conterá
 *                            o string lido
 *             tam (entrada) - número de elementos
 *                             do array
 *
 * Retorno: O número de caracteres digitados além do limite
 *          permitido
 *
 * Observações:
 *      1. Caracteres digitados além do limite especificado
 *         serão ignorados
 *
 *      2. O único espaço em branco permitido num nome é ' '
 *
 *      3. Esta função não testa se o string digitado é
 *         constituído apenas por espaços em branco
 *
 ****/
int LeNome(char *nome, int tam)
{
   int   remanescentes; /* Número de caracteres digitados */
                        /* além do permitido              */
   char *p;

   while (1) {
      printf( "\nDigite o nome (max = %d letras):\n\t> ",
              tam - 1 );
      remanescentes = LeString(nome, tam);

         /* Verifica se o string lido é vazio */
      if (!*nome) {
         printf("\a\n\t>> Um nome nao pode ser vazio <<\n");
         continue; /* Salta o restante do laço */
      }

         /***************************************/
         /*                                     */
         /* Verifica se o string lido só contém */
         /* letras e espaços em branco          */
         /*                                     */
         /***************************************/

         /* O ponteiro p será usado para testar o string */
      p = nome;

         /* O laço encerra quando p apontar para o */
         /* caractere terminal do string ou quando */
         /* for encontrado um caractere que nem é  */
         /* letra nem espaço em branco             */
      while (*p) {
            /* Se for encontrado um caractere que */
            /* não é letra ou espaço em branco    */
            /* encerra este laço                  */
         if( !isalpha(*p) && *p != ' ' ) {
            break;
         }

         p++; /* Aponta para o próximo caractere */
      }

         /* Se p apontar para o caractere terminal do */
         /* string, este foi aprovado no teste acima  */
      if (!*p) {
         break; /* String só contém letras e espaços */
      } else { /* String foi reprovado no teste */
         printf("\a\n\t>>> Um nome deve conter apenas <<<"
                "\n\t>>> letras e espacos em branco <<<\n");
      }
   }

   return remanescentes;
}

/****
 *
 * LeMatricula(): Lê uma matrícula ou similar via teclado
 *
 * Parâmetros:
 *     nomeMatr (entrada) - nome do tipo de string que será lido
 *     matr[] (saída) - array que conterá a matrícula lida
 *     tamArray (entrada) - número de elementos do array
 *
 * Retorno: O endereço do array apontado por matr
 *
 * Observação: Assume-se que o tamanho da matrícula é
 *             igual ao tamanho do array matr[] que a
 *             armazenará menos um.
 *
 ****/
char *LeMatricula( const char *nomeMatr, char *matr,
                   int tamArray )
{
   int   resto; /* Caracteres excedentes que */
                /* possam ter sido digitados */
   char *p;

      /* O laço while encerra quando for */
      /* lida uma matricula válida       */
   while (1) {
         /* Apresenta prompt */
      printf( "\nDigite um(a) %s com exatamente %d "
              "digitos:\n\t> ", nomeMatr, tamArray - 1 );

         /* Lê entrada do usuário */
      resto = LeString(matr, tamArray);

         /* Verifica se o usuário digitou */
         /* caracteres em excesso         */
      if (resto) {
         printf("\a\n\t>>> Excesso de caracteres <<<\n");
         continue; /* Faz nova tentativa */
      }

         /* Verifica se foram digitados menos caracteres */
         /* do que esperado. Note o uso do operador de   */
         /* conversão explícita (int) para converter o   */
         /* valor retornado por strlen(), que é do tipo  */
         /* size_t, e, assim, evitar a comparação de um  */
         /* valor sem sinal e outro com sinal.           */
      if ( (int)strlen(matr) != tamArray - 1 ) {
         printf("\a\n\t>>> Matricula incompleta <<<\n");
         continue; /* Faz nova tentativa */
      }

         /***************************************/
         /*                                     */
         /* Neste ponto, o tamanho da matrícula */
         /* está correto. Resta testar se ela   */
         /* só contém dígitos.                  */
         /*                                     */
         /***************************************/

         /* O ponteiro p será usado para testar o string */
      p = matr;

         /* O laço encerra quando p apontar para o     */
         /* caractere terminal do string ou quando for */
         /* encontrado um caractere que não é dígito   */
      while (*p) {
            /* Se for encontrado um caractere que */
            /* não é dígito encerra este laço     */
         if(!isdigit(*p)) {
            break; /* Encerra o laço interno */
         }

         p++; /* Aponta para o próximo caractere */
      }

         /* Se p apontar para o caractere terminal do */
         /* string, este foi aprovado no teste acima  */
      if (!*p) {
         break; /* Matrícula válida */
      } else { /* Matrícula inválida */
         printf("\a\n\t>>> \'%c\' nao e' digito <<<\n", *p);
      }
   }

   return matr;
}

/****
 *
 * LeNota(): Lê um valor x do tipo double
 *           tal que: 0.0 <= x <= 10.0
 *
 * Parâmetros: Nenhum
 *
 * Retorno: O valor lido
 *
 ****/
double LeNota(void)
{
   double valorLido;

      /* O laço while encerra apenas quando */
      /* for lido um valor válido           */
   while (1) {

      valorLido = LeReal();

         /* Verifica se o valor lido está */
         /* de acordo com o esperado      */
      if (valorLido >= 0.0 && valorLido <= 10.0) {
         break; /* O valor lido está OK */
      } else {
         printf( "\n\t>>> Nota incorreta. "
                 "Tente novamente\n\t> " );
      }
   }

   return valorLido;
}

/****
 *
 * LeChave(): Lê um valor x do tipo int
 *
 * Parâmetros: Nenhum
 *
 * Retorno: O valor lido
 *
 ****/
int LeChave( const char *nomeChave, int tamChave )
{
   char *ar = malloc(tamChave + 1),
        *str, *final;
   int   aChave;

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

/****
 *
 * LeChaveLonga(): Lê um valor x do tipo long long
 *
 * Parâmetros: Nenhum
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

/****
 *
 * LeSimNao(): Lê uma resposta do usuário que corresponde a sim
 *             ('s') ou não ('n')
 *
 * Parâmetros: prompt (entrada) - O prompt a ser apresentado
 *
 * Retorno: 's' ou 'n'
 *
 ****/
int LeSimNao(const char *prompt)
{
   int op; /* Opção do usuário */

   printf("\n\t>>> %s (s/n)? ", prompt);

   op = LeOpcao("sSnN");

   return op == 'n' || op == 'N' ? 'n' : 's';
}
