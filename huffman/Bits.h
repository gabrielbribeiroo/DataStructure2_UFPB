/****
 *
 * Arquivo: Bits.h
 *
 * Autor: Ulysses de Oliveira
 *
 * Data de Cria��o: 06/10/2017
 * �ltima modifica��o: 06/10/2017
 *
 * Descri��o: Interface do m�dulo de opera��es sobre bits para um
 *            array de 32 elementos do tipo unsigned char (256 bits)
 *
 ****/

#ifndef _Bits_H_
#define _Bits_H_

/******************* Alus�es de Fun��es *******************/

extern int ConsultaBitEmArray(const unsigned char *bits, unsigned char bit);
extern void DeslocaBitsEsquerda(unsigned char *bits, int shifts);
extern void DeslocaBitsDireita(unsigned char *bits, int shifts);
extern int IncrementaBits(unsigned char *bits);

#endif
