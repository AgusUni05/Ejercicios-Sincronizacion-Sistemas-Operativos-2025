#include "estructuraLabo24.h"


void corredor(int id){
    int idMem= shmget(generarKey(),sizeof(Sincronizador),0666);
    Sincronizador* s =(Sincronizador* ) shmat(idMem,0,0);

    while(1){
        pthread_mutex_lock(&(s->mutexCorredores));
        if(sem_trywait(&(s->semCorredores)) != 0)//soy el primer corredor
            sem_wait(&(s->instalaciones)); // quito la posibilidad de que entre otro atleta
        else
            sem_post(&(s->semCorredores));
        sem_post(&(s->semCorredores));
        pthread_mutex_unlock(&(s->mutexCorredores));

        printf(VERDE"Atleta  corredor con id %d entrando a las instalaciones...\n",id);
        fflush(stdout);
        usleep(rand() % 10000);
        printf(VERDE"Atleta corredor con id %d yendose...\n",id);
        fflush(stdout);
        sem_wait(&(s->semCorredores));

        pthread_mutex_lock(&(s->mutexCorredores));
        if(sem_trywait(&(s->semCorredores))!=0) // si soy el ultimo corredor
            sem_post(&(s->instalaciones)); // permito a otros atletas entrar
        else    
            sem_post(&(s->semCorredores));//sino devuelvo el semaforo que saque
        pthread_mutex_unlock(&(s->mutexCorredores));

        sleep(rand() % 3 +1);
        }
    exit(0);
}

int main(int argc,char *argv[]){
    srand(getpid());

    if(argc != 2){
        printf("Error en corredor, son 1 valor\n");
        return 1;
    }
    
    int id = atoi(argv[1]);

    corredor(id);

    return 0;
}