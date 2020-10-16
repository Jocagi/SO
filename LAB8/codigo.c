#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

int threadCount = -1;
bool showText = false;
pthread_mutex_t lock; 
pthread_t thread[100];

void *thread_process();
void createThread(void);
void destroyThread(void);
void destroyAll(void);

#define true 1
#define false 0

int main(int argc, char **argv) {

	//Iniciar proceso para sincronizacion de threads
	pthread_mutex_init(&lock, NULL);
	int opcion;
	
	while(true)
	{
		printf("1. Crear thread nuevo\n");
		printf("2. Detener thread\n");
		printf("3. Mostrar threads en ejecucion\n");
		printf("4. Salir\n");
		scanf("%d", &opcion);	

		switch (opcion)
		{
			case 1:
				createThread();
				
				break;
			case 2:
				destroyThread();
				break;
			case 3:
				showText = true;
				sleep(5);
				showText = false;
				break;
			case 4:
				destroyAll();
				goto Salir;
				break;
			default:
				printf("\nNo se encontro la opcion\n");
				exit(0);
				break;
		}
	}
	
	Salir:
	return 0;
}


void createThread(void) 
{
	//Crear nuevo thread y dirigirlo al thread_process
	threadCount++;
	pthread_create(&thread[threadCount], NULL, thread_process, NULL);
}

void destroyThread(void) 
{
	if (threadCount >= 0) 
	{
		//Destruir ultimo thread creado
		pthread_cancel( thread[threadCount] );
		threadCount--;
		printf("Se ha eliminado el thread %d\n", threadCount + 1);
	} 
	else 
	{
		printf("Ya no hay threads para eliminar...\n");
	}
}

void destroyAll(void) 
{
	while( threadCount >= 0 )
	{
		destroyThread();
	}
}


void *thread_process() 
{	
	int i = 0;
	
	while ( i < 255) 
	{
		if (showText) 
		{
			printf( "\nEl valor de j es %d y se modifico desde el thread %ld\n", i, pthread_self() );
		}
		sleep(1);
		i++;
	}
	
	pthread_mutex_lock( &lock );
	
	if (showText) 
	{
		printf("\nSe ha bloqueado el thread %ld\n", pthread_self());
		printf("\nEl contador es: %d \n", i);
		printf("\nSe ha desbloqueado el thread %ld\n", pthread_self());
	}
	
	pthread_mutex_unlock( &lock );
	
	return NULL;
}	
