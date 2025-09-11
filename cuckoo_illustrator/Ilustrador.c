#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h> 

#include <pthread.h>

#include "Ilustrador.h" 

#define DEBUG 0

#define PORTA_HTTP 8080
#define TAM_BUFFER 1024*2

#define MAX_OBJETOS 10000

#define X_INICIAL 2
#define TAMANHO_ELEMENTO 20

typedef enum {ARRAY_INT, VARIAVEL, NO_MULTI} tTipoObjeto;
typedef enum {NOVO, DESTAQUE, PERMUTA, DESLOCAMENTO, ATUALIZA, LINHA, REMOVE, TIRA_DESTAQUE, ALERTA, SAIDA, LIGA, ORGANIZA, ESCOPO} tTipoAcao;

typedef struct {
        int id;
        int ref;
        const void *endereco;
        tTipoObjeto tipoObjeto;
        char nome[TAM_BUFFER/3];
        void *objeto;
    } tObjetoSvg;

typedef struct {
        tObjetoSvg *este;
        tTipoVariavel tipo;
        const void *variavel;
    } tVariavelSvg;

typedef struct {
        tObjetoSvg *este;
        int *array;
        int tamanho;
    } tArraySvg;

typedef struct {
        int inicioDestaque;
        int quantidadeDestacada;
        tCor cor;
    } tAcaoDestaque;

typedef struct {
        int i, j;
    } tAcaoPermutaDesloca;


typedef struct {
        int id;
        tTipoAcao tipoAcao;
        int indiceObjeto;
        void *dados;
        char comando[TAM_BUFFER];
    } tAcao;

typedef struct {
        const char *nomeArquivo;
        char *conteudoArquivo;
        int tamanhoArquivo;
    } tRecurso;

static int idAtual = 0;
static int qObjetos = 0;
static int qAcoes = 0;
static tObjetoSvg objetos[MAX_OBJETOS];
static tAcao acoes[MAX_OBJETOS];

static char *conteudoArquivo = NULL;
static int tamanhoArquivo = -1;
static tRecurso recurso[MAX_OBJETOS];
static int qRecursos;

typedef struct {
    pthread_mutex_t mutex;
    pthread_cond_t condition;
    int value;
} tSemaphore;

static tSemaphore sem;

void semaphore_init(tSemaphore* sem, int initial_value) {
    pthread_mutex_init(&(sem->mutex), NULL);
    pthread_cond_init(&(sem->condition), NULL);
    sem->value = initial_value;
}

void semaphore_wait(tSemaphore* sem) {
    pthread_mutex_lock(&(sem->mutex));
    if (DEBUG) printf("[debug] esperando proximo...\n");
    while (sem->value <= 0) {
        pthread_cond_wait(&(sem->condition), &(sem->mutex));
    }
    sem->value--;
    pthread_mutex_unlock(&(sem->mutex));
}

void semaphore_signal(tSemaphore* sem) {
    pthread_mutex_lock(&(sem->mutex));
    if (DEBUG) printf("[debug] seguindo...\n");
    sem->value++;
    pthread_cond_signal(&(sem->condition));
    pthread_mutex_unlock(&(sem->mutex));
}

static int is_socket_connected(int sockfd) { 
    char buffer; 
    int result = recv(sockfd, &buffer, sizeof(buffer), MSG_DONTWAIT); 
    if (result == 0) { 
        return 0; // Socket is closed 
    } else if (result < 0) { 
        int error = 0;
        socklen_t len = sizeof (error);
        int retval = getsockopt (sockfd, SOL_SOCKET, SO_ERROR, &error, &len);
        //printf("error=%d, retval=%d\n", error, retval);

        return  (error != 0 || retval != 0) ? 0 : 1;//(errno == E || errno == EWOULDBLOCK) ? 1 : 0; // Check for non-blocking error 
    } 
    return 1; // Socket is connected 
} 

static char* TextoCor(tCor umaCor){
    switch (umaCor){
        case AMARELO:
            return "yellow";
        case AZUL:
            return "blue";  
        case VERMELHO:
            return "red";  
        case BRANCO:
            return "white";  
        case PRETO:
            return "black";
        case LARANJA:
            return "orange";
        case VERDE:
        default:
            return "green";
    }
}

static int ContaOcorrenciasChar(const char *str, char c){
    int contador = 0;

    for(int i = 0; str[i] != '\0'; i++){
        if (str[i] == c){
            contador++;
        }
    }

    return contador;
}

static int RecebeRequisicao(int socket, char *requisicao, int tamRequisicao){
    int lido = 0, r;

    while(lido <= tamRequisicao){
        r = read(socket, requisicao + lido, 1024);
        if (r <= 0){
            printf("Erro lendo requisicao!\n");
            return r;
        }
        lido += r;

        if (lido >= 4){
            requisicao[lido] = '\0';
            if (strcmp(&requisicao[lido-4], "\r\n\r\n") == 0){
                return lido;
            }
        }
    }

    return -1;
}

static void ExtraiPedido(const char *requisicao, char partesPedido[][51], int *numeroDePartes){ 
    char linhaReq[1024], *linhaTratada = NULL;
    sscanf(requisicao, "%*s%s%*s", linhaReq);
    
    //tirar barra da frente e de tras 
    char *c = linhaReq;
    while (*c == '/'){
        c++;
    }
    linhaTratada = c;
    c = linhaTratada + strlen(linhaTratada) - 1;
    while(*c == '/'){
        c--;
    }
    *(++c) = '\0';

    int nPartes = ContaOcorrenciasChar(linhaTratada, '/') + 1;
    *numeroDePartes = nPartes;
    //printf("[debug] req original: %s\n", linhaReq);
    //printf("[debug] linha usada (%d partes): %s\n", nPartes, linhaTratada);

    const char *separador = "/";
    char *tok = strtok(linhaReq, separador);
    int iParte = 0;
    while( tok != NULL ) {
        strcpy(partesPedido[iParte++], tok);
        tok = strtok(NULL, separador);
    }        
}

const char * ProcessaPedido(char *htmlDestino, const char partesPedido[][51], int nPartes){
    static int acaoAtual = 0;
    const char *ultimaParte = partesPedido[nPartes-1];
    static char retorno[TAM_BUFFER * 20];

    strcpy(retorno, "");
    if (DEBUG) printf("[debug] ultima parte pedido: %s (%d/%d)\n", ultimaParte, acaoAtual, qAcoes);
    if (strcmp(ultimaParte, "prox") == 0 && acaoAtual < qAcoes){
        while(acaoAtual < qAcoes){
            if (DEBUG) printf("[debug] prox = %s\n", acoes[acaoAtual].comando);
            tAcao acao = acoes[acaoAtual];
            const tObjetoSvg *objeto = (acao.indiceObjeto == -1) ? NULL : &objetos[acao.indiceObjeto];
            strcat(retorno, acao.comando);

            acaoAtual += acaoAtual < qAcoes ? 1 : 0;
        }
    }else if (strcmp(ultimaParte, "codigo") == 0){
        //enviar o codigo-fonte relacionado
        strcpy(retorno, conteudoArquivo);
        
    }else{
        //verificar se eh um recurso e enviar o arquivo
        for(int i = 0; i < qRecursos; i++){
            if (strcmp(recurso[i].nomeArquivo, ultimaParte) == 0){
                if (DEBUG) printf("[debug] enviar recurso \'%s\' de %d bytes.\n", recurso[i].nomeArquivo, recurso[i].tamanhoArquivo);
                strcpy(retorno, recurso[i].conteudoArquivo);
            }
        }
    }

    strcat(htmlDestino, retorno);
    return retorno;
}

static void * ThreadServidorHttp(void *vargp){
    int socketServidor, socketCliente, resultado, opt = 1, contador = 0, nPartes;
    struct sockaddr_in endereco;
    char requisicao[TAM_BUFFER*20], recurso[TAM_BUFFER], estrutura[TAM_BUFFER];
    char dadosHtml[TAM_BUFFER*100], partesRequisicao[20][51];

    puts("Iniciando servidor HTTP...\n");
    srand(time(NULL));
    socketServidor = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (socketServidor < 0){
        puts("Erro: Nao foi possivel criar o socket para comunicacao.");
        return NULL;
    }

    endereco.sin_family = AF_INET;
    endereco.sin_port = htons(PORTA_HTTP);
    endereco.sin_addr.s_addr = INADDR_ANY;
    
    resultado = bind(socketServidor, (struct sockaddr*) &endereco, sizeof(endereco));
    if (resultado < 0){
        puts("Erro: Nao foi possivel vincular o socket do servidor.");
        return NULL;
    }

    resultado = listen(socketServidor, 10);
    if (resultado < 0){
        puts("Erro: Nao foi possivel preparar o socket para receber conexoes.");
        return NULL;
    }

    while(1){
        puts("\nAguardando cliente...");
        socketCliente = accept(socketServidor, NULL, NULL);
        printf("Aceitando cliente #%d (socket = %d)...\n", ++contador, socketCliente);
        if (socketCliente < 0){
            puts("Erro: Nao foi possivel realizar a conexao com um cliente.");
            continue;
        }
        
        if (RecebeRequisicao(socketCliente, requisicao, sizeof(requisicao)) == -1){
            close(socketCliente);
            socketCliente = -1;
            puts("A requisicao nao pode ser lida corretamente, voltando...");
            continue;
        }
        
        ExtraiPedido(requisicao, partesRequisicao, &nPartes);
        printf("Cliente pediu: ");
        for(int i = 0; i < nPartes; i++){
            printf("\"%s\"%s", partesRequisicao[i], (i != nPartes-1) ? " | " : "\n");
        }
        strcpy(recurso, partesRequisicao[nPartes-1]);
        //printf("Ultimo recurso pedido: %s\n", recurso);
                
        //processar pedido
        strcpy(dadosHtml, "");
        ProcessaPedido(dadosHtml, partesRequisicao, nPartes);

        //responder e fechar

        //monta protocolo
        char cabecalho[1024], linhaTamanho[51];
        strcpy(cabecalho, "HTTP/1.1 200 OK\r\n");
        sprintf(linhaTamanho, "Content-Length: %lu\r\n", strlen(dadosHtml));
        strcat(cabecalho, linhaTamanho); 
        strcat(cabecalho, "Cache-Control: no-cache, no-store, must-revalidate\r\n");
        strcat(cabecalho, "Expires: 0\r\n");
        strcat(cabecalho, "Pragma: no-cache\r\n");
        strcat(cabecalho, "Access-Control-Allow-Origin: *\r\n");
        strcat(cabecalho, "Content-Type: text/html; charset=UTF-8\r\n");
        strcat(cabecalho, "Connection: close\r\n\r\n");

        //envia cabecalho e conteudo
        int enviado = 0, enviar = strlen(cabecalho), r;
        //if (DEBUG) printf("[debug] cab: %s\n", cabecalho);
        printf("Enviar %d bytes do cabecalho...\n", enviar);
        while(enviado < enviar && is_socket_connected(socketCliente)){
            //r = send(socketCliente, cabecalho + enviado, enviar - enviado, 0);
            r = write(socketCliente, cabecalho + enviado, enviar - enviado);
            //printf("%d enviados...\n", r);
            if (r <= 0){
                puts("Erro ao enviar.");
                exit(1);
            }
            enviado += r;
        }

        enviado = 0;
        char *bufferEnvio = dadosHtml;
        enviar = strlen(bufferEnvio);
        if (DEBUG) printf("[debug] html: %s\n", dadosHtml);
        printf("Enviar %d bytes do corpo...\n", enviar);
        while(enviado < enviar && is_socket_connected(socketCliente)){
            r = write(socketCliente, bufferEnvio + enviado, enviar - enviado);
    
            if (DEBUG) printf("Enviados %d bytes do corpo... (%d/%d)\n", r, enviado + r, enviar);
            if (r <= 0){
                puts("Erro ao enviar.");
                //exit(1);
                break;
            }
            enviado += r;
        }   
    

        printf("Fechando conexao do cliente #%d...\n", contador);
        if (strcmp(recurso, "prox") == 0){
            semaphore_signal(&sem);
        }
        r = shutdown(socketCliente, SHUT_RDWR);
        if (r){
            puts("Erro no shutdown().");
        }
        
        r = close(socketCliente);
        if (r){
            puts("Erro no close() 1.");
        }
        socketCliente = -1;
    }

    shutdown(socketServidor, SHUT_RDWR);
    close(socketServidor);
    
    return NULL;
}

void IniciaIlustrador(const char *arquivoCodigo){
    pthread_t threadHttp;

    srand(time(NULL));    

    if (arquivoCodigo != NULL){
        FILE *fp = fopen(arquivoCodigo, "r");
        if (!fp){
            printf("Erro ao abrir arquivo com o codigo: %s\n", arquivoCodigo);
            return;
        }

        fseek(fp, 0, SEEK_END);
        tamanhoArquivo = (int) ftell(fp);
        if ( (conteudoArquivo = malloc(tamanhoArquivo)+1) == NULL){
            printf("Erro ao criar buffer de %d bytes para conteudo do arquivo\n", tamanhoArquivo+1);
            tamanhoArquivo = -1;
            return;
        }

        rewind(fp);
        int lido = 0, r;
        while(lido < tamanhoArquivo){
            r = fread(conteudoArquivo+lido, 1, tamanhoArquivo-lido, fp);
            lido += r;
            //printf("%d/%d/%d\n", r, lido, tamanhoArquivo);
        }
        conteudoArquivo[tamanhoArquivo] = '\0';
        if (DEBUG) printf("[debug] arquivo de %d bytes carregado.\n", tamanhoArquivo);
        fclose(fp);
    }

    semaphore_init(&sem, 0);
    
    pthread_create(&threadHttp, NULL, ThreadServidorHttp, NULL);
    //pthread_join(threadHttp, NULL);

}

void IlustraAdicionaArquivo(const char *nomeArquivo){
    if (nomeArquivo != NULL && qRecursos < MAX_OBJETOS){
        FILE *fp = fopen(nomeArquivo, "r");
        if (!fp){
            printf("Erro ao abrir arquivo com o codigo: %s\n", nomeArquivo);
            return;
        }

        fseek(fp, 0, SEEK_END);
        tamanhoArquivo = (int) ftell(fp);

        tRecurso novoRecurso;
        novoRecurso.nomeArquivo = nomeArquivo;
        novoRecurso.tamanhoArquivo = tamanhoArquivo;

        if ( (novoRecurso.conteudoArquivo = malloc(novoRecurso.tamanhoArquivo)+1) == NULL){
            printf("Erro ao criar buffer de %d bytes para conteudo do arquivo\n", novoRecurso.tamanhoArquivo+1);
            novoRecurso.tamanhoArquivo = -1;
            return;
        }

        rewind(fp);
        int lido = 0, r;
        while(lido < novoRecurso.tamanhoArquivo){
            r = fread(novoRecurso.conteudoArquivo+lido, 1, novoRecurso.tamanhoArquivo-lido, fp);
            lido += r;
            //printf("%d/%d/%d\n", r, lido, novoRecurso.tamanhoArquivo);
        }
        novoRecurso.conteudoArquivo[novoRecurso.tamanhoArquivo] = '\0';
        recurso[qRecursos++] = novoRecurso;
        fclose(fp);
        if (DEBUG) printf("[debug] recurso %d/%d \'%s\' de %d bytes carregado.\n", qRecursos, MAX_OBJETOS, novoRecurso.nomeArquivo, novoRecurso.tamanhoArquivo);
    }else{
        if (DEBUG) printf("[debug] nao foi possivel adicionar arquivo %d/%d \'%s\'.\n", qRecursos+1, MAX_OBJETOS, nomeArquivo == NULL ? "" : nomeArquivo);
    }
}

int IlustraCriaVariavel(const void *v, const char *nome, tTipoVariavel tipo){
    //NOVO|VAR|ID|NOME|TIPO|VALOR|ENDERECO

    tVariavelSvg *novaVariavel = malloc(sizeof(tVariavelSvg));
    if (!novaVariavel){
        printf("Erro ao alocar a variavel generica #%d.\n", idAtual+1);
        exit(1);
    }
    novaVariavel->tipo = tipo;
    novaVariavel->variavel = v;
    novaVariavel->este = NULL;
    
    tObjetoSvg novoObjeto;
    novoObjeto.id = ++idAtual;
    novoObjeto.ref = novoObjeto.id;
    strcpy(novoObjeto.nome, nome);
    novoObjeto.tipoObjeto = VARIAVEL;
    novoObjeto.objeto = novaVariavel;
    novoObjeto.endereco = v;
    objetos[qObjetos] = novoObjeto;
    novaVariavel->este = &objetos[qObjetos++];

    char strTipo[21];
    char strValor[101];
    switch (tipo){
        case INT:
            strcpy(strTipo, NOME_VAR(INT));
            sprintf(strValor, "%d", *(int *)v);
            break;
        case DOUBLE:
            strcpy(strTipo, NOME_VAR(DOUBLE));
            sprintf(strValor, "%.3lf", *(double *)v);
            break;
        case PONTEIRO:
            strcpy(strTipo, NOME_VAR(PONTEIRO));
            sprintf(strValor, "%p", *(char **)v);
            break;
        case CHAR:
            strcpy(strTipo, NOME_VAR(CHAR));
            sprintf(strValor, "%c", *(char *)v);
            break;
        default:
            break;
    }

    tAcao novaAcao;
    novaAcao.id = ++idAtual;
    novaAcao.tipoAcao = NOVO;
    novaAcao.indiceObjeto = qObjetos-1;
    sprintf(novaAcao.comando, "NOVO|VAR|%d|%s|%s|%s|%p\n", novoObjeto.id, novoObjeto.nome, strTipo, strValor, v);
    acoes[qAcoes++] = novaAcao;
    
    return qAcoes-1;
}


int IlustraCriaVariavelInteira(const int *v, const char *nome){
    //NOVO|VAR|ID|NOME|TIPO|VALOR|ENDERECO

    tVariavelSvg *novaVariavel = malloc(sizeof(tVariavelSvg));
    if (!novaVariavel){
        printf("Erro ao alocar a variavel inteira #%d.\n", idAtual+1);
        exit(1);
    }
    novaVariavel->tipo = INT;
    novaVariavel->variavel = v;
    novaVariavel->este = NULL;
    
    tObjetoSvg novoObjeto;
    novoObjeto.id = ++idAtual;
    novoObjeto.ref = novoObjeto.id;
    strcpy(novoObjeto.nome, nome);
    novoObjeto.tipoObjeto = VARIAVEL;
    novoObjeto.objeto = novaVariavel;
    novoObjeto.endereco = v;
    objetos[qObjetos] = novoObjeto;
    novaVariavel->este = &objetos[qObjetos++];

    tAcao novaAcao;
    novaAcao.id = ++idAtual;
    novaAcao.tipoAcao = NOVO;
    novaAcao.indiceObjeto = qObjetos-1;
    sprintf(novaAcao.comando, "NOVO|VAR|%d|%s|INT|%d|%p\n", novoObjeto.id, novoObjeto.nome, *v, v);
    acoes[qAcoes++] = novaAcao;
    
    return qAcoes-1;
}

int IlustraAtualizaVariavel(const void *v){
    //ATUALIZA|VAR|ID|TIPO|VALOR
   for(int i = qObjetos-1; i >= 0; i--){
       if (objetos[i].tipoObjeto == VARIAVEL){
           tVariavelSvg *variavel = objetos[i].objeto;
           if (variavel->variavel == v){
               //achou a variavel q vai atualizar
               tAcao novaAcao;
               novaAcao.id = ++idAtual;
               novaAcao.tipoAcao = ATUALIZA;
               novaAcao.indiceObjeto = i;

               char strTipo[21];
               char strValor[101];
               switch (variavel->tipo){
                   case INT:
                       strcpy(strTipo, NOME_VAR(INT));
                       sprintf(strValor, "%d", *(int *)v);
                       break;
                   case DOUBLE:
                       strcpy(strTipo, NOME_VAR(DOUBLE));
                       sprintf(strValor, "%.3lf", *(double *)v);
                       break;
                   case PONTEIRO:
                       strcpy(strTipo, NOME_VAR(PONTEIRO));
                       sprintf(strValor, "%p", *(char **)v);
                       break;
                   case CHAR:
                       strcpy(strTipo, NOME_VAR(CHAR));
                       sprintf(strValor, "%c", *(char *)v);
                       break;
                   default:
                       break;
               }           

               sprintf(novaAcao.comando, "ATUALIZA|VAR|%d|%s|%s\n", objetos[i].id, strTipo, strValor);
               acoes[qAcoes++] = novaAcao;
               return qAcoes-1;              
           }
       }
   }

   return -1;
}

int IlustraAtualizaVariavelInteira(const int *v){
     //ATUALIZA|VAR|ID|TIPO|VALOR
    for(int i = qObjetos-1; i >= 0; i--){
        if (objetos[i].tipoObjeto == VARIAVEL){
            tVariavelSvg *variavel = objetos[i].objeto;
            if (variavel->variavel == v){
                //achou a variavel q vai atualizar
                tAcao novaAcao;
                novaAcao.id = ++idAtual;
                novaAcao.tipoAcao = ATUALIZA;
                novaAcao.indiceObjeto = i;
                sprintf(novaAcao.comando, "ATUALIZA|VAR|%d|INT|%d\n", objetos[i].id, *v);
                acoes[qAcoes++] = novaAcao;
                return qAcoes-1;              
            }
        }
    }

    return -1;
}

int IlustraCriaArrayDeInteiros(int ar[], int t, const char *nome){
    //NOVO|ARRAY_INT|id|NOME|t|elem0|elem1|..|elemN|endElem0|endElem1|..|endElemN

    tArraySvg *novoArray = malloc(sizeof(tArraySvg));
    if (!novoArray){
        printf("Erro ao alocar o array de inteiros #%d.\n", idAtual+1);
        exit(1);
    }
    novoArray->array = ar;
    novoArray->tamanho = t;
    novoArray->este = NULL;
    
    tObjetoSvg novoObjeto;
    novoObjeto.id = ++idAtual;
    novoObjeto.ref = novoObjeto.id;
    novoObjeto.endereco = ar;
    strcpy(novoObjeto.nome, nome);
    novoObjeto.tipoObjeto = ARRAY_INT;
    novoObjeto.objeto = novoArray;
    objetos[qObjetos] = novoObjeto;
    novoArray->este = &objetos[qObjetos++];

    char elementos[TAM_BUFFER] = "";
    for(int i = 0; i < t; i++){
        char temp[TAM_BUFFER/4];
        sprintf(temp, "|%d", ar[i]);
        strcat(elementos, temp);
    }
    char enderecos[TAM_BUFFER] = "";
    for(int i = 0; i < t; i++){
        char temp[TAM_BUFFER/4];
        sprintf(temp, "|%p", &ar[i]);
        strcat(enderecos, temp);
    }

    tAcao novaAcao;
    novaAcao.id = ++idAtual;
    novaAcao.tipoAcao = NOVO;
    novaAcao.indiceObjeto = qObjetos-1;
    sprintf(novaAcao.comando, "NOVO|ARRAY_INT|%d|%s|%d%s%s\n", novoObjeto.id, novoObjeto.nome, t, elementos, enderecos);
    acoes[qAcoes++] = novaAcao;
    
    return qAcoes-1;
}

int IlustraDestacaElementosArray(int ar[], int inicio, int q, tCor umaCor){
    //DESTAQUE|ARRAY_INT|id|ini|q|cor
    if (DEBUG) printf("[debug] procurar array para destaque\n");
    for(int i = 0; i < qObjetos; i++){
        if (objetos[i].tipoObjeto == ARRAY_INT){
            tArraySvg *array = objetos[i].objeto;
            
            if (array->array == ar){
                //achou o array que vai marcar elementos
                if (DEBUG) printf("[debug] achei array para destaque\n");
                tAcaoDestaque novoDestaque;
                tAcao novaAcao;

                novaAcao.id = ++idAtual;
                novaAcao.tipoAcao = DESTAQUE;
                novaAcao.indiceObjeto = i;

                novoDestaque.inicioDestaque = inicio;
                novoDestaque.quantidadeDestacada = q;
                novoDestaque.cor = umaCor;

                novaAcao.dados = &novoDestaque;
                sprintf(novaAcao.comando, "DESTAQUE|ARRAY_INT|%d|%d|%d|%s\n", objetos[i].id, inicio, q, TextoCor(umaCor));

                acoes[qAcoes++] = novaAcao;
                return qAcoes-1;              
            }
        }
    }
    
    if (DEBUG) printf("[debug] nao achei array para destaque\n");
    return -1;
}

int IlustraPermutaElementosArray(int ar[], int ini, int j){
    //PERMUTA|ARRAY_INT|id|i|j
    if (DEBUG) printf("[debug] procurar array para permuta\n");
    for(int i = 0; i < qObjetos; i++){
        if (objetos[i].tipoObjeto == ARRAY_INT){
            tArraySvg *array = objetos[i].objeto;
            
            if (array->array == ar){
                //achou o array que vai permutar elementos
                if (DEBUG) printf("[debug] achei array para permuta\n");
                tAcaoPermutaDesloca novaPermuta;
                tAcao novaAcao;

                novaAcao.id = ++idAtual;
                novaAcao.tipoAcao = PERMUTA;
                novaAcao.indiceObjeto = i;

                novaPermuta.i = ini;
                novaPermuta.j = j;

                novaAcao.dados = &novaPermuta;
                sprintf(novaAcao.comando, "PERMUTA|ARRAY_INT|%d|%d|%d\n", objetos[i].id, ini, j);
                acoes[qAcoes++] = novaAcao;
                return qAcoes-1;              
            }
        }
    }

    if (DEBUG) printf("[debug] nao achei array para permutar\n");
    return -1;
}

int IlustraMoveElementoArray(int ar[], int indiceOrigem, int indiceDestino){
    //DESLOCA|ARRAY_INT|id|iOrigem|jDestino
    if (DEBUG) printf("[debug] procurar array para mover elemento\n");
    for(int i = 0; i < qObjetos; i++){
        if (objetos[i].tipoObjeto == ARRAY_INT){
            tArraySvg *array = objetos[i].objeto;
            
            if (array->array == ar){
                //achou o array que vai mover elemento
                if (DEBUG) printf("[debug] achei array para mover elemento\n");
                tAcao novaAcao;

                novaAcao.id = ++idAtual;
                novaAcao.tipoAcao = DESLOCAMENTO;
                novaAcao.indiceObjeto = i;

                sprintf(novaAcao.comando, "DESLOCA|ARRAY_INT|%d|%d|%d\n", objetos[i].id, indiceOrigem, indiceDestino);
                acoes[qAcoes++] = novaAcao;
                return qAcoes-1;              
            }
        }
    }

    if (DEBUG) printf("[debug] nao achei array para mover elemento\n");
    return -1;
}

int IlustraAtualizaElementosArray(int ar[], int indice){
    //ATUALIZA|ARRAY_INT|ID|INDICE|VALOR
    for(int i = 0; i < qObjetos; i++){
        if (objetos[i].tipoObjeto == ARRAY_INT){
            tArraySvg *arraySvg = objetos[i].objeto;
            if (arraySvg->array == ar){
                //achou o array q vai atualizar
                tAcao novaAcao;
                novaAcao.id = ++idAtual;
                novaAcao.tipoAcao = ATUALIZA;
                novaAcao.indiceObjeto = i;
                sprintf(novaAcao.comando, "ATUALIZA|ARRAY_INT|%d|%d|%d\n", objetos[i].id, indice, ar[indice]);
                acoes[qAcoes++] = novaAcao;
                return qAcoes-1;              
            }
        }
    }

    return -1;
}

int IlustraDestacaLinha(int linhaDestaque, int bloqueia){
    //LINHA|i
    tAcao novaAcaoLinha;
    novaAcaoLinha.id = ++idAtual;
    novaAcaoLinha.tipoAcao = LINHA;
    novaAcaoLinha.indiceObjeto = linhaDestaque;
    sprintf(novaAcaoLinha.comando, "LINHA|%d\n", linhaDestaque);
    acoes[qAcoes++] = novaAcaoLinha;
    if (bloqueia){
        semaphore_wait(&sem);
    }
    return qAcoes-1;              
}

int IlustraAlerta(const char *textoAlerta){
    //ALERTA|texto
    tAcao novaAcao;
    novaAcao.id = ++idAtual;
    novaAcao.tipoAcao = ALERTA;
    novaAcao.indiceObjeto = -1;
    novaAcao.dados = (char *) textoAlerta;
    sprintf(novaAcao.comando, "ALERTA|%s\n", textoAlerta);
    acoes[qAcoes++] = novaAcao;
    
    return qAcoes-1;    
}

int IlustraSaida(const char *textoSaida){
    //SAIDA|texto
    tAcao novaAcao;
    novaAcao.id = ++idAtual;
    novaAcao.tipoAcao = SAIDA;
    novaAcao.indiceObjeto = -1;
    sprintf(novaAcao.comando, "SAIDA|%s\n", textoSaida);
    acoes[qAcoes++] = novaAcao;
    
    return qAcoes-1;    
}

int IlustraEscopoElemento(const void* endereco, int escopoAtual){
    //ESCOPO|id|ativo
    int achou = 0;
    for(int i = 0; i < qObjetos; i++){
        if (objetos[i].endereco == endereco){
            //achou objeto para remover
            tAcao novaAcao;
            novaAcao.id = ++idAtual;
            novaAcao.tipoAcao = ESCOPO;
            novaAcao.indiceObjeto = i;
            sprintf(novaAcao.comando, "ESCOPO|%d|%d\n", objetos[i].id, escopoAtual);
            acoes[qAcoes++] = novaAcao;
            achou = 1;
        }
    }
    return achou ? qAcoes-1 : -1;
}

int IlustraRemoveMemoriaPorEndereco(void *endereco){
    //REMOVE|id
    int achou = 0;
    for(int i = 0; i < qObjetos; i++){    
        if (objetos[i].endereco == endereco){
            //achou objeto para remover
            tAcao novaAcao;
            novaAcao.id = ++idAtual;
            novaAcao.tipoAcao = REMOVE;
            novaAcao.indiceObjeto = i;
            sprintf(novaAcao.comando, "REMOVE|%d\n", objetos[i].id);
            acoes[qAcoes++] = novaAcao;
            achou = 1;
        }
    }
    return achou ? qAcoes-1 : -1;
}

int IlustraRemoveMemoriaPorReferencia(int ref){
    //REMOVE|id
    int achou = 0;
    for(int i = 0; i < qObjetos; i++){    
        if (objetos[i].ref == ref){
            //achou objeto para remover
            tAcao novaAcao;
            novaAcao.id = ++idAtual;
            novaAcao.tipoAcao = REMOVE;
            novaAcao.indiceObjeto = i;
            sprintf(novaAcao.comando, "REMOVE|%d\n", objetos[i].id);
            acoes[qAcoes++] = novaAcao;
            achou = 1;
        }
    }
    return achou ? qAcoes-1 : -1;
}