#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define CANT_HOMBRES 10
#define CANT_MUJERES 10
#define COLOR_ROSA    "\x1b[38;5;205m"
#define COLOR_AZUL    "\x1b[34m"
sem_t bano,mujer,hombre,turno;
pthread_mutex_t mutexMujer,mutexHombre;

void* mujerLoop(void* args){
    int id = *((int* ) args);
    while(1){
        pthread_mutex_lock(&mutexMujer);
        if(sem_trywait(&mujer) != 0){//si soy la primer mujer
            sem_wait(&turno); // agarro si es mi turno
            sem_wait(&bano);//espero para entrar al bano
        }
        else
            sem_post(&mujer);
        sem_post(&mujer);
        pthread_mutex_unlock(&mutexMujer);

        printf(COLOR_ROSA"La mujer numero %d entro al sanitario \n",id);
        fflush(stdout);
        usleep(rand() %100000);
        printf(COLOR_ROSA"La mujer numero %d sale del sanitario \n",id);
        fflush(stdout);

        sem_wait(&mujer);

        pthread_mutex_lock(&mutexMujer);
        if(sem_trywait(&mujer) != 0){
            sem_post(&bano);//soy la ultima jermu libero la puerta del bano
            sem_post(&turno);
        }
        else 
            sem_post(&mujer);
        pthread_mutex_unlock(&mutexMujer);
        sleep(rand() %3 +1);
    }
    pthread_exit(0);
}

void* hombreLoop(void* args){
    int id = *((int*)args);
    while(1){
        pthread_mutex_lock(&mutexHombre);
        if(sem_trywait(&hombre) != 0){
            sem_wait(&turno);
            sem_wait(&bano);
        }
        else   
            sem_post(&hombre);
        sem_post(&hombre);
        pthread_mutex_unlock(&mutexHombre);

        printf(COLOR_AZUL"Hombre numero %d entro al sanitario\n",id);
        fflush(stdout);
        usleep(rand() % 100000);
        printf(COLOR_AZUL"Hombre numero %d saliendo del sanitario \n", id);
        fflush(stdout);

        sem_wait(&hombre);
        
        pthread_mutex_lock(&mutexHombre);
        if(sem_trywait(&hombre) != 0){
            sem_post(&turno);
            sem_post(&bano);
        }
        else  
            sem_post(&hombre);
        pthread_mutex_unlock(&mutexHombre);
        sleep(rand() % 3 +1);
    }
    pthread_exit(0);
}

int main(){
    srand(getpid());

    sem_init(&bano,0,1);
    sem_init(&hombre,0,0);
    sem_init(&mujer,0,0);
    sem_init(&turno,0,1);
    pthread_mutex_init(&mutexHombre,NULL);
    pthread_mutex_init(&mutexMujer,NULL);

    int id=0;
    pthread_t hombres[CANT_HOMBRES];
    for(int i=0; i<CANT_HOMBRES;i++){
        pthread_create(&hombres[i],NULL,hombreLoop,&id);
        id++;
    }
    
    pthread_t mujeres[CANT_MUJERES];
    for(int i=0;i<CANT_MUJERES;i++){
        pthread_create(&mujeres[i],NULL,mujerLoop,&id);
        id++;
    }

    for(int i=0;i<CANT_HOMBRES;i++){
        pthread_join(hombres[i],NULL);
    }
    for(int i=0;i<CANT_MUJERES;i++){
        pthread_join(mujeres[i],NULL);
    }

    sem_destroy(&hombre);
    sem_destroy(&mujer);
    sem_destroy(&bano);
    pthread_mutex_destroy(&mutexHombre);
    pthread_mutex_destroy(&mutexMujer);
    return 0;
}