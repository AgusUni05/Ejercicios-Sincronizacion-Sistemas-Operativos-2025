#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>


int main(){
	pid_t p= fork();
	
	if(p < 0)
		perror("Error en el fork");
		else if(p == 0)
			execlp("./tp1ej1","tp1ej1",NULL);
			else 
				if(p > 0){
				wait(NULL);
				printf("Mi hijo con pid %d, ya termino de ejecutarse \n",p);
			}
	return 1;
		}


