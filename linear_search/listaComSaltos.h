#ifndef _LISTACOMSALTOS_H_
#define _LISTACOMSALTOS_H_

#include "tipoFilme.h"

typedef struct rotNoListaComSaltos {
        tFilme conteudo;
        struct rotNoListaComSaltos **proximo; //array de proximos 
    } tNoListaComSalto;

typedef struct rotListaSaltos tListaComSaltos;


extern void IniciaListaComSaltos(tListaComSaltos **lista, int maxRegs);
extern void DestroiListaComSaltos(tListaComSaltos *lista);
extern int ComprimentoListaComSaltos(tListaComSaltos *lista);
extern int EstaVaziaListaComSaltos(tListaComSaltos *lista);

extern tFilme BuscaListaComSaltos(tListaComSaltos *lista, int chave);
extern tNoListaComSalto* InsereListaComSaltos(tListaComSaltos *lista, tFilme filme);
extern int RemoveListaComSaltos(tListaComSaltos *lista, int chave);

extern void ImprimeEstado(tListaComSaltos *lista);
extern int QuantOperacoesSaltos();
#endif