#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#define BUF_SIZE 256

//Funciones
int Random(void);

int main()
{
    /*Pipe values fd[0] is set up for reading, fd[1] is set up for writing*/
    //Escritura -> Lectura
    int pfd1[2]; //Productor -> Consumidor
    int pfd2[2]; //Consumidor -> Productor

    /*Arreglo donde se almacenan los datos de los procesos*/
    char values[50]={0,};
    /*Arreglo con el conteo de vocales*/
    char vocals[5]={0,};
    
    //buffer
    char buf[BUF_SIZE];

    //PID del proceso productor
    pid_t pidP;
	
    //variable para el proceso consumidor
    pid_t pidC;

    const char* messageOne = "Hello from child ONE.\n";
    const char* messageOne1 = "Hello from child ONE!!!.\n";
    const char* messageTwo = "Hello from child TWO.\n";

    const unsigned int commLen = strlen(messageOne) + 1;

    //Abrir pipes para la comunicacion entre procesos
    if (pipe(pfd1) == -1 || pipe(pfd2) == -1)
    {
        printf("Error opening pipe!\n");
        exit(1);
    }

    // Proceso Productor
    pidP = fork();
    switch (pidP)
    {
        case -1:
            printf("Error creando al Productor!\n");
            exit(1);

	/*El valor del padre sera el PID y el hijo es 0, 
	por eso este case es la accion del hijo*/
        case 0:
            printf("\nEjecutando proceso productor...\n");
            
            /* Cerrar sector de lectura del pipe 1 */
            close(pfd1[0]);
            /* Cerrar sector de escritura del pipe 2 */
            close(pfd2[1]);

            /* Escribir en pipe 1 */
            write(pfd1[1], messageOne, commLen);
            write(pfd1[1], messageOne1, commLen);
	    close(pfd1[1]);

            /* Leer del pipe 2 */
            read(pfd2[0], buf, commLen);
            close(pfd2[0]);

            printf("Message received in child ONE: %s", buf);
            printf("Terminando productor...\n");
            _exit(0);

        default:
        printf("Se ha creado al Productor con PID %d\n", pidP);
            break;
    }

    // Proceso consumidor
    pidC = fork();
    switch (pidC)
    {
        case -1:
            printf("Error creando al consumidor!\n");
            exit(1);
            
        case 0:
        
            printf("\nEjecutando proceso consumidor...\n");
            /*  Cerrar sector de lectura del pipe 2 */
            close(pfd2[0]);
            /* Cerrar sector de escritura del pipe 1 */
            close(pfd1[1]);

            /* Leer en pipe 1 */
            read(pfd1[0], buf, commLen);
	    printf("Message received in child TWO: %s", buf);
	    read(pfd1[0], buf, commLen);
	    printf("Message received in child TWO: %s", buf);
	    close(pfd1[0]);

            /* Escribir en pipe 2 */
            write(pfd2[1], messageTwo, commLen);
	    close(pfd2[1]);
            
            
            printf("Terminando consumidor...\n");
            _exit(EXIT_SUCCESS);

        default:
         printf("Se ha creado al Consumidor con PID %d\n", pidC);
            break;
    }

    /* Cerrar todos los pipes (Padre)*/
    if (close(pfd1[1]) == -1 || close(pfd2[0]) == -1 || 
        close(pfd2[1]) == -1 || close(pfd1[0]) == -1)
    {
        printf("Error closing pipes.\n");
        exit(EXIT_FAILURE);
    }

    printf("Finishing...\n");
    exit(EXIT_SUCCESS);
}

int Random(void)
{
	srand(time(NULL));
	return (97 + rand()%(25));
}
