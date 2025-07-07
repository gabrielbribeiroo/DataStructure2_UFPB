class IlustracaoNo extends Ilustracao {
    #cloneNo = null;
    #cloneTextoValor = null;
    #valor = '';
    #endereco = null;
    #destaque = null;

    /**
     * Cria uma Ilustração de um Nó genérico. 
     *
     * @param {number} id 
     * @param {string} nome 
     * @param {string} valor
     * @param {string} cor
     * @param {boolean} [criarEmNovaLinha=false] 
     * @param {number} [noAnterior=-1] será criado após este nó
     */
    constructor(id, nome, valor, cor, criarEmNovaLinha = false, noAnterior = -1){
        super(id, nome, Ilustracao.novaPosicaoSugerida(Ilustrador.larguraPadraoRetangulo, Ilustrador.alturaPadraoRetangulo+Ilustrador.alturaPadraoNomeVariavel, criarEmNovaLinha, noAnterior), cor);

        //iniciar campos
        this.#cloneNo = null;
        this.#cloneTextoValor = null;
        this.#valor = valor;
        this.#endereco = null;
        this.#destaque = null;

        //criar elementos
        let novoGrupoPrincipal = document.createElementNS('http://www.w3.org/2000/svg','g');
        novoGrupoPrincipal.setAttribute('fill', this.cor);
        super.grupoPrincipal = novoGrupoPrincipal;

        let novoNo = document.getElementById("umNo").cloneNode(true);
        this.#cloneNo = novoNo;
        
        let novoTextoValor = document.getElementById("textoValorNo").cloneNode(true);
        novoTextoValor.setAttribute('fill', super.cor);
        novoTextoValor.textContent = this.#valor;
        novoNo.appendChild(novoTextoValor);
        this.#cloneTextoValor = novoTextoValor;

        //posiciona no local correto e insere
        super.grupoPrincipal.setAttribute('transform', `translate(${this.x}, ${this.y})`);
        super.grupoPrincipal.appendChild(this.#cloneNo);
        Ilustrador.elementoSvg.appendChild(super.grupoPrincipal);

        Ilustrador.autoAjustarTamanhoDoTexto(novoTextoValor);

        this.animaSurgimento();
        //this.animaCrescimento();

        this.#cloneNo.setAttribute('onclick', `Ilustracao.ilustracaoPorId(${this.id}).focar();`);
        this.#cloneNo.setAttribute('style', "cursor: move");
    }

    get valor(){
        return this.#valor;
    }

    /**
     * Define um novo valor para a Variável.
     */
    set valor(novoValor){
        this.#valor = novoValor;

        let novaAnimacao = document.getElementById("animacaoAlterar").cloneNode(true);
        this.#cloneTextoValor.appendChild(novaAnimacao);
        novaAnimacao.beginElement();

        const timeoutId = setTimeout(() => {
                this.#cloneTextoValor.textContent = this.#valor;
                Ilustrador.autoAjustarTamanhoDoTexto(this.#cloneTextoValor);
            }, Ilustrador.tempoPadraoAnimacoes/2);

        const timeoutId2 = setTimeout(() => {
                novaAnimacao.remove();
            }, Ilustrador.tempoPadraoAnimacoes/2);        
    }

    animaCrescimento(cresce = true){
        let animacaoLargura = document.getElementById("animacaoAumentaLargura").cloneNode(true);
        animacaoLargura.setAttribute(`${cresce ? 'from' : 'to'}`, `${0}`);
        animacaoLargura.setAttribute(`${cresce ? 'to' : 'from'}`, `${super.grupoPrincipal.getBBox().width}`);

        let animacaoAltura = document.getElementById("animacaoAumentaAltura").cloneNode(true);
        animacaoAltura.setAttribute(`${cresce ? 'from' : 'to'}`, `${0}`);
        animacaoAltura.setAttribute(`${cresce ? 'to' : 'from'}`, `${super.grupoPrincipal.getBBox().height}`);

        Ilustrador.procuraElementoFilhoPorId(this.#cloneNo, 'caixaExternaNo').appendChild(animacaoLargura);
        Ilustrador.procuraElementoFilhoPorId(this.#cloneNo, 'caixaExternaNo').appendChild(animacaoAltura);
        animacaoLargura.beginElement();
        animacaoAltura.beginElement();

        const timeoutId = setTimeout(() => {
            //this.#grupoPrincipal.removeChild(novaAnimacao);
            animacaoLargura.remove();
            animacaoAltura.remove();
        }, Ilustrador.tempoPadraoAnimacoes);
    }
    
    /**
     * Adiciona um destaque à variável que é auto removido de acordo com o tempo em milissegundos especificado.
     * O valor padrão é o tempo padrão para animações (Ilustrador.tempoPadraoAnimacoes). 
     * Informe o valor 0 (zero) para não remover automaticamente.
     * 
     * @param {string} [cor=Ilustrador.corPadraoDestaque] 
     * @param {number} milisAutoRemover 
     */
    destaca(cor = super.grupoPrincipal.getAttribute('fill'), milisAutoRemover = Ilustrador.tempoPadraoAnimacoes){
        //remove destaque anterior
        this.removeDestaque();

        //retangulo do destaque
        let novaCaixaDestaque = document.createElementNS('http://www.w3.org/2000/svg','use');
        novaCaixaDestaque.setAttribute('href', "#caixaDestaque");
        novaCaixaDestaque.setAttribute('fill', cor);
        novaCaixaDestaque.setAttribute('stroke', cor);

        this.#destaque = novaCaixaDestaque;
        super.grupoPrincipal.insertBefore(novaCaixaDestaque, this.#cloneNo);

        if (milisAutoRemover != 0){
            const timeoutId = setTimeout(() => {
                    this.removeDestaque();
                }, milisAutoRemover);
        }
    }  

    removeDestaque(){
        if (this.#destaque != null){
            this.#destaque.remove();
            this.#destaque = null;
        }
    }
}