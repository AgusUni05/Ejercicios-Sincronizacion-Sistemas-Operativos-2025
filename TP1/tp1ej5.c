#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int sumaArreglo(int indice,int *arr,int tope){
	int x =0;
	for(int i = indice; i< tope ;i++)
		x += *(arr+ i);

	return x;
		
}
int main(){
	
	int arr[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
	int pfd[2];
	int saltos = 5;
	
	pipe(pfd);
	close(pfd[1]);
	int hijos[5];
	int cant=0;
	pid_t p;
	for(int i = 0; i<20;i+=5){
		p=fork();
		hijos[cant] = p;
		cant++;
		if(p==0){
			close(pfd[0]);
			int resParcial = sumaArreglo(i,arr,saltos);
			write(pfd[1],&resParcial,sizeof(resParcial));
			exit(0);
		}
		saltos+=5;
	
	}				
	for(int i=0; i <5; i++)
		waitpid(hijos[i],NULL,0);
	int sumatotal=0;
	int x=0;
	
	for(int i = 0; i<5; i++){
		read(pfd[0],&x,sizeof(x));
		sumatotal +=x;
	}
	
	return 1;		
		}
	

			
			
