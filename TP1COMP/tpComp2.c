#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void esperarHijos(int i){
    printf("ESPERANDO HIJOS %d \n",i);
    sleep(2);
    for(int j=0;j<i;j++){
        wait(NULL);
    }
}

void trabajarEnTareaP(int i){
        printf("Proceso %d trabajando en la tarea p%d, su padre es %d \n",getpid(),i,getppid());
        sleep(1);
        exit(0);
    }
void trabajarEnTareaP2(int i){
        printf("Proceso %d trabajando en la tarea p%d, su padre es %d \n",getpid(),i,getppid());
        sleep(1);
    }

void crearHijo(int i){
    pid_t pid;
    pid=fork();
    if(pid==0){
        trabajarEnTareaP(i);
    }
}
int main(){
    printf("Ejecucion previa S....\n");
   trabajarEnTareaP2(1);
   crearHijo(2);
   crearHijo(6);
   trabajarEnTareaP2(3);
   trabajarEnTareaP2(4);
   trabajarEnTareaP2(5);
   esperarHijos(2);
   crearHijo(7);
    crearHijo(8);
    esperarHijos(2);
    printf("Finalizacion de grafo paralelo, sigue la tarea F\n");
    return 0;



}