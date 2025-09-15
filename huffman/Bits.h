/****
 *
 * Arquivo: Bits.h
 *
 * Autor: Ulysses de Oliveira
 *
 * Data de Criação: 06/10/2017
 * Última modificação: 06/10/2017
 *
 * Descrição: Interface do módulo de operações sobre bits para um
 *            array de 32 elementos do tipo unsigned char (256 bits)
 *
 ****/

#ifndef _Bits_H_
#define _Bits_H_

/******************* Alusões de Funções *******************/

extern int ConsultaBitEmArray(const unsigned char *bits, unsigned char bit);
extern void DeslocaBitsEsquerda(unsigned char *bits, int shifts);
extern void DeslocaBitsDireita(unsigned char *bits, int shifts);
extern int IncrementaBits(unsigned char *bits);

#endif
