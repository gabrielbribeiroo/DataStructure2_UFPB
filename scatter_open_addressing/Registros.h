#ifndef _Registros_H_
#define _Registros_H_

#include "Tipos.h"   /* Tipos usados pelo programa */

/******************* Alusões de Funções *******************/

extern void ExibeRegistro(const tRegistroCEP *registro);
extern tRegistroCEP *LeRegistro(tRegistroCEP *regCEP);
extern tRegistroCEP *SubstituiRegistro( tRegistroCEP *novo,
                                     const tRegistroCEP *antigo );
extern int TamanhoDeArquivo(FILE *stream);
extern int NumeroDeRegistros( FILE *stream, int tamRegistro );

#endif
