#include <pthread.h>
#include <malloc.h>
#include "pokeLib.h"

static pthread_t *processadoresVirtuais;
static int idTrabalhoAtual = 0;

typedef struct Trabalho {
   int idTrabalho; //Id do trabalho
   void* (*funcao)(void*); //Função que vai ser executada
   void* parametrosFuncao; // ParâmetroS de entrada para a função
   void* resultado; // Retorno da função 
   Trabalho *anterior,*proximo;//Ponteiros da lista duplamente encadeada
}Trabalho;

typedef struct Sentinela{
    Trabalho *primeiraPosicao;
    Trabalho *ultimaPosicao;
}Sentinela;

Sentinela sentinela;

Trabalho *listaTrabalhosProntos, *listaTrabalhosTerminados;

void* criaProcessadorVirtual(void* dta) {
    void* resultado;//Resultado da função
    Trabalho *trabalhoAtual;
    while(listaTrabalhosProntos!=NULL) {//TODO verificar essa lógica depois da implementação do Darlei
        trabalhoAtual = pegaUmTrabalho();
        resultado = trabalhoAtual->funcao(trabalhoAtual->parametrosFuncao );  
        finalizaUmTrabalho(trabalhoAtual,resultado);
    }
    return NULL;
}

Trabalho* pegaUmTrabalho(void){//TODO: DARLEI VAI MEXER AQUI PRA DAR FREE NO TRABALHO ANTERIOR
    Trabalho* trabalhoAtual = malloc(sizeof(Trabalho));
    //Copia a primeira posição da lista
    trabalhoAtual = listaTrabalhosProntos;
    //incrementa a lista
    listaTrabalhosProntos = listaTrabalhosProntos->proximo;
    //desfaz as conexões da lista encadeada
    listaTrabalhosProntos->anterior = NULL;
    trabalhoAtual->proximo = NULL;
    
    return trabalhoAtual;
}

void armazenaResultados(Trabalho *t, void * resultadoT){//Todo: incrementar a lista encadeada
    t->resultado = resultadoT;
    //todo: DARLEI AQUI ADICIONA O TRABALHO NO VETOR DE TERMINADOS
}

void inicializaListas(void){
    //Inicializa a lista de trabalhos prontos
    listaTrabalhosProntos = malloc(sizeof(Trabalho));
    listaTrabalhosProntos->anterior = NULL;

    sentinela.primeiraPosicao = listaTrabalhosProntos;

    //Inicializa a lista de trabalhos terminados
    listaTrabalhosTerminados = malloc(sizeof(Trabalho));
    listaTrabalhosTerminados->anterior = NULL;

}


int start (int m){
/*Esta primitiva lança o núcleo de execução, instanciando m processadores virtuais, indicados pelo parâmetro m.
O retorno 0 (zero) indica falha na instanciação dos processadores virtuais. 
Um valor maior que 0 indica criação bem sucedida.*/
    inicializaListas();
    processadoresVirtuais = malloc(m*sizeof(pthread_t));
    for(int i=0; i<m; i++){
        int threadCriada = pthread_create(&processadoresVirtuais[i], NULL, criaProcessadorVirtual, NULL);//Apenas cria uma thread, sem nenhum tipo de parâmetro
        if(threadCriada != 0){//Se houve problema na criação de um thread
            return 0;//Retorna falha na instanciação
        }
    }
    return 1;//Criação de todas as threads de maneira bem sucedida
}

void finish(void){
/*Esta primitiva é bloqueante, retornando após todos os processadores virtuais terem finalizado.*/

}

int spawn( struct Atrib* atrib, void *(*t) (void *), void* dta ){
/*A primitiva spawn lança a execução da tarefa descrita no ponteiro para função 
indicada pelo parâmetro t.O parâmetro para a função *t é descrito no parâmetro dta.
O parâmetro struct Atrib* atrib descreve os atributos a serem considerados no 
escalonamento da tarefa. A função retorna 0 (zero) em caso de falha na criação da 
tarefa ou um valor inteiro positivo maior que 0, considerado o identificador único 
da tarefa no programa. Caso NULL seja passado como endereço para atrib, devem ser 
considerados os valores default para os atributos.*/
struct Trabalho* novoTrabalho = malloc(sizeof(novoTrabalho));
novoTrabalho->idTrabalho = idTrabalhoAtual;
idTrabalhoAtual++;//Incrementa a variável global que faz a contagem dos valores dos IDs
novoTrabalho->funcao = t;
novoTrabalho->parametrosFuncao = dta;
novoTrabalho->proximo = NULL;

//Adiciona elemento na lista encadeada
novoTrabalho->anterior = sentinela.ultimaPosicao;
sentinela.ultimaPosicao->proximo = novoTrabalho;
sentinela.ultimaPosicao = novoTrabalho;

}

int sync( int tId, void** res ){
/*A primitiva sync é bloqueante: a tarefa que invoca a primitiva sync informa qual tarefa, 
identificada no parâmetro tId, que deve ser sincronizada. O retorno da primitiva é 0 (zero) em caso de 
falha ou 1 (um), em caso de sincronização bem sucedida. O parâmetro res contém, como saída, o endereço de 
memória que contém os resultados de saída. Importante observar: uma tarefa somente pode ser sincroniza uma 
única vez. Não é permitido múltiplos syncs de uma mesma tarefa*/

}

