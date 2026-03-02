#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define SILLAS 5
#define CLIENTES 10
#define COLOR_ROJO    "\x1b[31m"
#define COLOR_AZUL    "\x1b[34m"
#define COLOR_VIOLETA "\x1b[35m"
#define COLOR_RESET   "\x1b[0m"

sem_t semBarbero, silla, atendido;
pthread_mutex_t mutex;



void* barbero(){
    
    while(1){
        if(sem_trywait(&semBarbero) != 0){//si no tengo clientes esperando
        printf(COLOR_RESET "Barbero durmiendo...\n");
        sem_wait(&semBarbero); // duermo hasta que me despierten
        printf(COLOR_VIOLETA"Barbero despierto, atiende un cliente\n");
        fflush(stdout);
        }
        else{ printf(COLOR_VIOLETA "Barbero no se duerme porque sigue habiendo clientes\n");}
        fflush(stdout);
        
        usleep(500000); // atiendo a un cliente 
        sem_post(&atendido); // señalo que terminé de atender
        
       
        printf(COLOR_VIOLETA"Un cliente fue atendido \n");
        fflush(stdout);
        
        
    }
    pthread_exit(NULL);
}

void* cliente(void* arg){
    int id = *(int*)arg;
    
    while(1){
        
        // Intento ocupar una silla
        pthread_mutex_lock(&mutex);
        if(sem_trywait(&silla) == 0){ 
            pthread_mutex_unlock(&mutex);
            printf(COLOR_AZUL"Cliente %d entra a la barberia y se sienta\n", id);
            fflush(stdout);
            
            sem_post(&semBarbero);// despierto al barbero

            sem_wait(&atendido);// Espero a ser atendido

            printf(COLOR_AZUL"Cliente %d sale de la barberia\n", id);
            fflush(stdout);
            sem_post(&silla);//  libero una silla para el próximo
            sleep(rand()% 4+1); //si me pudieron atender tardo un rato mas en entrar
        } else {
            pthread_mutex_unlock(&mutex);
            printf(COLOR_ROJO"Cliente %d se fue por falta de espacio \n",id);
            fflush(stdout);
        }
        
        // Tiempo antes de volver a intentar
        sleep(rand()%3+1); 
    }
    pthread_exit(NULL);
}

int main(){
    srand(time(NULL)); 
    
    sem_init(&semBarbero,0,0);
    sem_init(&silla,0,SILLAS);
    sem_init(&atendido,0,0);
    pthread_mutex_init(&mutex,NULL);

    pthread_t b, clientes[CLIENTES];
    int ids[CLIENTES];
    
    pthread_create(&b,NULL,barbero,NULL);
    
    for(int i=0; i< CLIENTES;i++){
        ids[i] = i+1;
        pthread_create(&clientes[i],NULL,cliente,&ids[i]);
    }

    for(int i=0; i < CLIENTES; i++)
        pthread_join(clientes[i], NULL);
    

    pthread_join(b, NULL);

    sem_destroy(&semBarbero);
    sem_destroy(&silla);
    sem_destroy(&atendido);
    pthread_mutex_destroy(&mutex);

    return 0;
}