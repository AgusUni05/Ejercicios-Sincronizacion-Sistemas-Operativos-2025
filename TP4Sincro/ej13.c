#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N 10

sem_t impresora1,impresora2;
pthread_mutex_t mutex;


int requerirImpresora(int id);
void liberar(int n);


void* usuario(void* args){
    int id = *((int*) args);  
    while(1){
        int imp= requerirImpresora(id);
        sleep(rand()%3+1);
        liberar(imp);
    }
}
void liberar(int n){
    if(n == 1)
        sem_post(&impresora1);
        else sem_post(&impresora2);
    printf("Impresora liberada\n");
}

int requerirImpresora(int id){
    while(1){
        pthread_mutex_lock(&mutex);
        if(sem_trywait(&impresora1) == 0){
            pthread_mutex_unlock(&mutex);
            printf("Usando impresora 1 cliente numero %d\n",id);
            return 1;
        }
        if(sem_trywait(&impresora2)== 0){
            pthread_mutex_unlock(&mutex);
            printf("Usando impresora 2 cliente  numero %d\n",id);
            return 2;
        }
    pthread_mutex_unlock(&mutex);
    }
}


int main(){
    sem_init(&impresora1,0,1);
    sem_init(&impresora2,0,1);

    pthread_mutex_init(&mutex,NULL);

    pthread_t hilos[N];
    int ids[N];  // Array para almacenar los IDs
    
    for(int i=0;i<N;i++){
        ids[i] = i + 1;  // ID de 1 a 10
        pthread_create(&hilos[i],NULL,usuario, &ids[i]);  // Pasar la dirección del ID correspondiente
    }

    for(int i=0;i<N;i++)
        pthread_join(hilos[i],NULL);
    
    pthread_mutex_destroy(&mutex);
    sem_destroy(&impresora1);
    sem_destroy(&impresora2);
    return 0;
}