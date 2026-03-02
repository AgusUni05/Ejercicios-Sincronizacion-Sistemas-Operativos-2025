#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(){
    pid_t p1 = fork();
    if(p1<0)
        perror("Error creando el primer hijo");
    else if(p1 == 0){
        execlp("./tp1ls","./tp1ls",NULL);
    }
        else if(p1>0){
            pid_t p2 = fork();
            if(p2 == 0){
                execlp("./tp1contar","./tp1contar",NULL);
            }
            else if(p2>0)
            {
                wait(NULL);
                wait(NULL);
                printf("Soy el padre y mis hijos ya han terminado \n");
            }

        }
        
}