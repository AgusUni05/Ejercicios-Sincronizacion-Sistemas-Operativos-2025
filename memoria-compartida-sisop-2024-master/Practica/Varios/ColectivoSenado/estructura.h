#ifndef estructura

#define estructura

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <time.h>

#define CLIENTES 55
#define CAPACIDAD_COLECTIVO 25

typedef struct Estructura{
	sem_t lugarVacio,lugarOcupado,bajar,viajar,turno;
	pthread_mutex_t mutex;
} Sincronizador;


static inline int generarKey(){
	return ftok("/bin/ls",123);
}

#endif