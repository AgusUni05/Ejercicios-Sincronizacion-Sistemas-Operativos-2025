#ifndef estructura
#define estructura

#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <stdlib.h>

#define CANT_PRODUCTORES 3
#define CANT_CONSUMIDORES 5
#define BUFFER 20
#define COLOR_AZUL  "\x1b[34m"
#define COLOR_ROJO  "\x1b[31m"
#define COLOR_RESET "\x1b[0m"

typedef struct{
    int buffer[BUFFER];
    int in;
    int out;
    sem_t lleno;
    sem_t vacio;
    pthread_mutex_t mutex;
} Sincronizador;

static inline int generarKey(){
    return ftok("/bin/ls",123);
}

#endif