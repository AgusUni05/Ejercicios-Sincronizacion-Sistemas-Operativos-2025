#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

struct mensaje{
    long mtype;
    char cadena[255];

};

int main(){
    key_t key = ftok("/bin/ls", 33);
    
    int queueID = msgget(key, IPC_CREAT | 0666);

    pid_t pid = fork();

    if(pid){
        msgget(key,0666);
        struct mensaje msg;
        msg.mtype = 1;
        
        strcpy(msg.cadena,"Hola soy el padre, y estoy enviandole un mensaje a mi hijo \n");
        long longitud = sizeof(struct mensaje) - sizeof(long);

        msgsnd(queueID,&msg,longitud,0);
        wait(0);
    }
    else{
         msgget(key,0666);

        struct mensaje msgRecibido;
        //quiero recibir el mensaje de tipo 1 enviado por el padre
        msgrcv(queueID,&msgRecibido,sizeof(struct mensaje)-sizeof(long),1,0);
        printf("El mensaje recibido del padre es: \n %s",msgRecibido.cadena);
        exit (0);
    }
    //cierro la cola
    msgctl(queueID, IPC_RMID, NULL);

    return 0;
}

