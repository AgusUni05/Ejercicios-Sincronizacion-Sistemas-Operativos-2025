#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

struct mensajeMemoria{
    int valor;
};

int main(){
    key_t key = ftok("/bin/ls",33);
    int memoriaID = shmget(key, sizeof(struct mensajeMemoria), IPC_CREAT | 0666);

    if(memoriaID<0){printf("Fallo el shmget"); exit(2);}

    pid_t pid = fork();
    if(pid<0){ printf("Fallo el fork"); exit(2);}

    if(pid){
        shmget(key,sizeof(struct mensajeMemoria),0);
        
        struct mensajeMemoria* msg = (struct mensajeMemoria*) shmat(memoriaID,0,0);
        if(msg < (struct mensajeMemoria*) 0){ printf("Error en shmat"); exit(2);}
        msg->valor = 69;
        shmdt(msg);
    }
    else{
        shmget(key,sizeof(struct mensajeMemoria),0);

        struct mensajeMemoria* recibido = shmat(memoriaID,0,0);
        printf("Soy el hijo y el entero que habia en memoria es: %d \n ",recibido->valor);
        if(recibido < (struct mensajeMemoria *) 0){ printf("Error en shmat"); exit(2);}
        shmdt(recibido);
        exit(0);

    }
    waitpid(pid,NULL,0);
    shmctl(memoriaID, IPC_RMID, NULL);
    return 0;

}