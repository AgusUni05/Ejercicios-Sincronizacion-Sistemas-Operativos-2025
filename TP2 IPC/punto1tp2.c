#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>		// fork(), getpid(), getppid(), exelp()
#include <sys/wait.h>	// wait(), waitpid()
#include <string.h>     // strlen(), strcmp()

int main() {

    int pfd[2];
    pipe(pfd);

    int pid = fork();

    if(pid < 0){
        perror("Error al crear hijo");
    } else {
        if(pid == 0){
            close(pfd[1]);
            char leido[255];
            int seguir = 1;
            while(seguir){
                read(pfd[0], &leido, sizeof(leido));   // ahora llega con '\0'
                printf("Recibi: %s\n", leido); 
                if(strcmp(leido, "-1") == 0){
                    seguir = 0;
                }
            }
            close(pfd[0]);
            exit(0);
        } else {
            close(pfd[0]);
            char mensaje[255];
            int seguir = 1;
            while(seguir){
                printf("Ingrese el mensaje: \n");
                if (fgets(mensaje, sizeof(mensaje), stdin) == NULL) {
                    break;
                }

                // quitar '\n' final si viene
                size_t n = strlen(mensaje);
                if (n > 0 && mensaje[n - 1] == '\n') {
                    mensaje[n - 1] = '\0';
                }

                // enviar también el '\0' para que el hijo tenga una cadena válida
                write(pfd[1], mensaje, strlen(mensaje) + 1);

                if(strcmp(mensaje, "-1") == 0){
                    seguir = 0;
                }
            }
            close(pfd[1]);
            wait(NULL);
        }
    }

    return 0;
}