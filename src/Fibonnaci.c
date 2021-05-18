#include <stdio.h>
#include <malloc.h>
#include "pokeLib.h"

void* fibo (void* dta){
	unsigned int* n = (int*) dta; // Esta linha foi modificada de *dta para dta
	int *n1, *n2, *r1, *r2;
	int *r = (int *) malloc (sizeof(int));
	struct Atrib a1, a2;
	int t1,t2;

	if (*n <= 2){
		*r = 1;
	}
	else{
		n1 = (int *) malloc (sizeof(int));
		*n1 = (*n)-1;
		a1.p = 0;
		a1.c = *n1;
		t1 = spawn (&a1, fibo, (void*) n1);
		n2 = (int *) malloc (sizeof(int));
		*n2 = *n -2;
		a2.p = 0;
		a1.c = *n2;
		t2 = spawn (&a2, fibo, (void*) n2);
		printf("Retorno do sync 1: %d \n",sync(t1, (void**) &r1));
		printf("Retorno do sinc 2: %d \n",sync(t2, (void**) &r2));
		printf("%p\n %p ",r1,r2);
		//*r = *r1 + *r2;
		free(r1);
		free(r2);
		free(n1);
		free(n2);
	}
	return r;
}
void * funcao(void *dta){
	int* res =  (int *)dta;
	//printf("%d\n", *res+1);
	*res+=1;
	return (void*) res;
}

int main(){
	/*int n, *r, tId;
	struct Atrib a;
	start (4);
	n = 10;
	a.p = 0;
	a.c = n;
	tId = spawn (&a, fibo, &n);
	sync(tId, (void**)&r);
	finish();
	printf("fim");
	//printf("Fibonacci (%d) = %d \n", n, *r); //Ponto de possível erro*/

	

	int param = 1;
	//int* res =  (int *)funcao((void*)&param);
	//printf("%d", *res);
	start(1);
	int id,r;
	struct Atrib a;
	a.p = 0;
	a.c = 0;
	id = spawn(&a, funcao, &param);
	printf("na main: %d \n", id);
	sync(id, (void**)&r);
	printf("Chegou no finish");
	finish();
	printf("Chegou no final");
	//printf("%d", r);
	return 0;

}
