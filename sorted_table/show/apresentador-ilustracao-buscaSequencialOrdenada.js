class IlustracaoBuscaSequencialOrdenada extends IlustracaoSlide {
    #tamanhoLista = 0;
    #valorChave = -1;
    #valoresLista = new Array();

    #varChave = null;
    
    #idCabecaLista = -1;
    #idBase = 200;
    #rodando = false;

    constructor(id, tamanhoLista){
        super(id, "Busca Sequencial Ordenada");
        
        this.#tamanhoLista = tamanhoLista;
        for(let i = 0; i <this.#tamanhoLista; i++){
            let valor = Ilustrador.geraInteiroAleatorio(10*i, 10*i+9);
            this.#valoresLista.push(valor);
        }
    }

    montar(){
        let ids = 100;
        this.limpar();
        this.#varChave = new IlustracaoVariavel(ids++, "chave", this.#valorChave, 'lightsteelblue', true);

        for(let i = 0; i < this.#tamanhoLista; i++){
            let novoNo = new IlustracaoNoListaEncadeada(this.#idBase+this.#valoresLista[i], this.#valoresLista[i], 'sienna', i > 0 ? this.#idBase+this.#valoresLista[i-1] : -1, -1, true);
            if (i == 0){
                this.#idCabecaLista = novoNo.id;
            }
        }
    }
    
    async ilustrar(slide){
        let tempoPadrao = 1000;
        this.#rodando = true;
        while(this.#rodando){

            //destaca chave
            slide.destacaLinhas("1-2");
            this.#valorChave = Ilustrador.geraInteiroAleatorio(1, this.#tamanhoLista*10);
            this.#varChave.valor = this.#valorChave;
            this.#varChave.destaca(this.#varChave.grupoPrincipal.getAttribute('fill'), tempoPadrao);
            await Ilustrador.sleep(tempoPadrao);

            //destaca linha cabeca
            let i = 0;
            slide.destacaLinhas("4");

            //percorre
            let achou = false;
            while(i < this.#tamanhoLista && this.#valoresLista[i] <= this.#valorChave){
                if (!this.#rodando){
                    break;
                }

                //destaca atual
                Ilustracao.ilustracaoPorId(this.#idBase+this.#valoresLista[i]).destaca("sienna", tempoPadrao);
                await Ilustrador.sleep(tempoPadrao);

                if (this.#valoresLista[i] == this.#valorChave){
                    Ilustracao.ilustracaoPorId(this.#idBase+this.#valoresLista[i]).destaca("greenyellow", tempoPadrao);
                    slide.destacaLinhas("7");
                    await Ilustrador.sleep(tempoPadrao);
                    achou = true;
                    break;
                }else{
                    Ilustracao.ilustracaoPorId(this.#idBase+this.#valoresLista[i]).destaca("tomato", tempoPadrao);
                    await Ilustrador.sleep(tempoPadrao);
                }
                i++;
            }

            if (!achou){
                slide.destacaLinhas("11");
                this.#varChave.destaca('tomato', tempoPadrao);
                await Ilustrador.sleep(tempoPadrao);
            }
        }
    }

    parar(){
        this.#rodando = false;
    }

    #removeLista(){
        if (this.#idCabecaLista != -1){
            while(this.#idCabecaLista != -1){
                let umNo = Ilustracao.ilustracaoPorId(this.#idCabecaLista);
                if (umNo != null){
                    this.#idCabecaLista = umNo.idProximo;
                    umNo.remove();
                }else{
                    this.#idCabecaLista = -1;
                }
            }
        }
    }

    limpar(){
        if (this.#varChave != null){
            this.#varChave.remove();
        }
        this.#removeLista();
    }

}