/****
 *
 * T�tulo: LeituraFacil
 *
 * Autor: Ulysses de Oliveira
 *
 * Data de Cria��o: 15/07/2012
 * �ltima modifica��o: 15/07/2012
 *
 * Descri��o: Defini��o do m�dulo da biblioteca LeituraFacil
 *
 ****/

#ifndef _Leitura_H_
#define _Leitura_H_

extern int LeCaractere(void);
extern int LeInteiro(void);
extern int LeNatural(void);
extern int LeNaturalPositivo(void);
extern int LeInteiroEntre(int valor1, int valor2);
extern double LeReal(void);
extern int LeString(char *ar, int nElementos);
extern int LeOpcao(const char *opcoes);
extern int LeNome(char *nome, int tam);
extern char *LeMatricula( const char *nomeMatr, char *matr,
                          int tamArray );
extern double LeNota(void);

#endif
