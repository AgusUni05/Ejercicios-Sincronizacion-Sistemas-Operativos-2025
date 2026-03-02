#include "estructuraLabo24.h"

int main(){
    

    printf("Creando los atletas\n");

    int idMsg = shmget(generarKey(),sizeof(Sincronizador),IPC_CREAT | 0666);

    Sincronizador* s = (Sincronizador*) shmat(idMsg,0,0);
    sem_init(&(s->semCorredores),1,0);
    sem_init(&(s->instalaciones),1,1);

    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_setpshared(&attr,PTHREAD_PROCESS_SHARED);
    
    pthread_mutex_init(&(s->mutexCorredores),&attr);

    pthread_mutexattr_destroy(&attr);

    pid_t p;
    int id=0;
    for(int i = 0; i<CANT_JABALINAS;i++){
        p=fork();
        if(p == 0){
            char id_str[10];
            snprintf(id_str,sizeof(id_str),"%d", id);
            
            char *args[] = {"./jabalina", id_str, NULL};
            execv("./jabalina",args);
            
            perror("execv falló");
            exit(1);
        }
        id++;
        }
    
    for(int i = 0; i<CANT_MARTILLOS;i++){
        p=fork();
        if(p == 0){
            char id_str[10];
            snprintf(id_str,sizeof(id_str),"%d",id);
            
            char *args[] = {"./martillo", id_str, NULL};
            execv("./martillo",args);
            
            perror("execv falló");
            exit(1);
        }
        id++;
        }
    for(int i = 0; i<CANT_CORREDORES;i++){
        p=fork();
        if(p == 0){
            char id_str[10];
            snprintf(id_str,sizeof(id_str),"%d", id);
            
            char *args[] = {"./corredor", id_str, NULL};
            execv("./corredor",args);
            
            perror("execv falló");
            exit(1);
        }
        id++;
        }
    for(int i=0;i< CANT_CORREDORES + CANT_JABALINAS + CANT_MARTILLOS;i++)
        wait(NULL);
    sem_destroy(&(s->semCorredores));
    pthread_mutex_destroy(&(s->mutexCorredores));

    shmdt(s);
    shmctl(idMsg,IPC_RMID,NULL);
    return 1;
}