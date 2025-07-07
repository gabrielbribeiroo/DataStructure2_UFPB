class IlustracaoSlide extends Ilustracao{

    constructor(id, nome){
        super(id, nome, Ilustracao.novaPosicaoSugerida(Ilustrador.dimensoesGraficosSVG.largura*0.41, Ilustrador.dimensoesGraficosSVG.altura, true), 'ghostwhite');
    }

    montar(){

    }
    
    /**
     * 
     * @param {IlustracaoSlide} slide 
     */

    async ilustrar(slide){

    }

    parar(){

    }

    limpar(){
        
    }

}