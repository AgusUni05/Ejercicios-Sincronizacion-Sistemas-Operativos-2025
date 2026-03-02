
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>

struct mensaje{
    long mtype;
    char cadena[255];

};

int main(){
    key_t key = ftok("/bin/ls",33);
    
    int n;
    printf("Ingrese la cantidad de mensajes a enviar: ");
    scanf("%d",&n);
    int queueID = msgget(key, IPC_CREAT | 0666);
    pid_t lector = fork();
    if(lector){
    
    for(int i = 0; i < n; i++){
        pid_t pid = fork();
        
        if(!pid){
            msgget(key,0666);
            long longitud = sizeof(struct mensaje) - sizeof(long);
            struct mensaje enviar;
            enviar.mtype = 1;

            snprintf(enviar.cadena, sizeof(enviar.cadena), "Hola soy el proceso numero: %d\n", i);
            msgsnd(queueID,&enviar,longitud,0);
            exit(0);
        }

        waitpid(pid,NULL,0);
    }
    }
    else{
        //leer los n mensajes
        msgget(key,0666);
        long longitud = sizeof(struct mensaje) - sizeof(long);
        struct mensaje msg;

        for(int i = 0; i< n;i++){
            msgrcv(queueID,&msg,longitud,0,0);
            printf("Soy el lector y recibi el mensaje numero %d, y el mensaje es: \n %s",i,msg.cadena);
        }
        exit(0);
    }
    waitpid(lector,NULL,0);
    msgctl(queueID, IPC_RMID, NULL);
    
    return 0;

}