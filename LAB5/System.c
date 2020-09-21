#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 

int main() 
{ 
	// Llamar a funcion system
        system("./Hello"); 	 
      
        //sytem NO reemplaza el proceso actual, por lo que las siguientes instrucciones si van a correr
        printf("This will be seen\n"); 
      
    return 0; 
} 

