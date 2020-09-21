#include <stdio.h> 
#include <sys/types.h> 
#include <unistd.h> 

int main() 
{ 
  
    // Esta funcion va a crear una copia del proceso actual
    // El mismo programa se va a ejecutar de nuevo
    fork(); 
  
    printf("Hello world!\n"); 
    return 0; 
} 

