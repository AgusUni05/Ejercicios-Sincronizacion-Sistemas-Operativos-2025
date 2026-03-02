#include <stdio.h>


#include <unistd.h> //PARA FORK()
#include <sys/types.h> // para pid_t

#include <pthread.h> //para pthreads
#include <bits/pthreadtypes.h>

#include <sys/ipc.h>
#include <sys/msg.h>//para cola de mensajes
#include <sys/shm.h>//para segmento compartido

void * ejemplo(void * args){
    int x = (int) args;
    printf("Testeo %d",x);
}

int main(){

/*PROCESOS---------------------------------------------------------------------------------------------------------------------------------------------------------------------------- */

    pid_t pid = fork(); 
    //CREO UN HIJO DESDE ACA, RETORNO 0 SI SOY EL HIJO
    // PID DEL HIJO SI ESTOY EN EL PADRE

    wait(NULL); // espero a que algun hijo termine
    //retorna el pid del proceso que termino

    waitpid(pid,NULL,0); 
    //espero a que termine el hijo pid, el segundo es el estado en el que termino

    exceclp("ls","ls",NULL); // carga img ejecutable

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------- */




/*HILOS---------------------------------------------------------------------------------------------------------------------------------------------------------------------------- */
     int pthread_create(
    pthread_t *thread,           // puntero a pthread_t donde se guarda el tid del hilo

    const pthread_attr_t *attr,  // atributos del hilo (NULL = por defecto) sino pthread_attr_init(&attr);

    void *(*start_routine)(void*), // función que ejecuta el hilo (tener en cuenta, que la funcion a la que llama 
                                //debe recibir *void como argumento retornar *void y
    void *arg                    // argumento que recibe la función del hilo (si necesitas pasar datos, generalmente se pasa un puntero a
                                    //un struct
); // retorna 0 si el hilo fue creado correctamente

pthread_t hilo;
int num =10; // le podria pasar un struct
pthread_create(&hilo,NULL,ejemplo,num);


int pthread_join(
    pthread_t thread,   // identificador del hilo a esperar
    void **retval       // puntero donde guardar el valor retornado por el hilo (si no espero nada, pongo NULL)
);

pthread_join(hilo,NULL);
//ACORDARSE COMPILAR CON -pthread

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------- */




/*PIPES---------------------------------------------------------------------------------------------------------------------------------------------------------------------------- */
    
// Se declara un arreglo de enteros, de dos componentes
int arreglo[2];
// Que lo usaremos con los valores 0 para lectura y 1 para escritura
//Luego ejecutamos la function pipe(), donde el parametron sera el arreglo creado anteriormente
pipe(arreglo);
//Con esto ya tenemos el pipe creado, que luego al crear un proceso fork(), se duplicara
//Hacer pipe antes de crear hijos, antes del fork
//CERRAR LO QUE NO SE VA A USAR
close(arreglo[0]); //CIERRO LECTURA
close(arreglo[1]); //CIERRO ESCRITURA

//lo que cierro es opuesto entre padre e hijo, si escribo del padre al hijo, padre cierra 0 hijo cierra 1
char *msg = "HOla";
char buffer[20];
write(arreglo[1],msg,sizeof(msg));
read(arreglo[0],buffer,sizeof(buffer));
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------- */




/*COLA DE MENSAJES---------------------------------------------------------------------------------------------------------------------------------------------------------------------------- */


//Se debera usar un struct
struct mensaje{
	long mtype; // SI O SI DEBE ESTAR ESTO
	char msg[100]; //LO que sea
};

//crear una key
key_t key=ftok("bin/ls",123); //asi o directamenteu un numero

// creamos la cola de mensajes
int queueID = msgget(key, IPC_CREAT | 0666); //Esto crea la cola y se vincula (Solo uno debe crearla)
// Luego si otro proceso quiere unirse usara:
int queueID = msgget(key, 0666);
//puede retornar valor -1 si hubo error
//Luego para enviar mensaje
/*int msgsnd(int msqid, // queueID
	   struct msgbuf *msgp, // Puntero al struct que contendra el mensaje
	   int msgsz,	// Size del struct menos sizeof(long)
	   int msgflg);	// Son flags, normalmente 0 (comportamiento bloqueante, espera si la cola esta llena)
*/
long longitud = sizeof(struct mensaje) - sizeof(long); //SIEMPRE
struct mensaje enviar;
enviar.mtype = 1; //SIEMPRE DARLE ALGUN TIPO
//snprintf(enviar.cadena, sizeof(enviar.cadena), "Hola soy el proceso numero: %d\n", i);
msgsnd(queueID,&enviar,longitud,0);

/* para recibir el mensaje
int msgrcv(int msqid,	// queueID
           struct msgbuf *msgp, // Puntero al struct que va a recibir el mensaje
           int msgsz,  // Size del struct menos long  (sizeof(struct mensaje) - sizeof(long))
	   long msgtyp,  // Si es 0, devuelve el primero que este en la cola sin importar el tipo, si pones 1 te da el primero en la cola
			    de ese tipo (En nuestro caso, con 1 nos daria el primer char [100] que este en la cola)
           int msgflg);	// Son flags, normalmente 0 (comportamiento bloqueante, espera si la cola esta vacia)

*/
long tipo=1; //SI ES 0 ES EL PRIMERO DE LA COLA, SI ES DISTINTO DE CERO EL PRIMERO DE LA COLA CON ESE NUMERO
struct mensaje recibido;
msgrcv(queueID,&recibido,longitud,tipo,0);

//una vez terminada 
msgctl(queueID, IPC_RMID, NULL);
//SIEMPRE ACORDARSE DE CERRAR LA COLA
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------- */




/*SEGMENTO MEMORIA COMPARTIDA---------------------------------------------------------------------------------------------------------------------------------------------------------------------------- */
struct ejemplo{
    int i;
    char buf[10]; //lo que sea, debo crear el struct con algo
};
key_t key2 = ftok("/bin/ls",33);
    int memoriaID = shmget(key2,sizeof(struct ejemplo), IPC_CREAT | 0666);//creo el segmento solo 1 y antes del fork
    struct ejemplo* ej = (struct ejemplo *) shmat(memoriaID,0,0); // lo hace el padre y todo el resto que quiera mapearse al segmento
    //SI ES PUNTERO ACCEDER A LOS CAMPOS CON ->, ej->i =1;
    if( ej < (struct ejemplo *) 0) {printf("Eror en el shmat"); exit(2);}
    shmdt(ej); // proceso se desvincula del segmento

    shmctl(memoriaID,IPC_RMID,NULL); //cierro el segmento de memoria

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------- */
#include <semaphore.h>
#include <pthread.h>

sem_init(&(s->semCorredores),1,0);
    sem_init(&(s->instalaciones),1,1);

    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_setpshared(&attr,PTHREAD_PROCESS_SHARED);
    
    pthread_mutex_init(&(s->mutexCorredores),&attr);

    pthread_mutexattr_destroy(&attr);
}

p=fork();
        if(p == 0){
            char id_str[10];
            snprintf(id_str,sizeof(id_str),"%d", id);
            
            char *args[] = {"./jabalina", id_str, NULL};
            execv("./jabalina",args);
            
            perror("execv falló");
            exit(1);