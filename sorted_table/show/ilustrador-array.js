class IlustracaoArray extends Ilustracao {
    static #tamanhoArrays = 1;
    static #animacaoFocoId = 0;
    static #animacaoFocoSeguirPara = -1;

    /**
     * Esse método deve ser chamado antes da utilização de Ilustrações Arrays para definir o tamanho dos arrays.
     * Atualmente todos os arrays terão o mesmo tamanho.
     * 
     * @param {number} tamanho quantidade de elementos de cada array
     */
    static ajustaDefinicoesGerais(tamanho) {
        //ajusta o array de acordo com o tamanho
        let larguraTotal = tamanho * Ilustrador.larguraPadraoRetangulo;
        let alturaTotal = Ilustrador.alturaPadraoRetangulo + 2*Ilustrador.alturaPadraoNomeVariavel;
        let defArray = document.getElementById("umArray");
        let defCaixaNome = document.getElementById("caixaNomeArray");
        let defNome = document.getElementById("textoNomeArray");
        let defEndereco = document.getElementById("caixaEnderecoArray");
        
        //define tamanho da caixa externa
        let defCaixaExterna = document.getElementById("caixaExternaArray");
        defCaixaExterna.setAttribute('width', larguraTotal);    

        //cria elementos a mais
        let defElemento = document.getElementById("umElementoArray");
        for(let i = 1; i < tamanho; i++){
            let outroElemento = defElemento.cloneNode(true);

            let xOriginal = parseFloat(Ilustrador.procuraElementoFilhoPorId(outroElemento, 'caixaExternaElementoArray').getAttribute('x')) || 0;
            let yOriginal = parseFloat(Ilustrador.procuraElementoFilhoPorId(outroElemento, 'caixaExternaElementoArray').getAttribute('y')) || 0;
            let xFinal = xOriginal + Ilustrador.larguraPadraoRetangulo*i ;
            Ilustrador.procuraElementoFilhoPorId(outroElemento, 'caixaExternaElementoArray').setAttribute('x', xFinal);
            Ilustrador.procuraElementoFilhoPorId(outroElemento, 'caixaExternaElementoArray').setAttribute('y', yOriginal);

            yOriginal = parseFloat(Ilustrador.procuraElementoFilhoPorId(outroElemento, 'caixaIndiceArray').getAttribute('y')) || 0;
            Ilustrador.procuraElementoFilhoPorId(outroElemento, 'caixaIndiceArray').setAttribute('x', xFinal);
            Ilustrador.procuraElementoFilhoPorId(outroElemento, 'caixaIndiceArray').setAttribute('y', yOriginal);

            xOriginal = parseFloat(Ilustrador.procuraElementoFilhoPorId(outroElemento, 'textoIndiceElementoArray').getAttribute('x')) || 0;
            yOriginal = parseFloat(Ilustrador.procuraElementoFilhoPorId(outroElemento, 'textoIndiceElementoArray').getAttribute('y')) || 0;
            xFinal = xOriginal + Ilustrador.larguraPadraoRetangulo*i;
            Ilustrador.procuraElementoFilhoPorId(outroElemento, 'textoIndiceElementoArray').setAttribute('x', xFinal);
            Ilustrador.procuraElementoFilhoPorId(outroElemento, 'textoIndiceElementoArray').setAttribute('y', yOriginal);
            Ilustrador.procuraElementoFilhoPorId(outroElemento, 'textoIndiceElementoArray').textContent = `${i}`;
            
            defArray.appendChild(outroElemento);
        }

        defArray.setAttribute('width', larguraTotal);
        defCaixaNome.setAttribute('width', larguraTotal-4);
        defNome.setAttribute('x', `${larguraTotal/2}`);
        //defEndereco.setAttribute('width', larguraTotal);

        IlustracaoArray.#tamanhoArrays = tamanho;
    }

    static get tamanhoArrays(){
        return IlustracaoArray.#tamanhoArrays;
    }

    #usoUmArray = null;
    #cloneGrupoTextoValorElemento = null;
    #cloneTextoValorElemento = null;
    #cloneTextoNome = null;
    #destaque = null;
    #animacaoFoco = null;
    
    #valoresElementos = null;

    /**
     * Cria uma Ilustração de um Array.
     * 
     * @param {number} id 
     * @param {string} nome 
     * @param {Array} valores 
     * @param {string} cor 
     * @param {boolean} [criarEmNovaLinha=false] 
     */
    constructor(id, nome, valores, cor, criarEmNovaLinha = false){
        super(id, nome, Ilustracao.novaPosicaoSugerida(IlustracaoArray.tamanhoArrays*Ilustrador.larguraPadraoRetangulo, Ilustrador.alturaPadraoRetangulo+2*Ilustrador.alturaPadraoNomeVariavel, criarEmNovaLinha), cor);

        //iniciar campos
        this.#usoUmArray = null;
        this.#cloneTextoNome = null;
        this.#destaque = null;
        this.#animacaoFoco = null;

        //criar elementos
        let novoGrupoPrincipal = document.createElementNS('http://www.w3.org/2000/svg','g');
        novoGrupoPrincipal.setAttribute('fill', this.cor);
        super.grupoPrincipal = novoGrupoPrincipal;

        let novoArray = document.createElementNS('http://www.w3.org/2000/svg','use');
        novoArray.setAttribute('href', '#umArray');
        this.#usoUmArray = novoArray;
        super.grupoPrincipal.appendChild(this.#usoUmArray);

        let novoTextoNome = document.getElementById("textoNomeArray").cloneNode(true);
        novoTextoNome.textContent = super.nome;
        super.grupoPrincipal.appendChild(novoTextoNome);
        this.#cloneTextoNome = novoTextoNome;

        //elementos
        this.#cloneGrupoTextoValorElemento = new Array();
        this.#cloneTextoValorElemento = new Array();
        this.#valoresElementos = new Array();
        for(let i = 0; i < valores.length; i++){       
            this.#valoresElementos.push(valores[i]);
            
            let novoGTextoValorElemento = document.getElementById("gTextoValorElementoArray").cloneNode(true);
            let novoValorElemento = Ilustrador.procuraElementoFilhoPorId(novoGTextoValorElemento, 'textoValorElementoArray');
            
            let xOriginal = parseFloat(novoValorElemento.getAttribute('x')) || 0;
            let yOriginal = parseFloat(novoValorElemento.getAttribute('y')) || 0;
            let xFinal = xOriginal + Ilustrador.larguraPadraoRetangulo*i;

            novoValorElemento.setAttribute('x', xFinal);
            novoValorElemento.setAttribute('y', yOriginal);
            novoValorElemento.textContent = this.getValorElemento(i) == -1 ? "" : this.getValorElemento(i);
            novoValorElemento.setAttribute('fill', super.cor);
            super.grupoPrincipal.appendChild(novoGTextoValorElemento);

            novoGTextoValorElemento.setAttribute('onclick', `Ilustracao.ilustracaoPorId(${this.id}).focarElemento(${i});`);
            novoGTextoValorElemento.setAttribute('style', "cursor: cell");

            this.#cloneGrupoTextoValorElemento.push(novoGTextoValorElemento);
            this.#cloneTextoValorElemento.push(novoValorElemento);
        }

        //posiciona no local correto, insere e ajusta textos 
        super.grupoPrincipal.setAttribute('transform', `translate(${this.x}, ${this.y})`);
        Ilustrador.elementoSvg.appendChild(super.grupoPrincipal);

        Ilustrador.autoAjustarTamanhoDoTexto(this.#cloneTextoNome, this.largura-5);
        for(let i = 0; i < this.#valoresElementos.length; i++){
            Ilustrador.autoAjustarTamanhoDoTexto(this.#cloneTextoValorElemento[i]);
        }

        this.animaSurgimento(super.grupoPrincipal);

        this.#usoUmArray.setAttribute('onclick', `Ilustracao.ilustracaoPorId(${this.id}).focar();`);
        this.#usoUmArray.setAttribute('style', "cursor: pointer");
    }

    /**
     * Recupera o valor do elemento no índice especificado.
     * 
     * @param {number} i o índice
     * @returns {*} 
     */
    getValorElemento(i){
        return this.#valoresElementos[i];
    }

    /**
     * Altera o valor do elemento no índice especificado.
     * 
     * @param {*} valor 
     * @param {number} i 
     */
    setValorElemento(valor, i){
        this.#valoresElementos[i] = valor;

        let novaAnimacao = document.getElementById("animacaoAlterar").cloneNode(true);
        this.#cloneGrupoTextoValorElemento[i].appendChild(novaAnimacao);
        novaAnimacao.beginElement();
        
        const timeoutId = setTimeout(() => {
                this.#cloneTextoValorElemento[i].textContent = this.#valoresElementos[i];
                this.#cloneTextoValorElemento[i].setAttribute('opacity', 100);
                Ilustrador.autoAjustarTamanhoDoTexto(this.#cloneTextoValorElemento[i]);
            }, Ilustrador.tempoPadraoAnimacoes/2);

        const timeoutId2 = setTimeout(() => {
                this.#cloneGrupoTextoValorElemento[i].removeChild(novaAnimacao);
            }, Ilustrador.tempoPadraoAnimacoes);        

        //this.destacaElementos(i, 1, 'skyblue', Ilustrador.tempoPadraoAnimacoes/2);
    }

    /**
     * Adiciona um destaque aos elementos da Ilustração Array que é auto removido de acordo com o tempo em milissegundos especificado.
     * O valor padrão 10 vezes o valor do tempo padrão para animações (Ilustrador.tempoPadraoAnimacoes). 
     * Informe o valor 0 (zero) para não remover automaticamente.
     * 
     * @param {*} iInicio índice inicial 
     * @param {number} [quantidade=1] quantidade de elementos do destaque
     * @param {string} [cor=Ilustrador.corPadraoDestaque] 
     * @param {number} [milisAutoRemover=Ilustrador.tempoPadraoAnimacoes/2] 
     */
    destacaElementos(iInicio, quantidade = 1, cor = Ilustrador.corPadraoDestaque, milisAutoRemover = Ilustrador.tempoPadraoAnimacoes/2){
        //remove destaque anterior
        //this.removeDestaque();

        //retangulo do destaque
        let novaCaixaDestaque = document.getElementById("caixaDestaque").cloneNode(true);
        //novaCaixaDestaque.setAttribute('href', "#caixaDestaque");
        novaCaixaDestaque.setAttribute('stroke', cor);
        novaCaixaDestaque.setAttribute('rx', 5);
        novaCaixaDestaque.setAttribute('fill', cor);
        novaCaixaDestaque.setAttribute('height', Ilustrador.larguraPadraoRetangulo-4);

        if (iInicio == -1){
            //retangulo ao redor de tudo
            novaCaixaDestaque.setAttribute('width', this.largura-4);
            novaCaixaDestaque.setAttribute('x', '2'); 
            novaCaixaDestaque.setAttribute('y', '2'); 
            //novaCaixaDestaque.setAttribute('fill', 'none');
        }else{
            //retangulo por cima dos elementos
            //novaCaixaDestaque.setAttribute('stroke-width','2');
            if (quantidade == 1){
                novaCaixaDestaque.setAttribute('width', Ilustrador.larguraPadraoRetangulo-4);
            }else{
                novaCaixaDestaque.setAttribute('width', `${(Ilustrador.larguraPadraoRetangulo-4)*parseInt(quantidade) + (parseInt(quantidade)-1)*4}`);    
                //console.debug(`|Array| tamanho do destaque: ${novaCaixaDestaque.getAttribute('width')}`);
            }
            novaCaixaDestaque.setAttribute('x', Ilustrador.larguraPadraoRetangulo*parseInt(iInicio)+2); //x do elemento inicial
            novaCaixaDestaque.setAttribute('y', 2); //y do elemento inicial
        }    

        this.#destaque = novaCaixaDestaque;
        super.grupoPrincipal.insertBefore(novaCaixaDestaque, this.#usoUmArray);

        if (milisAutoRemover != 0){
            const timeoutId = setTimeout(() => {
                    //this.removeDestaque();
                    novaCaixaDestaque.remove();
                }, milisAutoRemover);

        }
    }

    removeDestaque(){
        if (this.#destaque != null){
            this.#destaque.remove();
            this.#destaque = null;
        }
    }

    #animaCopiaDeElemento(iOrigem, iDestino, porCima = true, apagaOrigem = true){
        const raioCirculo = 5 / 1.5;
        const opacidadeCirculo = 0.50;

        let copiaTextoValorOrigem = this.#cloneTextoValorElemento[iOrigem].cloneNode(true);
        //novoGTextoValorElemento.setAttribute('onclick', `Ilustracao.ilustracaoPorId(${this.id}).focarElemento(${i});`);
        let circulo = document.getElementById("umCirculo").cloneNode(true);//document.createElementNS('http://www.w3.org/2000/svg','use'); //circulo da movimentacao
        circulo.setAttribute("cx", `${Ilustrador.larguraPadraoRetangulo/2 + iOrigem*Ilustrador.larguraPadraoRetangulo}`);
        circulo.setAttribute("cy", `${Ilustrador.alturaPadraoRetangulo/2}`);
        circulo.setAttribute("fill", copiaTextoValorOrigem.getAttribute('fill'));
        //circulo.setAttribute("fill-opacity", document.getElementById("caixaIndiceArray").getAttribute('fill-opacity'));
        circulo.setAttribute('stroke-dasharray', document.getElementById("caixaExternaElementoArray").getAttribute('stroke-dasharray'));
        //circulo.appendChild(copiaTextoValorOrigem);

        //criar animações que serão utilizadas
        //apagar e reaparecer
        let animacaoApagar = document.getElementById("animacaoSumir").cloneNode(true);
        animacaoApagar.setAttribute('dur', `${Ilustrador.tempoPadraoAnimacoes/4}ms`);
        let animacaoAparecer = document.getElementById("animacaoSurgir").cloneNode(true);
        animacaoAparecer.setAttribute('dur', animacaoApagar.getAttribute('dur'));

        //diminuicao e aumento do texto (valor)
        let animacaoDiminuiTexto = document.getElementById("animacaoTamanhoTexto").cloneNode(true);
        animacaoDiminuiTexto.setAttribute('dur', animacaoApagar.getAttribute('dur'));
        animacaoDiminuiTexto.setAttribute('values', `${this.#cloneTextoValorElemento[iOrigem].getAttribute('font-size')};${raioCirculo}`);
        let animacaoAumentaTexto = document.getElementById("animacaoTamanhoTexto").cloneNode(true);
        animacaoAumentaTexto.setAttribute('dur', animacaoApagar.getAttribute('dur'));
        animacaoAumentaTexto.setAttribute('values', `${raioCirculo};${this.#cloneTextoValorElemento[iOrigem].getAttribute('font-size')}`);

        //anima deslocamento vertical (subir/descer para o 'trilho')
        const yTrilho = porCima ? 0 : Ilustrador.alturaPadraoRetangulo+Ilustrador.alturaPadraoNomeVariavel;
        let animacaoSubir = document.getElementById("animacaoVertical").cloneNode(true);
        animacaoSubir.setAttribute('dur', animacaoApagar.getAttribute('dur'));
        animacaoSubir.setAttribute('values', `${Ilustrador.alturaPadraoRetangulo/2};${yTrilho}`);
        let animacaoDescer = document.getElementById("animacaoVertical").cloneNode(true);
        animacaoDescer.setAttribute('dur', animacaoApagar.getAttribute('dur'));
        animacaoDescer.setAttribute('values', `${yTrilho};${Ilustrador.alturaPadraoRetangulo/2}`);

        //animacao raio
        let animacaoRaio = document.getElementById("animacaoRaio").cloneNode(true);
        animacaoRaio.setAttribute('dur', animacaoApagar.getAttribute('dur'));
        animacaoRaio.setAttribute('values', `${0};${raioCirculo}`);
        
        //animacao vertical do circulo
        let animacaoSubirCirculo = animacaoSubir.cloneNode(true);
        animacaoSubirCirculo.setAttribute('attributeName', 'cy');
        let animacaoDescerCirculo = animacaoDescer.cloneNode(true);
        animacaoDescerCirculo.setAttribute('attributeName', 'cy');

        //animacao horizontal
        let animacaoTrilho = document.getElementById("animacaoHorizontal").cloneNode(true);
        animacaoTrilho.setAttribute('dur', `${Ilustrador.tempoPadraoAnimacoes/2}ms`); //.setAttribute('dur', `${0.25}`); //.setAttribute('dur', animacaoApagar.getAttribute('dur'));
        animacaoTrilho.setAttribute('values', `${this.#cloneTextoValorElemento[iOrigem].getAttribute('x')};${this.#cloneTextoValorElemento[iDestino].getAttribute('x')}`);
        animacaoTrilho.setAttribute('attributeName', 'cx');

        //adiciona e controla animacao
        copiaTextoValorOrigem.appendChild(animacaoApagar);
        copiaTextoValorOrigem.appendChild(animacaoDiminuiTexto);
        copiaTextoValorOrigem.appendChild(animacaoSubir);
        circulo.appendChild(animacaoAparecer);
        circulo.appendChild(animacaoRaio);
        circulo.appendChild(animacaoSubirCirculo);
        if (apagaOrigem){
            this.#cloneTextoValorElemento[iOrigem].appendChild(animacaoApagar);
            this.#cloneTextoValorElemento[iOrigem].appendChild(animacaoDiminuiTexto);
        }

        super.grupoPrincipal.appendChild(copiaTextoValorOrigem);
        this.grupoPrincipal.appendChild(circulo);

        animacaoApagar.beginElement();
        animacaoDiminuiTexto.beginElement();
        animacaoSubir.beginElement();
        animacaoAparecer.beginElement();
        animacaoRaio.beginElement();
        animacaoSubirCirculo.beginElement();

        const timeoutId = setTimeout(() => {
            //fim da primeira parte da animação
            //apagar animações e setar novos atributos para proxima etapa da animacao
            
            copiaTextoValorOrigem.setAttribute('opacity', `${0}`);
            copiaTextoValorOrigem.setAttribute('font-size', `${raioCirculo}`);
            copiaTextoValorOrigem.setAttribute('x', `${this.#cloneTextoValorElemento[iDestino].getAttribute('x')}`);
            copiaTextoValorOrigem.setAttribute('y', `${0}`);
            if (apagaOrigem){
                this.#cloneTextoValorElemento[iOrigem].setAttribute('opacity', `${0}`);
            }
            animacaoApagar.remove();
            animacaoDiminuiTexto.remove();
            animacaoSubir.remove();
            
            circulo.setAttribute('r', `${raioCirculo}`);
            circulo.setAttribute('opacity', `${100}`);
            circulo.setAttribute('cx', `${this.#cloneTextoValorElemento[iOrigem].getAttribute('x')}`);
            circulo.setAttribute('cy', `${yTrilho}`);
            animacaoAparecer.remove();
            animacaoRaio.remove();
            animacaoSubirCirculo.remove();

            circulo.appendChild(animacaoTrilho);
            animacaoTrilho.beginElement();

            animacaoApagar.setAttribute('dur', animacaoTrilho.getAttribute('dur'));
            animacaoDiminuiTexto.setAttribute('dur', animacaoApagar.getAttribute('dur'));
            if (this.#cloneTextoValorElemento[iDestino].getAttribute('opacity') != 0){
                this.#cloneTextoValorElemento[iDestino].appendChild(animacaoApagar);
                this.#cloneTextoValorElemento[iDestino].appendChild(animacaoDiminuiTexto);
                animacaoApagar.beginElement();
                animacaoDiminuiTexto.beginElement();
            }

            const timeoutId2 = setTimeout(() => {
                //fim da segunda etapa (trilho), falta descer o novo valor no local

                this.#cloneTextoValorElemento[iDestino].setAttribute('opacity', `${0}`);
                circulo.setAttribute('cx', `${this.#cloneTextoValorElemento[iDestino].getAttribute('x')}`);
                circulo.setAttribute('opacity', `${100}`);
                animacaoTrilho.remove();

                copiaTextoValorOrigem.appendChild(animacaoDescer);
                copiaTextoValorOrigem.appendChild(animacaoAparecer);
                copiaTextoValorOrigem.appendChild(animacaoAumentaTexto);
                this.#cloneTextoValorElemento[iDestino].appendChild(animacaoApagar);

                animacaoRaio.remove();
                animacaoRaio.setAttribute('values', `${raioCirculo};${0}`);
                circulo.appendChild(animacaoDescerCirculo);
                circulo.appendChild(animacaoApagar);
                circulo.appendChild(animacaoRaio);

                animacaoDescer.beginElement();
                animacaoAparecer.beginElement();
                animacaoAumentaTexto.beginElement();
                animacaoApagar.beginElement();
                animacaoDescerCirculo.beginElement();
                animacaoApagar.beginElement();
                animacaoRaio.beginElement();
                const timeoutId3 = setTimeout(() => {
                    //ultima etapa, definir resultados

                    copiaTextoValorOrigem.setAttribute('font-size', `${this.#cloneTextoValorElemento[iOrigem].getAttribute('font-size')}`);
                    copiaTextoValorOrigem.setAttribute('opacity', `${100}`);
                    copiaTextoValorOrigem.setAttribute('x', `${this.#cloneTextoValorElemento[iDestino].getAttribute('x')}`);
                    copiaTextoValorOrigem.setAttribute('y', `${this.#cloneTextoValorElemento[iDestino].getAttribute('y')}`);

                    copiaTextoValorOrigem.textContent = this.#valoresElementos[iDestino];
                    this.#cloneTextoValorElemento[iDestino].remove();
                    this.#cloneTextoValorElemento[iDestino] = copiaTextoValorOrigem;
                    this.#cloneGrupoTextoValorElemento[iDestino].appendChild(this.#cloneTextoValorElemento[iDestino]);                    

                    animacaoDescer.remove();
                    animacaoAparecer.remove();
                    animacaoAumentaTexto.remove();
                    animacaoApagar.remove();
                    animacaoDescerCirculo.remove();
                    animacaoApagar.remove();
                    animacaoRaio.remove();

                    circulo.remove();
                }, 125);
            }, 250);
        }, 125);

    }

    /**
     * Cópia o valor de um elemento para outro e anima esse deslocamento.
     * 
     * @param {number} iOrigem o índice do valor a ser copiado
     * @param {number} iDestino o índice de destino
     * @param {boolean} porCima define se a animação será por cima ou por baixo
     * @param {boolean} apagaOrigem define se o valor da origem será apagado
     */
    copiaElementoArray(iOrigem, iDestino, porCima = true, apagaOrigem = false){
        this.#valoresElementos[iDestino] = this.#valoresElementos[iOrigem];
        
        this.#animaCopiaDeElemento(iOrigem, iDestino, porCima, apagaOrigem);
    }

    permutaElementosArray(iOrigem, iDestino){
        let aux = this.#valoresElementos[iOrigem];
        this.#valoresElementos[iOrigem] = this.#valoresElementos[iDestino];
        this.#valoresElementos[iDestino] = aux;

        this.#animaCopiaDeElemento(iOrigem, iDestino);
        this.#animaCopiaDeElemento(iDestino, iOrigem, false);
    }   

    /**
     * Faz a interface focar em um elemento da dp array da Ilustração, movendo-se
     * para permitir a exibição caso seja necessário.
     * 
     * @param {number} iElemento o índice do elemento
     * @param {boolean} zoom se um zoom deve ser aplicado no elemento
     * @param {boolean} centralizar se o elemento deve ser centralizado mesmo que já esteja em tela
     * @returns 
     */
    focarElemento(iElemento, zoom = true, centralizar = false){
        //TODO: funcao que destaca/foca eventos (alerta, confirmacao, atencao, foco, erro)

        //this.destacaElementos(iElemento, 1, 'sandybrown', Ilustrador.tempoPadraoAnimacoes/2);

        if (!zoom && !centralizar && Ilustrador.ilustracaoEstaEmTela(this.#cloneGrupoTextoValorElemento[iElemento])){
            //console.debug(`|Array| ignocando foco no elemento [${iElemento}] já em tela`);
            return;
        }

        const escalaAtual = (Ilustrador.dimensoesGraficosSVG.largura)/ parseFloat(Ilustrador.elementoSvg.getAttribute('viewBox').split(' ')[2]);
        let escala = 1;
        if (zoom){ 
            //calcular escala de zoom de acordo com as dimensões da ilustração
            const escalaPadrao = 2;
            const proporcao = (Ilustrador.dimensoesGraficosSVG.largura - 5*Ilustrador.separacaoPadrao) / Ilustrador.larguraPadraoRetangulo;
            escala = proporcao > escalaPadrao ? escalaPadrao : proporcao; //para nao ficar muito perto, muito grande na tela
            escala = escalaAtual > escala ? escalaAtual : escala; //só dar zoom se estiver menor que a escala atual?
        }else{
            escala = escalaAtual;
        }
        
        const larguraFocada = Ilustrador.dimensoesGraficosSVG.largura / escala;
        const alturaFocada = Ilustrador.dimensoesGraficosSVG.altura / escala;
        const novaPosicao = new Dimensao(this.x - larguraFocada/2 + iElemento*Ilustrador.larguraPadraoRetangulo + Ilustrador.larguraPadraoRetangulo/2, this.y - alturaFocada/2 + Ilustrador.larguraPadraoRetangulo/2, larguraFocada, alturaFocada-2*Ilustrador.separacaoPadrao);

        let meuId = -1;
        if (this.#animacaoFoco == null){
            //console.debug(`|Array| criando animacao foco`);
            if (!centralizar && Ilustrador.ilustracaoEstaEmTela(this.#cloneGrupoTextoValorElemento[iElemento]) && escalaAtual >= escala){
                //console.debug(`|Array| ignorando a criação de animação para elemento já em tela em escala maior ou igual a pedida`);
                return;
            }
            meuId = ++IlustracaoArray.#animacaoFocoId;
            this.#animacaoFoco = document.getElementById("animacaoViewBox").cloneNode(true);
            this.#animacaoFoco.setAttribute('to', `${novaPosicao.x} ${novaPosicao.y} ${novaPosicao.largura} ${novaPosicao.altura}`);
            Ilustrador.elementoSvg.appendChild(this.#animacaoFoco);
            this.#animacaoFoco.beginElement();
        }else{
            IlustracaoArray.#animacaoFocoSeguirPara = iElemento;
            return;
        }

        const timeoutId = setTimeout(() => {
                if (meuId == IlustracaoArray.#animacaoFocoId && this.#animacaoFoco != null){
                    //console.debug(`|Array| finalizando animacao foco`);
                    Ilustrador.elementoSvg.setAttribute('viewBox', `${novaPosicao.x} ${novaPosicao.y} ${novaPosicao.largura} ${novaPosicao.altura}`);      
                    this.#animacaoFoco.remove();
                    this.#animacaoFoco = null;
                    if (IlustracaoArray.#animacaoFocoSeguirPara != -1){
                        this.focarElemento(IlustracaoArray.#animacaoFocoSeguirPara, zoom, centralizar);
                        IlustracaoArray.#animacaoFocoSeguirPara = -1;
                    }
                }
            }, Ilustrador.tempoPadraoAnimacoes);
    }    
}
