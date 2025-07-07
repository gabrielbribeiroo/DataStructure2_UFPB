let contaIds = 0;

function aulaBuscaTabelaOrdenada(){
    let slides = new Array();
    let novoSlide = null;
    let novoEvento = null;

    let tamanhoLista = 7;

    IlustracaoArray.ajustaDefinicoesGerais(tamanhoLista);
    
    //novo slide (insere)
    novoSlide = new Slide("Inserção em Tabela Ordenada", `printf("Chave a inserir: ");\nscanf("%d", &chave);\n\nfor(i = 0; i < tamanhoTabela; i++){\n    if (tabela[i] > chave){\n        //encontrou a posicao\n        for(j = tamanhoTabela; j > i; j--){ //afasta\n            tabela[j] = tabela[j-1];\n            break;\n        }\n    }\n}\n\ntabela[i] = chave;\nnovoNo->valor = chave;\nnovoNo->prox = noAnterior->prox;\nnoAnterior->prox = novoNo;\ntamanhoTabela++;\n`);
    novoEvento = new Evento(-1, null, new IlustracaoInsereTabelaOrdenada(++contaIds, tamanhoLista));
    novoSlide.adicionaEvento(novoEvento);

    slides.push(novoSlide);

    //novo slide (busca seq ordenada)
    novoSlide = new Slide("Busca Sequencial em Tabela Ordenada", `printf("Chave a procurar: ");\nscanf("%d", &chave);\n\nnoAtual = cabecaLista;\nwhile(noAtual != NULL && noAtual->chave <= chave){\n    if (noAtual->chave == chave){\n        return noAtual; //encontrou\n    }\n    noAtual = noAtual->prox;\n}\nreturn NULL; //nao encontrada\n\n\n\n\n\n\n\n`);
    novoEvento = new Evento(-1, null, new IlustracaoBuscaSequencialOrdenada(++contaIds, tamanhoLista));
    novoSlide.adicionaEvento(novoEvento);

    slides.push(novoSlide);

    //novo slide (busca binaria)
    novoSlide = new Slide("Busca Binária", `printf("Chave a procurar: ");\nscanf("%d", &chave);\n\nwhile(ini <= fim){\n    meio = ini + (fim - ini)/2;\n\n    if (tabela[meio] == chave){\n        return meio; //encontrou\n    }\n\n    if (tabela[meio] > chave){\n        fim = meio - 1;\n    }else{\n        ini = meio + 1;\n    }\n\n}\n\nreturn -1; //nao encontrou\n\n\n`);
    novoEvento = new Evento(-1, null, new IlustracaoBuscaBinaria(++contaIds, tamanhoLista));
    novoSlide.adicionaEvento(novoEvento);

    slides.push(novoSlide);

    //novo slide (busca interpolacao)
    novoSlide = new Slide("Busca com Interpolação", `printf("Chave a procurar: ");\nscanf("%d", &chave);\n\nwhile(ini <= fim){\n    meio = ini + (fim - ini) * (chave - tabela[ini])/(tabela[fim] - tabela[ini]);\n\n    if (tabela[meio] == chave){\n        return meio; //encontrou\n    }\n\n    if (tabela[meio] > chave){\n        fim = meio - 1;\n    }else{\n        ini = meio + 1;\n    }\n\n}\n\nreturn -1; //nao encontrou\n\n\n`);
    novoEvento = new Evento(-1, null, new IlustracaoBuscaComInterpolacao(++contaIds, tamanhoLista));
    novoSlide.adicionaEvento(novoEvento);

    slides.push(novoSlide);

    return slides;
}

