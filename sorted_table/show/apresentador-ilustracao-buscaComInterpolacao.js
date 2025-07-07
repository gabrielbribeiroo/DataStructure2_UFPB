class IlustracaoBuscaComInterpolacao extends IlustracaoSlide {
    #tamanhoLista = 0;
    #valorChave = -1;
    #valoresLista = new Array();

    #varChave = null;
    #arrayLista = null;
        
    #rodando = false;

    constructor(id, tamanhoLista){
        super(id, "Busca com Interpolação");
        
        this.#tamanhoLista = tamanhoLista;
        for(let i = 0; i <this.#tamanhoLista; i++){
            let valor = (i > 0 ? this.#valoresLista[i-1] : i) + Ilustrador.geraInteiroAleatorio(1, 3);
            this.#valoresLista.push(parseInt(valor));
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
            this.#valorChave = Ilustrador.geraInteiroAleatorio(1, this.#tamanhoLista+3);
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
                meio = ini + (fim - ini) * ((this.#valorChave - this.#valoresLista[ini])*1.0/parseFloat(this.#valoresLista[fim] - this.#valoresLista[ini]));

                meio = ini + ((this.#valorChave - this.#valoresLista[ini]) * (fim - ini)) / (this.#valoresLista[fim] - this.#valoresLista[ini]);
                if (meio < ini){
                    meio = ini;
                }
                if (meio > fim){
                    meio = fim;
                }

                console.debug(`ini: ${ini}, fim: ${fim}, meio: ${meio}, chave: ${this.#valorChave}, ini: ${this.#valoresLista[ini]}, fim: ${this.#valoresLista[fim]}`);
                meio =  parseInt(meio);
                console.debug(`meio: ${meio}`);
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