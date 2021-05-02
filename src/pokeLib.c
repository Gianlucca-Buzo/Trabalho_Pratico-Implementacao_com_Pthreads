#include <pthread.h>
#include "pokeLib.h"

int start (int m){
/*Esta primitiva lança o núcleo de execução, instanciando m processadores virtuais, indicados pelo parâmetro m.
O retorno 0 (zero) indica falha na instanciação dos processadores virtuais. 
Um valor maior que 0 indica criação bem sucedida.*/
pthread_t pvs[m];
    for(int i=0;i< m;i++){
        //TODO: revisar linha abaixo
       int threadCriada = pthread_create(&pvs[i], NULL, NULL, NULL);//Apenas cria uma thread, sem nenhum tipo de parâmetro
       if(threadCriada != 0){//Se houve problema na criação de um thread
            return 0;//Retorna falha na instanciação
        }
    }
    return 3;//Criação de todas as threads de maneira bem sucedida
}

void finish(void){
/*Esta primitiva é bloqueante, retornando após todos os processadores virtuais terem finalizado.*/

}

int spawn( struct Atrib* atrib, void *(*t) (void *), void* dta ){
/*A primitiva spawn lança a execução da tarefa descrita no ponteiro para função indicada pelo parâmetro t. 
O parâmetro para a função *t é descrito no parâmetro dta. O parâmetro struct Atrib* atrib descreve os 
atributos a serem considerados no escalonamento da tarefa. A função retorna 0 (zero) em caso de falha na 
criação da tarefa ou um valor inteiro positivo maior que 0, considerado o identificador único da tarefa no 
programa. Caso NULL seja passado como endereço para atrib, devem ser considerados os valores default para os 
atributos.*/

}

int sync( int tId, void** res ){
/*A primitiva sync é bloqueante: a tarefa que invoca a primitiva sync informa qual tarefa, 
identificada no parâmetro tId, que deve ser sincronizada. O retorno da primitiva é 0 (zero) em caso de 
falha ou 1 (um), em caso de sincronização bem sucedida. O parâmetro res contém, como saída, o endereço de 
memória que contém os resultados de saída. Importante observar: uma tarefa somente pode ser sincroniza uma 
única vez. Não é permitido múltiplos syncs de uma mesma tarefa*/

}

