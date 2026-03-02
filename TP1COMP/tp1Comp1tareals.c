#include <stdio.h>
#include <unistd.h>


int main(){
    printf("Hola soy el primer hijo y estoy haciendo ls \n");
    execlp("ls","ls",NULL);
    return 0;
}