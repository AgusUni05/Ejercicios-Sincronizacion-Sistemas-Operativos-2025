#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>


//ID de servicios
#define C2F 5
#define F2C 6
#define CM2IN 7
#define IN2CM 8

//ID de tipo mensaje
#define REQ 1
#define DONE 3

typedef struct Mensaje{
    long mtype;
    int idCliente;
    int idServicio;
    float valorConversion;
    float valorResultado;
} Msg;

// Fahrenheit a Celsius: C = (F - 32) * 5/9
float fahrenheit_a_celsius(float fahrenheit) {
    return (fahrenheit - 32.0) * 5.0 / 9.0;
}

// Celsius a Fahrenheit: F = C * 9/5 + 32
float celsius_a_fahrenheit(float celsius) {
    return celsius * 9.0 / 5.0 + 32.0;
}

// Centímetros a Pulgadas: 1 cm = 0.393701 pulgadas
float cm_a_pulgadas(float cm) {
    return cm * 0.393701;
}

// Pulgadas a Centímetros: 1 pulgada = 2.54 cm
float pulgadas_a_cm(float pulgadas) {
    return pulgadas * 2.54;
}


void cliente(int idCliente,int numPedidos,key_t key){
    int qID= msgget(key,0);
    int i=0;
    long longitud = sizeof(Msg) - sizeof(long);
    Msg mensaje;
    mensaje.idCliente = idCliente;

    while(1){
        if(i<numPedidos){
            printf("CLiente numero %d, realizando el pedido numero %d \n",idCliente,i);
            fflush(stdout);

            mensaje.mtype = REQ;// requiero un servicio
            mensaje.idServicio = rand() % 4 + 5; // pedido entre 5 y 8
            mensaje.valorConversion = (rand() % 100) + 1; // valor aleatorio entre 1 y 100
            msgsnd(qID,&mensaje,longitud,0);
            i++;
            
        }
        else{
            mensaje.mtype =DONE;
            mensaje.idServicio = 0;
            mensaje.valorConversion =0;
            printf("Soy el cliente %d y ya termine.\n",idCliente);
            fflush(stdout);
            msgsnd(qID,&mensaje,longitud,0);
            exit(0);

        }

        msgrcv(qID,&mensaje,longitud,idCliente,0);
        printf("Cliente numero %d, recibi mi resultado, pedi el servicio %d y  con valor %.2f y el resultado fue %.2f \n",idCliente,mensaje.idServicio,mensaje.valorConversion,mensaje.valorResultado);
        fflush(stdout);
    }

}

void servidor(key_t key,int cantCLientes){
    int qID = msgget(key,0);
    int atendidos = 0;

    long longitud = sizeof(Msg) - sizeof(long);
    Msg mensaje;
    while(1){
        if (msgrcv(qID, &mensaje, longitud, REQ, IPC_NOWAIT) != -1){
            printf("Servidor recibio un req, cliente: %d\n",mensaje.idCliente);
            fflush(stdout);
            switch (mensaje.idServicio)
            {
            case C2F:
                mensaje.valorResultado = celsius_a_fahrenheit(mensaje.valorConversion);
                break;
            case F2C: 
                mensaje.valorResultado = fahrenheit_a_celsius(mensaje.valorConversion);
                break;
            case CM2IN:
                mensaje.valorResultado= cm_a_pulgadas(mensaje.valorConversion);
                break;
            case IN2CM:
                mensaje.valorResultado = pulgadas_a_cm(mensaje.valorConversion);
                break;
            default:
                break;
            }
            mensaje.mtype = mensaje.idCliente;
            msgsnd(qID,&mensaje,longitud,0);
            printf("Servidor termino un pedido..\n");
            fflush(stdout);
        }
        else{
            if(msgrcv(qID,&mensaje,longitud,DONE,IPC_NOWAIT) != -1)
                atendidos ++;
            if(atendidos == cantCLientes){
                printf("Servidor termino de atender todo. \n");
                fflush(stdout);
                exit(0);
            }

        }
    }
}



int main(int argc,char *argv[]){
    if(argc != 3) {
        printf("Uso: %s <numero1> <numero2>\n", argv[0]);
        return 1;
    }
    
    srand(time(NULL)); // Inicializar semilla para números aleatorios

    int numeroClientes = atoi(argv[1]);
    int numeroPedidos = atoi(argv[2]);

    //crear cola
    key_t key = ftok("bin/ls",123);

    int qID = msgget(key, IPC_CREAT | 0666);

    //creo servidor
    pid_t pidServ = fork();
    if(!pidServ){
        servidor(key,numeroClientes);
    }
    
    //creo los n clientes
    for(int i = 0 ; i<numeroClientes;i++){
        pid_t pidCliente= fork();
        if(!pidCliente){
            cliente(i + 10, numeroPedidos, key);  // ID único: 10, 11, 12, etc.
        }
    }

    //espero a todos antes de finalizar
    for(int i =0; i< (numeroClientes + 1); i++)
        wait(NULL);
    
    //cierro la cola
    msgctl(qID,IPC_RMID,NULL);
    return 0;

}