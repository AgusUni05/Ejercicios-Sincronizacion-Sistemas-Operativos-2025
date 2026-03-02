#include "estructura.h"


void productor(int id){
    int idMsg = shmget(generarKey(),sizeof(Sincronizador),0666);
    Sincronizador* s= (Sincronizador*) shmat(idMsg,0,0);
    int prod=-1;
    while(1){

        sem_wait(&(s->vacio));
        prod = rand() % 100;
        printf(COLOR_ROJO "Productor con id %d, esta por ingresar un valor al buffer\n",id);
        pthread_mutex_lock(&(s->mutex));
        s->buffer[s->in] = prod;
        s->in = (s->in +1) % BUFFER;
        pthread_mutex_unlock(&(s->mutex));
        printf(COLOR_ROJO"Productor con id %d, produjo un elemento\n");
        sem_post(&(s->lleno));

        sleep(rand() % 4 +1);
    }
    exit(0);
}

int main(int argc,char* argv[]){
    srand(getpid());

    if(argc != 2){
        printf(COLOR_RESET"Error en productor, son 1 elemento\n");
        return 1;
    }

    int id = atoi(argv[1]);
    productor(id);
    return 0;
}