
/*
Início Globais
*/
const alturaCaixaPadrao = 29;
const alturaNomeVariavel = 16;
const larguraCaixaPadrao = alturaCaixaPadrao + alturaNomeVariavel;
const separacaoPadrao = 8;
const xPadrao = separacaoPadrao;
const tamanhoValorPadrao = 16; 
const tamanhoNomePadrao = 10; 
const tracoCircular = '18,2,2,2';
const fimCircular = '24';
const duracaoCircular = '3s';

let objetos = new Array();
let linhasCodigo = new Array();
let codigoCompleto = null;

let animacaoAnterior = null;

let respostaFoiVazia = false;
let velocidadeExecucao = 20;

/*
Início Funções
*/
function distance(coordenadas){
    const dx = parseFloat(coordenadas.x2) - parseFloat(coordenadas.x1);
    const dy = parseFloat(coordenadas.y2) - parseFloat(coordenadas.y1);

    //console.log(`coord: ${JSON.stringify(coordenadas)}, distancia: ${Math.sqrt(dx * dx + dy * dy)}`);

    return Math.sqrt(dx * dx + dy * dy);
}

function calculatePath(coordenadas) {
    const center = {
        x: (parseFloat(coordenadas.x1) + parseFloat(coordenadas.x2)) / 2,
        y: (parseFloat(coordenadas.y1) + parseFloat(coordenadas.y2)) / 2,
    };

    const controlPoint = {
        x: coordenadas.x1,
        y: Math.abs(center.y + Math.min(distance(coordenadas), Math.abs(parseFloat(coordenadas.x2)-parseFloat(coordenadas.x1))/2, separacaoPadrao*2)*0.25)
    };

    //return `M ${coordenadas.x1},${coordenadas.y1} Q ${coordenadas.x1}, ${coordenadas.y1} ${center.x},${center.y} T ${coordenadas.x2},${coordenadas.y2}`
    return `M ${coordenadas.x1},${coordenadas.y1} Q ${controlPoint.x},${controlPoint.y} ${center.x},${center.y} T ${coordenadas.x2},${coordenadas.y2}`
}

function getChildById(elem, id){
    if (!elem || !elem.hasChildNodes()){
        return null;
    }

    for(let i = 0; i < elem.childNodes.length; i++){
        let child = elem.childNodes[i];
        if (child.nodeType == Node.ELEMENT_NODE && child.getAttribute('id') == id){
            return child;
        }
    }

    return null;
}

// Função para gerar uma cor aleatória em formato hexadecimal
function getRandomColor() {
    const letters = '0123456789ABCDEF';
    let color = '#';
    for (let i = 0; i < 6; i++) {
        color += letters[Math.floor(Math.random() * letters.length)];
    }
    return color;
}

// Função sleep que retorna uma Promise resolvida após um tempo específico
function sleep(ms) {
    return new Promise(resolve => setTimeout(resolve, ms));
}

function adjustTextToFit(elemento) {
    const rectWidth = larguraCaixaPadrao; 
    let fontSize = parseInt(elemento.getAttribute('font-size')); //20; // Tamanho inicial
    //console.log(`p1: rectWidth=${rectWidth}, fontSize=${fontSize}, getComputedTextLength=${elemento.getComputedTextLength()+2}`);
    
    let originalFontSize = elemento.getAttribute('originalFontSize');
    if (!originalFontSize){
        elemento.setAttribute("originalFontSize", fontSize);
    }else{
        elemento.setAttribute("font-size", originalFontSize);
        fontSize = originalFontSize;
    }
    // Reduz o font-size até o texto caber
    while (parseFloat(elemento.getComputedTextLength())+2 > parseFloat(rectWidth) && fontSize > 1) {
        //console.log(`p2: rectWidth=${rectWidth}, fontSize=${fontSize}, getComputedTextLength=${elemento.getComputedTextLength()+2}`);
        fontSize -= 1;
        elemento.setAttribute("font-size", fontSize);
        //console.log(`p3: rectWidth=${rectWidth}, fontSize=${fontSize}`);
    }
}

function calculaPosicao(){
    let qObjetos = objetos.length;
    let xAtual = xPadrao;
    let yAtual = qObjetos*(alturaCaixaPadrao + 2*alturaNomeVariavel + separacaoPadrao) + xPadrao;
    let ultimoIndice = -1;

    for(ultimoIndice = qObjetos-1; ultimoIndice >=0 ; ultimoIndice--){
        //procurando ultimo objeto sendo exibido
        if (!objetos[ultimoIndice].objetosSvg.removido){
            break;
        }
    }
    if (ultimoIndice >= 0 && objetos[ultimoIndice].tipo == 'VAR'){
        //mesma linha no final 
        xAtual = objetos[ultimoIndice].objetosSvg.xAtual + separacaoPadrao + parseInt(objetos[ultimoIndice].objetosSvg.largura);
        yAtual = objetos[ultimoIndice].objetosSvg.yAtual;
        if (xAtual > 900){
            xAtual = xPadrao;
            yAtual = objetos[ultimoIndice].objetosSvg.yAtual + objetos[ultimoIndice].objetosSvg.altura + 3*separacaoPadrao;
        }
    }

    let posicao = {
        x: xAtual,
        y: yAtual
    };

    return posicao;
}

function removeDestaques(){
    for (let i = 0; i < objetos.length; i++){
        if (objetos[i].objetosSvg.destaque != null){
            objetos[i].objetosSvg.destaque.remove();
            objetos[i].objetosSvg.destaque = null;
        }    
        if (objetos[i].objetosSvg.destaqueGeral != null){
            objetos[i].objetosSvg.destaqueGeral.remove();
            objetos[i].objetosSvg.destaqueGeral = null;
        }    
        if (objetos[i].objetosSvg.destaqueElementos != null){
            objetos[i].objetosSvg.destaqueElementos.remove();
            objetos[i].objetosSvg.destaqueElementos = null;
        }    
    }
}

function animaSurgimento(elemento){
    var novaAnimacao = document.getElementById("animacaoSurgir").cloneNode(true);
    elemento.appendChild(novaAnimacao);
    novaAnimacao.beginElement();

    const timeoutId = setTimeout(() => {
        elemento.removeChild(novaAnimacao);
    }, 500);
}

function animaAlteracao(gElemento, novoValor, objetosSvg = null){
    var novaAnimacao = document.getElementById("animacaoAlterar").cloneNode(true);
    gElemento.appendChild(novaAnimacao);
    novaAnimacao.beginElement();

    const timeoutId = setTimeout(() => {
        gElemento.childNodes[1].textContent = novoValor;
        adjustTextToFit(gElemento.childNodes[1]);

        if (objetosSvg && objetosSvg.tipoVariavel && objetosSvg.tipoVariavel == 'PONTEIRO'){
            //procura objeto
            let posicao = encontraPosicaoApontado(novoValor);
            if (posicao){
                //cria seta
                let yDeste = objetosSvg.yAtual;
                let yDestino = posicao.y;
    
                let coordenadas = null;
                if (yDestino < yDeste){
                    //aponta pra cima
                    //console.log('cima');
                    coordenadas = geraCoordenadasMovimento(objetosSvg.xAtual + larguraCaixaPadrao/2, objetosSvg.yAtual, `${posicao.x+larguraCaixaPadrao/4}`, `${yDestino+alturaCaixaPadrao}`);
                }else{
                    coordenadas = geraCoordenadasMovimento(objetosSvg.xAtual + larguraCaixaPadrao/2, objetosSvg.yAtual+alturaCaixaPadrao+alturaNomeVariavel, `${posicao.x+larguraCaixaPadrao/4}`, yDestino);
                }
                let path = calculatePath(coordenadas);
                //console.log(`coordenadas: ${JSON.stringify(coordenadas)}, path: ${path}`);

                let pathOriginal = null;
                if (objetosSvg.setaPonteiro){
                    pathOriginal = objetosSvg.setaPonteiro.getAttribute('d');
                }

                if (pathOriginal){
                    let novaAnimacaoSetaPonteiro = document.getElementById("animacaoSetaVarPonteiro").cloneNode(true);
                    novaAnimacaoSetaPonteiro.setAttribute('values', `${pathOriginal}; ${path}`);
                    objetosSvg.setaPonteiro.appendChild(novaAnimacaoSetaPonteiro);
                    novaAnimacaoSetaPonteiro.beginElement();
                    const timeoutAnimacao = setTimeout(() => {
                        objetosSvg.setaPonteiro.setAttribute('d', path);
                        objetosSvg.setaPonteiro.removeChild(novaAnimacaoSetaPonteiro);
                    }, 500);
                }else{
                    let novaSetaPonteiro = document.getElementById("setaVarPonteiro").cloneNode(true);
                    novaSetaPonteiro.setAttribute('d', path);
                    svg.appendChild(novaSetaPonteiro);
                    objetosSvg.setaPonteiro = novaSetaPonteiro;
                }
            }else{
                console.log('-> objeto para onde ponteiro vai napontar não encontrado.');
                if (objetosSvg.setaPonteiro){
                    objetosSvg.setaPonteiro.remove();
                    objetosSvg.setaPonteiro = null;
                }
            }
            
        }
        }, 250);

    const timeoutId2 = setTimeout(() => {
        gElemento.removeChild(novaAnimacao);
    }, 500);

}

function encontraPosicaoApontado(endereco){
    //console.log(`procurando end: ${endereco}`);
    for(let i = 0; i < objetos.length; i++){
        //console.log(`comparando com: [${objetos[i].tipo}] ${objetos[i].endereco}`);
        if (objetos[i].tipo == 'ARRAY_INT'){
            for (let j = 0; j < objetos[i].tamanho; j++){
                //console.log(`comparando com: ${objetos[i].endereco[j]}`);
                if (endereco == objetos[i].endereco[j]){
                    //return objetos[i].objetosSvg.valoresElementos[j]; 
                    let posicao = {
                        x: objetos[i].objetosSvg.xAtual+j*larguraCaixaPadrao,
                        y: objetos[i].objetosSvg.yAtual
                    };
                    return posicao;
                }
            }
        }
        if (endereco == objetos[i].endereco){
            //return objetos[i].objetosSvg.textoValor;
            let posicao = {
                x: objetos[i].objetosSvg.xAtual,
                y: objetos[i].objetosSvg.yAtual
            };
            return posicao;
}
    }
    return null;
}

function geraCoordenadasMovimento(x1, y1, x2, y2){
    let coordenadas = {
        x1: x1,
        y1: y1, 
        x2: x2,
        y2: y2
    };

    return coordenadas;
}

function animaMovimentacaoInterna(gElemento, coordenadas){
    let xOriginal = parseFloat(gElemento.childNodes[1].getAttribute('x')) || 0;
    //let yOriginal = parseFloat(gElemento.childNodes[1].getAttribute('y')) || 0;
    let xFinal = xOriginal + coordenadas.x2;

    var animacao = document.getElementById("animacaoMover").cloneNode(true);
    animacao.setAttribute('values', `${coordenadas.x1},${coordenadas.y1}; ${coordenadas.x1},${coordenadas.y2}; ${coordenadas.x2},${coordenadas.y2}; ${coordenadas.x2},${coordenadas.y1}`);
    gElemento.appendChild(animacao);
    animacao.beginElement();

    const timeoutId = setTimeout(() => {
        //atualiza x e y
        gElemento.childNodes[1].setAttribute('x', xFinal);
        //gElemento.setAttribute('transform', 'translate(0,0)');
        gElemento.removeChild(animacao);
    }, 500);

}

function criaElementosNovaVariavel(novoObjetoVariavel){
    const svg = document.getElementById("graficosSVG");

    let qObjetos = objetos.length;
    let umaCor = getRandomColor();
    let posicao = calculaPosicao();

    
    let objetosSvg = {
        tipo: 'VAR',
        tipoVariavel: novoObjetoVariavel.tipoVariavel,
        xAtual: posicao.x,
        yAtual: posicao.y,
        largura: larguraCaixaPadrao,
        altura: alturaCaixaPadrao + alturaNomeVariavel,
        cor: umaCor,
        grupo: null,
        umaVariavel: null,
        gTextoValor: null,
        textoValor: null,
        textoNome: null,
        emEscopo: true,
        removido: false
    };

    let novoGrupo = document.createElementNS('http://www.w3.org/2000/svg','g');
    novoGrupo.setAttribute('transform', `translate(${objetosSvg.xAtual}, ${objetosSvg.yAtual})`);
    novoGrupo.setAttribute('fill', objetosSvg.cor);
    objetosSvg.grupo = novoGrupo;

    let novaVar = document.createElementNS('http://www.w3.org/2000/svg','use');
    novaVar.setAttribute('href', '#umaVariavel');
    objetosSvg.umaVariavel = novaVar;
    novoGrupo.appendChild(novaVar);

    //posiciona no local correto e insere
    novoGrupo.setAttribute('transform', `translate(${objetosSvg.xAtual}, ${objetosSvg.yAtual})`);
    svg.appendChild(novoGrupo); 

    let novoTextoNome = document.getElementById("textoNomeVar").cloneNode(true);
    novoTextoNome.textContent = novoObjetoVariavel.nome;
    novoGrupo.appendChild(novoTextoNome);
    adjustTextToFit(novoTextoNome);
    objetosSvg.textoNome = novoTextoNome;

    let novoEndereco = document.getElementById("caixaEnderecoVar").cloneNode(true);
    getChildById(novoEndereco, 'enderecoVar').textContent = `Endereço: ${novoObjetoVariavel.endereco}`;
    novoGrupo.appendChild(novoEndereco);

    let novoGTextoValor = document.getElementById("gTextoValorVar").cloneNode(true);
    let novoTextoValor = novoGTextoValor.childNodes[1];
    novoTextoValor.textContent = novoObjetoVariavel.valor;
    objetosSvg.gTextoValor = novoGTextoValor;
    objetosSvg.textoValor = novoTextoValor;
    novoGrupo.appendChild(novoGTextoValor);
    adjustTextToFit(novoTextoValor);

    animaSurgimento(novoGrupo);

    if (novoObjetoVariavel.tipoVariavel == 'PONTEIRO'){
        //procura objeto
        let posicao = encontraPosicaoApontado(novoObjetoVariavel.valor);
        if (posicao){
            //cria seta
            let yDeste = objetosSvg.yAtual;
            let yDestino = posicao.y;

            let coordenadas = null;
            if (yDestino < yDeste){
                //aponta pra cima
                //console.log('cima');
                coordenadas = geraCoordenadasMovimento(objetosSvg.xAtual + larguraCaixaPadrao/2, objetosSvg.yAtual, `${posicao.x+larguraCaixaPadrao/4}`, `${yDestino+alturaCaixaPadrao}`);
            }else{
                coordenadas = geraCoordenadasMovimento(objetosSvg.xAtual + larguraCaixaPadrao/2, objetosSvg.yAtual+alturaCaixaPadrao+alturaNomeVariavel, `${posicao.x+larguraCaixaPadrao/4}`, yDestino);
            }
            let path = calculatePath(coordenadas);
            //console.log(`coordenadas: ${JSON.stringify(coordenadas)}, path: ${path}`);
            let novaSetaPonteiro = document.getElementById("setaVarPonteiro").cloneNode(true);
            novaSetaPonteiro.setAttribute('d', path);
            svg.appendChild(novaSetaPonteiro);
            animaSurgimento(novaSetaPonteiro);
            objetosSvg.setaPonteiro = novaSetaPonteiro;
        }else{
            console.log('-> objeto para onde ponteiro apontar não encontrado.');
            if (objetosSvg.setaPonteiro){
                objetosSvg.setaPonteiro.remove();
            }
        }
    }

    return objetosSvg;
}

function destacaVariavel(objetoVar){
    //terminaAnimacaoAnterior();
    const svg = document.getElementById("graficosSVG");

    //remove destaque anterior
    if (objetoVar.objetosSvg.destaque != null){
        objetoVar.objetosSvg.destaque.remove();
        objetoVar.objetosSvg.destaque = null;
    }

    //retangulo do destaque
    let novaCaixaDestaque = document.createElementNS('http://www.w3.org/2000/svg','use');
    novaCaixaDestaque.setAttribute('href', "#caixaDestaque");
    novaCaixaDestaque.setAttribute('fill', 'lightblue');

    objetoVar.objetosSvg.destaque = novaCaixaDestaque;
    objetoVar.objetosSvg.grupo.appendChild(novaCaixaDestaque);
}  

function defineEscopoVariavel(objetoVar, escopoAtual = true){
    if (escopoAtual){
        objetoVar.objetosSvg.grupo.setAttribute('opacity', '1');
        objetoVar.objetosSvg.grupo.setAttribute('fill', objetoVar.objetosSvg.cor);
        objetoVar.objetosSvg.emEscopo = true;
    }else{
        objetoVar.objetosSvg.grupo.setAttribute('opacity', '0.25');
        objetoVar.objetosSvg.grupo.setAttribute('fill', 'light-grey');
        objetoVar.objetosSvg.emEscopo = false;
    }

}

function criaElementosNovoArray(novoObjetoArray){
    if ( typeof criaElementosNovoArray.ajustado == 'undefined' ) {
        //ajusta o array de acordo com o tamanho
        const tam = novoObjetoArray.tamanho;
        let larguraTotal = (tam)*larguraCaixaPadrao;
        let alturaTotal = alturaCaixaPadrao + alturaNomeVariavel + separacaoPadrao;
        let defArray = document.getElementById("umArray");
        let defCaixaNome = document.getElementById("caixaNomeArray");
        let defNome = document.getElementById("textoNomeArray");
        let defEndereco = document.getElementById("caixaEnderecoArray");
        
        //define tamanho da caixa externa
        let defCaixaExterna = document.getElementById("caixaExternaArray");
        defCaixaExterna.setAttribute('width', larguraTotal);    

        //cria elementos a mais
        let defElemento = document.getElementById("umElementoArray");
        for(let i = 1; i < tam; i++){
            let outroElemento = defElemento.cloneNode(true);

            let xOriginal = parseFloat(getChildById(outroElemento, 'caixaExternaElementoArray').getAttribute('x')) || 0;
            let yOriginal = parseFloat(getChildById(outroElemento, 'caixaExternaElementoArray').getAttribute('y')) || 0;
            let xFinal = xOriginal + larguraCaixaPadrao*i;
            //outroElemento.setAttribute('transform', `translate(${larguraCaixaPadrao*i}, 0)`);
            getChildById(outroElemento, 'caixaExternaElementoArray').setAttribute('x', xFinal);
            getChildById(outroElemento, 'caixaExternaElementoArray').setAttribute('y', yOriginal);

            yOriginal = parseFloat(getChildById(outroElemento, 'caixaIndiceArray').getAttribute('y')) || 0;
            getChildById(outroElemento, 'caixaIndiceArray').setAttribute('x', xFinal);
            getChildById(outroElemento, 'caixaIndiceArray').setAttribute('y', yOriginal);

            xOriginal = parseFloat(getChildById(outroElemento, 'textoIndiceElementoArray').getAttribute('x')) || 0;
            yOriginal = parseFloat(getChildById(outroElemento, 'textoIndiceElementoArray').getAttribute('y')) || 0;
            xFinal = xOriginal + larguraCaixaPadrao*i;
            getChildById(outroElemento, 'textoIndiceElementoArray').setAttribute('x', xFinal);
            getChildById(outroElemento, 'textoIndiceElementoArray').setAttribute('y', yOriginal);
            getChildById(outroElemento, 'textoIndiceElementoArray').textContent = `${i}`;
            
            defArray.appendChild(outroElemento);
        }

        defArray.setAttribute('width', larguraTotal);
        defCaixaNome.setAttribute('width', larguraTotal);
        defCaixaNome.setAttribute('stroke-dasharray', `0,${larguraTotal},${larguraTotal+2*alturaNomeVariavel}`)
        defNome.setAttribute('x', `${larguraTotal/2}`);
        defEndereco.setAttribute('width', larguraTotal);
        
        criaElementosNovoArray.ajustado = true;
    }
    
    const svg = document.getElementById("graficosSVG");

    let umaCor = getRandomColor();
    let posicao = calculaPosicao();
    const tam = novoObjetoArray.tamanho;

    let objetosSvg = {
        tipo: 'ARRAY_INT',
        xAtual: posicao.x,
        yAtual: posicao.y,
        largura: larguraCaixaPadrao*novoObjetoArray.tamanho,
        altura: alturaCaixaPadrao + alturaNomeVariavel,
        cor: umaCor,
        grupo: null,
        umArray: null,
        gValoresElementos: new Array(),
        valoresElementos: new Array(),
        textoNome: null,
        emEscopo: true,
        removido: false
    };

    
    //grupo para posicoes relativas através do transform
    let novoGrupo = document.createElementNS('http://www.w3.org/2000/svg','g');

    novoGrupo.setAttribute('transform', `translate(${objetosSvg.xAtual}, ${objetosSvg.yAtual})`);
    novoGrupo.setAttribute('fill', objetosSvg.cor);
    objetosSvg.grupo = novoGrupo;
    svg.appendChild(novoGrupo); 

    let novoArray = document.createElementNS('http://www.w3.org/2000/svg','use');
    novoArray.setAttribute('href', '#umArray');
    objetosSvg.umArray = novoArray;
    novoGrupo.appendChild(novoArray);
    
    var novoTextoNome = document.getElementById("textoNomeArray").cloneNode(true);
    novoTextoNome.textContent = novoObjetoArray.nome;
    adjustTextToFit(novoTextoNome);
    novoGrupo.appendChild(novoTextoNome);
    objetosSvg.textoNome = novoTextoNome;

    let novoEndereco = document.getElementById("caixaEnderecoArray").cloneNode(true);
    getChildById(novoEndereco, 'enderecoArray').textContent = `Endereço: ${novoObjetoArray.endereco[0]}`;
    novoGrupo.appendChild(novoEndereco);

    svg.appendChild(novoGrupo);

    //elementos
    for(let i = 0; i < tam; i++){        
        let novoGTextoValorElemento = document.getElementById("gTextoValorElementoArray").cloneNode(true);
        let novoValorElemento = getChildById(novoGTextoValorElemento, 'textoValorElementoArray');
        
        let xOriginal = parseFloat(novoValorElemento.getAttribute('x')) || 0;
        let yOriginal = parseFloat(novoValorElemento.getAttribute('y')) || 0;
        let xFinal = xOriginal + larguraCaixaPadrao*i;
        novoValorElemento.setAttribute('x', xFinal);
        novoValorElemento.setAttribute('y', yOriginal);
        novoValorElemento.textContent = novoObjetoArray.array[i] == -1 ? "" : novoObjetoArray.array[i];
        //novoGTextoValorElemento.setAttribute('transform', `translate(${larguraCaixaPadrao*i}, 0)`);
        novoGrupo.appendChild(novoGTextoValorElemento);
        adjustTextToFit(novoValorElemento);

        objetosSvg.gValoresElementos.push(novoGTextoValorElemento);
        objetosSvg.valoresElementos.push(novoValorElemento);
3333
        let novoEndereco = document.getElementById("caixaEnderecoElementoArray").cloneNode(true);
        getChildById(novoEndereco, 'enderecoElementoArray').textContent = `Endereço: ${novoObjetoArray.endereco[i]}`;
        novoEndereco.setAttribute('x', `${novoEndereco.getAttribute('x') + larguraCaixaPadrao*i}`);
        novoGrupo.appendChild(novoEndereco);
    }

    animaSurgimento(novoGrupo);

    return objetosSvg;
}

function destacaElementosArray(objetoArray, ini, q, cor){
    //retangulo da variavel
    let novaCaixaDestaque = document.getElementById("caixaDestaque").cloneNode(true);
    novaCaixaDestaque.setAttribute('stroke', cor);
    if (ini == -1){
        //retangulo ao redor de tudo
        novaCaixaDestaque.setAttribute('width', objetoArray.objetosSvg.largura);
        novaCaixaDestaque.setAttribute('x', '0'); 
        novaCaixaDestaque.setAttribute('y', '0'); 
        novaCaixaDestaque.setAttribute('fill', 'none');

        if (objetoArray.objetosSvg.destaqueGeral != null){
            objetoArray.objetosSvg.destaqueGeral.remove();
            objetoArray.objetosSvg.destaqueGeral = null;
        }
        objetoArray.objetosSvg.destaqueGeral = novaCaixaDestaque;
    }else{
        //retangulo por cima dos elementos
        novaCaixaDestaque.setAttribute('stroke-width','2');
        novaCaixaDestaque.setAttribute('width', larguraCaixaPadrao*parseInt(q));
        novaCaixaDestaque.setAttribute('x', larguraCaixaPadrao*parseInt(ini)); //x do elemento inicial
        novaCaixaDestaque.setAttribute('y', 0); //y do elemento inicial
        novaCaixaDestaque.setAttribute('fill', cor);

        if (objetoArray.objetosSvg.destaqueElementos != null){
            objetoArray.objetosSvg.destaqueElementos.remove();
            objetoArray.objetosSvg.destaqueElementos = null;
        }
        objetoArray.objetosSvg.destaqueElementos = novaCaixaDestaque;
    }    

    objetoArray.objetosSvg.grupo.appendChild(novaCaixaDestaque);
}

async function deslocaElementoArray(objetoArray, i, j){
    const svg = document.getElementById("graficosSVG");
    let diferenca = j - i;
    let coordenadas = geraCoordenadasMovimento(0, 0, 45*diferenca, 0-alturaCaixaPadrao/2);
    let xOriginal = parseFloat(objetoArray.objetosSvg.valoresElementos[i].getAttribute('x')) || 0;
    let xFinal = xOriginal + coordenadas.x2;
    let auxG = objetoArray.objetosSvg.gValoresElementos[j];
    let auxV = objetoArray.objetosSvg.valoresElementos[j];

    //console.log(`xorig=${xOriginal}, xfim=${xFinal}`);
    //console.log('c e1: ' + JSON.stringify(coordenadas));
    animaMovimentacaoInterna(objetoArray.objetosSvg.gValoresElementos[i], coordenadas);

    objetoArray.array[j] = objetoArray.array[i];
    objetoArray.objetosSvg.gValoresElementos[j] = objetoArray.objetosSvg.gValoresElementos[i];
    objetoArray.objetosSvg.valoresElementos[j] = objetoArray.objetosSvg.valoresElementos[i];

    objetoArray.objetosSvg.gValoresElementos[i] = auxG;
    objetoArray.objetosSvg.valoresElementos[i] = auxV;
    objetoArray.objetosSvg.valoresElementos[i].setAttribute('x', xOriginal);
    //objetoArray.objetosSvg.valoresElementos[i].textContent = "ok";
}

async function permutaElementosArray(objetoArray, i, j){
    const svg = document.getElementById("graficosSVG");

    //setAttribute('transform', `translate(${larguraCaixaPadrao*i}, 0)`);
    let diferenca = j - i;

    let coordenadas = geraCoordenadasMovimento(0, 0, 45*diferenca, 0-alturaCaixaPadrao/2);
    //console.log('c e1: ' + JSON.stringify(coordenadas));
    animaMovimentacaoInterna(objetoArray.objetosSvg.gValoresElementos[i], coordenadas, objetoArray.objetosSvg.valoresElementos[j].textContent.toString());

    coordenadas = geraCoordenadasMovimento(0, 0, 45*diferenca*-1, 0+alturaCaixaPadrao/2);
    //console.log('c e2: ' + JSON.stringify(coordenadas));
    animaMovimentacaoInterna(objetoArray.objetosSvg.gValoresElementos[j], coordenadas, objetoArray.objetosSvg.valoresElementos[i].textContent.toString());

    let aux = objetoArray.array[i];
    objetoArray.array[i] = objetoArray.array[j];
    objetoArray.array[j] = aux;

    aux = objetoArray.objetosSvg.gValoresElementos[i];
    //console.log('aux1: ' + aux.childNodes[1].textContent);
    objetoArray.objetosSvg.gValoresElementos[i] = objetoArray.objetosSvg.gValoresElementos[j];
    objetoArray.objetosSvg.gValoresElementos[j] = aux;
    //console.log('aux2: ' + aux.childNodes[1].textContent);
    //console.log('i: ' + objetoArray.objetosSvg.gValoresElementos[i].childNodes[1].textContent);

    aux = objetoArray.objetosSvg.valoresElementos[i];
    objetoArray.objetosSvg.valoresElementos[i] = objetoArray.objetosSvg.valoresElementos[j];
    objetoArray.objetosSvg.valoresElementos[j] = aux;
}

async function mostraMensagem(texto, alerta){
    const svg = document.getElementById("graficosSVG");

    let cor = alerta ? 'lightyellow' : 'lightblue';

    //retangulo da variavel
    let novaCaixaDestaque = document.createElementNS('http://www.w3.org/2000/svg','rect');
    novaCaixaDestaque.setAttribute('stroke', cor);
    novaCaixaDestaque.setAttribute('stroke-opacity', '1');
    novaCaixaDestaque.setAttribute('fill-opacity', '0.6');

    novaCaixaDestaque.setAttribute('stroke-width','2');
    novaCaixaDestaque.setAttribute('width', '300');
    novaCaixaDestaque.setAttribute('height', '50');
    novaCaixaDestaque.setAttribute('x', '100'); //x do elemento inicial
    novaCaixaDestaque.setAttribute('y', '250'); //y do elemento inicial
    novaCaixaDestaque.setAttribute('fill', cor);
    svg.appendChild(novaCaixaDestaque);

    let novoTextoMensagem = document.createElementNS('http://www.w3.org/2000/svg','text');
    novoTextoMensagem.setAttribute('x', `${5 + 100 + 150}`);
    novoTextoMensagem.setAttribute('y', `${250 + 50/2}`);
    novoTextoMensagem.setAttribute('fill', 'black');
    novoTextoMensagem.setAttribute('font-size', '20');
    novoTextoMensagem.setAttribute('font-weight', 'bold');
    novoTextoMensagem.setAttribute('font-family', 'Arial');
    novoTextoMensagem.setAttribute('text-anchor', 'middle');
    novoTextoMensagem.setAttribute('dominant-baseline', 'middle');
    novoTextoMensagem.textContent = texto;
    svg.appendChild(novoTextoMensagem);

    //animacao do retangulo
    let novaAnimacao = document.createElementNS('http://www.w3.org/2000/svg','animate');
    novaAnimacao.setAttribute('attributeName','fill-opacity');
    novaAnimacao.setAttribute('values','0.35;0.50;0.70;0.50;0.35');
    novaAnimacao.setAttribute('dur','4s');
    novaAnimacao.setAttribute('repeatCount','indefinite');
    novaCaixaDestaque.appendChild(novaAnimacao);
    novaAnimacao.beginElement();

    await sleep(4000);
    novoTextoMensagem.remove();
    novaCaixaDestaque.remove();
}

async function processaProximoPasso(){
    //console.log("resposta servidor: " + this.responseText);
    removeDestaques();
    //document.getElementById("log").innerText = this.responseText;

    if (this.responseText == ""){
        respostaFoiVazia = true;
    }else{
        respostaFoiVazia = false;
    }
    
    let linhas = this.responseText.toString().split("\n");
    for(let i = 0; i < linhas.length; i++){
        if (linhas[i].trim() == ""){
            continue;
        }

        let partes = linhas[i].split("|");

        if (partes[0] == "NOVO"){
            //procurar se ja tem
            let jaPresente = false;
            let idObj = parseInt(partes[2]);
            for(let i = 0; i < objetos.length; i++){
                if (idObj == objetos[i].id){
                    jaPresente = true;
                    break;
                }
            }
            if (jaPresente){
                continue;
            }

            let novoObjeto = {
                id: idObj,
                tipo: partes[1],
                nome: partes[3],
                objetosSvg: null
            };

            if (novoObjeto.tipo == "ARRAY_INT"){
                novoObjeto.tamanho = parseInt(partes[4]);
                novoObjeto.array = new Array();
                novoObjeto.endereco = new Array();
                for(let i = 0; i < novoObjeto.tamanho; i++){
                    novoObjeto.array[i] = parseInt(partes[i+5]);
                }
                for(let i = 0; i < novoObjeto.tamanho; i++){
                    novoObjeto.endereco[i] = partes[i+5+novoObjeto.tamanho];
                }
                let objetoResultado = criaElementosNovoArray(novoObjeto);
                novoObjeto.objetosSvg = objetoResultado;
            }else if (novoObjeto.tipo == "NO_MULTI"){
                novoObjeto.grau = parseInt(partes[4]);
                novoObjeto.qFilhos = parseInt(partes[5]);
                novoObjeto.arrayChaves = new Array();
                for(let i = 6; i < partes.length; i++){
                    novoObjeto.arrayChaves.push(parseInt(partes[i]));
                }
                for (let i = novoObjeto.arrayChaves.length; i < novoObjeto.grau-1; i++){
                    novoObjeto.arrayChaves[i] = -1;
                }
                novoObjeto.arrayIdFilhos = new Array();
                for(let i = 0; i < novoObjeto.grau; i++){
                    novoObjeto.arrayIdFilhos[i] = -1;
                }
                let objetoResultado = criaElementosNovoNoMulti(novoObjeto);
                novoObjeto.objetosSvg = objetoResultado;
            }else if (novoObjeto.tipo == "VAR"){
                novoObjeto.tipoVariavel = partes[4];
                novoObjeto.valor = partes[5];
                novoObjeto.endereco = partes[6];
                let objetoResultado = criaElementosNovaVariavel(novoObjeto);
                novoObjeto.objetosSvg = objetoResultado;
            }

            if (!jaPresente){
                objetos.push(novoObjeto);
            }else{
                console.log('ja tinha obj');
            }
        }else if (partes[0] == "ATUALIZA"){
            let idObj = parseInt(partes[2]);
            
            let objeto = null;
            for(let i = 0; i < objetos.length; i++){
                if (idObj == objetos[i].id){
                objeto = objetos[i];
                break;
                }
            }

            if (objeto == null){
                console.error('Não foi possível encontrar um objeto com id = ' + idObj);
                return;
            }

            if (objeto.tipo == "VAR"){
                objeto.valor = partes[4];
                //console.log(objeto);
                //objeto.objetosSvg.textoValor.textContent = objeto.valor;
                animaAlteracao(objeto.objetosSvg.gTextoValor, objeto.valor, objeto.objetosSvg);
                destacaVariavel(objeto);
            }else if (objeto.tipo == "ARRAY_INT"){
                objeto.indiceElemento = parseInt(partes[3]);
                objeto.valorElemento = parseInt(partes[4]);
                objeto.array[objeto.indiceElemento] = objeto.valorElemento;
                //objeto.objetosSvg.valoresElementos[objeto.indiceElemento].textContent =  objeto.valorElemento;
                animaAlteracao(objeto.objetosSvg.gValoresElementos[objeto.indiceElemento], objeto.valorElemento);
                destacaElementosArray(objeto, objeto.indiceElemento, 1, 'lightblue');
            }else if (objeto.tipo == "NO_MULTI"){
                objeto.indiceElemento = parseInt(partes[3]);
                objeto.valorElemento = parseInt(partes[4]);
                if (objeto.indiceElemento < 0){
                    //atualizacao do numero de filhos
                    let qFilhos = objeto.indiceElemento * -1;
                    objeto.objetosSvg.textoFilhos.textContent = qFilhos;
                    //objeto.objetosSvg.caixasFilhos[qFilhos-1].setAttribute('fill', 'white');
                }else{
                    //atualizacao de uma chave
                    objeto.arrayChaves[objeto.indiceElemento] = objeto.valorElemento;
                    objeto.objetosSvg.valoresElementos[objeto.indiceElemento].textContent =  objeto.valorElemento;
                    destacaElementosNoMulti(objeto, objeto.indiceElemento, 1, 'blue');
                }
            }
        }else if (partes[0] == "DESTAQUE"){
            let idObj = parseInt(partes[2]);
            
            let objeto = null;
            for(let i = 0; i < objetos.length; i++){
                if (idObj == objetos[i].id){
                objeto = objetos[i];
                break;
                }
            }

            if (objeto == null){
                console.error('Não foi possível encontrar um objeto com id = ' + idObj);
                return;
            }

            if (objeto.tipo == "ARRAY_INT"){
                //destaque em elementos de um array, cria retangulo q some
                destacaElementosArray(objeto, parseInt(partes[3]), parseInt(partes[4]), partes[5]);
            }else if (objeto.tipo == "NO_MULTI"){
                //destaque em elementos de um no ou o no inteiro
                destacaElementosNoMulti(objeto, parseInt(partes[3]), parseInt(partes[4]), partes[5]);
            }
        }else if (partes[0] == "TIRA_DESTAQUE"){
            let idObj = parseInt(partes[2]);
            
            let objeto = null;
            for(let i = 0; i < objetos.length; i++){
                if (idObj == objetos[i].id){
                objeto = objetos[i];
                break;
                }
            }

            if (objeto == null){
                console.error('Não foi possível encontrar um objeto com id = ' + idObj);
                return;
            }
            if (objeto.tipo == "VAR"){
                if (objeto.objetosSvg.destaque != null){
                    objeto.objetosSvg.destaque.remove();
                    objeto.objetosSvg.destaque = null;
                }
            }else if (objeto.tipo == "ARRAY_INT"){
                //destaque em elementos de um array, cria retangulo q some
                if (objeto.objetosSvg.destaque != null){
                    objeto.objetosSvg.destaque.remove();
                    objeto.objetosSvg.destaque = null;
                }
            }else if (objeto.tipo == "NO_MULTI"){
                if (parseInt(partes[3]) < 0){
                //remove destaque geral
                if (objeto.objetosSvg.destaqueGeral != null){
                    objeto.objetosSvg.destaqueGeral.remove();
                    objeto.objetosSvg.destaqueGeral = null;
                }
                }else{
                //remore destaque elementos
                if (objeto.objetosSvg.destaqueElementos != null){
                    objeto.objetosSvg.destaqueElementos.remove();
                    objeto.objetosSvg.destaqueElementos = null;
                }
                }
            }
        }else if (partes[0] == "PERMUTA"){
            let idObj = parseInt(partes[2]);
            
            let objeto = null;
            for(let i = 0; i < objetos.length; i++){
                if (idObj == objetos[i].id){
                objeto = objetos[i];
                break;
                }
            }

            if (objeto == null){
                console.error('Não foi possível encontrar um objeto com id = ' + idObj);
                return;
            }

            if (objeto.tipo == "ARRAY_INT"){
                //destaque em elementos de um array, cria retangulo q some
                await permutaElementosArray(objeto, parseInt(partes[3]), parseInt(partes[4]));
            }
        }else if (partes[0] == "DESLOCA"){
            let idObj = parseInt(partes[2]);
            
            let objeto = null;
            for(let i = 0; i < objetos.length; i++){
                if (idObj == objetos[i].id){
                objeto = objetos[i];
                break;
                }
            }

            if (objeto == null){
                console.error('Não foi possível encontrar um objeto com id = ' + idObj);
                return;
            }
            if (objeto.tipo == "ARRAY_INT"){
                //mover elementos array
                await deslocaElementoArray(objeto, parseInt(partes[3]), parseInt(partes[4]));
            }else if (objeto.tipo == "NO_MULTI"){
                //destaque em elementos de um array, cria retangulo q some
                deslocaElementosNoMulti(objeto, parseInt(partes[3]), parseInt(partes[4]));
            }
        }else if (partes[0] == "LINHA"){
            let linhaDestaque = linhasCodigo[parseInt(partes[1])-1]+1;
            //console.log('destacar linha: ' + linhaDestaque);
            let preCodigo = document.getElementById("code");
            let codeCodigo = document.getElementById("fonte");
            //codeCodigo.textContent = codigoCompleto;
            preCodigo.setAttribute("data-line", linhaDestaque);            
            Prism.plugins.lineHighlight.highlightLines(preCodigo, linhaDestaque, codeCodigo);
            Prism.highlightElement(codeCodigo);
            //scrollToLine(linhaDestaque);
        }else if (partes[0] == "REMOVE"){
            let idObj = parseInt(partes[1]);
            for(let i = 0; i < objetos.length; i++){
                if (objetos[i].id == idObj){
                    //achou objeto para remover
                    if (objetos[i].objetosSvg.setaPonteiro){
                        objetos[i].objetosSvg.setaPonteiro.remove();
                        objetos[i].objetosSvg.setaPonteiro = null;
                    }
                    objetos[i].objetosSvg.grupo.remove();
                    objetos[i].objetosSvg.removido = true;
                }
            }
        }else if (partes[0] == "ESCOPO"){
            let idObj = parseInt(partes[1]);
            let escopoAtual = parseInt(partes[2]);
            for(let i = 0; i < objetos.length; i++){
                if (objetos[i].id == idObj){
                //achou objeto para escopo
                    defineEscopoVariavel(objetos[i], escopoAtual);
                }
            }
        }else if (partes[0] == "ALERTA" || partes[0] == "SAIDA"){
            let texto = partes[1];
            let alerta = partes[0] == "ALERTA";
            mostraMensagem(texto, alerta);
        }else if (partes[0] == "LIGA"){
            //LIGA|idPai|indiceFilho|idFilho
            let idPai = parseInt(partes[1]);
            let indiceFilho = parseInt(partes[2]);
            let idFilho = parseInt(partes[3]);

            let svgPai = null;
            let svgFilho = null;
            let achou = (idFilho == -1) ? 1 : 0;
            //procurar pai e filho
            for(let i = 0; i < objetos.length; i++){
                if (objetos[i].id == idPai){
                svgPai = objetos[i];
                achou++;
                }
                if (objetos[i].id == idFilho){
                svgFilho = objetos[i];
                achou++;
                }
            }
            if (svgFilho == null){
                //svgPai.objetosSvg.caixasFilhos[indiceFilho].setAttribute('fill', 'white');
                svgPai.objetosSvg.idFilhos[indiceFilho] = '-1';
            }else{
                //svgPai.objetosSvg.caixasFilhos[indiceFilho].setAttribute('fill', svgFilho.objetosSvg.caixaInfo.getAttribute('fill'));
                svgPai.objetosSvg.idFilhos[indiceFilho] = idFilho;

                //seta
                let seta = svgPai.objetosSvg.setasFilhos[indiceFilho];
                let posX = indiceFilho == svgPai.objetosSvg.grau-1 ? larguraCaixaPadrao*indiceFilho-8 : (indiceFilho == 0) ? 0 : larguraCaixaPadrao*indiceFilho-4;
                posX += svgPai.objetosSvg.xAtual + 4;
        
                let x1 = posX;
                let y1 = svgPai.objetosSvg.yAtual+57;
                
                let x2 = svgFilho.objetosSvg.xAtual + larguraCaixaPadrao/2;
                let y2 = svgFilho.objetosSvg.yAtual;
                seta.setAttribute('points', `${x1},${y1} ${x2},${y2}`);
                svgFilho.setaPai = seta;

                const svg = document.getElementById("graficosSVG");
                //svg.appendChild(novaSeta);
            }
        }else if (partes[0] == "ORGANIZA"){
            //ORGANIZA|NO_MULTI|idRaiz|alturaArvore
            let idRaiz = parseInt(partes[2]);
            let altura = parseInt(partes[3]);
            
            let objeto = null;
            for(let i = 0; i < objetos.length; i++){
                if (idRaiz == objetos[i].id){
                objeto = objetos[i];
                break;
                }
            }

            if (objeto == null){
                console.error('Não foi possível encontrar um objeto com id = ' + idObj);
                return;
            }

            reorganizaArvoreMulti(objeto, altura);
        }

        //await sleep(125);
    }
}

async function executa(){
    while(!respostaFoiVazia){
        const req = new XMLHttpRequest();
        req.addEventListener("load", processaProximoPasso);
        req.open("GET", "http://127.0.0.1:8080/prox");
        req.send();
        await sleep(velocidadeExecucao * 25);            
    }
}

async function alteraVelocidade(inc){
    velocidadeExecucao += inc;
    if (velocidadeExecucao < 1){
        velocidadeExecucao = 20;
        respostaFoiVazia = true;
    }
}

async function proximoPasso(){
    const req = new XMLHttpRequest();
    req.addEventListener("load", processaProximoPasso);
    req.open("GET", "http://127.0.0.1:8080/prox");
    req.send();
}

async function registraLog(){
    document.getElementById("log").innerText = this.responseText.toString();
}

async function iniciaRodada(){
    const req = new XMLHttpRequest();
    req.addEventListener("load", registraLog);
    req.open("GET", "http://127.0.0.1:8080/novaRodada");
    req.send();
}

async function processaCodigo(){
    //console.log("resposta servidor: " + this.responseText);
    let code = document.getElementById('fonte');
    codigoCompleto = this.responseText.toString();
    let linhas = codigoCompleto.split("\n");
    let linhasAqui = new Array();
    let indiceLinhaAqui = 0;
    for(let i = 0; i < linhas.length; i++){
        if (linhas[i].search("Ilustra") != -1 /*&& linhas[i].search("\"Ilustrador.h\"") == -1*/){
            linhasCodigo.push(-1);
            //console.log(`linha ${i} -> -1 : ${linhas[i]}`);
            continue;
        }
        linhasAqui.push(linhas[i]);
        linhasCodigo.push(indiceLinhaAqui);
        //console.log(`linha ${i} -> ${indiceLinhaAqui} : ${linhas[i]}`);
        indiceLinhaAqui++;
    }

    codigoCompleto = linhasAqui.join("\n");
    code.textContent = codigoCompleto;
    Prism.highlightElement(code);
}

async function carregarCodigo(){
    const req = new XMLHttpRequest();
    req.addEventListener("load", processaCodigo);
    req.open("GET", "http://127.0.0.1:8080/codigo");
    req.send();
}