#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>

#define ITERATIONS 10
sem_t semAB,semC,semD,semE,semX;
pthread_mutex_t m;

//secuencia (A o)
void * printA(void * args){
    for(int i=0 ; i< ITERATIONS ; i++){
        pthread_mutex_lock(&m);
        sem_wait(&semAB);
        sem_wait(&semX);
        printf("A\n");
        sem_post(&semC);
        sem_post(&semD);
        pthread_mutex_unlock(&m);
        
    }
    return NULL;
}

void* printB(void* args){
    for(int i=0;i<ITERATIONS;i++){
        pthread_mutex_lock(&m);
        sem_wait(&semAB);
        sem_wait(&semX);
            printf("B\n");
        sem_post(&semC);
        sem_post(&semD);
        pthread_mutex_unlock(&m);
    }
    return NULL;
}

void* printC(void * args){
    for(int i=0;i<ITERATIONS;i++){
        sem_wait(&semC);
        sem_wait(&semC);
        sem_wait(&semC);
        printf("C\n");
        sem_post(&semE);
        sem_post(&semX);
        sem_post(&semX);
        sem_post(&semAB);
    }
    return NULL;
}

void* printD(void * args){
    for(int i=0; i<ITERATIONS;i++){
        sem_wait(&semD);
        sem_wait(&semD);
        sem_wait(&semD);
        printf("D\n");
        sem_post(&semE);
        sem_post(&semX);
    }
    return NULL;
}

void* printE(void* args){
    for(int i =0; i<ITERATIONS;i++){
        sem_wait(&semE);
        printf("E\n");
        sem_post(&semAB);
    }
    return NULL;
}

int main(){

    sem_init(&semAB,0,1);
    sem_init(&semC,0,2);
    sem_init(&semD,0,0);
    sem_init(&semE,0,0);
    sem_init(&semX,0,1);

    pthread_mutex_init(&m,NULL);
    pthread_t ha,hb,hc,hd,he;
    pthread_create(&ha,NULL,printA,NULL);
    pthread_create(&hb,NULL,printB,NULL);
    pthread_create(&hc,NULL,printC,NULL);
    pthread_create(&hd,NULL,printD,NULL);
    pthread_create(&he,NULL,printE,NULL);

    pthread_join(ha,NULL);
    pthread_join(hb,NULL);
    pthread_join(hc,NULL);
    pthread_join(hd,NULL);
    pthread_join(he,NULL);

    sem_destroy(&semAB);
    sem_destroy(&semC);
    sem_destroy(&semD);
    sem_destroy(&semE);
    sem_destroy(&semX);

    pthread_mutex_destroy(&m);
    return 0;
}