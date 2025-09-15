/****
 *
 * Arquivo: Bits.c
 *
 * Autor: Ulysses de Oliveira
 *
 * Data de Cria��o: 06/10/2017
 * �ltima modifica��o: 10/10/2017
 *
 * Descri��o: Implementa��o do m�dulo de opera��es sobre bits
 *            para um array de 32 elementos do tipo unsigned char (256 bits)
 *
 ****/

/*********************** Includes *************************/

#include <limits.h>
#include <string.h>
#include "Tipos.h"
#include "Bits.h"

/***************** Defini��es de Fun��es ******************/

/****
 *
 * ConsultaBitEmArray(): Retorna o valor de um �nico bit de um array de 32
 *                       elementos do tipo unsigned char
 *
 * Par�metros:
 *     bits (entrada) - ponteiro para um array de 32 elementos
 *                      do tipo unsigned char
 *     bit (entrada) - �ndice do bit a ser testado
 *
 * Retorno: 0 se o bit testado � 0; 1, em caso contr�rio
 *
 * Observa��o: O MSB de bit[0] � bit 0 dos 256 bits. O LSB de
 *             bit[31] � bit 255 dos 256 bits
 *
 ****/
int ConsultaBitEmArray(const tByte *bits, tByte bit)
{
   int   byte;
   tByte mascara;

   byte = bit / CHAR_BIT; /* Byte desejado */
   bit = bit % CHAR_BIT;  /* Bit desejado em byte */

   mascara = (0x80 >> bit);

   return (bits[byte] & mascara) != 0;
}

/****
 *
 * DeslocaBitsEsquerda(): Desloca � esquerda os bits de um array de 32
 *                    bytes pela quantia de posi��es especificada
 *
 * Par�metros:
 *     bits (entrada e sa�da) - ponteiro para um array de 32 bytes
 *                              (unsigned char) bits[31] � o byte
 *                              mais � direita
 *     desloca (entrada) - n�mero de deslocamentos
 *
 * Retorno: Nada
 *
 ****/
void DeslocaBitsEsquerda(tByte *bits, int desloca)
{
   int          i;
   unsigned int excedente;
   int          bytes;

      /* N�mero de bytes inteiros deslocados */
   bytes = desloca / CHAR_BIT;

      /* N�mero de bits que restam para delocar */
   desloca = desloca % CHAR_BIT;

      /* Primeiro lida com big jumps de bytes */
   if (bytes > 0) {
      for (i = 0; (i + bytes) < MAX_BYTES; i++)
         bits[i] = bits[i + bytes];

         /* Agora zera os novos bytes � direita */
      for (i = MAX_BYTES; bytes > 0; bytes--)
         bits[i - bytes] = 0;
   }

      /* Agora lida com os delocamentos restantes (menos de 8 bits) */
   if (desloca > 0) {
      bits[0] <<= desloca;

      for (i = 1; i < MAX_BYTES; i++) {
         excedente = bits[i];
         excedente <<= desloca;
         bits[i] = (tByte)excedente;

            /* Lida com delocamentos al�m dos limites dos bytes */
         if (excedente & 0xFF00)
            bits[i - 1] |= (tByte)(excedente >> 8);
      }
   }
}

/****
 *
 * DeslocaBitsDireita(): Desloca � direita os bits de um array de 32
 *                   bytes pelo n�mero de posi��es especificado
 *
 * Par�metros:
 *     bits (entrada e sa�da) - ponteiro para array of 32 bytes
 *                              (unsigned char) bits[31] � o byte mais � direita
 *     desloca (entrada) - n�mero de deslocamentos
 *
 * Retorno: Nada
 *
 ****/
void DeslocaBitsDireita(tByte *bits, int desloca)
{
   int          i;
   unsigned int excedente;
   int          bytes;

      /* N�mero de bytes inteiros deslocados */
   bytes = desloca / CHAR_BIT;

      /* N�mero de bits que restam para delocar */
   desloca = desloca % CHAR_BIT;

      /* Primeiro lida com big jumps de bytes */
   if (bytes > 0) {
      for (i = MAX_BYTES - 1; (i - bytes) >= 0; i--)
         bits[i] = bits[i - bytes];

         /* Agora zero out novos bytes na direita */
      for (; bytes > 0; bytes--)
         bits[bytes - 1] = 0;
   }

      /* Agora lida com os delocamentos restantes (menos de 8 bits) */
   if (desloca > 0) {
      bits[MAX_BYTES - 1] >>= desloca;

      for (i = MAX_BYTES - 2; i >= 0; i--) {
         excedente = bits[i];
         excedente <<= (CHAR_BIT - desloca);
         bits[i] = (tByte)(excedente >> CHAR_BIT);

            /* Lida com delocamentos al�m dos limites dos bytes */
         if (excedente & 0xFF)
            bits[i + 1] |= (tByte)excedente;
      }
   }
}

/****
 *
 * IncrementaBits(): Incrementa um array de 32 elementos do tipo
 *                   unsigned char como se ele fosse um �nico valor
 *                   unsigned de 256 bit
 *
 * Par�metros:
 *     bits (entrada e sa�da) - ponteiro para array of 32 elementos
 *                              do tipo unsigned char (256 bits)
 *
 * Retorno: 1 se ocorrer excedente; 0, em caso contr�rio
 *
 ****/
int IncrementaBits(tByte *bits)
{
   int i;

   for (i = MAX_BYTES - 1; i >= 0; i--) {
      if (bits[i] != 0xFF) {
         bits[i] = bits[i] + 1;
         break;
      } else {
            /* Vai um pr�ximo byte */
         bits[i] = 0;

         if (i == 0)
               /* Ocorreu overflow */
            return 1;
      }
   }

   return 0;
}

