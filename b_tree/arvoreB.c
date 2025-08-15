#include <stdio.h>
#include <stdlib.h>
#include "arvoreB.h"
#include "pilhaEncadeada.h"

static int posicaoRaiz = POSICAO_NULA;
static FILE *fpArvore = NULL;

static void CarregaNo(int posicao, tNoB *no){
    fseek(fpArvore, posicao * sizeof(tNoB), SEEK_SET);
    fread(no, sizeof(tNoB), 1, fpArvore);
}

static void SalvaNo(int posicao, const tNoB *no){
    fseek(fpArvore, posicao * sizeof(tNoB), SEEK_SET);
    fwrite(no, sizeof(tNoB), 1, fpArvore);
}

int NumeroDeNosB(){
    int nNos = 0;
    tNoB umNo;

    fseek(fpArvore, 0, SEEK_SET);
    while (1){
        fread(&umNo, sizeof(umNo), 1, fpArvore);

        if (ferror(fpArvore) || feof(fpArvore))
            break;

        if (umNo.nFilhos > 0)
            ++nNos;
    }

    return nNos;
}

static int IndiceTeto(tChave chave, const tNoB *no){
    int i, nChaves = no->nFilhos - 1;

    for (i = 0; i < nChaves; ++i)
        if (chave <= no->chaves[i].chave)
            return i; 

    return nChaves; //chave maior que todas as chaves do no, retorna posicao do ultimo filho mais a direita
}

int BuscaB(tChave chave){
    tNoB umNo;
    int posNoAtual, i;

    posNoAtual = posicaoRaiz;
    while (posNoAtual != POSICAO_NULA){
        CarregaNo(posNoAtual, &umNo);
        i = IndiceTeto(chave, &umNo);

        if (i < umNo.nFilhos - 1 && chave == umNo.chaves[i].chave) // verifica se encontrou a chave ou se precisa descer e carregar outro no
            return umNo.chaves[i].indice;

        posNoAtual = umNo.filhos[i]; // desce
    }

    return POSICAO_NULA; // nao encontrada
}

static void IniciaNoB(const tChaveIndice *chaveIndice, tNoB *no){
    int i;

    if (chaveIndice)
        no->chaves[0] = *chaveIndice;

    no->nFilhos = chaveIndice ? 2 : 0; //se a chave for valida, o no tem dois filhos (esq/dir)

    for (i = 0; i < G; ++i)
        no->filhos[i] = POSICAO_NULA; //todos os filhos sao nulos inicialmente
}

static void InsereEmNoB(tNoB *pNo, int indice, tChaveIndice chaveEIndice, int pFilho){
    int i;

    for (i = pNo->nFilhos - 1; i > indice; --i){ //shift de chaves e filhos
        pNo->filhos[i + 1] = pNo->filhos[i];
        pNo->chaves[i] = pNo->chaves[i - 1];
    }

    pNo->chaves[indice] = chaveEIndice; /* Insere o par */
    pNo->filhos[indice + 1] = pFilho;   /* Insere seu filho direito */

    pNo->nFilhos++;
}

static void CopiaChavesB(const tNoB *pNo1, tNoB *pNo2, int indiceI, int indiceF){
    int i, nChaves; 
    
    nChaves = indiceF - indiceI + 1;


    for (i = 0; i < nChaves; ++i){
        pNo2->chaves[i] = pNo1->chaves[indiceI + i];
        pNo2->filhos[i] = pNo1->filhos[indiceI + i];
    }

    pNo2->filhos[nChaves] = pNo1->filhos[indiceI + nChaves]; //o ultimo filho
    pNo2->nFilhos = nChaves + 1;
}

static void DivideNoB(tNoB *pNoOriginal, int indiceInsercao, tChaveIndice chave, int posFilhoDireita, int *posNoDireita,
                      tChaveIndice *chaveParaSubir){
    const int meio = G / 2; //indice central do no
    tNoB novoNoDireita;

    IniciaNoB(NULL, &novoNoDireita); //cria novo no vazio

    if (indiceInsercao > meio){
        //chave fica no no da direita   
        CopiaChavesB(pNoOriginal, &novoNoDireita, meio + 1, G - 2);
        InsereEmNoB(&novoNoDireita, indiceInsercao - meio - 1, chave, posFilhoDireita);

        pNoOriginal->nFilhos = meio + 1;
        *chaveParaSubir = pNoOriginal->chaves[meio]; //a ultima do original
    }else if (indiceInsercao == meio){   
        CopiaChavesB(pNoOriginal, &novoNoDireita, meio, G - 2);

        pNoOriginal->nFilhos = meio + 1;
        novoNoDireita.filhos[0] = posFilhoDireita; // o primeiro filho do novo no eh o filho da chave
        *chaveParaSubir = chave; //a propria chave sobe
    }else{   
        //chave fica no no orignal
        CopiaChavesB(pNoOriginal, &novoNoDireita, meio, G - 2);
        
        pNoOriginal->nFilhos = meio + 1;
        InsereEmNoB(pNoOriginal, indiceInsercao, chave, posFilhoDireita);

        *chaveParaSubir = pNoOriginal->chaves[meio];
        --pNoOriginal->nFilhos;
    }

    *posNoDireita = NumeroDeNosB();    
    SalvaNo(*posNoDireita, &novoNoDireita); //salva
}

static void EmpilhaCaminhoB(tChave chave, int *indiceDaChaveNoNo, int *encontrada, tNo **pilha){
    int posAtual, indiceNoPai; 
    tNoB umNo;
    tNoCaminhoB umNoCaminho;

    if (!fpArvore){
        puts("Erro: o arquivo da arvore nao foi aberto.");
        exit(1);
    }

    posAtual = posicaoRaiz;
    indiceNoPai = -1; //raiz nao tem pai
    *encontrada = 0; 

    IniciaPilha(pilha);

    while (posAtual != POSICAO_NULA){
        CarregaNo(posAtual, &umNo);

        umNoCaminho.no = umNo;
        umNoCaminho.posArquivo = posAtual;
        umNoCaminho.indiceNoPai = indiceNoPai;

        Push(pilha, umNoCaminho);

        indiceNoPai = IndiceTeto(chave, &umNo);
        if ((indiceNoPai < umNo.nFilhos - 1) && chave == umNo.chaves[indiceNoPai].chave){
            *encontrada = 1; //encontrada
            *indiceDaChaveNoNo = indiceNoPai; //posicao que foi encontrada
            return;
        }

        //desce um nivel
        posAtual = umNo.filhos[indiceNoPai];
    }

    *indiceDaChaveNoNo = indiceNoPai; //onde deveria descer
}

int InsereB(tChaveIndice *chaveEIndice){ //ponteiro pq, caso tente inserir uma chave existente, atualiza com a posicao no arquivo de registros da ja cadastrada
    int chaveEncontrada, indiceDaChaveNo, posFilhoDireito, posNoDireito;
    tNo *pilhaCaminho; 
    tNoCaminhoB umNoCaminho; 
    tChaveIndice chaveAInserir, chaveQueSobe;

    if (!fpArvore){
        puts("Erro: o arquivo da arvore nao foi aberto.");
        exit(1);
    }

    if (posicaoRaiz == POSICAO_NULA){
        //cria a raiz (e arvore)
        tNoB novoNo;

        /* Cria a raiz  */
        IniciaNoB(chaveEIndice, &novoNo);
        indiceDaChaveNo = 0;
        posicaoRaiz = 0;

        SalvaNo(posicaoRaiz, &novoNo);
        return 1; //inserida
    }

    //encontrar o caminho ate o no de insercao
    EmpilhaCaminhoB(chaveEIndice->chave, &indiceDaChaveNo, &chaveEncontrada, &pilhaCaminho);
   
    if (chaveEncontrada){
        //foi pedido para inserir uma chave que ja existia, atualizando o indice dela no arquivo para quem chamou a funcao
        umNoCaminho = Pop(&pilhaCaminho);
        chaveEIndice->indice =umNoCaminho.no.chaves[indiceDaChaveNo].indice;
        EsvaziaPilha(&pilhaCaminho);
        return 0; //nao inseriu
    }

    //processar a insercao, setando a chave e seu filho direito (comeca nulo mas pode ocorrer divisao de nos)
    chaveAInserir = *chaveEIndice;
    posFilhoDireito = POSICAO_NULA;
    while(pilhaCaminho != NULL){
        umNoCaminho = Pop(&pilhaCaminho);

        if (umNoCaminho.no.nFilhos < G){ //tem espaco no no
            InsereEmNoB(&umNoCaminho.no, indiceDaChaveNo, chaveAInserir, posFilhoDireito);
            SalvaNo(umNoCaminho.posArquivo, &umNoCaminho.no); //atualiza
            EsvaziaPilha(&pilhaCaminho);
            return 1;
        }    

        //nao tem espaco, dividir o no
        DivideNoB(&umNoCaminho.no, indiceDaChaveNo, chaveAInserir, posFilhoDireito, &posNoDireito, &chaveQueSobe);

        SalvaNo(umNoCaminho.posArquivo, &umNoCaminho.no); //atualiza o no original

        chaveAInserir = chaveQueSobe;
        indiceDaChaveNo = umNoCaminho.indiceNoPai;
        posFilhoDireito = posNoDireito;
    }

    //a raiz estava completa
    tNoB novaRaiz;
    IniciaNoB(&chaveQueSobe, &novaRaiz);
    novaRaiz.filhos[0] = umNoCaminho.posArquivo;
    novaRaiz.filhos[1] = posFilhoDireito;

    posicaoRaiz = NumeroDeNosB();
    SalvaNo(posicaoRaiz, &novaRaiz);

    return 1;
}


void AbrirArquivoArvore(const char *nomeArquivoArvore){
    FILE *fp = fopen(nomeArquivoArvore, "rb+");
    if (!fp){
        fp = fopen(nomeArquivoArvore, "wb+");
        if (!fp){
            puts("Erro: nao foi possivel abrir ou criar arquivo da arvore.");
            exit(1);
        }
    }

    fpArvore = fp;
}

void FechaArquivoArvore(){
    fclose(fpArvore);
    fpArvore = NULL;
}

int CarregaPosicaoRaiz(const char *nomeArquivoRaiz){
    FILE *fp = fopen(nomeArquivoRaiz, "rb");
    int posicao = POSICAO_NULA;
    
    if (!fp){
        //puts("Erro ao abrir arquivo com a posicao da raiz (leitura).");
    }else{
        fread(&posicao, sizeof(int), 1, fp);
        fclose(fp);
    }

    posicaoRaiz = posicao;
    return posicaoRaiz;
}

void AtualizaPosicaoRaiz(int novaPosicaoRaiz){
    posicaoRaiz = novaPosicaoRaiz;
}

void SalvaPosicaoRaiz(const char *nomeArquivoRaiz){
    FILE *fp = fopen(nomeArquivoRaiz, "wb");
    
    if (!fp){
        puts("Erro ao abrir arquivo com a posicao da raiz (escrita).");
        exit(1);
    }

    fwrite(&posicaoRaiz, sizeof(int), 1, fp);
    fclose(fp);
}