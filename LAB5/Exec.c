#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 

int main() 
{ 
	// Llamar a funcion exec
        execl("./Hello", '\0'); 
        //Exec reemplaza el proceso actual, por lo que las siguientes instrucciones ya no van a correr
        printf("This will not be seen"); 
      
    return 0; 
} 

