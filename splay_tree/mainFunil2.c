#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include "ArvoreFunil.h"

#define MAX_LINHA 2048
#define NUM_CHAVES 800
#define NUM_SORTEIOS 30000

const char *diretorioBase = "../../../../turma 2024.2/compartilhamentos/wikiart";;

//trim
void RemoveQuebraLinha(char *str){
    if (str[strlen(str) - 1] == '\n')
        str[strlen(str) - 1] = '\0';
}

//primeira letra maiuscula 
void NormalizaString(char *str){
    if ( (str[0] = isalpha(str[0]) ? toupper(str[0]) : str[0]) == '\0' ){
        return;
    }

    for(int i = 1; str[i] != '\0'; i++){
        if (str[i-1] == ' ' && isalpha(str[i])){
            str[i] = toupper(str[i]);
        }
    }
}

//altera char
void TrocaChars(char *str, char procurado, char substituto){
    for(int i = 0; str[i] != '\0'; i++){
        if (str[i] == procurado){
            str[i] = substituto;
        }
    }
}

int CarregaArquivo(const char *arq,  tNoFunil **arvoreFunil){
    FILE *fp;
    char linha[MAX_LINHA+1], *arquivo, *artista, *genero, *descricao, *largura, *altura, *virg, *aspas;
    int cont;

    fp = fopen(arq, "r");
    if (!fp){
        printf("Erro ao abrir arquivo \"%s\".", arq);
        exit(1);
    }

    cont = 0;
    fgets(linha, MAX_LINHA+1, fp); //ignorar primeira linha
    while(1){
        fgets(linha, MAX_LINHA+1, fp);
        RemoveQuebraLinha(linha);
        if (feof(fp)){
            break;
        }
        virg = strchr(linha, ',');
        if (virg){
            *virg = '\0';
            arquivo = linha;
            
            artista = virg + 1;
            virg = strchr(artista, ',');
            *virg = '\0';
            NormalizaString(artista);
            
            genero = virg + 1;
            if (*genero == '\"'){
                virg = strchr(genero+1, '\"') - 2;
                *virg = '\0';    
                virg += 2;
            }else{
                virg = strchr(genero, ',') - 1;
                *virg = '\0';    
                virg += 1;
            }
            genero += 1;

            descricao = virg + 1;
            virg = strchr(descricao, ',');
            *virg = '\0';
            //printf("%s/", descricao);
            TrocaChars(descricao, '-', ' ');
            //printf("%s/", descricao);
            NormalizaString(descricao);
            //printf("%s\n", descricao);

            virg = virg + 1;
            virg = strchr(virg, ',');
            *virg = '\0';

            largura = virg + 1;
            virg = strchr(largura, ',');
            *virg = '\0';
            
            altura = virg + 1;
            virg = strchr(altura, ',');
            *virg = '\0';

            //printf("arquivo: \"%s\"\nartista: \"%s\"\ngenero: \"%s\"\ndesc: \"%s\"\nlargura: \"%s\"\naltura: \"%s\"\n---------\n", arquivo, artista, genero, descricao, largura, altura);
            cont++;
            tArte *umaArte = CriaArte(cont, artista, descricao, genero, arquivo, atoi(largura), atoi(altura));

            //testa arquivo
            char nomeArquivo[strlen(diretorioBase) + strlen(umaArte->nomeArquivo) + 2];
            sprintf(nomeArquivo, "%s/%s", diretorioBase, umaArte->nomeArquivo);
            FILE *fpTeste = fopen(nomeArquivo, "rb");
            if (!fpTeste){
                free(umaArte);
                cont--;
                continue;
            }
            fclose(fpTeste);
            
            InsereArteArvoreFunil(arvoreFunil, umaArte);
            //free(umaArte);
        }
    }

    fclose(fp);
    return cont;
}


int main(){
    tNoFunil * arvore;
    srand(time(NULL));

    IniciaArvoreFunil(&arvore);
    char nomeArquivo[1024];
    sprintf(nomeArquivo, "%s/classes.csv", diretorioBase);
    int qArtes = CarregaArquivo(nomeArquivo, &arvore);
    
    int chaves[NUM_CHAVES], qChaves = 0;
    while(qChaves < NUM_CHAVES){
        int umNumero = rand()%qArtes + 1;
        int presente = 0;
        for(int i = 0; i < qChaves; i++){
            if (chaves[i] == umNumero){
                presente = 1;
                break;
            }
        } 
        if (!presente){
            chaves[qChaves++] = umNumero;
        }
    }

    int qOperacoes[NUM_CHAVES][10] = {{0}};
    const int totalSorteios = NUM_SORTEIOS;
    int iSorteio = 0;
    while(iSorteio < totalSorteios){
        int umIndice = rand()%NUM_CHAVES;
        int chave = chaves[umIndice];
        BuscaNoArvoreFunil(&arvore, chave);

        //chama outros aleatorios
        for(int i = 0; i < 3; i++){
            int umaChave = rand()%qArtes + 1;
            BuscaNoArvoreFunil(&arvore, umaChave);
        }


        if (iSorteio % (NUM_SORTEIOS/10) == 0){
            for (int i = 0; i < NUM_CHAVES; i++){
                BuscaArvoreFunil(&arvore, chaves[i]);
                qOperacoes[i][iSorteio/(NUM_SORTEIOS/10)] = QuantOperacoesFunil();
            }
        }

        iSorteio++;
    }

    for(int i = 0; i < NUM_CHAVES ; i++){
        printf("Chave #%05d: ", chaves[i]);
        for(int j = 0; j < 10; j++){
            printf("%5d%s", qOperacoes[i][j], (j == 9) ? "\n" : ", ");
        }
    }

    return 0;
}