class Ilustracao {
    static #ilustracoes = new Array(); /* array com todas as ilustrações criadas */

    /**
     * Método privado que adiciona uma Ilustração na lista de criadas.
     * 
     * @param {Ilustracao} ilustracao 
     */
    static #adicionaIlustracao(ilustracao){
        this.#ilustracoes.push(ilustracao);
    }

    /**
     * Recupera a referência para uma Ilustração que foi adicionada.
     * 
     * @param {number} id 
     * @returns {Ilustracao}
     */
    static ilustracaoPorId(id){
        for (let i = 0; i < Ilustracao.#ilustracoes.length; i++){
            if (Ilustracao.#ilustracoes[i].id == id){
                return this.#ilustracoes[i];
            }
        }

        return null;
    }

    static defineEscopoAtivo(escopo = 'nenhum'){
        for (let i = 0; i < this.#ilustracoes.length; i++){
            if (escopo == 'nenhum' || Ilustracao.#ilustracoes[i].escopo == escopo){
                //escopo ativo
                Ilustracao.#ilustracoes[i].grupoPrincipal.setAttribute('opacity', '1');
                Ilustracao.#ilustracoes[i].grupoPrincipal.setAttribute('fill', Ilustracao.#ilustracoes[i].cor);
            }else{
                //escopo inativo
                Ilustracao.#ilustracoes[i].grupoPrincipal.setAttribute('opacity', '0.25');
                Ilustracao.#ilustracoes[i].grupoPrincipal.setAttribute('fill', Ilustrador.corPadraoInativa);
            }
        }
    }
    
    /**
     * Calcula uma nova posicão (x, y) ideal para posicionar uma nova Ilustração com base nas demais já existentes.
     * 
     * @param {number} [altura=-1] 
     * @param {number} [largura=-1] 
     * @param {boolean} [preenchimentoVertical=false] se serão organizadas verticalmente ou horizontalmente.
     * @param {number} [criarAposIlustracaoId=-1] 
     * 
     * @returns {Dimensao} uma nova Dimensão com a posição (x, y) adequada, com largura e altura iguais à -1.
     */
    static novaPosicaoSugerida(largura = -1, altura = -1, preenchimentoVertical = false, criarAposIlustracaoId = -1){

        if (criarAposIlustracaoId != -1){
            let ilustracaoAnterior = Ilustracao.ilustracaoPorId(criarAposIlustracaoId);
            if (ilustracaoAnterior != null){
                let novoX = ilustracaoAnterior.x + ilustracaoAnterior.largura + Ilustrador.separacaoPadrao;
                let novoY = ilustracaoAnterior.y;

                //console.debug(`novo local gerado apos ilustracao anterior: ${new Dimensao(novoX, novoY, largura, altura)}`);
                return new Dimensao(novoX, novoY, largura, altura);
            }
        }

        let qObjetos = Ilustracao.#ilustracoes.length;
        let novoX = Ilustrador.xPadrao;
        let novoY = qObjetos*(Ilustrador.alturaPadraoRetangulo + 2*Ilustrador.alturaPadraoNomeVariavel + Ilustrador.separacaoPadrao) + Ilustrador.separacaoPadrao;
        let ultimoIndice = -1;

        //procura um bom y 
        novoY = Ilustrador.separacaoPadrao;
        for(let i = 0; i < qObjetos; i++){
            if ((Ilustracao.#ilustracoes[i] instanceof IlustracaoVariavel || Ilustracao.#ilustracoes[i] instanceof IlustracaoArray) && !Ilustracao.#ilustracoes[i].foiRemovido){
                novoY = Ilustracao.#ilustracoes[i].y + (Ilustrador.alturaPadraoRetangulo + 2*Ilustrador.alturaPadraoNomeVariavel + Ilustrador.separacaoPadrao) + Ilustrador.separacaoPadrao;
                //console.debug(`passando por ilustracao ativa (y = ${novoY}/${Ilustracao.#ilustracoes[i].y}): ${Ilustracao.#ilustracoes[i]}`);
            }else{
                //console.debug(`passando por ilustracao inativa: ${Ilustracao.#ilustracoes[i]}`);
            }

        }
        
        for(ultimoIndice = qObjetos-1; ultimoIndice >=0 ; ultimoIndice--){
            //procurando ultimo objeto sendo exibido
            if (!Ilustracao.#ilustracoes[ultimoIndice].foiRemovido){
                break;
            }
        }

        if (!preenchimentoVertical && ultimoIndice >= 0 && Ilustracao.#ilustracoes[ultimoIndice] instanceof Ilustracao){
            //mesma linha no final
            novoX = Ilustracao.#ilustracoes[ultimoIndice].x + Ilustrador.separacaoPadrao + parseInt(Ilustracao.#ilustracoes[ultimoIndice].largura);
            novoY = Ilustracao.#ilustracoes[ultimoIndice].y;
            if (preenchimentoVertical ||  novoX+Ilustrador.larguraPadraoRetangulo+Ilustrador.separacaoPadrao > Ilustrador.dimensoesGraficosSVG.largura){
                novoX = Ilustrador.xPadrao;
                novoY = Ilustracao.#ilustracoes[ultimoIndice].y + Ilustracao.#ilustracoes[ultimoIndice].altura +Ilustrador.alturaPadraoNomeVariavel + 2*Ilustrador.separacaoPadrao;
            }
        }

        //console.debug(`novo local gerado: ${new Dimensao(novoX, novoY, largura, altura)}`);
        return new Dimensao(novoX, novoY, largura, altura);
    }

    #id = -1;
    #nome = ''
    #dimensao = new Dimensao(Ilustrador.separacaoPadrao, Ilustrador.separacaoPadrao);
    #cor = '#000000';
    #grupoPrincipal = null;
    #escopo = 'principal';
    #removido = false;
    
    
    /**
     * Cria uma nova ilustração com as dimensões especificadas.
     * 
     * @param {number} id 
     * @param {string} nome 
     * @param {Dimensao} dimensao 
     * @param {string} cor
     */
    constructor(id, nome, dimensao, cor){
        this.#id = id;
        this.#nome = nome;
        this.#dimensao = dimensao;
        this.#cor = cor;

        this.#grupoPrincipal = null;
        this.#escopo = 'principal';
        this.#removido = false;
        
        Ilustracao.#adicionaIlustracao(this);
    }

    get id(){
        return this.#id;
    }

    get nome(){
        return this.#nome;
    }

    /**
     * Altera o nome de uma variável.
     * 
     * @param {string} novoNome 
     */
    set nome(novoNome){
        this.#nome = novoNome;
    }

    get dimensao(){
        return this.#dimensao;
    }

    get x() {
        return this.#dimensao.x;
    }

    get y() {
        return this.#dimensao.y;
    }

    get largura() {
        return this.#dimensao.largura;
    }

    get altura() {
        return this.#dimensao.altura;
    }

    get cor() {
        return this.#cor;
    }

    get grupoPrincipal(){
        return this.#grupoPrincipal;
    }
    
    /**
     * Informa o grupo SVG (tag 'g') que contém todos os demais elementos SVG da Ilustração.
     * 
     * @param {SVGGElement} [grupo]
     */
    set grupoPrincipal(grupo){
        this.#grupoPrincipal = grupo;
    }

    get escopo() {
        return this.#escopo;
    }

    /**
     * Define o escopo da Ilustração.
     * 
     * @param {string} escopo o identificador do escopo da Ilustração.
     */
    set escopo(escopo){
        this.#escopo = escopo;
    }

    get foiRemovido(){
        return this.#removido;
    }

    /**
     * Remove o objeto dos gráficos e o define como removido.
     */
    remove(){
        if (this.#removido){
            return;
        }
        
        let animacaoOpacidade = document.getElementById("animacaoSumir").cloneNode(true);
        this.#grupoPrincipal.appendChild(animacaoOpacidade);
        animacaoOpacidade.beginElement();

        const timeoutId = setTimeout(() => {
            animacaoOpacidade.remove();
            if (this.#grupoPrincipal != null){
                this.#grupoPrincipal.remove();
            }
        }, Ilustrador.tempoPadraoAnimacoes);

        this.#removido = true;
    }

    moverPara(x, y){        
        let animacaoMover = document.getElementById("animacaoMover").cloneNode(true);
        animacaoMover.setAttribute('from', `${this.x}, ${this.y}`);
        animacaoMover.setAttribute('to', `${x}, ${y}`);
        this.#grupoPrincipal.appendChild(animacaoMover);
        animacaoMover.beginElement();

        this.#dimensao.x = x;
        this.#dimensao.y = y;       

        const timeoutId = setTimeout(() => {
            this.#grupoPrincipal.setAttribute('transform', `translate(${this.x}, ${this.y})`);

            animacaoMover.remove();
        }, Ilustrador.tempoPadraoAnimacoes);
    }

    /**
     * Anima o surgimento da Ilustração.
     */
    animaSurgimento(){
        let animacaoOpacidade = document.getElementById("animacaoSurgir").cloneNode(true);
        this.#grupoPrincipal.appendChild(animacaoOpacidade);
        animacaoOpacidade.beginElement();

        //let animacaoCrescer = document.getElementById("animacaoCrescer").cloneNode(true);
        //this.#grupoPrincipal.appendChild(animacaoCrescer);
        //animacaoCrescer.beginElement();
    
        const timeoutId = setTimeout(() => {
            //this.#grupoPrincipal.removeChild(novaAnimacao);
            //animacaoCrescer.remove();
            animacaoOpacidade.remove();
        }, Ilustrador.tempoPadraoAnimacoes);
    }

    /**
     * Recupera a posição e tamanho da ilustração em valores absolutos em relação ao SVG.
     * 
     * @returns {Dimensao} as dimensões reais da ilustração
     */
    getGlobalBBox() {
        // 1. Pega a bounding box no espaço local
        const element = this.grupoPrincipal;
        const bbox = element.getBBox();

        // 2. Obtém a matriz de transformação acumulada (incluindo pais)
        const ctm = element.getScreenCTM();

        // 3. Calcula os pontos extremos transformados
        const points = [
            // Canto superior esquerdo
            { x: bbox.x, y: bbox.y },
            // Canto superior direito
            { x: bbox.x + bbox.width, y: bbox.y },
            // Canto inferior direito
            { x: bbox.x + bbox.width, y: bbox.y + bbox.height },
            // Canto inferior esquerdo
            { x: bbox.x, y: bbox.y + bbox.height }
        ].map(p => {
            // Aplica a matriz de transformação
            const transformedX = ctm.a * p.x + ctm.c * p.y + ctm.e;
            const transformedY = ctm.b * p.x + ctm.d * p.y + ctm.f;
            return { x: transformedX, y: transformedY };
        });

        // 4. Calcula a nova bounding box global
        const xs = points.map(p => p.x);
        const ys = points.map(p => p.y);
        const minX = Math.min(...xs);
        const minY = Math.min(...ys);
        const width = Math.max(...xs) - minX;
        const height = Math.max(...ys) - minY;

        //return { x: minX, y: minY, width, height };
        return new Dimensao(minX, minY, width, height);
    }

    focar(zoom = true) {
        let escala = 1;
        if (zoom){
            //calcular escala de zoom de acordo com as dimensões da ilustração
            const escalaPadrao = 2.5;
            const proporcao = (Ilustrador.dimensoesGraficosSVG.largura - 5*Ilustrador.separacaoPadrao) / this.largura;
            escala = proporcao > escalaPadrao ? escalaPadrao : proporcao;
        }
        const larguraFocada = Ilustrador.dimensoesGraficosSVG.largura / escala;
        const alturaFocada = Ilustrador.dimensoesGraficosSVG.altura / escala;
        const novaPosicao = new Dimensao(this.x - larguraFocada/2 + this.largura/2, this.y - alturaFocada/2 + this.altura/2, larguraFocada, alturaFocada-2*Ilustrador.separacaoPadrao);

        let novaAnimacao = document.getElementById("animacaoViewBox").cloneNode(true);
        novaAnimacao.setAttribute('to', `${novaPosicao.x} ${novaPosicao.y} ${novaPosicao.largura} ${novaPosicao.altura}`);

        Ilustrador.elementoSvg.appendChild(novaAnimacao);
        novaAnimacao.beginElement();

        const timeoutId = setTimeout(() => {
                Ilustrador.elementoSvg.setAttribute('viewBox', `${novaPosicao.x} ${novaPosicao.y} ${novaPosicao.largura} ${novaPosicao.altura}`);      
                novaAnimacao.remove();
            }, Ilustrador.tempoPadraoAnimacoes);
    }    
}