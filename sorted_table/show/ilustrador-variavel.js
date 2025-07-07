class IlustracaoVariavel extends IlustracaoNo {
    #cloneTextoNome = null;
    
    /**
     * Cria uma Ilustração de uma Variável com a cor especificada. 
     *
     * @param {number} id 
     * @param {string} nome 
     * @param {string} valor
     * @param {string} cor 
     * @param {boolean} [criarEmNovaLinha=false] 
     */
    constructor(id, nome, valor, cor, criarEmNovaLinha = false){
        super(id, nome, valor, cor, criarEmNovaLinha);

        //iniciar campos
        this.#cloneTextoNome = null;

        //criar elementos        
        let novaCaixaNome = document.getElementById("caixaNomeNo").cloneNode(true);
        super.grupoPrincipal.appendChild(novaCaixaNome);

        let novoTextoNome = document.getElementById("textoNomeNo").cloneNode(true);
        super.grupoPrincipal.appendChild(novoTextoNome); //sera que nao é pra procurar e inserir em NO?
        this.#cloneTextoNome = novoTextoNome;

        this.nome = nome;
    }

    /**
     * Altera o nome de um nó.
     * 
     * @param {string} novoNome 
     */
    set nome(novoNome){
        super.nome = novoNome;
        
        this.#cloneTextoNome.textContent = novoNome;  
        Ilustrador.autoAjustarTamanhoDoTexto(this.#cloneTextoNome);
    }
}