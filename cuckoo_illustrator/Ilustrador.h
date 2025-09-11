#ifndef _Ilustrador_H_
#define _Ilustrador_H_

#define NOME_VAR(x) #x

typedef enum {INT, DOUBLE, PONTEIRO, CHAR} tTipoVariavel;
typedef enum {AZUL, AMARELO, VERDE, VERMELHO, BRANCO, PRETO, LARANJA} tCor;

extern void IniciaIlustrador(const char *arquivoCodigo);
extern void IlustraAdicionaArquivo(const char *nomeArquivo);
extern int IlustraCriaVariavel(const void *v, const char *nome, tTipoVariavel tipo);
extern int IlustraCriaVariavelInteira(const int *v, const char *nome);
extern int IlustraAtualizaVariavel(const void *v);
extern int IlustraAtualizaVariavelInteira(const int *v);
extern int IlustraCriaArrayDeInteiros(int ar[], int t, const char *nome);
extern int IlustraDestacaElementosArray(int ar[], int inicio, int q, tCor umaCor);
extern int IlustraPermutaElementosArray(int ar[], int i, int j);
extern int IlustraMoveElementoArray(int ar[], int indiceOrigem, int indiceDestino);
extern int IlustraAtualizaElementosArray(int ar[], int indice);

extern int IlustraDestacaLinha(int linhaDestaque, int bloqueia);

extern int IlustraAlerta(const char *textoAlerta);
extern int IlustraSaida(const char *textoSaida);

extern int IlustraEscopoElemento(const void* endereco, int escopoAtual);
extern int IlustraRemoveMemoriaPorEndereco(void *endereco);
extern int IlustraRemoveMemoriaPorReferencia(int ref);

#endif