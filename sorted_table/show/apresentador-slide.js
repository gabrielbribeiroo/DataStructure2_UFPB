class Slide {
    #titulo = "Sem Título";
    #codigo = "Sem código";
    #listaEventos = new Array();
    #eventoAtual = -1;

    #elementoCodigo = null;
    #postIt = null;
    
    constructor (titulo, codigo){
        this.#titulo = titulo;
        this.#codigo = codigo;

        this.#eventoAtual = -1;
    }

    get titulo(){
        return this.#titulo;
    }

    get codigo(){
        return this.#codigo;
    }

    get numeroDeEventos(){
        return this.#listaEventos.length;
    }

    getEvento(i){
        return this.#listaEventos[i];
    }
    
    /**
     * Adiciona um novo evento.
     * 
     * @param {Evento} evento 
     */
    adicionaEvento(evento){
        this.#listaEventos.push(evento);
    }

    destacaLinhas(linhas){
        let preCodigo = Ilustrador.procuraElementoFilhoPorId(this.#elementoCodigo, "code");
        let codeCodigo = Ilustrador.procuraElementoFilhoPorId(this.#elementoCodigo, "fonte");

        preCodigo.setAttribute("data-line", linhas != -1 ? linhas : "");            
        Prism.plugins.lineHighlight.highlightLines(preCodigo, linhas, codeCodigo);
        Prism.highlightElement(codeCodigo);
    }

    /**
     * Monta o slide.
     */
    montaSlide(){
        const elementoSvg = Ilustrador.elementoSvg;

        if (this.#elementoCodigo != null){
            this.#elementoCodigo = document.getElementById('codigoSlide').cloneNode(true);
            let code = Ilustrador.procuraElementoFilhoPorId(this.#elementoCodigo, 'fonte');
            code.textContent = "";
        }
 
        if (this.#codigo != null){
            this.#elementoCodigo = document.getElementById('codigoSlide').cloneNode(true);
            let code = Ilustrador.procuraElementoFilhoPorId(this.#elementoCodigo, 'fonte');
            code.textContent = this.#codigo;

            //this.#elementoCodigo.setAttribute('x', Ilustrador.dimensoesGraficosSVG.largura*0.40);
            Ilustrador.xPadrao = Ilustrador.dimensoesGraficosSVG.largura*0.25 + Ilustrador.separacaoPadrao;

            Ilustrador.elementoSvg.appendChild(this.#elementoCodigo);
            Prism.highlightElement(code);
        }

        this.#eventoAtual = -1;
    }

    #mostraPostIt(texto){
        if (this.#postIt != null){
            this.#postIt.remove();
            this.#postIt = null;
        }
       
        this.#postIt = document.getElementById('umPostIt').cloneNode(true); 
        let htmlPostIt = Ilustrador.procuraElementoFilhoPorId(this.#postIt, 'htmlPostIt');
        let conteudoPostIT = htmlPostIt.querySelectorAll('div')[1];
        let retanguloPostIt = Ilustrador.procuraElementoFilhoPorId(this.#postIt, 'retanguloPostIt');

        conteudoPostIT.innerHTML = texto;
        let dadosTexto = Apresentador.calcularDimensoesTextoSVG(conteudoPostIT.textContent, Apresentador.tamanhoPadraoFonte, 4);
        let destacado = Ilustrador.procuraElementoFilhoPorId(this.#elementoCodigo, "code").getElementsByClassName(' line-highlight');

        if (destacado.length > 0){
            this.#postIt.setAttribute('transform', `translate(${destacado[0].getBoundingClientRect().x + destacado[0].getBoundingClientRect().width - dadosTexto.largura*1.1}, ${destacado[0].getBoundingClientRect().y + destacado[0].getBoundingClientRect().height - 10})`);
        }else{
            this.#postIt.setAttribute('transform', `translate(${this.#elementoCodigo.getBoundingClientRect().x + this.#elementoCodigo.getBoundingClientRect().width - dadosTexto.largura*1.1}, ${this.#elementoCodigo.getBoundingClientRect().y + this.#elementoCodigo.getBoundingClientRect().height*0.35})`);
        }


        htmlPostIt.setAttribute('width', `${dadosTexto.largura}`);
        htmlPostIt.setAttribute('height', `${dadosTexto.altura}`);
        retanguloPostIt.setAttribute('width', `${dadosTexto.largura}`);
        retanguloPostIt.setAttribute('height', `${dadosTexto.altura}`);

        Ilustrador.elementoSvg.appendChild(this.#postIt);
    }

    proximoEvento(){
        if (this.#eventoAtual >= 0){
            const eventoAtual = this.getEvento(this.#eventoAtual);
            eventoAtual.encerrar();
        }

        this.#eventoAtual++;
        if (this.#eventoAtual >= this.numeroDeEventos){
            return;
        }

        const evento = this.getEvento(this.#eventoAtual);
        if (evento.linhasDestaque != null){
            this.destacaLinhas(evento.linhasDestaque);
        }else{
            this.destacaLinhas(-1);
        }

        if (this.#postIt != null){
            this.#postIt.remove();
            this.#postIt = null;
        }

        if (evento.comentario != null){
            this.#mostraPostIt(evento.comentario);
        }
        evento.iniciar();
        evento.rodar(this);
    }

    removeSlide(){
        if (this.#eventoAtual >= 0){
            const eventoAtual = this.getEvento(this.#eventoAtual);
            if (eventoAtual != null){
                eventoAtual.encerrar();
            }
        }

        if (this.#elementoCodigo != null){
            this.#elementoCodigo = document.getElementById('codigoSlide').cloneNode(true);
            let code = Ilustrador.procuraElementoFilhoPorId(this.#elementoCodigo, 'fonte');
            code.textContent = "";

            //this.#elementoCodigo.remove();
            //this.#elementoCodigo = null;
        }

        if (this.#postIt != null){
            this.#postIt.remove();
            this.#postIt = null;
        }

    }

}

class Evento {
    #linhasDestaque;
    #comentario;
    #ilustracaoSlide;

    /**
     * Cria um evento de um slide, com linha de destaque, comentario e funcao ilustração.
     * 
     * @param {string|number} linhasDestaque 
     * @param {string} comentario 
     * @param {IlustracaoSlide} [ilustracaoSlide=null] 
     */
    constructor (linhasDestaque = -1, comentario = null, ilustracaoSlide = null){
        this.#linhasDestaque = linhasDestaque;
        this.#comentario = comentario;
        this.#ilustracaoSlide = ilustracaoSlide;
    }

    get linhasDestaque(){
        return this.#linhasDestaque;
    }

    get comentario(){
        return this.#comentario;
    }

    iniciar(){
        if (this.#ilustracaoSlide != null){
            this.#ilustracaoSlide.montar();
        }
    }

    /**
     * 
     * @param {IlustracaoSlide} slide 
     */
    rodar(slide){
        if (this.#ilustracaoSlide != null){
            this.#ilustracaoSlide.ilustrar(slide);
        }
    }

    encerrar(){
        if (this.#ilustracaoSlide != null){
            this.#ilustracaoSlide.parar();
            this.#ilustracaoSlide.limpar();
        }
    }
}