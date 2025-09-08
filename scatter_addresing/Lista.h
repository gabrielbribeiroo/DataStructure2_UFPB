#ifndef _Lista_H_
#define _Lista_H_

#include "Tipos.h" /* Tipos usados pelo programa */

/****************** Definições de Tipos *******************/


/************************* Alusões ************************/

extern void IniciaListaSE(tListaSE *lista);
extern int ComprimentoListaSE(tListaSE lista);
extern void InsereListaSE( tListaSE *lista, const tCEP_Ind *conteudo );
extern int RemoveListaSE(tListaSE *lista, tCEP chave);
extern int EstaVaziaListaSE(tListaSE lista);
extern int BuscaListaSE( tListaSE *lista, tCEP chave );
extern void DestroiListaSE(tListaSE *lista);
extern tCEP_Ind *ProximoListaSE(tListaSE lista);

#endif
