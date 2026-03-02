#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

sem_t oxigeno,hidrogeno;

void* generadorOxigeno(){
    while(1){
        printf("Generando moleculas de oxigeno\n");
        fflush(stdout);
        for(int i=0;i<15;i++)
            sem_post(&oxigeno);
        printf("terminando de generar oxigeno..\n");
        fflush(stdout);

        usleep(15);
    }
    pthread_exit(NULL);
}

void* generadorHidrogeno(){
    while(1){
        printf("Generando moleculas de hidrogeno..\n");
        fflush(stdout);
        for(int i=0;i<20;i++)
            sem_post(&hidrogeno);
        printf("Hidrogeno generado\n");
        fflush(stdout);
        usleep(20);
    }
    pthread_exit(NULL);
}

int main(){
    srand(time(NULL));
    sem_init(&hidrogeno,0,0);
    sem_init(&oxigeno,0,0);

    pthread_t tipo;
    pthread_create(&tipo,NULL,generadorOxigeno,NULL);
    pthread_create(&tipo,NULL,generadorHidrogeno,NULL);

    while(1){
        sem_wait(&hidrogeno);
        sem_wait(&hidrogeno);
        sem_wait(&oxigeno);
        printf("Se armo una molecula de agua\n");
        fflush(stdout);
        usleep(rand()% 10000);
    }
    return 0;
}