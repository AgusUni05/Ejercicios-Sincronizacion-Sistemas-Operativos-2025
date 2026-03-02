#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N 10

sem_t impresora1,impresora2,turno[3],solicito[3];
pthread_mutex_t mutex;



int requerirImpresora(int id);
void liberar(int n);

void* manager(){
    while(1){
        pthread_mutex_lock(&mutex);
        // Revisar prioridad 2 primero (mayor prioridad)
        if(sem_trywait(&solicito[2]) == 0) {
            sem_post(&turno[2]);
        }
        else if(sem_trywait(&solicito[1])== 0) {
            sem_post(&turno[1]);
        }
        else if(sem_trywait(&solicito[0])== 0) {
            sem_post(&turno[0]);
        }
        pthread_mutex_unlock(&mutex);
        usleep(10000); 
    }
}

void* usuario(void* args){
    int id = *((int*) args); 
    int prior = rand() % 3;  // 0, 1, 2 (0=baja, 1=media, 2=alta)
    printf("Usuario %d tiene prioridad %d\n", id, prior);
    
    while(1){
        sem_post(&solicito[prior]); // solicito con mi prioridad que quiero entrar
        sem_wait(&turno[prior]);// espero a que el manager me deje pedir impresora
        
        int n = requerirImpresora(id);
        printf("Usuario %d (prioridad %d) usando impresora %d\n", id, prior, n);
        sleep(rand()%2+1);  // Tiempo de impresión
        
        liberar(n);
        
        // Mayor prioridad espera mas para dejar a los de menos que soliciten
        sleep(prior + rand()%2);
    }
}
void liberar(int n){
    if(n == 1) {
        sem_post(&impresora1);
    } else {
        sem_post(&impresora2);
    }
    printf("Impresora %d liberada\n", n);
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
    srand(getpid());

    sem_init(&impresora1,0,1);
    sem_init(&impresora2,0,1);
    for(int i=0;i<3;i++){
        sem_init(&solicito[i],0,0);
        sem_init(&turno[i],0,0);
    
    }
    pthread_mutex_init(&mutex,NULL);

    pthread_t hilos[N];
    pthread_t man;
    int ids[N];  // Array para almacenar los IDs
    
    for(int i=0;i<N;i++){
        ids[i] = i + 1;  // ID de 1 a 10
        pthread_create(&hilos[i],NULL,usuario, &ids[i]);  // Pasar la dirección del ID correspondiente
    }
    pthread_create(&man,NULL,manager,NULL);

    for(int i=0;i<N;i++)
        pthread_join(hilos[i],NULL);
    
    pthread_join(man,NULL);
    pthread_mutex_destroy(&mutex);
    sem_destroy(&impresora1);
    sem_destroy(&impresora2);
    for(int i=0;i<3;i++){
        sem_destroy(&solicito[i]);
        sem_destroy(&turno[i]);
    
    }
    return 0;
}