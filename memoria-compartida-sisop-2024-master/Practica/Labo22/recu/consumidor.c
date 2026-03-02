#include "estructura.h"

void consumidor(int id){
    int idMsg = shmget(generarKey(),sizeof(Sincronizador),0666);
    Sincronizador* s= (Sincronizador*) shmat(idMsg,0,0);
    int res=-1;
    while(1){

        sem_wait(&(s->lleno));// consumidor espera a que haya un espacio ocupado
        printf(COLOR_AZUL "Consumidor numero %d, esta por consumir un valor del buffer\n",id);
        fflush(stdout);

        pthread_mutex_lock(&(s->mutex));
        res = s->buffer[s->out];
        s->out = (s->out +1 )%BUFFER;
        pthread_mutex_unlock(&(s->mutex));

        sem_post(&(s->vacio));
        printf(COLOR_AZUL "Consumidor numero %d consumio un recurso del buffer y obtuvo: %d\n",id,res);
        fflush(stdout);
        sleep(rand()% 4 +1);
        res=-1;
    }
    exit(0);
}

int main(int argc,char* argv[]){
    srand(getpid());

    if(argc != 2){
        printf(COLOR_RESET "Error en consumidor, son 2 args\n");
        return 1;
    }

    int id = atoi(argv[1]);
    consumidor(id);
}