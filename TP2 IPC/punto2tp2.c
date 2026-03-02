#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

int main(void){
    int pfd1[2], pfd2[2];
    pipe(pfd1);
    pipe(pfd2);
    pid_t p= fork();

    if(p == 0){
        close(pfd1[1]);
        close(pfd2[0]);
    char buffer[100];
    read(pfd1[0], buffer, sizeof(buffer));
    printf("el mensaje que me llego es (hijo): %s\n", buffer);
    strcpy(buffer, "esta es la respuesta del hijo");
    write(pfd2[1], buffer, sizeof(buffer));
        exit(0);
    }
    else {
        close(pfd2[1]);
        close(pfd1[0]);
    char buffer[100];
    strcpy(buffer, "Este es el mensaje del padre");
    write(pfd1[1], buffer, sizeof(buffer));
    wait(NULL);
    read(pfd2[0], buffer, sizeof(buffer));
    printf("el mensaje que me llego es (padre): %s\n", buffer);
        
    }
    return 1;
}