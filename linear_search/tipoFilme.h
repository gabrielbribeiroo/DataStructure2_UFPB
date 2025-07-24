#ifndef _TIPOFILME_H_
#define _TIPOFILME_H_

typedef struct rotFilme{
    int id;                 //chave primaria
    char *titulo;           //deve ser alocado 
    float avaliacaoMedia;
    int qVotos;
    } tFilme;

extern void ImprimeFilme(tFilme *filme);
extern void LiberaFilme(tFilme *filme);

#endif