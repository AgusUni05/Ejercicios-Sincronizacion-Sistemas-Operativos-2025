#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define BUFFER 20

static inline int generarKey(){
    return ftok("/bin/ls/",123);
}

typedef struct Estructura{
    sem_t vacioH,vacioO;
    sem_t llenoH,llenoO;
    pthread_mutex_t mutexH,mutexO;
    char bufH[BUFFER],bufO[BUFFER];
    int inO,inH;
    int outO,outH;
} Sincronizador;


void generadorOxigeno(){
    int msgID = shmget(generarKey(),sizeof(Sincronizador),0666);
    Sincronizador* s= (Sincronizador*  ) shmat(msgID,0,0);

    while(1){
        printf("Generando oxigeno...\n");
        fflush(stdout);

        for(int i=0; i<15 ; i++){
            sem_wait(&(s->vacioO));//espero a que haya un espacio en el buffer

            pthread_mutex_lock(&(s->mutexO));// exclusion mutua para tocar el buffer
            s->bufO[s->inO] = 'O';
            s->inO = (s-> inO + 1) % BUFFER;
            pthread_mutex_unlock(&(s->mutexO));

            sem_post(&(s->llenoO));// marco que llene un lugar
        }
        printf("Fin generacion oxigeno, yendo a dormir\n");
        fflush(stdout);
        usleep(15);
    }
    exit(0);
}

void generadorHidrogeno(){
    int idMsg = shmget(generarKey(),sizeof(Sincronizador),0666);
    Sincronizador* s= (Sincronizador* ) shmat(idMsg,0,0);

    while(1){
        printf("Generando hidrogeno \n");
        fflush(stdout);

        for(int i=0; i<20;i++){
            sem_wait(&(s->vacioH));// espero a que haya lugar disponible en el bufer h

            pthread_mutex_lock(&(s->mutexH));
            s->bufH[s->inH] = 'H';
            s->inH = (s->inH + 1) % BUFFER;
            pthread_mutex_unlock(&(s->mutexH));

            sem_post(&(s->llenoH)); // produci un hidrogeno, lo marco
        }
        printf("Fin generacion hidrogeno, durmiendo..\n");
        fflush(stdout);
        usleep(20);
    }
}

void generadorAgua(){
    int idMsg = shmget(generarKey(),sizeof(Sincronizador),0666);
    Sincronizador* s= (Sincronizador* ) shmat(idMsg,0,0);

    while(1){
        sem_wait(&(s->llenoH));//espero un hidrogeno
        sem_wait(&(s->llenoH));//espero el siguiente hidrogeno
        pthread_mutex_lock(&(s->mutexH));//modifico las variables compartidas con H, necesito exclusion mutua
        s->bufH[s->outH]= '\0';
        s->bufH[s->outH + 1] ='\0';
        s->outH = (s->outH +2) % BUFFER;
        pthread_mutex_unlock(&(s->mutexH));
        sem_post(&(s->vacioH));
        sem_post(&(s->vacioH)); // aviso que gaste dos lugares del buffer compartido
        
        sem_wait(&(s->llenoO));
        pthread_mutex_lock(&(s->mutexO));
        s->bufO[s->outO] = '\0';
        s->outO = (s->outO +1) % BUFFER;
        pthread_mutex_unlock(&(s->mutexO));
        sem_post(&(s->vacioO));

        printf("Agua generada\n");
        fflush(stdout);

    }
    exit(0);
}

int main(){
    
    int idMsg = shmget(generarKey(),sizeof(Sincronizador),IPC_CREAT | 0666);
    Sincronizador* s = (Sincronizador* ) shmat(idMsg,0,0);

    sem_init(&(s->vacioH),1,BUFFER);
    sem_init(&(s->llenoH),1,0);
    
    sem_init(&(s->vacioO),1,BUFFER);
    sem_init(&(s->llenoO),1,0);

    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_setpshared(&attr,PTHREAD_PROCESS_SHARED);

    pthread_mutex_init(&(s->mutexH),&attr);
    pthread_mutex_init(&(s->mutexO),&attr);

    pthread_mutexattr_destroy(&attr);

    for(int i=0; i<BUFFER;i++){
        s->bufH[i] = '\0';
        s->bufO[i] = '\0';
    }
    s->inH = 0;
    s->outH= 0;

    s->inO=0;
    s->outO = 0;

    pid_t p;
    
    p=fork();
    if(p == 0){
        generadorAgua();
    }

    p=fork();
    if(p==0){
        generadorHidrogeno();
    }

    p=fork();
    if(p==0){
        generadorOxigeno();
    }

    wait(NULL);
    return 0;
}