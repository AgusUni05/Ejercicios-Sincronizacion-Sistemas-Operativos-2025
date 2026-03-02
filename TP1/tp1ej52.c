#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int arr[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
int len = 20;

typedef struct{
	int ini;
	int fin;
	int* res;
} Trabajo;


void *sumador(void *arg){
	Trabajo *t = (Trabajo*) arg;
	printf("Soy el hilo %lu sumando desde %d hasta %d\n",pthread_self(),t->ini,t->fin);
	int x =0;
	for(int i= t-> ini; i< t->fin;i++)
		x+=arr[i];
		
	*(t->res) = x;
}

int main(){
	Trabajo trabajo[5];
	
	pthread_t hilos[5];
	int cant =0;
	int salto =5;
	int sumasParciales[5];
	
	for(int i=0;i<20;i+=5){
		trabajo[cant].ini = i;
		trabajo[cant].fin =salto;
		trabajo[cant].res = &sumasParciales[cant];
		pthread_create(&hilos[cant],NULL,sumador,&trabajo[cant]);
		cant++;
		salto+=5;
	}
	
	int res =0;
	for(int i = 0;i<cant;i++){
		pthread_join(hilos[i],NULL);
		res +=sumasParciales[i];
	}
	
	printf("Resultado final: %d\n",res);
	
	return 1;
}
