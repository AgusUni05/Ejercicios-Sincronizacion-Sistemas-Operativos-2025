#include "estructura.h"

int main(){
	srand(time(NULL));

	int shmID = shmget(generarKey(),sizeof(Sincronizador),IPC_CREAT | 0666);
	Sincronizador* s= (Sincronizador*) shmat(shmID,0,0);

	sem_init(&s->bajar,1,0);
	sem_init(&s->lugarVacio,1,CAPACIDAD_COLECTIVO);
	sem_init(&s->lugarOcupado,1,0);
	sem_init(&s->viajar,1,0);
	sem_init(&s->turno,1,0);

	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_setpshared(&attr,PTHREAD_PROCESS_SHARED);
	
	pthread_mutex_init(&s->mutex,&attr);
	pthread_mutexattr_destroy(&attr);

	pid_t p;
	p=fork();
	if(p == 0){
	char* args[] = {"./colectivo",NULL};

	execv("./colectivo",args);
	
	perror("Fallo execv colectivo\n");
	return 1;
	}

	int ids[CLIENTES];

	for(int i=0;i<CLIENTES;i++){
	ids[i] = i+1;
	p=fork();
	if(p==0){
		char id_str[10];
            	snprintf(id_str,sizeof(id_str),"%d", ids[i]);

	char* args[] = {"./cliente",id_str,NULL};

	execv("./cliente",args);
	
	perror("Fallo execv cliente\n");
	return 1;
	 }

	}
	wait(NULL);
	return 0;
}







