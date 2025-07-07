class Dimensao {
    #x;
    #y;
    #largura;
    #altura;

    /**
     * Constrói uma nova Dimensão para representar os valores dos parâmetros.
     * 
     * @param {number} x 
     * @param {number} y 
     * @param {number} largura 
     * @param {number} altura 
     */
    constructor(x, y, largura = -1, altura = -1){
        this.#x = x;
        this.#y = y;
        this.#largura = largura;
        this.#altura = altura;
    }

    get x(){
        return this.#x;
    }

    set x(x){
        this.#x = x;
    }

    get y(){
        return this.#y;
    }

    set y(y){
        this.#y = y;
    }

    get largura(){
        return this.#largura;
    }

    set largura(largura){
        this.#largura = largura;
    }

    get altura(){
        return this.#altura;
    }

    set altura(altura){
        this.#altura = altura;
    }

    /**
     * Atualiza os valores da Dimensão para os novos valores representados pelos parâmetros.
     * 
     * @param {number} x 
     * @param {number} y 
     * @param {number} largura 
     * @param {number} altura 
     */
    atualiza(x, y, largura, altura){
        this.#x = x;
        this.#y = y;
        this.#largura = largura;
        this.#altura = altura;
    }

    toString(){
        return `${this.#largura}x${this.#altura} em (${this.#x}, ${this.#y})`;
    } 
}

class Ilustrador {
    static #dimensoesSVG = new Dimensao(0, 0, 0, 0);
    static #svgCanvas = null;

    static #alturaPadraoRetangulo = 30;
    static #alturaPadraoNomeVariavel = 20;
    static #larguraPadraoRetangulo = Ilustrador.#alturaPadraoRetangulo + Ilustrador.#alturaPadraoNomeVariavel;
    static #separacaoPadrao = 15;
    static #xPadrao = Ilustrador.#separacaoPadrao;
    static #tamanhoPadraoTexto = 18; 
    static #tamanhoPadraoNome = 12; 
    static #tempoPadraoAnimacoes = 500; //ms
    static #corPadraoDestaque = 'lightskyblue';
    static #corPadraoInativa = 'whitesmoke';

    static get dimensoesGraficosSVG(){
        return Ilustrador.#dimensoesSVG;
    }

    /**
     * Define as dimensões do canvas SVG.
     * 
     * @param {Dimensao} novasDimensoes
     */
    static set dimensoesGraficosSVG (novasDimensoes){
        Ilustrador.#dimensoesSVG = novasDimensoes;
        console.debug(`|Ilustrador| dimensoes do canvas svg definidas: ${Ilustrador.#dimensoesSVG}`);
    }

    static get elementoSvg(){
        return Ilustrador.#svgCanvas;
    }

    /**
     * Define qual é o canvas SVG a ser utilizado.
     * 
     * @param {SVGSVGElement} elementoHtmlSvg
     */
    static set elementoSvg(elementoHtmlSvg){
        Ilustrador.#svgCanvas = elementoHtmlSvg;
        console.debug(`|Ilustrador| definido o elemento html que contém os gráficos svg`);
    }

    static get alturaPadraoRetangulo(){
        return Ilustrador.#alturaPadraoRetangulo;
    }

    static get alturaPadraoNomeVariavel(){
        return Ilustrador.#alturaPadraoNomeVariavel;
    }

    static get larguraPadraoRetangulo(){
        return Ilustrador.#larguraPadraoRetangulo;
    }

    static get separacaoPadrao(){
        return Ilustrador.#separacaoPadrao;
    }

    static get xPadrao(){
        return Ilustrador.#xPadrao;
    }

    static set xPadrao(novoXPadrao){
        this.#xPadrao = novoXPadrao;
    }

    static get tamanhoPadraoTexto(){
        return Ilustrador.#tamanhoPadraoTexto;
    }

    static get tamanhoPadraoNome(){
        return Ilustrador.#tamanhoPadraoNome;
    }

    static get tempoPadraoAnimacoes(){
        return this.#tempoPadraoAnimacoes;
    }

    static get corPadraoDestaque(){
        return this.#corPadraoDestaque;
    }

    static get corPadraoInativa(){
        return this.#corPadraoInativa;
    }

    /**
     * Calcula e retorna uma nova cor aleatória.
     * 
     * @returns uma cor aletória no formato '#XXXXXX'
     */
    static geraCorAleatoria() {
        const letras = '0123456789ABCDEF';
        let cor = '#';
        for (let i = 0; i < 6; i++) {
            cor += letras[Math.floor(Math.random() * letras.length)];
        }

        //console.debug(`|Ilustrador| nova cor aleatória gerada: ${cor}`);
        return cor;
    }

    /**
     * Método que auxilia no processo de, a partir de um elemento HTML pai, procurar
     * um filho com a identificação especificada.
     * 
     * @param {HTMLElement} elementoPai 
     * @param {string} idFilho 
     * @returns 
     */
    static procuraElementoFilhoPorId(elementoPai, idFilho){
        if (!elementoPai || !elementoPai.hasChildNodes()){
            return null;
        }

        let filhos = new Array();
        for(let i = 0; i < elementoPai.childNodes.length; i++){
            let umFilho = elementoPai.childNodes[i];
            filhos.push(umFilho);
        }

        while(filhos.length > 0){
            let umFilho = filhos.shift();
            if (umFilho.nodeType == Node.ELEMENT_NODE && umFilho.getAttribute('id') == idFilho){
                return umFilho;
            }
            
            if (umFilho.hasChildNodes){
                for(let i = 0; i < umFilho.childNodes.length; i++){
                    let outroFilho = umFilho.childNodes[i];
                    filhos.push(outroFilho);
                }
            }
        }

        return null;
    }

    /**
     * Calcula a distância entre as posições referentes às dimensões informadas.
     * 
     * @param {Dimensao} umaDimensao 
     * @param {Dimensao} outraDimensao 
     * @returns 
     */
    static distancia(umaDimensao, outraDimensao){
        const dx = parseFloat(outraDimensao.x) - parseFloat(umaDimensao.x);
        const dy = parseFloat(outraDimensao.y) - parseFloat(umaDimensao.y);

        return Math.sqrt(dx*dx + dy*dy);
    }

    /**
     * Ajusta automaticamente o tamanho da fonte do elemento referenciado de acordo com a largura disponível.
     * 
     * @param {SVGTextElement} elementoTexto 
     * @param {number} larguraDisponivel 
     */
    static autoAjustarTamanhoDoTexto(elementoTexto, larguraDisponivel = Ilustrador.larguraPadraoRetangulo-4) {
        const larguraCaixa = larguraDisponivel; 
        let tamanhoAtualFonte = parseFloat(elementoTexto.getAttribute('font-size')); // Tamanho inicial
        
        let tamanhoOriginal = elementoTexto.getAttribute('_original-font-size');
        if (!tamanhoOriginal){
            elementoTexto.setAttribute("_original-font-size", tamanhoAtualFonte);
        }else{
            elementoTexto.setAttribute("font-size", tamanhoOriginal);
            tamanhoAtualFonte = tamanhoOriginal;
        }
        
        // Reduz o font-size até o texto caber
        while (parseFloat(elementoTexto.getComputedTextLength()) > parseFloat(larguraCaixa) && tamanhoAtualFonte > 0.01 ) {
            //diminui a fonte em intervalos diferentes de acordo com o valor atual
            if (tamanhoAtualFonte <= 0.100){
                tamanhoAtualFonte -= 0.005
            }else if (tamanhoAtualFonte <= 1.000){
                tamanhoAtualFonte -= 0.050;
            }else{
                tamanhoAtualFonte -= 0.500;
            }
            elementoTexto.setAttribute("font-size", tamanhoAtualFonte);
        }
    }
    
    static geraInteiroAleatorio(min, max) {
        return Math.floor(Math.random() * (max - min)) + min;
    }

    /**
     * Calcula o 'path' de uma seta que liga duas Ilustrações, como a seta de um ponteiro.
     * 
     * @param {Dimensao} umaDimensao 
     * @param {Dimensao} outraDimensao 
     * @returns 
     */
    static geraInfoCaminho(umaDimensao, outraDimensao) {
        const pontoCentral = new Dimensao( (parseFloat(umaDimensao.x)+parseFloat(outraDimensao.x))/2, (parseFloat(umaDimensao.y)+parseFloat(outraDimensao.y))/2);
        const pontoControle = new Dimensao(umaDimensao.x, Math.abs(pontoCentral.y + Math.min(Ilustrador.distancia(umaDimensao, outraDimensao), Math.abs(parseFloat(outraDimensao.x)-parseFloat(umaDimensao.x))/2, Ilustrador.separacaoPadrao*2)*0.25));

        //return `M ${coordenadas.x1},${coordenadas.y1} Q ${coordenadas.x1}, ${coordenadas.y1} ${center.x},${center.y} T ${coordenadas.x2},${coordenadas.y2}`
        return `M ${umaDimensao.x},${umaDimensao.y} Q ${pontoControle.x},${pontoControle.y} ${pontoCentral.x},${pontoCentral.y} T ${outraDimensao.x},${outraDimensao.y}`;
    }
    
    /**
     * Verifica se o elemento está em tela.
     * 
     * @param {SVGElement} elementoSvg 
     * @returns 
     */
    static ilustracaoEstaEmTela(elementoSvg) {
        if (!Ilustrador.#svgCanvas) 
            return false;

        const viewBox = Ilustrador.#svgCanvas.viewBox.baseVal;
        const currentViewBox = {
            x: viewBox.x,
            y: viewBox.y,
            width: viewBox.width,
            height: viewBox.height
        };

        const bbox = elementoSvg.getBBox();
        
        return (
            bbox.x >= currentViewBox.x &&
            bbox.y >= currentViewBox.y &&
            (bbox.x + bbox.width) <= (currentViewBox.x + currentViewBox.width) &&
            (bbox.y + bbox.height) <= (currentViewBox.y + currentViewBox.height)
        );
    }

    static sleep(milissegundos) {
        return new Promise(resolve => setTimeout(resolve, milissegundos));
    }

}