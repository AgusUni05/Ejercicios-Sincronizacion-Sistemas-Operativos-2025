
#include "estructura.h"

int main(){
	srand(time(NULL));
	int shmID= shmget(generarKey(),sizeof(Sincronizador),0666);
	Sincronizador* s = (Sincronizador*) shmat(shmID,0,0);

	while(1){
	
	printf("[COLECTIVO] Colectivo llegando a la parada...\n");
	fflush(stdout);
	sem_post(&s->turno);
	usleep(rand() % 50000 );
	if(sem_trywait(&s->lugarOcupado) != 0){ // Llego y no hay nadie esperando
		sem_wait(&s->turno);//no dejo que prueben mas

		printf("[COLECTIVO] El colectivo llego a la parada y no habia nadie, viajando..\n");
		fflush(stdout);
		sleep(rand() % 2 +1);
		printf("[COLECTIVO] Colectivo completo su viaje vacio\n");

		
	}	
	  else{	
		sem_post(&s->lugarOcupado);
		printf("[COLECTIVO] Hay gente esperando, subiendola....\n");
		sem_wait(&s->viajar);// El colectivo espera a que se cuando se llene viaje

		usleep(rand() % 500000 + 200000);
		printf("[COLECTIVO] Viajando...\n");
		sleep(rand() % 3 + 1);
		printf("[COLECTIVO] Terminado el viaje, permitiendo bajar a los clientes...\n");
		
		for(int i=0 ; i<CAPACIDAD_COLECTIVO ;i++){
			sem_post(&s->bajar);
			sem_post(&s->lugarVacio);
		}
		
		
		
		}
	sleep(rand() % 4 +1); 

	}
return 0;
}

