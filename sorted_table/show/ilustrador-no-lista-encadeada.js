class IlustracaoNoListaEncadeada extends IlustracaoNo {
    #clonePonteiroProx = null;
    #setaPonteiro = null;
    #idAnterior = -1;
    #idProximo = -1;

    /**
     * Cria uma Ilustração de uma Variável com a cor especificada. 
     *
     * @param {number} id 
     * @param {string} nome 
     * @param {string} valor
     * @param {string} cor 
     * @param {number} [idAnterior=-1] id de quem aponta para este nó.
     * @param {number} [idProximo=-1] 
     * @param {boolean} [criarEmNovaLinha=false] 
     */
    constructor(id, valor, cor, idAnterior = -1, idProximo = -1, criarEmNovaLinha = false){
        super(id, `umNo_${id}`, valor, cor, criarEmNovaLinha, idAnterior);

        //iniciar campos
        this.#clonePonteiroProx = null;
        this.#setaPonteiro = null;
        this.#idAnterior = idAnterior;
        this.#idProximo = idProximo;

        //criar elementos        
        let novoPonteiroProx = document.getElementById("caixaPonteiro").cloneNode(true);
        novoPonteiroProx.setAttribute('fill', super.cor);
        novoPonteiroProx.setAttribute('x', `${Ilustrador.larguraPadraoRetangulo - novoPonteiroProx.getAttribute('width') - 2}`);
        //novoNo.appendChild(novoPonteiroProx);
        super.grupoPrincipal.appendChild(novoPonteiroProx);
        this.#clonePonteiroProx = novoPonteiroProx;
        
        if (this.#idProximo != -1){
            let proxNo = Ilustracao.ilustracaoPorId(this.#idProximo);
            if (proxNo != null){
                proxNo.moverPosicoes(1);
                this.apontarPara(proxNo.id);
                proxNo.idAnterior = this.id;
            }else{
                this.apontarPara();
            }
        }else{
            this.apontarPara();
        }

        if (this.#idAnterior != -1){
            let noAnt = Ilustracao.ilustracaoPorId(idAnterior);
            if (noAnt != null){
                noAnt.apontarPara(this.id);
            }
        }
    }

    /**
     * Aponta para outro nó especificado pelo id.
     * 
     * @param {number} id 
     */
    apontarPara(id = -1){
        /*
        if (this.#setaPonteiro != null){
            this.#setaPonteiro.remove();
            this.#setaPonteiro = null;
            this.#idProximo = -1;
            //animar
        }
        */

        if (this.#setaPonteiro == null){
            this.#setaPonteiro = document.getElementById("linhaRetaPonteiro").cloneNode(true);
            this.#setaPonteiro.setAttribute('opacity', `${0}`);
            this.#setaPonteiro.setAttribute('x1', `${this.x + parseFloat(this.#clonePonteiroProx.getAttribute('x')) + parseFloat(this.#clonePonteiroProx.getAttribute('width'))/2}`);
            this.#setaPonteiro.setAttribute('x2', `${parseFloat(this.#setaPonteiro.getAttribute('x1')) + parseFloat(this.#clonePonteiroProx.getAttribute('width'))/2-2}`);        
            this.#setaPonteiro.setAttribute('y1', `${this.y + parseFloat(this.#clonePonteiroProx.getAttribute('y')) + parseFloat(this.#clonePonteiroProx.getAttribute('height'))/2}`);
            this.#setaPonteiro.setAttribute('y2', `${parseFloat(this.#setaPonteiro.getAttribute('y1'))}`);

            Ilustrador.elementoSvg.appendChild(this.#setaPonteiro);

            let animacaoOpacidade = document.getElementById("animacaoSurgir").cloneNode(true);
            this.#setaPonteiro.appendChild(animacaoOpacidade);
            animacaoOpacidade.beginElement();

            const timeoutId = setTimeout(() => {
                this.#setaPonteiro.setAttribute('opacity', `${100}`);
                animacaoOpacidade.remove();
            }, Ilustrador.tempoPadraoAnimacoes);
        }

        let outroNo = id != -1 ? Ilustracao.ilustracaoPorId(id) : null;
        if (outroNo == null && id != -1){
            console.debug(`|NoEncadeada| não foi possível encontrar a Ilustracao de id (${id}) para apontar.`);
            //this.apontarPara()
            //return;
        }

        let x2 = outroNo != null ? outroNo.x : parseFloat(this.#setaPonteiro.getAttribute('x1')) + parseFloat(this.#clonePonteiroProx.getAttribute('width'))/2-2;
        let y2 = outroNo != null ? outroNo.y + parseFloat(this.#clonePonteiroProx.getAttribute('y')) + parseFloat(this.#clonePonteiroProx.getAttribute('height'))/2 : parseFloat(this.#setaPonteiro.getAttribute('y1'));
        
        let animacaoX2 = document.getElementById("animacaoApontar").cloneNode(true);
        animacaoX2.setAttribute('attributeName', 'x2');
        animacaoX2.setAttribute('from', `${this.#setaPonteiro.getAttribute('x2')}` );
        animacaoX2.setAttribute('to', `${x2}` );

        let animacaoY2 = document.getElementById("animacaoApontar").cloneNode(true);
        animacaoY2.setAttribute('attributeName', 'y2');
        animacaoY2.setAttribute('from', `${this.#setaPonteiro.getAttribute('y2')}` );
        animacaoY2.setAttribute('to', `${y2}` );
        
        this.#setaPonteiro.appendChild(animacaoX2);
        this.#setaPonteiro.appendChild(animacaoY2);
        animacaoX2.beginElement();
        animacaoY2.beginElement();

        const timeoutId = setTimeout(() => {
            this.#setaPonteiro.setAttribute('x2', `${x2}`);        
            this.#setaPonteiro.setAttribute('y2', `${y2}`);

            animacaoX2.remove();
            animacaoY2.remove();
        }, Ilustrador.tempoPadraoAnimacoes);

        this.#idProximo = outroNo != null ? outroNo.id : -1;

        /* antigo
        let path = calculatePath(coordenadas);
        //console.log(`coordenadas: ${JSON.stringify(coordenadas)}, path: ${path}`);
        let novaSetaPonteiro = document.getElementById("setaVarPonteiro").cloneNode(true);
        novaSetaPonteiro.setAttribute('d', path);
        Ilustrador.elementoSvg.appendChild(novaSetaPonteiro); //precisar ser no geral, pois liga em posições relativas
        animaSurgimento(novaSetaPonteiro);
        objetosSvg.setaPonteiro = novaSetaPonteiro;
        */
    }

    get idProximo(){
        return this.#idProximo;
    }

    get idAnterior(){
        return this.#idAnterior;
    }

    set idAnterior(idAnterior){
        this.#idAnterior = idAnterior;
    }

    moverPara(x, y){  
        let difX = x - this.x;
        let difY = y - this.y;

        super.moverPara(x, y);

        //mover seta ponteiro
        let x1 = parseFloat(this.#setaPonteiro.getAttribute('x1')) + parseFloat(difX);
        let y1 = parseFloat(this.#setaPonteiro.getAttribute('y1')) + parseFloat(difY);
        
        let animacaoX1 = document.getElementById("animacaoApontar").cloneNode(true);
        animacaoX1.setAttribute('attributeName', 'x1');
        animacaoX1.setAttribute('from', `${this.#setaPonteiro.getAttribute('x1')}` );
        animacaoX1.setAttribute('to', `${x1}` );

        let animacaoY1 = document.getElementById("animacaoApontar").cloneNode(true);
        animacaoY1.setAttribute('attributeName', 'y1');
        animacaoY1.setAttribute('from', `${this.#setaPonteiro.getAttribute('y1')}` );
        animacaoY1.setAttribute('to', `${y1}` );
        
        this.#setaPonteiro.appendChild(animacaoX1);
        this.#setaPonteiro.appendChild(animacaoY1);
        animacaoX1.beginElement();
        animacaoY1.beginElement();

        this.#setaPonteiro.setAttribute('x1', `${x1}`);        
        this.#setaPonteiro.setAttribute('y1', `${y1}`);

        const timeoutId = setTimeout(() => {
            animacaoX1.remove();
            animacaoY1.remove();
        }, Ilustrador.tempoPadraoAnimacoes);   

        let proxNo = this.#idProximo != -1 ? Ilustracao.ilustracaoPorId(this.#idProximo) : null;
        
        let x2 = proxNo != null ? proxNo.x : parseFloat(this.#setaPonteiro.getAttribute('x1')) + parseFloat(this.#clonePonteiroProx.getAttribute('width'))/2-2;
        let y2 = proxNo != null ? proxNo.y + parseFloat(this.#clonePonteiroProx.getAttribute('y')) + parseFloat(this.#clonePonteiroProx.getAttribute('height'))/2 : parseFloat(this.#setaPonteiro.getAttribute('y1'));

        //let x2 = proxNo == null ? parseFloat(this.#setaPonteiro.getAttribute('x2')) + parseFloat(difX);
        //let y2 = proxNo == null ? parseFloat(this.#setaPonteiro.getAttribute('y2')) + parseFloat(difY);
        
        let animacaoX2 = document.getElementById("animacaoApontar").cloneNode(true);
        animacaoX2.setAttribute('attributeName', 'x2');
        animacaoX2.setAttribute('from', `${this.#setaPonteiro.getAttribute('x2')}` );
        animacaoX2.setAttribute('to', `${x2}` );

        let animacaoY2 = document.getElementById("animacaoApontar").cloneNode(true);
        animacaoY2.setAttribute('attributeName', 'y2');
        animacaoY2.setAttribute('from', `${this.#setaPonteiro.getAttribute('y2')}` );
        animacaoY2.setAttribute('to', `${y2}` );
        
        this.#setaPonteiro.appendChild(animacaoX2);
        this.#setaPonteiro.appendChild(animacaoY2);
        animacaoX2.beginElement();
        animacaoY2.beginElement();

        this.#setaPonteiro.setAttribute('x2', `${x2}`);        
        this.#setaPonteiro.setAttribute('y2', `${y2}`);

        const timeoutId2 = setTimeout(() => {

            animacaoX2.remove();
            animacaoY2.remove();
        }, Ilustrador.tempoPadraoAnimacoes);
    }

    moverPosicoes(nPosicoes){        
        if (this.#idProximo != -1){
            let proximoNo = Ilustracao.ilustracaoPorId(this.#idProximo);
            proximoNo.moverPosicoes(nPosicoes);
        }
        
        let novoX = this.x + nPosicoes*(this.largura + Ilustrador.separacaoPadrao);
        this.moverPara(novoX, this.y);

        /*
        let chamou = false;
        let novoX = this.x + nPosicoes*(this.largura + Ilustrador.separacaoPadrao);

        if (nPosicoes < 0){
            this.moverPara(novoX, this.y);
            chamou = true;
        }


        if (!chamou){
            this.moverPara(novoX, this.y);
        }      
        */  
    }

    remove(){
        if (this.foiRemovido){
            return;
        }

        super.remove();
        if (this.#setaPonteiro != null){
            let animacaoOpacidade = document.getElementById("animacaoSumir").cloneNode(true);
            this.#setaPonteiro.appendChild(animacaoOpacidade);
            animacaoOpacidade.beginElement();

            const timeoutId = setTimeout(() => {
                animacaoOpacidade.remove();
                this.#setaPonteiro.remove();
                this.#setaPonteiro = null;
            }, Ilustrador.tempoPadraoAnimacoes);
        }

        let idProx = -1;
        if (this.#idProximo != -1){
            let prox = Ilustracao.ilustracaoPorId(this.#idProximo);
            if (prox != null){
                prox.moverPosicoes(-1);
                //avisar ao proximo q ele nao tem mais anterior
                prox.idAnterior = this.#idAnterior;
                idProx = prox.id;
            }
        }

        if (this.#idAnterior != -1){
            let ant = Ilustracao.ilustracaoPorId(this.#idAnterior);
            if (ant != null){
                ant.apontarPara(idProx);
            }
        }

        this.#idAnterior = -1;
        this.#idProximo = -1;
    }
}