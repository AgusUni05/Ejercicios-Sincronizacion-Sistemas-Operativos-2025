#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

void *imprimeid(){
	pthread_t x= pthread_self();
	printf("Hola soy un hilo, mi identificador es: %lu \n",x);
	pthread_exit(NULL);
}

int main(){
	printf("Ingresar los m hilos a mostrar");
	int m = 0;
	scanf("%d",&m);
	pthread_t hilos[m];
	
	for(int i = 0 ; i<m ; i++)
		pthread_create(&hilos[i],NULL,imprimeid,NULL);
		
	for(int j=0; j<m ; j++)
		pthread_join(hilos[j],NULL);
		
	printf("Mis hilos ya terminaron");

return 1;
}
