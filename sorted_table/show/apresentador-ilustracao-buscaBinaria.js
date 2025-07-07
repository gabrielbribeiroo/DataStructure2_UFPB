class IlustracaoBuscaBinaria extends IlustracaoSlide {
    #tamanhoLista = 0;
    #valorChave = -1;
    #valoresLista = new Array();

    #varChave = null;
    #arrayLista = null;
        
    #rodando = false;

    constructor(id, tamanhoLista){
        super(id, "Busca Binaria");
        
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
        this.#arrayLista = new IlustracaoArray(ids++, "tabela de busca", this.#valoresLista, "midnightblue", true);
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

            //destaca while
            let ini = 0;
            let fim = this.#tamanhoLista-1;
            let meio = 0;
            //slide.destacaLinhas("4");
            //this.#arrayLista.destacaElementos(ini, fim-ini+1, "yellow", tempoPadrao);
    
            let achou = false;
            while(ini <= fim){
                slide.destacaLinhas("4");
                this.#arrayLista.removeDestaque();
                this.#arrayLista.destacaElementos(ini, fim-ini+1, "yellow", tempoPadrao);
                await Ilustrador.sleep(tempoPadrao);

                slide.destacaLinhas("5");
                meio = ini + (fim -ini)/2;
                this.#arrayLista.destacaElementos(meio, 1, "gold", tempoPadrao);
                await Ilustrador.sleep(tempoPadrao);

                if (this.#valoresLista[meio] == this.#valorChave){
                    this.#arrayLista.destacaElementos(meio, 1, "limegreen", tempoPadrao);
                    slide.destacaLinhas("8");
                    await Ilustrador.sleep(tempoPadrao);
                    achou = true;
                    break;
                }

                if (this.#valoresLista[meio] > this.#valorChave){
                    fim = meio - 1;
                    slide.destacaLinhas("12");
                }else{
                    ini = meio + 1;
                    slide.destacaLinhas("14");
                }
                this.#arrayLista.destacaElementos(ini, fim-ini+1, "yellow", 0);
                await Ilustrador.sleep(tempoPadrao);
            }

            if (!achou){
                slide.destacaLinhas("19");
                this.#varChave.destaca('tomato', tempoPadrao);
                await Ilustrador.sleep(tempoPadrao);
            }
        }
    }

    parar(){
        this.#rodando = false;
    }

    limpar(){
        if (this.#varChave != null){
            this.#varChave.remove();
        }
        if (this.#arrayLista != null){
            this.#arrayLista.remove();
        }
    }

}