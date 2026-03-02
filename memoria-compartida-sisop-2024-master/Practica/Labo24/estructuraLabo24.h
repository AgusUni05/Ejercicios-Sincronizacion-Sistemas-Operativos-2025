#ifndef estructuraLabo24

#define estructuraLabo24
#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <sys/shm.h>

#define CANT_MARTILLOS 10
#define CANT_CORREDORES 10
#define CANT_JABALINAS 10
#define ROJO    "\033[1;31m"
#define VERDE   "\033[1;32m"
#define AMARILLO "\033[1;33m"

typedef struct {
    sem_t instalaciones;
    pthread_mutex_t mutexCorredores;
    sem_t semCorredores;
} Sincronizador;

static inline int generarKey(){
    return ftok("/bin/ls",123);
}

#endif