
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

function criaElementosNovaVariavel(novoObjetoVariavel){
    const svg = document.getElementById("graficosSVG");

    let qObjetos = objetos.length;
    let umaCor = getRandomColor();
    let posicao = calculaPosicao();

    
    let objetosSvg = {
        tipo: 'VAR',
        tipoVariavel: 'INT',
        xAtual: posicao.x,
        yAtual: posicao.y,
        largura: larguraCaixaPadrao,
        altura: alturaCaixaPadrao + alturaNomeVariavel,
        cor: umaCor,
        grupo: null,
        umaVariavel: null,
        textoValor: null,
        textoNome: null,
        emEscopo: true,
        removido: false
    };

    //grupo para posicoes relativas através do transform
    //<g transform="translate(50, 50)">
    //<use href="#umaVariavel" x="10" y="10" transform="translate(20, 50)"/>    document.g
    //<g>
    //<use href="#umaVariavel" />
    //<text x="22.5" y="15" fill="black" font-size="16" font-family="Arial" font-weight="bold" text-anchor="middle" dominant-baseline="middle" textContent="2">3</text>
    //</g>
    let novoGrupo = document.createElementNS('http://www.w3.org/2000/svg','g');
    novoGrupo.setAttribute('transform', `translate(${objetosSvg.xAtual}, ${objetosSvg.yAtual})`);
    novoGrupo.setAttribute('fill', objetosSvg.cor);
    objetosSvg.grupo = novoGrupo;

    let novaVar = document.createElementNS('http://www.w3.org/2000/svg','use');
    novaVar.setAttribute('href', '#umaVariavel');
    objetosSvg.umaVariavel = novaVar;
    novoGrupo.appendChild(novaVar);

    var novoTextoNome = document.getElementById("textoNomeVar").cloneNode(true);
    novoTextoNome.textContent = novoObjetoVariavel.nome;
    novoGrupo.appendChild(novoTextoNome);
    objetosSvg.textoNome = novoTextoNome;

    var novoTextoValor = document.getElementById("textoValorVar").cloneNode(true);
    novoTextoValor.textContent = novoObjetoVariavel.valor;
    novoGrupo.appendChild(novoTextoValor);
    objetosSvg.textoValor = novoTextoValor;

    //posiciona no local correto e insere
    novoGrupo.setAttribute('transform', `translate(${objetosSvg.xAtual}, ${objetosSvg.yAtual})`);
    svg.appendChild(novoGrupo); 

    var novaAnimacao = document.getElementById("animacaoSurgir").cloneNode(true);
    novoGrupo.appendChild(novaAnimacao);
    novaAnimacao.beginElement();

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
    novaCaixaDestaque.setAttribute('fill', 'blue');

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
        
        //define tamanho da caixa externa
        let defCaixaExterna = document.getElementById("caixaExternaArray");
        defCaixaExterna.setAttribute('width', larguraTotal);

        //cria elementos a mais
        let defElemento = document.getElementById("umElementoArray");
        for(let i = 1; i < tam; i++){
            let outroElemento = defElemento.cloneNode(true);
            outroElemento.setAttribute('transform', `translate(${larguraCaixaPadrao*i}, 0)`);
            getChildById(outroElemento, 'textoIndiceElementoArray').textContent = `${i}`;
            defArray.appendChild(outroElemento);
        }

        defArray.setAttribute('width', larguraTotal);
        defCaixaNome.setAttribute('width', larguraTotal);
        defCaixaNome.setAttribute('stroke-dasharray', `0,${larguraTotal},${larguraTotal+2*alturaNomeVariavel}`)
        defNome.setAttribute('x', `${larguraTotal/2}`);
        
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
    novoGrupo.appendChild(novoTextoNome);
    objetosSvg.textoNome = novoTextoNome;

    //elementos
    for(let i = 0; i < tam; i++){        
        let novoValorElemento = document.getElementById("textoValorElementoArray").cloneNode(true);
        novoValorElemento.textContent = novoObjetoArray.array[i] == -1 ? "" : novoObjetoArray.array[i];
        novoValorElemento.setAttribute('transform', `translate(${larguraCaixaPadrao*i}, 0)`);
        novoGrupo.appendChild(novoValorElemento);
        objetosSvg.valoresElementos.push(novoValorElemento);
    }

    svg.appendChild(novoGrupo);

    var novaAnimacao = document.getElementById("animacaoSurgir").cloneNode(true);
    novoGrupo.appendChild(novaAnimacao);
    novaAnimacao.beginElement();

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

function permutaElementosArray(objetoArray, i, j){
    //implementar animacoes que tb servirao pros multis

    //terminaAnimacaoAnterior();
    const svg = document.getElementById("graficosSVG");

    /*
    let caixaElementoI = objetoArray.objetosSvg.caixasElementos[i];
    let textoElementoI = objetoArray.objetosSvg.valoresElementos[i];
    let caixaElementoJ = objetoArray.objetosSvg.caixasElementos[j];
    let textoElementoJ = objetoArray.objetosSvg.valoresElementos[j];

    //animacao 
    let xi = parseFloat(objetoArray.objetosSvg.caixasElementos[i].getAttribute('x'));
    let yi = parseFloat(objetoArray.objetosSvg.caixasElementos[i].getAttribute('y'));
    let xj = parseFloat(objetoArray.objetosSvg.caixasElementos[j].getAttribute('x'));
    let yj = parseFloat(objetoArray.objetosSvg.caixasElementos[j].getAttribute('y'));
    let xti = parseFloat(objetoArray.objetosSvg.valoresElementos[i].getAttribute('x'));
    let yti = parseFloat(objetoArray.objetosSvg.valoresElementos[i].getAttribute('y'));
    let xtj = parseFloat(objetoArray.objetosSvg.valoresElementos[j].getAttribute('x'));
    let ytj = parseFloat(objetoArray.objetosSvg.valoresElementos[j].getAttribute('y'));
    let largura = parseFloat(objetoArray.objetosSvg.caixasElementos[i].getAttribute('width'));
    let altura = parseFloat(objetoArray.objetosSvg.caixasElementos[i].getAttribute('height'));

    //i vertical
    let animacaoVerticalI = document.createElementNS('http://www.w3.org/2000/svg','animate');
    animacaoVerticalI.setAttribute('attributeName','y');
    animacaoVerticalI.setAttribute('values', `${yi};${yi-altura/2};${yi}`);
    animacaoVerticalI.setAttribute('dur','0.5s');
    animacaoVerticalI.setAttribute('repeatCount','1');
    animacaoVerticalI.setAttribute('fill','freeze');
    objetoArray.objetosSvg.caixasElementos[i].appendChild(animacaoVerticalI);
    let animacaoVerticalTI = document.createElementNS('http://www.w3.org/2000/svg','animate');
    animacaoVerticalTI.setAttribute('attributeName','y');
    animacaoVerticalTI.setAttribute('values', `${yti};${yti-altura/2};${yti}`);
    animacaoVerticalTI.setAttribute('dur','0.5s');
    animacaoVerticalTI.setAttribute('repeatCount','1');
    animacaoVerticalTI.setAttribute('fill','freeze');
    objetoArray.objetosSvg.valoresElementos[i].appendChild(animacaoVerticalTI);

    //i horizontal
    let animacaoHorizontalI = document.createElementNS('http://www.w3.org/2000/svg','animate');
    animacaoHorizontalI.setAttribute('attributeName','x');
    animacaoHorizontalI.setAttribute('values', `${xi};${xj}`);
    animacaoHorizontalI.setAttribute('dur','0.5s');
    animacaoHorizontalI.setAttribute('repeatCount','1');
    animacaoHorizontalI.setAttribute('fill','freeze');
    objetoArray.objetosSvg.caixasElementos[i].appendChild(animacaoHorizontalI);
    let animacaoHorizontalTI = document.createElementNS('http://www.w3.org/2000/svg','animate');
    animacaoHorizontalTI.setAttribute('attributeName','x');
    animacaoHorizontalTI.setAttribute('values', `${xti};${xtj}`);
    animacaoHorizontalTI.setAttribute('dur','0.5s');
    animacaoHorizontalTI.setAttribute('repeatCount','1');
    animacaoHorizontalTI.setAttribute('fill','freeze');
    objetoArray.objetosSvg.valoresElementos[i].appendChild(animacaoHorizontalTI);

    //j vertital
    let animacaoVerticalJ = document.createElementNS('http://www.w3.org/2000/svg','animate');
    animacaoVerticalJ.setAttribute('attributeName','y');
    animacaoVerticalJ.setAttribute('values', `${yj};${yi+altura/2};${yj}`);
    animacaoVerticalJ.setAttribute('dur','0.5s');
    animacaoVerticalJ.setAttribute('repeatCount','1');
    animacaoVerticalJ.setAttribute('fill','freeze');
    objetoArray.objetosSvg.caixasElementos[j].appendChild(animacaoVerticalJ);
    let animacaoVerticalTJ = document.createElementNS('http://www.w3.org/2000/svg','animate');
    animacaoVerticalTJ.setAttribute('attributeName','y');
    animacaoVerticalTJ.setAttribute('values', `${ytj};${yti+altura/2};${ytj}`);
    animacaoVerticalTJ.setAttribute('dur','0.5s');
    animacaoVerticalTJ.setAttribute('repeatCount','1');
    animacaoVerticalTJ.setAttribute('fill','freeze');
    objetoArray.objetosSvg.valoresElementos[j].appendChild(animacaoVerticalTJ);

    //j horizontal
    let animacaoHorizontalJ = document.createElementNS('http://www.w3.org/2000/svg','animate');
    animacaoHorizontalJ.setAttribute('attributeName','x');
    animacaoHorizontalJ.setAttribute('values', `${xj};${xi}`);
    animacaoHorizontalJ.setAttribute('dur','0.5s');
    animacaoHorizontalJ.setAttribute('repeatCount','1');
    animacaoHorizontalJ.setAttribute('fill','freeze');
    objetoArray.objetosSvg.caixasElementos[j].appendChild(animacaoHorizontalJ);
    let animacaoHorizontalTJ = document.createElementNS('http://www.w3.org/2000/svg','animate');
    animacaoHorizontalTJ.setAttribute('attributeName','x');
    animacaoHorizontalTJ.setAttribute('values', `${xtj};${xti}`);
    animacaoHorizontalTJ.setAttribute('dur','0.5s');
    animacaoHorizontalTJ.setAttribute('repeatCount','1');
    animacaoHorizontalTJ.setAttribute('fill','freeze');
    objetoArray.objetosSvg.valoresElementos[j].appendChild(animacaoHorizontalTJ);

    console.log(`[debug] antes de animar: xti=${objetoArray.objetosSvg.valoresElementos[i].getAttribute('x')}`);

    animacaoVerticalI.beginElement();
    animacaoVerticalTI.beginElement();
    animacaoHorizontalI.beginElement();
    animacaoHorizontalTI.beginElement();

    animacaoVerticalJ.beginElement();
    animacaoVerticalTJ.beginElement();
    animacaoHorizontalJ.beginElement();
    animacaoHorizontalTJ.beginElement();

    let aux = objetoArray.objetosSvg.caixasElementos[i].getAttribute('x');
    objetoArray.objetosSvg.caixasElementos[i].setAttribute('x', objetoArray.objetosSvg.caixasElementos[j].getAttribute('x'));
    objetoArray.objetosSvg.caixasElementos[j].setAttribute('x', aux);
    aux = objetoArray.objetosSvg.valoresElementos[i].getAttribute('x');
    objetoArray.objetosSvg.valoresElementos[i].setAttribute('x', objetoArray.objetosSvg.valoresElementos[j].getAttribute('x'));
    objetoArray.objetosSvg.valoresElementos[j].setAttribute('x', aux);

    console.log(`[debug] depois de animar: xti=${objetoArray.objetosSvg.valoresElementos[i].getAttribute('x')}`);
    */
    //permuta valores
    /*
    let aux = objetoArray.objetosSvg.caixasElementos[i].textContent;
    objetoArray.objetosSvg.caixasElementos[i] = objetoArray.objetosSvg.caixasElementos[j];
    objetoArray.objetosSvg.caixasElementos[j].textContent = aux;
    */

    //console.log(`[debug] antes de permutar: xti=${objetoArray.objetosSvg.valoresElementos[i].getAttribute('x')}`);

    //console.log(`destino: ${i}, origem: ${j}, len: ${objetoArray.array.length}`);
    let aux = objetoArray.objetosSvg.valoresElementos[i].textContent.toString();
    objetoArray.objetosSvg.valoresElementos[i].textContent = objetoArray.objetosSvg.valoresElementos[j].textContent.toString();
    objetoArray.objetosSvg.valoresElementos[j].textContent = aux;

    aux = objetoArray.array[i];
    objetoArray.array[i] = objetoArray.array[j];
    objetoArray.array[j] = aux;
    //console.log(`[debug] depois de permutar: xti=${objetoArray.objetosSvg.valoresElementos[i].getAttribute('x')}`);

    //permutar nos arrays
    /*
    let aux = objetoArray.objetosSvg.caixasElementos[i];
    objetoArray.objetosSvg.caixasElementos[i] = objetoArray.objetosSvg.caixasElementos[j];
    objetoArray.objetosSvg.caixasElementos[j] = aux;

    aux = objetoArray.objetosSvg.valoresElementos[i];
    objetoArray.objetosSvg.valoresElementos[i] = objetoArray.objetosSvg.valoresElementos[j];
    objetoArray.objetosSvg.valoresElementos[j] = aux;
    */

}

async function mostraMensagem(texto, alerta){
    const svg = document.getElementById("graficosSVG");

    let cor = alerta ? 'yellow' : 'blue';

    //retangulo da variavel
    let novaCaixaDestaque = document.createElementNS('http://www.w3.org/2000/svg','rect');
    novaCaixaDestaque.setAttribute('stroke', cor);
    novaCaixaDestaque.setAttribute('stroke-opacity', '1');
    novaCaixaDestaque.setAttribute('fill-opacity', '0.6');

    novaCaixaDestaque.setAttribute('stroke-width','2');
    novaCaixaDestaque.setAttribute('width', '300');
    novaCaixaDestaque.setAttribute('height', '50');
    novaCaixaDestaque.setAttribute('x', '100'); //x do elemento inicial
    novaCaixaDestaque.setAttribute('y', '50'); //y do elemento inicial
    novaCaixaDestaque.setAttribute('fill', cor);
    svg.appendChild(novaCaixaDestaque);

    let novoTextoMensagem = document.createElementNS('http://www.w3.org/2000/svg','text');
    novoTextoMensagem.setAttribute('x', `${5 + 100 + 150}`);
    novoTextoMensagem.setAttribute('y', `${50 + 50/2}`);
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
            for(let i = 5; i < partes.length; i++){
            novoObjeto.array[i-5] = parseInt(partes[i]);
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
            objeto.objetosSvg.textoValor.textContent = objeto.valor;
            destacaVariavel(objeto);
        }else if (objeto.tipo == "ARRAY_INT"){
            objeto.indiceElemento = parseInt(partes[3]);
            objeto.valorElemento = parseInt(partes[4]);
            objeto.array[objeto.indiceElemento] = objeto.valorElemento;
            objeto.objetosSvg.valoresElementos[objeto.indiceElemento].textContent =  objeto.valorElemento;
            destacaElementosArray(objeto, objeto.indiceElemento, 1, 'blue');
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
                permutaElementosArray(objeto, parseInt(partes[3]), parseInt(partes[4]));
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

            if (objeto.tipo == "NO_MULTI"){
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