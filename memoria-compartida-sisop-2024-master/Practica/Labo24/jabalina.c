#include "estructuraLabo24.h"

void jabalina(int id){
    int idMem = shmget(generarKey(),sizeof(Sincronizador),0666);
    Sincronizador* s= (Sincronizador*) shmat(idMem,0,0);

    while(1){
        sem_wait(&(s->instalaciones));
        printf(ROJO "Entro un atleta de jabalina con id %d\n",id);
        fflush(stdout);

        usleep(rand()%10000);

        printf(ROJO "Se esta yendo el atleta de jabalina con id %d\n",id);
        fflush(stdout);

        sem_post(&(s->instalaciones));
        sleep(rand() % 3+1);
    }
    exit(0);
}

int main(int argc,char* argv[]){
    srand(getpid());
    if(argc != 2){
        printf("Error en jabalina, 1 atributo.\n");
        return 1;
    }
    int id = atoi(argv[1]);
    jabalina(id);
    return 0;
}