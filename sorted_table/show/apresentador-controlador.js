/*
Início Globais
*/

let linhasCodigo = new Array();
let codigoCompleto = null;
let tamanhoAtualFonte = 12;

let velocidadeExecucao = 50;
let unidadeTempoExecucao = 10;
let arrayIniciados = false;

let slides = null;
let slideAtual = -1;
let eventoAtual = 0;

function sleep(milissegundos) {
    return new Promise(resolve => setTimeout(resolve, milissegundos));
}

async function executa(){
    while(!respostaFoiVazia){
        proximoPasso();
        await sleep(unidadeTempoExecucao * velocidadeExecucao);
        //console.debug('aqui');
    }
    console.debug(`|Controlador| encerrando execução automática`);
}

async function alteraVelocidade(inc){
    velocidadeExecucao += inc;
    if (velocidadeExecucao < 1){
        velocidadeExecucao = 0; //max
        respostaFoiVazia = true;
    }
}

async function proximoPasso(){
    if (slideAtual < 0){
        slideAtual = 0;
        eventoAtual = 0;

        //console.debug('|Apresentador-Controlador| montando primeiro slide');
        slides[slideAtual].montaSlide();
        return;
    }

    if (eventoAtual < slides[slideAtual].numeroDeEventos){
        slides[slideAtual].proximoEvento();
        eventoAtual++;
    }else{
        //console.debug('|Apresentador-Controlador| removendo slide atual');
        slides[slideAtual].proximoEvento();
        slides[slideAtual].removeSlide();

        eventoAtual = 0;
        slideAtual++;
        if (slideAtual < slides.length){
            //console.debug('|Apresentador-Controlador| montando próximo slide');
            slides[slideAtual].montaSlide();
        }else{
            slideAtual = -1;
        }
    }
}

function processaCodigo(codigo){
    let code = document.getElementById('fonte');
    codigoCompleto = codigo.toString();
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
        indiceLinhaAqui++;
    }

    codigoCompleto = linhasAqui.join("\n");
    code.textContent = codigoCompleto;
    Prism.highlightElement(code);
}

function destacaLinhas(linhaInicial, linhaFinal = -1){
    let preCodigo = document.getElementById("code");
    let codeCodigo = document.getElementById("fonte");

    let linhaDestaque = linhaInicial;
    if (linhaFinal != -1){
        linhaDestaque = `${linhaInicial}-${linhaFinal}`;
    }

    preCodigo.setAttribute("data-line", linhaDestaque);            
    Prism.plugins.lineHighlight.highlightLines(preCodigo, linhaDestaque, codeCodigo);
    Prism.highlightElement(codeCodigo);
}

function aumentaFonte(){
    tamanhoAtualFonte += 1;
    document.querySelectorAll('pre[class*="language-"]').forEach(el => {
        el.style.fontSize = `${tamanhoAtualFonte}px`;
    });

}

function diminuiFonte(){
    tamanhoAtualFonte = tamanhoAtualFonte > 1 ? tamanhoAtualFonte-1 : tamanhoAtualFonte;
    document.querySelectorAll('pre[class*="language-"]').forEach(el => {
        el.style.fontSize = `${tamanhoAtualFonte}px`;
    });
    
}


function iniciaAula(){
    slides = aulaBuscaTabelaOrdenada();
    slideAtual = -1;
}

let contador = 0;
let minhaVariavel = null;
let meuArray = null;
let meuNo = null;

function teste1() {
    destacaLinhas(1, 10);

    let titulo = document.getElementById('tituloSlide');
    titulo.innerHTML = "<b>Conhecendo a estrutura de um código-fonte em C</b>"

    let conteudo = document.getElementById('conteudoSlide');
    conteudo.innerHTML = `<p>Um código-fonte normalmente inclui um texto de apresentação. Esses textos são incluídos no formato de <b><i>comentários de código</b></i>.</p>`;

    let preCode = document.getElementById('code'); 
    let destacado = preCode.getElementsByClassName(' line-highlight');
    console.debug(`destacado: ${destacado[0].getBoundingClientRect().y}`);

    let postit = document.getElementById('postIt').cloneNode(true);
    let htmlPostIt = Ilustrador.procuraElementoFilhoPorId(postit, 'htmlPostIt');
    let conteudoPostIT = htmlPostIt.querySelectorAll('div')[1];

    console.debug(`html: ${htmlPostIt} conteudo: ${conteudoPostIT.innerHTML}`);

    conteudoPostIT.innerHTML = `<p>Um código-fonte normalmente inclui um texto de apresentação. Esses textos são incluídos no formato de <b><i>comentários de código</b></i>.</p>`;
    //postit.setAttribute('x', destacado[0].getBoundingClientRect().x + destacado[0].getBoundingClientRect().width*0.90);
    //postit.setAttribute('y', destacado[0].getBoundingClientRect().y + destacado[0].getBoundingClientRect().height*0.90);
    postit.setAttribute('transform', `translate(${destacado[0].getBoundingClientRect().x + destacado[0].getBoundingClientRect().width*0.90}, ${destacado[0].getBoundingClientRect().y + destacado[0].getBoundingClientRect().height*0.90})`);

    let retanguloPostIt = Ilustrador.procuraElementoFilhoPorId(postit, 'retanguloPostIt');
    let dadosTexto = Apresentador.calcularDimensoesTextoSVG(conteudoPostIT.textContent, 16, 3);
    htmlPostIt.setAttribute('width', `${dadosTexto.largura}`);
    htmlPostIt.setAttribute('height', `${dadosTexto.altura}`);
    retanguloPostIt.setAttribute('width', `${dadosTexto.largura}`);
    retanguloPostIt.setAttribute('height', `${dadosTexto.altura}`);

    Ilustrador.elementoSvg.appendChild(postit);
}


async function teste2() {    
    const reader = new FileReader();
    reader.onload = function(e) {
    console.debug(`${e.target.result}`);
    };
    reader.readAsText('ilustradorC/ehPar.c');
}

async function teste3() {    
    for (let i = IlustracaoArray.tamanhoArrays-1; i > 0; i--){
        meuArray.focarElemento(i, false, false);
        meuArray.copiaElementoArray(i-1, i);
        await sleep(50);
        //await sleep(10);
    }
    meuArray.setValorElemento(Ilustrador.geraInteiroAleatorio(0, 20), 0);
}