#include "estructura.h"

int main(){
    srand(getpid());
    int idMsg = shmget(generarKey(),sizeof(Sincronizador),IPC_CREAT | 0666);
    Sincronizador* s= (Sincronizador*) shmat(idMsg,0,0);

    sem_init(&(s->lleno),1,0);
    sem_init(&(s->vacio),1,BUFFER);
    
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_setpshared(&attr,PTHREAD_PROCESS_SHARED);

    pthread_mutex_init(&(s->mutex),&attr);
    pthread_mutexattr_destroy(&attr);

    s->in =0;
    s->out=0;
    for(int i=0;i<BUFFER;i++){
        s->buffer[i]=-1;
    }
    pid_t p;

    for(int i=0;i<CANT_CONSUMIDORES;i++){
        p=fork();
        if(p==0){
            char buf[20];
            snprintf(buf,sizeof(buf),"%d",(i+1));

            char* args[]={"./consumidor",buf,NULL};
            execv("./consumidor",args);

            perror("Execv de consumidor fallo\n");
            exit(0);
        }
    }

    for(int i=0;i<CANT_PRODUCTORES;i++){
        p=fork();
        if(p==0){
            char buf[20];
            snprintf(buf,sizeof(buf),"%d",(i+1));

            char* args[]={"./productor",buf,NULL};
            execv("./productor",args);

            perror("Execv de productor fallo\n");
            exit(0);
        }
    }

    for(int i=0;i < (CANT_PRODUCTORES+CANT_CONSUMIDORES);i++)
        wait(NULL);
    
    return 1;
}