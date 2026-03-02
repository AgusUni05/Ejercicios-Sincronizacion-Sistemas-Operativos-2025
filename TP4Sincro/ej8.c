#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

#define LECTORES 20
#define ESCRITORES 5
#define ITERACIONES 6

sem_t escritura,lectura;
pthread_mutex_t mutex;

void* escritor(void* args){
    int* id = (void*) args;
    for(int i=0; i< ITERACIONES;i++){
        sem_wait(&escritura);
        printf("Soy el escritor %d y estoy escribiendo el archivo...\n",id);
        usleep(10000);
        sem_post(&escritura);
    }
    printf("Escritor %d, termino\n",id);
    pthread_exit(NULL);
}

void* lector(void* args){
    int* id= (void*) args;
    for(int i=0;i<ITERACIONES;i++){

        pthread_mutex_lock(&mutex);
        if(sem_trywait(&lectura)== -1) // si  soy el primer lector en llegar
            sem_wait(&escritura); // no dejo que escriban los escritores
        else
            sem_post(&lectura); // sino devuelvo la escritura
        pthread_mutex_unlock(&mutex);

        sem_post(&lectura); //marco como que estoy leyendo
        printf("Lector con id %d esta leyendo el archivo \n",id);
        usleep(10000);
        sem_wait(&lectura); // termine de leer

        pthread_mutex_lock(&mutex);
        if(sem_trywait(&lectura)== -1)// soy el ultimo en leer
            sem_post(&escritura); // permito que escriban los escritores
        else sem_post(&lectura); //devuelvo el semaforo lectura
        pthread_mutex_unlock(&mutex);
        
    }
    printf("Lector %d, termino\n",id);
    pthread_exit(NULL);
}

int main(){
    sem_init(&lectura,0,0);
    sem_init(&escritura,0,1);
    pthread_mutex_init(&mutex,NULL);

    int id =0;
    pthread_t lectores[LECTORES];
    pthread_t escritores[ESCRITORES];

    for(int i=0;i<LECTORES;i++){
        id++;
        pthread_create(&lectores[i],NULL,lector,(void* ) id);
    }
    
    for(int i=0;i<ESCRITORES;i++){
        id++;
        pthread_create(&escritores[i],NULL,escritor,(void*) id);
    }


    for(int i=0;i<LECTORES;i++){
        pthread_join(lectores[i],NULL);
    }
    for(int i=0;i<ESCRITORES;i++){
        pthread_join(escritores[i],NULL);
    }

    sem_destroy(&lectura);
    sem_destroy(&escritura);
    pthread_mutex_destroy(&mutex);
    return 0;
}   