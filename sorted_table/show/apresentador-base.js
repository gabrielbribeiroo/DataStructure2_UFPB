class Apresentador {
    static #tamanhoPadraoFonte = 16;

    static get tamanhoPadraoFonte(){
        return Apresentador.#tamanhoPadraoFonte;
    }

    /**
     * 
     * @param {string} texto 
     * @param {number} fontSize 
     * @param {number} numLinhas 
     * @param {number} padding 
     * @returns 
     */    
    static calcularDimensoesTextoSVG(texto, fontSize = Apresentador.#tamanhoPadraoFonte, numLinhas = 4, padding = 10) {
        // Divide o texto em linhas estimadas
        const linhas = [];
        let linhaAtual = '';
        
        // Simulação básica de quebra de palavras (simplificado)
        texto.split(' ').forEach(palavra => {
            if ((linhaAtual + palavra).length * (fontSize * 0.6) > fontSize * 20) { // 20 caracteres por linha estimado
            linhas.push(linhaAtual);
            linhaAtual = palavra;
            } else {
            linhaAtual += (linhaAtual ? ' ' : '') + palavra;
            }
        });
        if (linhaAtual) linhas.push(linhaAtual);
        
        // Calcula a maior linha
        const maiorLinha = linhas.reduce((max, linha) => Math.max(max, linha.length), 0);
        
        return {
            largura: Math.ceil(maiorLinha * fontSize * 0.6) + padding * 2, // 0.6 é fator de proporção
            altura: Math.ceil(numLinhas * fontSize * 1.2) + padding * 2,
            linhasReais: linhas.length
        };
    }
}