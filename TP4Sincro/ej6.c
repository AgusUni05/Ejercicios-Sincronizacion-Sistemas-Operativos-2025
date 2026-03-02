#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>


typedef struct{
    sem_t tabaco,papel,fosforo,agente;
    pthread_mutex_t mesa;
}Sinc;

void fumadorTabaco(key_t k){
    int msgID= shmget(k,sizeof(Sinc),0666);
    Sinc* s= (Sinc*) shmat(msgID,0,0);
    

    while( 1){
        pthread_mutex_lock(&( s->mesa ));
            if(sem_trywait(& (s->fosforo)) == 0){ // si puedo agarrar el fosforo de la mesa
                if(sem_trywait(& (s-> papel) )==0){//si puedo agarrar el papel de la mesa
                    pthread_mutex_unlock(& (s->mesa));
                    printf("Fumador con tabaco esta armando\n");
                    usleep(10000);
                    printf("Fumador con tabaco fumando y avisando al agente\n");
                    sem_post(& (s->agente));

                } else { //no pude agarrar papel pero si fosforo, lo devuelvo a la mesa y me voy
                    sem_post(&( s->fosforo));
                    pthread_mutex_unlock(&(s->mesa));
                    }
            } else{
                //no pude agarrar fosforo, devuelvo la mesa y me voy
                pthread_mutex_unlock(&(s->mesa));
            }
            sleep(1); // espero un poco para terminar
    }
    exit(0);

}

void fumadorPapel(key_t k){
    int mID= shmget(k,sizeof(Sinc),0666);
    Sinc* s= (Sinc*) shmat(mID,0,0);

    while(1){
        pthread_mutex_lock(&(s->mesa));
        if(sem_trywait(& (s->fosforo)) == 0){ // si puedo agarrar fosforo
            if(sem_trywait(&( s->tabaco )) ==0){//si puedo agararr tabaco ya puedo armar y fumar
                pthread_mutex_unlock(&(s->mesa));
                printf("Fumador con papel armando..\n");
                usleep(10000);
                printf("Fumador con papel fumando y avisando al agente\n");
                sem_post(&(s->agente));
            } else{//pude agarrar fosforo pero no tabaco, devuelvo el fosforo y la mesa
                sem_post(&(s->fosforo));
                pthread_mutex_unlock(&(s->mesa));
            }

        }else{//no pude agarrar fosforo, devuelvo la mesa
            pthread_mutex_unlock(&(s->mesa));
        }
        sleep(1); //espero un poco para
    }
    exit(0);

}

void fumadorFosforo(key_t k){
    int mID = shmget(k,sizeof(Sinc),0666);
    Sinc* s= (Sinc*) shmat(mID,0,0);
    while(1){
        pthread_mutex_lock(& (s->mesa));
        if(sem_trywait(& (s->tabaco))==0) {//puedo agarrar tabaco
            if(sem_trywait(&(s->papel))==0){//puedo agarrar papel
                pthread_mutex_unlock(&(s->mesa));
                printf("Fumador con fosforo armando...\n");
                usleep(10000);
                printf("Fumador con fosforo fumando y avisando a agente\n");
                sem_post(&(s->agente));
            } else{//no pude agarrar papel pero si tabaco
                sem_post(&(s->tabaco));
                pthread_mutex_unlock(&(s->mesa));
            }
        } else{//no pude agarrar nada, devuelvo la mesa
            pthread_mutex_unlock(&(s->mesa));
        }
        sleep(1);
    }
    exit(0);

}
void agente(key_t k){
    int mID=shmget(k,sizeof(Sinc),0666);
    Sinc* s= (Sinc*) shmat(mID,0,0);
    int random;

    
    
    while(1){
        sem_wait(&(s->agente));
        pthread_mutex_lock(&(s->mesa)); // el agente tiene la mesa para llenarla
        random= rand() % 3 + 1;
        switch(random){
            case 1: 
                sem_post(&(s->fosforo));
                sem_post(&(s->papel));
                printf("Agente generando fosforo y papel para la mesa\n");
                break;
            case 2:
                sem_post(&(s->papel));
                sem_post(&(s->tabaco));
                printf("Agente generando papel y tabaco para la mesa\n");
                break;
            case 3:
                sem_post(&(s->tabaco));
                sem_post(&(s->fosforo));
                printf("Agente generando tabaco y fosforo para la mesa\n");
                break;
        }
        pthread_mutex_unlock(&(s->mesa));
    }
    
    exit(0);
}
int main(){
    srand(time(NULL)); // Inicializar semilla aleatoria
    
    key_t k= ftok("/bin/ls",12);

    int mID= shmget(k,sizeof(Sinc),IPC_CREAT | 0666);
    Sinc* s = (Sinc* ) shmat(mID,0,0);

    sem_init(&(s->agente),1,1);
    sem_init(&(s->fosforo),1,0);
    sem_init(&(s->papel),1,0);
    sem_init(&( s->tabaco),1,0);

    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&(s->mesa),&attr);
    pthread_mutexattr_destroy(&attr);

    pid_t p;

    p=fork();
    if(p==0)
        agente(k);
    
    p=fork();
    if(p==0)
        fumadorPapel(k);

    p=fork();
    if(p==0)
        fumadorFosforo(k);
    
    p=fork();
    if(p==0)
        fumadorTabaco(k);
    
    //innecesario poruqe nunca va a llegar aca
    for(int i=0;i<4;i++)
        wait(NULL);
    
    sem_destroy(&(s->agente));
    sem_destroy(&(s->fosforo));
    sem_destroy(&(s->papel));
    sem_destroy(&( s->tabaco));
    pthread_mutex_destroy(&( s->mesa));
    
    shmdt(s);
    shmctl(mID,IPC_RMID,NULL);

    return 0;
    

}