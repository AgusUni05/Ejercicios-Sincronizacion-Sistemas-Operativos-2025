#include "estructura.h"


int main(int argc,char* argv[]){
	
	if(argc != 2){
		perror("Error, pasar el id solo\n");
		return 1;
	}

	int id= atoi(argv[1]);

	srand(time(NULL));
	int shmID= shmget(generarKey(),sizeof(Sincronizador),0666);
	Sincronizador* s = (Sincronizador* ) shmat(shmID,0,0);

	while(1){

	sem_wait(&s->turno);

	pthread_mutex_lock(&s->mutex);
	sem_wait(&s->lugarVacio);//se sube un cliente
	sem_post(&s->lugarOcupado);
	printf("Cliente  %d se acaba de subir al colectivo\n",id);
	fflush(stdout);
	if(sem_trywait(&s->lugarVacio) != 0){
		printf("Ultimo cliente avisando que hay que viajar, %d \n",id);
		sem_post(&s->viajar);// ultimo cliente avisa que debe viajar
	}
	else{
		sem_post(&s->lugarVacio);
		sem_post(&s->turno);
	}
	pthread_mutex_unlock(&s->mutex);
	
	sem_wait(&s->bajar);
	printf("Cliente %d se bajo..\n",id);
	sleep(rand() % 10 + 8);
  }
return 0;

}