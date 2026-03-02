#include <unistd.h>
#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <sys/wait.h>

#define N 10
#define M 25

typedef struct{
    sem_t oso,turno,vacio;
}Sincronizador;

void oso(key_t key){
    int mID= shmget(key,sizeof(Sincronizador),0666);
    Sincronizador* s = (Sincronizador*) shmat(mID,0,0);

    while(1){
        sem_wait(&(s->oso)); // oso duerme hasta poder consumir la miel
        printf("Tarro lleno, oso consumiendo la miel...\n");
        sleep(rand() % 3 +1);
	
        for(int i=0; i<M;i++){
            sem_post(&(s->vacio)); // consumo y devuelvo el espacio
        }
	printf("Oso termino de consumir la miel...\n");
	fflush(stdout);
	sleep(rand() % 3 +1);
	sem_post(&s->turno);
	}
    exit(0);
}


void abeja(key_t key,int id){
    int mID= shmget(key,sizeof(Sincronizador),0666);
    Sincronizador* s=(Sincronizador*) shmat(mID,0,0);

    while(1){
	sem_wait(&(s->turno));

	
        sem_wait(&(s->vacio));  // abeja intenta llenar tarro con miel
        printf("Abeja numero %d, puso miel en el tarro\n",id);
        usleep(rand() % 200000+ 100000);
        if(sem_trywait(&(s->vacio))== -1){ // si soy la ultima abeja en llenar el tarro
            sem_post(&(s->oso)); // despierto al oso
            printf("Abeja numero %d habilitando tarro\n",id);
            

        } else {
            sem_post(&(s->vacio)); // sino devuelvo la porcion
            sem_post(&s->turno);
            }
  	sleep(rand() % 3 +1);
    }
    exit(0);
}


int main(){

    key_t key=ftok("/bin/ls",123);
    int mID= shmget(key,sizeof(Sincronizador),IPC_CREAT | 0666);
    Sincronizador* s= (Sincronizador* ) shmat(mID,0,0);

    sem_init(&(s->oso),1,0);
    sem_init(&(s->vacio),1,M);
    sem_init(&(s->turno),1,1);
	
	
    

    pid_t p;
    if( (p=fork()) == 0)
        oso(key);
	int ids[N];
    for(int i=0;i<N;i++){
        p=fork(); 
	ids[i]= i+1;
        if(p== 0) {
            abeja(key, ids[i]);  // Pasar i+1 como ID (1 a 10)
        }
    }

    for(int i=0;i< N+1;i++)
        wait(NULL);

    sem_destroy(&(s->turno));
    sem_destroy(&(s->vacio));
    sem_destroy(&(s->oso));

    shmctl(mID,IPC_RMID,NULL);
    return 0;
}
