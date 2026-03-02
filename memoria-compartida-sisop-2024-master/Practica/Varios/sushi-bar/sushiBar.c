#include <unistd.h>
#include <stdio.h>
#include <semaphore.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <time.h>
#include <pthread.h>


#define CLIENTES 30
#define ASIENTOS 5

typedef struct Estructura{
	sem_t asientoLibre,turno,permitirIrse;
	pthread_mutex_t mutex;
} Sincronizador;

static inline int generarKey(){
	return ftok("/bin/ls",123);
}


void comer(Sincronizador* s){
	printf("Hay 5 personas sentadas en la mesa, se ponen a comer todos juntos...\n");
	fflush(stdout);
	sleep(rand() % 4 +1);
	printf("Liberando asientos...\n");
	fflush(stdout);
	for(int i=0;i<ASIENTOS;i++)
		sem_post(&(s->asientoLibre));
	for(int i=0;i<ASIENTOS;i++)
		sem_post(&s->permitirIrse);
	sem_post(&s->turno);
}

void cliente(int args){
	int id=args;
	int shmID = shmget(generarKey(),sizeof(Sincronizador), 0666);
	Sincronizador* s =(Sincronizador*) shmat(shmID,0,0);
	
	while(1){
		sem_wait(&s->turno); // espero a que sea mi turno para intentar sentarme
	
		pthread_mutex_lock(&s->mutex);
		sem_wait(&s->asientoLibre);

		printf("El cliente numero %d se sento ,pid = %d\n",id,getpid());
		if(sem_trywait(&s->asientoLibre)) // fui el ultimo en sentarme
		{printf("SOY EL ULTIMO id =%d\n",id);comer(s);}//nos ponemos a comer todos juntos
		else {sem_post(&s->asientoLibre); sem_post(&s->turno);}//devuelvo el asiento extra que agarre
		pthread_mutex_unlock(&s->mutex);
		sem_wait(&s->permitirIrse);//una vez sentado espero a que me dejen levantarme
		sleep(rand() %4 +1);
	
	}
}

int main(){
	srand(time(NULL));
	
	int idMsg= shmget(generarKey(),sizeof(Sincronizador),IPC_CREAT | 0666);
	Sincronizador* s =(Sincronizador*) shmat(idMsg,0,0);

	sem_init(&s->asientoLibre,1,ASIENTOS);
	sem_init(&s->permitirIrse,1,0);
	sem_init(&s->turno,1,1);
	
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_setpshared(&attr,PTHREAD_PROCESS_SHARED);
	pthread_mutex_init(&s->mutex,&attr);
	pthread_mutexattr_destroy(&attr);
	
	pid_t p;
	int args[CLIENTES];

	for(int i=0;i<CLIENTES;i++)
	{
	args[i]= i+1;
	p=fork();
	if(p==0){
		cliente(args[i]);
	}
	}
	wait(NULL);
	return 0;
}

