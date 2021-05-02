#include <pthread.h>

struct Atrib{
/*descreve atributos a serem considerados para escalonamento da tarefa. O exemplo abaixo é uma 
configuração mínima, podendo ser adicionados outros atributos.O campo p representa a prioridade da 
tarefa e o campo c o seu custo computacional. Note: o custo computacional deverá ser indicado pelo 
programador.*/
	int p;
	int c;
};

int start (int m);
void finish(void);
int spawn( struct Atrib* atrib, void *(*t) (void *), void* dta );
int sync( int tId, void** res );