#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
	pid_t p = fork();
	
	if(p<0)
		perror("Error con fork");
		else if(p == 0)
			printf("Soy el hijo, mi pid = %d, el pid de mi padre = %d \n", (int) getpid(), (int) getppid());
			else if(p > 0){
				wait(NULL);
				printf("Soy el padre, mi pid = %d, pid de mi hijo = %d, pid de mi padre %d \n",(int) getpid(),p, getppid());
			}
	return 1;
}

