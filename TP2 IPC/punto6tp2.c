#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define SIZE 20

struct enteros{
    int arr[20];
    int sumasParciales[10];
};

int main(){
    key_t key = ftok("/bin/ls",33);
    int memoriaID = shmget(key,sizeof(struct enteros), IPC_CREAT | 0666);//creo el segmento
    
    
    shmat(memoriaID,0,0);//padre se mapea al segmento
    struct enteros* msg = (struct enteros*) shmat(memoriaID,0,0);//obtengo el segmento para acceder a el
    if(msg < (struct enteros*) 0){ printf("Error en shmat"); exit(2);}
    for(int i=0; i<SIZE; i++){//lleno el segmento con valores
        msg->arr[i] = i+1;
    }
    int tope=2;
    int indiceSuma=0;

    for(int i = 0; i<SIZE; i+=2){
        pid_t pid =fork();

        if(!pid){
            struct enteros* msg = shmat(memoriaID,0,0);
            if(msg < (struct enteros*) 0 ){printf("Eror en el shmat"); exit(0);}
            int x= 0;
            for(int j=i;j<tope;j++)
                x+=msg->arr[j];
            msg->sumasParciales[indiceSuma] = x;
            printf("Hola soy un hijo, y la suma parcial del arreglo que hice fue desde %d hasta %d, y el resultado es %d \n",i,tope,x);
            shmdt(msg);
            exit(0);

        }  
        indiceSuma++;
        tope+=2;
        wait(0);
    }
    printf("Los hijos ya terminaron, calculando resultado total...\n \n");
    int sumaTotal =0;

    for(int i=0; i<indiceSuma;i++)
        sumaTotal += msg->sumasParciales[i];
    
    printf("El resultado total es: %d \n",sumaTotal);
    
    shmdt(msg);
    shmctl(memoriaID,IPC_RMID,NULL);
    return(0);
}
