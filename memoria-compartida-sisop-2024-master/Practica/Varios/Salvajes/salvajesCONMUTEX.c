#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>


#define PORCIONES 10
#define SALVAJES 5

sem_t refill;
sem_t despertarCocinero;
sem_t ollaVacia;
sem_t ollaLlena;
pthread_mutex_t mutex;    

void* salvaje(void* arg) {
    while(1){
    
        pthread_mutex_lock(&mutex);
        if(sem_trywait(&ollaLlena) != 0){
            printf("No hay mas porciones, despierto al cocinero\n");
            sem_post(&despertarCocinero);
            sem_wait(&refill);
        }
        pthread_mutex_unlock(&mutex);
        
        printf("Salvaje comiendo una porcion\n");
        sem_post(&ollaVacia);
    }

}

void* cocinero(void* arg) {
    while(1){
        sem_wait(&despertarCocinero);
        for(int i = 0; i < PORCIONES; i++){
            sem_post(&ollaLlena);
            printf("El cocinero prepara una porcion\n");
            sleep(1);
            sem_wait(&ollaVacia);
        }
        printf("El cocinero termino de preparar las porciones\n");
        sem_post(&refill);
    }


}

int main(){
pthread_t hiloSalvajes[SALVAJES];
pthread_t hiloCocinero;

sem_init(&despertarCocinero,0,0);
sem_init(&ollaLlena,0,PORCIONES);
sem_init(&ollaVacia,0,0);
sem_init(&refill,0,0); 

pthread_mutex_init(&mutex,NULL);      

for(int i = 0; i < SALVAJES; i++){
    pthread_create(&hiloSalvajes[i], NULL, salvaje, NULL);
}

pthread_create(&hiloCocinero, NULL, cocinero, NULL);

for(int i = 0; i < SALVAJES; i++){
    pthread_join(hiloSalvajes[i], NULL);
}

pthread_join(hiloCocinero, NULL);

sem_destroy(&despertarCocinero);
sem_destroy(&ollaLlena);
sem_destroy(&ollaVacia);
sem_destroy(&refill);
pthread_mutex_destroy(&mutex);

return 0;
}