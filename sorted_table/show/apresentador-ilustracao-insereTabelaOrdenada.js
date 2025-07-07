class IlustracaoInsereTabelaOrdenada extends IlustracaoSlide {
    #tamanhoLista = 0;
    #valorChave = -1;
    #qChaves = 0;
    #valoresLista = new Array();
    
    #varChave = null;
    #arrayLista = null;
    #idCabecaLista = -1;

    #rodando = false;

    constructor(id, tamanhoLista){
        super(id, "Inserção em Tabela Ordenada");
        
        this.#tamanhoLista = tamanhoLista;
        for(let i = 0; i <this.#tamanhoLista; i++){
            this.#valoresLista.push("");
        }
    }

    montar(){
        let ids = 100;
        this.limpar();
        this.#varChave = new IlustracaoVariavel(ids++, "chave", this.#valorChave, 'lightsteelblue', true);
        this.#arrayLista = new IlustracaoArray(ids++, "tabela de busca", this.#valoresLista, "lightseagreen", true);
    }
    
    async ilustrar(slide){
        let idBase = 300;
        let tempoPadrao = 1000;
        this.#rodando = true;
        this.#qChaves = 0;

        while(this.#rodando){
            //verificar se deve reiniciar
            if (this.#qChaves >= this.#tamanhoLista){
                this.#qChaves = 0;
                for(let i = 0; i < this.#tamanhoLista; i++){
                    this.#arrayLista.setValorElemento("", i);
                    this.#valoresLista[i] = "";
                }
                this.#removeLista();
                idBase += 100;
                continue;
            }

            //destaca chave
            slide.destacaLinhas("1-2");
            let achou = true;
            while(achou){
                achou = false;
                this.#valorChave = Ilustrador.geraInteiroAleatorio(1, this.#tamanhoLista*3);
                for(let i = 0; i < this.#tamanhoLista; i++){
                    if (this.#valoresLista[i] == this.#valorChave){
                        achou = true;
                        break;
                    }
                }
            }
            this.#varChave.valor = this.#valorChave;
            this.#varChave.destaca(this.#varChave.grupoPrincipal.getAttribute('fill'), tempoPadrao);
            await Ilustrador.sleep(tempoPadrao);

            //destaca linha for
            slide.destacaLinhas("4");
            await Ilustrador.sleep(tempoPadrao);

            let i = 0;
            for(i = 0; i < this.#qChaves; i++){
                if (!this.#rodando){
                    break;
                }
                //destaca elementos atuais
                slide.destacaLinhas("5");
                this.#arrayLista.destacaElementos(i, 1, "teal", tempoPadrao);
                Ilustracao.ilustracaoPorId(idBase+this.#valoresLista[i]).destaca("teal", tempoPadrao);
                await sleep(tempoPadrao);

                //verificar se achou posicao
                if (parseInt(this.#valoresLista[i]) > parseInt(this.#valorChave)){
                    //achou a posicao, destaca verde os elementos
                    slide.destacaLinhas("6");
                    this.#arrayLista.destacaElementos(i, 1, "greenyellow", tempoPadrao);
                    Ilustracao.ilustracaoPorId(idBase+this.#valoresLista[i]).destaca("greenyellow", tempoPadrao);
                    await sleep(tempoPadrao);

                    //animar afastamento
                    for(let j = this.#qChaves; j > i; j--){
                        if (!this.#rodando){
                            break;
                        }

                        //anima o deslocamento
                        slide.destacaLinhas("8");
                        this.#arrayLista.copiaElementoArray(j-1, j, true, true);
                        this.#valoresLista[j] = this.#valoresLista[j-1];
                        await sleep(250);
                    }
                    //afastamento da lista
                    //Ilustracao.ilustracaoPorId(idBase+this.#valoresLista[i]).moverPosicoes(1);
                    break;
                }else{
                    //nao achou, marcar vermelho 
                    slide.destacaLinhas("5");
                    this.#arrayLista.destacaElementos(i, 1, "tomato", tempoPadrao);

                    Ilustracao.ilustracaoPorId(idBase+this.#valoresLista[i]).destaca("tomato", tempoPadrao);
                    await sleep(tempoPadrao);
                }
            }

            if (!this.#rodando){
                break;
            }

            //colocar na posicao correta (i)

            console.debug(`i: ${i}`);
            slide.destacaLinhas("14");            
            this.#arrayLista.setValorElemento(this.#valorChave, i);
            this.#arrayLista.destacaElementos(i, 1, this.#arrayLista.grupoPrincipal.getAttribute('fill'), tempoPadrao);
            this.#valoresLista[i] = this.#valorChave;
            await Ilustrador.sleep(tempoPadrao);

            let novoNo = new IlustracaoNoListaEncadeada(idBase+this.#valorChave, this.#valorChave, 'sienna', i > 0 ? idBase+this.#valoresLista[i-1] : -1, i <= this.#qChaves ? idBase+this.#valoresLista[i+1] : -1, true);
            novoNo.destaca("deepskyblue", tempoPadrao);

            if (i == 0){
                this.#idCabecaLista = novoNo.id;
            }

            this.#qChaves++;
            await Ilustrador.sleep(tempoPadrao);
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
        console.debug(`limpando`);
       if (this.#varChave != null){
            this.#varChave.remove();
        }
        if (this.#arrayLista != null){
            this.#arrayLista.remove();
        }
        this.#removeLista();
    }

}