#include <pthread.h>
#include <malloc.h>
#include "pokeLib.h"

static pthread_t *processadoresVirtuais;
static int idTrabalhoAtual = 0;
static int quantidadeProcessadoresVirtuais = 0;

typedef struct Trabalho
{
    int idTrabalho;               //Id do trabalho
    void *(*funcao)(void *);      //Função que vai ser executada
    void *parametrosFuncao;       //ParâmetroS de entrada para a função
    void *resultado;              //Retorno da função
    struct Trabalho *anterior, *proximo; //Ponteiros da lista duplamente encadeada
} Trabalho;

typedef struct Sentinela
{
    Trabalho *primeiraPosicao;
    Trabalho *ultimaPosicao;
} Sentinela;

Sentinela sentinela, sentinelaTerminados;

Trabalho *listaTrabalhosProntos, *listaTrabalhosTerminados;

Trabalho* pegaUmTrabalho(Sentinela sentinelaAux)
{
    Trabalho *trabalhoAtual = malloc(sizeof(Trabalho));
    //Copia a primeira posição da lista
    if(sentinelaAux.primeiraPosicao == NULL){
        return NULL;
    }
    trabalhoAtual = sentinelaAux.primeiraPosicao;
    //incrementa a lista
    sentinelaAux.primeiraPosicao = sentinelaAux.primeiraPosicao->proximo;
    if(sentinelaAux.primeiraPosicao == NULL){
        return trabalhoAtual;
    }
    //desfaz as conexões para tirar o trabalhoAtual lista encadeada
    sentinelaAux.primeiraPosicao->anterior = NULL;
    trabalhoAtual->proximo = NULL;
    
    return trabalhoAtual;
}

void armazenaResultados(Trabalho *trabalho, void *resultadoT)
{ //Todo: incrementar a lista encadeada
    trabalho->resultado = resultadoT;
    if (listaTrabalhosTerminados == NULL)
    {
        //aloca a lista
        listaTrabalhosTerminados = malloc(sizeof(Trabalho));
        listaTrabalhosTerminados->anterior = NULL;
        //organiza a sentinela
        sentinelaTerminados.primeiraPosicao = listaTrabalhosTerminados;
        sentinelaTerminados.ultimaPosicao = listaTrabalhosTerminados;
        //Adiciona na lista
        listaTrabalhosTerminados = trabalho;
    }
    else
    {
        sentinelaTerminados.ultimaPosicao->proximo = trabalho;
        trabalho->anterior = sentinelaTerminados.ultimaPosicao;
        sentinelaTerminados.ultimaPosicao = trabalho;
    }
}

void* criaProcessadorVirtual(void *dta)
{
    void *resultado; //Resultado da função
    Trabalho *trabalhoAtual;
    while(1){
        if(listaTrabalhosProntos != NULL)
        { //TODO verificar essa lógica depois da implementação do Darlei
            trabalhoAtual = pegaUmTrabalho(sentinela);
            resultado = trabalhoAtual->funcao(trabalhoAtual->parametrosFuncao);
            armazenaResultados(trabalhoAtual, resultado);
        }
    }
}

Trabalho* pegaUmTrabalhoPorId(int id, Sentinela sentinelaAux)
{
    Trabalho *trabalhoAtual = malloc(sizeof(Trabalho));
    Trabalho *trabalhoAux = malloc(sizeof(Trabalho));

    trabalhoAtual = sentinelaAux.primeiraPosicao;
    while ((trabalhoAtual->idTrabalho != id) && (trabalhoAtual != NULL))
    {
        trabalhoAtual = trabalhoAtual->proximo;
    }
    
    if(trabalhoAtual == NULL){
        return NULL;
    }

    else if (trabalhoAtual->proximo != NULL && trabalhoAtual->anterior!=NULL) // Não é a primeira nem a última posição
    {
        trabalhoAtual->anterior->proximo = trabalhoAtual->proximo;
        trabalhoAtual->proximo->anterior = trabalhoAtual->anterior;
        trabalhoAtual->proximo = NULL;
        trabalhoAtual->anterior = NULL;
    }
    else if (trabalhoAtual->proximo == NULL){ // É a última
        trabalhoAtual->anterior->proximo = NULL;
        sentinelaAux.ultimaPosicao = trabalhoAtual->anterior;
        trabalhoAtual->anterior = NULL;
    }else if (trabalhoAtual->anterior == NULL){// É a primeira
        trabalhoAtual->proximo->anterior = NULL;
        sentinelaAux.primeiraPosicao = trabalhoAtual->proximo;
        trabalhoAtual->proximo = NULL;
    }
    
    return trabalhoAtual;
}



int start(int m)
{
    /*Esta primitiva lança o núcleo de execução, instanciando m processadores virtuais, indicados pelo parâmetro m.
O retorno 0 (zero) indica falha na instanciação dos processadores virtuais. 
Um valor maior que 0 indica criação bem sucedida.*/
    quantidadeProcessadoresVirtuais = m;
    listaTrabalhosProntos = NULL;
    listaTrabalhosTerminados = NULL;
    processadoresVirtuais = malloc(m * sizeof(pthread_t));
    for (int i = 0; i < m; i++)
    {
        int threadCriada = pthread_create(&processadoresVirtuais[i], NULL, criaProcessadorVirtual, NULL); //Apenas cria uma thread, sem nenhum tipo de parâmetro
        if (threadCriada != 0)
        {             //Se houve problema na criação de um thread
            return 0; //Retorna falha na instanciação
        }
    }
    return 1; //Criação de todas as threads de maneira bem sucedida
}

void finish(void)
{
    /*Esta primitiva é bloqueante, retornando após todos os processadores virtuais terem finalizado.*/
    for (int i = 0; i < quantidadeProcessadoresVirtuais; i++)
    {
        pthread_join(processadoresVirtuais[i], NULL);
    }
}

int spawn(struct Atrib *atrib, void *(*t)(void *), void *dta)
{
    /*A primitiva spawn lança a execução da tarefa descrita no ponteiro para função 
indicada pelo parâmetro t.O parâmetro para a função *t é descrito no parâmetro dta.
O parâmetro struct Atrib* atrib descreve os atributos a serem considerados no 
escalonamento da tarefa. A função retorna 0 (zero) em caso de falha na criação da 
tarefa ou um valor inteiro positivo maior que 0, considerado o identificador único 
da tarefa no programa. Caso NULL seja passado como endereço para atrib, devem ser 
considerados os valores default para os atributos.*/
    Trabalho *novoTrabalho = malloc(sizeof(Trabalho));
    novoTrabalho->idTrabalho = idTrabalhoAtual;
    idTrabalhoAtual++; //Incrementa a variável global que faz a contagem dos valores dos IDs
    novoTrabalho->funcao = t;
    novoTrabalho->parametrosFuncao = dta;
    novoTrabalho->proximo = NULL;

    //Adiciona elemento na lista encadeada
    if (listaTrabalhosProntos == NULL)
    { //se é o primeiro
        //aloca
        listaTrabalhosProntos = malloc(sizeof(Trabalho));
        listaTrabalhosProntos->anterior = NULL;
        //organiza o sentinela
        sentinela.primeiraPosicao = listaTrabalhosProntos;
        sentinela.ultimaPosicao = listaTrabalhosProntos;
        //insere na lista
        listaTrabalhosProntos = novoTrabalho;
    }
    else // Caso já tenha algo na lista
    {
        novoTrabalho->anterior = sentinela.ultimaPosicao;
        sentinela.ultimaPosicao->proximo = novoTrabalho;
        sentinela.ultimaPosicao = novoTrabalho;
    }

    return 1;
    //TODO return  (zero) em caso de falha na criação da tarefa ou um valor inteiro positivo maior que 0
}

int sync(int tId, void **res)
{
    /*A primitiva sync é bloqueante: a tarefa que invoca a primitiva sync informa qual tarefa, 
identificada no parâmetro tId, que deve ser sincronizada. O retorno da primitiva é 0 (zero) em caso de 
falha ou 1 (um), em caso de sincronização bem sucedida. O parâmetro res contém, como saída, o endereço de 
memória que contém os resultados de saída. Importante observar: uma tarefa somente pode ser sincroniza uma 
única vez. Não é permitido múltiplos syncs de uma mesma tarefa*/
    Trabalho *aux;

    aux = pegaUmTrabalhoPorId(tId, sentinelaTerminados); // Retorna o trabalho ou null se não encontrar
    if (aux == NULL){
        aux = pegaUmTrabalhoPorId(tId, sentinela);
        if (aux == NULL){
            aux = pegaUmTrabalho(sentinelaTerminados);
            if (aux == NULL){
                return 0;
            }          
        }
    }
    res = (void **) aux->resultado;
    return 1;
}