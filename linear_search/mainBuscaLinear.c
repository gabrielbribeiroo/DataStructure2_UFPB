#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "tabelaIndexada.h"
#include "tabelaEncadeada.h"
#include "listaComSaltos.h"

#define MAX_LINHA 5000

typedef int BuscaMovimentacaoIdx(tTabelaIdx *tab, int chave);
typedef tDado * BuscaMovimentacaoEnc(tTabelaEnc **tab, int chave);


void RemoveQuebraLinha(char *str){
    if (str[strlen(str) - 1] == '\n')
        str[strlen(str) - 1] = '\0';
}

int CarregaArquivo(const char *arq, tTabelaIdx *tabIdx, tTabelaEnc **tabEnc, tListaComSaltos *listaSaltos){
    FILE *fp;
    char linha[MAX_LINHA+1], *id, *titulo, *avaliacao, *votos, *virg, *aspas;
    int cont;
    tFilme filme;

    fp = fopen(arq, "r");
    if (!fp){
        puts("Erro ao abrir arquivo.");
        exit(1);
    }

    cont = 0;
    while(1){
        fgets(linha, MAX_LINHA+1, fp);
        if (feof(fp)){
            break;
        }
        RemoveQuebraLinha(linha);
        virg = strchr(linha, ',');
        if (virg){
            virg[-1] = '\0';
            id = linha+1;
            
            titulo = virg + 2;
            aspas = strchr(titulo, '\"');
            *aspas = '\0';
            filme.id = atoi(id);
            filme.titulo = malloc(sizeof(char) * strlen(titulo) + 1);
            if (!filme.titulo){
                printf("Erro ao alocar titulo do filme #%d\n", cont);
            }
            strcpy(filme.titulo, titulo);
            
            avaliacao = aspas + 3;
            aspas = strchr(avaliacao, '\"');
            *aspas = '\0';
            filme.avaliacaoMedia = strtod(avaliacao, NULL);

            votos = aspas + 3;
            aspas = strchr(votos, '\"');
            *aspas = '\0';
            filme.qVotos = strtod(votos, NULL);

            cont++;
            //printf("Filme: %s [%s] (id=%s, votos=%s)\n", titulo, avaliacao, id, votos);
            AcrescentaElementoIdx(tabIdx, &filme);
            AcrescentaElementoEnc(tabEnc, &filme);
            InsereListaComSaltos(listaSaltos, filme);
        }
    }

    fclose(fp);
    return cont;
}

int main(void){
    tTabelaIdx *tabIdx;
    tTabelaEnc *tabEnc;
    tListaComSaltos *listaSaltos;
    int q, id;
    clock_t inicio, fim;
    double tempoDeCpu;

    tabIdx = CriaTabelaIdx();
    CriaTabelaEnc(&tabEnc);
    IniciaListaComSaltos(&listaSaltos, 1150000);
    inicio = clock();
    q = CarregaArquivo("../../TMDB_movie_dataset_v11.csv", tabIdx, &tabEnc, listaSaltos);
    fim = clock();
    tempoDeCpu = ((double) (fim - inicio)) / CLOCKS_PER_SEC;

    printf("Iniciado:\n");
    printf("\t%d filmes carregados.\n", q);
    printf("\tTamanho tabela indexada: %d\n", ComprimentoIdx(tabIdx));
    printf("\tTamanho tabela encadeada: %d\n", ComprimentoEnc(tabEnc));
    printf("\tTamanho lista com saltos: %d\n", ComprimentoListaComSaltos(listaSaltos));
    ImprimeEstado(listaSaltos);
    printf("\tTempo de CPU: %lf\n", tempoDeCpu);
    inicio = clock();
    Ordena(tabIdx);
    fim = clock();
    tempoDeCpu = ((double) (fim - inicio)) / CLOCKS_PER_SEC;
    printf("\tTempo de CPU ordenacao: %lf\n", tempoDeCpu);

    //BuscaMovimentacaoIdx *tipoBuscaIdx = BuscaTransposicao;
    BuscaMovimentacaoIdx *tipoBuscaIdx = BuscaMovimentacaoInicio;
    //BuscaMovimentacaoEnc *tipoBuscaEnc = BuscaTransposicaoEnc;
    BuscaMovimentacaoEnc *tipoBuscaEnc = BuscaMovimentacaoInicioEnc;

    while(1){
        printf("\nDigite o ID do filme para buscar: ");
        scanf("%d", &id);

        printf("Lista Indexada:\n");
        inicio = clock();
        int ind = BuscaSequencial(tabIdx, id);
        fim = clock();
        tempoDeCpu = ((double) (fim - inicio)) / CLOCKS_PER_SEC;
        printf("\tOperacoes busca sequencial: %d\n", QuantOperacoes());
        printf("\tTempo de CPU: %lf\n", tempoDeCpu);
        
        /*
        inicio = clock();
        ind = tipoBuscaIdx(tabIdx, mat);
        fim = clock();
        tempoDeCpu = ((double) (fim - inicio)) / CLOCKS_PER_SEC;
        printf("\tOperacoes busca com movimentacao: %d\n", QuantOperacoes());
        printf("\tTempo de CPU: %lf\n", tempoDeCpu);
        */

        inicio = clock();
        ind = BuscaBinaria(tabIdx, id);
        fim = clock();
        tempoDeCpu = ((double) (fim - inicio)) / CLOCKS_PER_SEC;
        printf("\tOperacoes busca binaria: %d\n", QuantOperacoes());
        printf("\tTempo de CPU: %lf\n", tempoDeCpu);

        inicio = clock();
        ind = BuscaInterpolacao(tabIdx, id);
        fim = clock();
        tempoDeCpu = ((double) (fim - inicio)) / CLOCKS_PER_SEC;
        printf("\tOperacoes busca com interpolacao: %d\n", QuantOperacoes());
        printf("\tTempo de CPU: %lf\n", tempoDeCpu);

        if (ind < 0){
            puts("\tfilme nao encontrado");
        }else{
            tFilme filme = ObtemElementoIdx(tabIdx, ind);
            ImprimeFilme(&filme);
        }

        printf("\nLista Enacadeada:\n");
        inicio = clock();
        tFilme * filme = BuscaSequencialEnc(tabEnc, id);
        fim = clock();
        tempoDeCpu = ((double) (fim - inicio)) / CLOCKS_PER_SEC;
        printf("\tOperacoes busca encadeada: %d\n", QuantOperacoesEnc());
        printf("\tTempo de CPU: %lf\n", tempoDeCpu);
        
        inicio = clock();
        filme = tipoBuscaEnc(&tabEnc, id);
        fim = clock();
        tempoDeCpu = ((double) (fim - inicio)) / CLOCKS_PER_SEC;
        printf("\tOperacoes busca com movimentacao: %d\n", QuantOperacoesEnc());
        printf("\tTempo de CPU: %lf\n", tempoDeCpu);

        if (!filme){
            puts("\tfilme nao encontrado");
        }else{
            ImprimeFilme(filme);
        }

        printf("\nLista com Saltos:\n");
        if (!filme){
            filme = malloc(sizeof(tFilme));
            if (!filme){
                puts("Erro ao alocar filme para buscar.");
                exit(1);
            }
        }
        inicio = clock();
        *filme = BuscaListaComSaltos(listaSaltos, id);
        fim = clock();
        tempoDeCpu = ((double) (fim - inicio)) / CLOCKS_PER_SEC;
        printf("\tOperacoes busca lista com saltos: %d\n", QuantOperacoesSaltos());
        printf("\tTempo de CPU: %lf\n", tempoDeCpu);
        
        if (filme->id < 0){
            puts("\tfilme nao encontrado");
        }else{
            ImprimeFilme(filme);
        }

   }

    //DestroiTabelaEnc(tabEnc);
    DestroiTabelaIdx(tabIdx);
    return 0;
}