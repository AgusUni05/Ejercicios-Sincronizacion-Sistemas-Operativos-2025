#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(){
	int n;
	printf("Ingrese la cantidad de hijos a crear: \n");
	scanf("%d",&n);
	int p[n];
	for(int i=0; i<n;i++){
		p[i]= fork();
		if(p[i] == 0){
			printf("Hola soy un hijo, mi pid es %d, y el pid de mi padre es %d \n",(int) getpid(),(int) getppid());
			exit(0);
		}
	}
	
	for(int i=0; i<n;i++)
		waitpid(p[i],NULL,0);
		
	return 1;
}

