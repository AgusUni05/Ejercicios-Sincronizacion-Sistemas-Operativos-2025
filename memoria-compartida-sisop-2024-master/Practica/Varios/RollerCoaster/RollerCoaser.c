#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <stdlib.h>

#define CAPACIDAD_VEHICULO 5
#define CLIENTES 30
#define COLOR_VERDE "\033[0;32m"
#define COLOR_AZUL "\033[0;34m"
#define COLOR_RESET "\033[0m"

sem_t asientoLibre,llenado,permitirBajar,turno;

void* cliente(void* args){
    int id = *((int* )args);

    while(1){
        
	sem_wait(&turno); // espero a que sea mi turno de subir
        
        sem_wait(&asientoLibre); // me siento
        printf(COLOR_AZUL"Se sento el cliente numero %d\n",id);
        fflush(stdout);
        if(sem_trywait(&asientoLibre) != 0){//no quedan espacios libres
            printf("EL ultimo en sentarse avisa que se lleno, id %d\n",id);
            sem_post(&llenado); // el que se acaba de sentar ya lleno el carro, hay que viajar
            
        }
        else{
            sem_post(&asientoLibre);//devuelvo el asiento que reste
	    sem_post(&turno);
            
        }
        
	
        
        printf(COLOR_AZUL"Cliente %d esperando por viajar\n",id);
        sem_wait(&permitirBajar);//espero a que me dejen bajar
        usleep(rand() % 700000+ 500000);
        printf(COLOR_AZUL"Cliente %d bajandose luego del viaje\n",id);
        sleep(rand() % 3 +1);
    }
    pthread_exit(0);
}

void* carro(){

    while(1){
        printf(COLOR_VERDE"Habilitando asientos\n");
        fflush(stdout);
        
        
        for(int i=0;i< CAPACIDAD_VEHICULO;i++){
            sem_post(&asientoLibre); // permito que se sienten clientes
        }
       	sem_post(&turno);//permito que empiezen a subirse los clientes

        sem_wait(&llenado);//espero a que me avisen que se lleno el carro
        sleep(1);//arrancando
        printf(COLOR_VERDE"Carro viajando...\n");
        fflush(stdout);
        sleep(rand()% 5+ 1);
        printf(COLOR_VERDE"Viaje terminado, permitiendo bajar a los clientes\n");
        fflush(stdout);

        for(int i=0;i<CAPACIDAD_VEHICULO;i++){
            sem_post(&permitirBajar); // terminado el viaje
        }
        sleep(2);// esperando para volver a habilitar
    }
    pthread_exit(0);
}

int main(){
    srand(time(NULL));

    sem_init(&asientoLibre,0,0);
    sem_init(&llenado,0,0);
    sem_init(&permitirBajar,0,0);
    sem_init(&turno,0,0);

   

    pthread_t vehiculo,personas[CLIENTES];

    pthread_create(&vehiculo,NULL,carro,NULL);
    int ids[CLIENTES];
    for(int i=0;i<CLIENTES;i++){
        ids[i]=i+1;
        pthread_create(&personas[i],NULL,cliente,&ids[i]);
    }

    for(int i=0;i<CLIENTES;i++){
        pthread_join(personas[i],NULL);
    }
    pthread_join(vehiculo,NULL);

    sem_destroy(&llenado);
    sem_destroy(&asientoLibre);
    sem_destroy(&permitirBajar);
    
    return 0;
}
