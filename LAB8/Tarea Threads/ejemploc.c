#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

int contadorThread = 0;
pthread_mutex_t lock; 
pthread_t thread[100];
int counter = 0;
int outCounter = 0;
bool showText = false;
int i = 0;


void *entry_point() {
	int j = 0;
	while (j < 250000000) {
		if (showText) {printf("\nEl valor de j es %d y se modifico desde el thread %ld\n", j, pthread_self());}
		j++;
	}
	pthread_mutex_lock(&lock);
		if (showText) {printf("\nSe ha bloqueado el thread %ld\n", pthread_self());}
		sleep(5);
		counter += 100;
		if (showText) {printf("\nEl contador es: %d \n", counter);}
		if (showText) {printf("\nSe ha desbloqueado el thread %ld\n", pthread_self());}
	pthread_mutex_unlock(&lock);
	return NULL;
}

void createThread() {
	pthread_create(&thread[contadorThread], NULL, entry_point, NULL);
	contadorThread++;
}


int main(int argc, char **argv) {

	pthread_mutex_init(&lock, NULL);
	int opcion;
	while(1)
	{
		printf("1. Crear thread nuevo\n");
		printf("2. Detener thread\n");
		printf("3. Mostrar threads en ejecucion\n");
		//printf("4. ");
		scanf("%d", &opcion);	

		switch (opcion)
		{
			case 1:
				createThread();
				
				break;
			case 2:
				if (contadorThread >= 0) {
					pthread_cancel(thread[contadorThread]);
					contadorThread--;
				} else {
					printf("Ya no hay threads para eliminar\n");
					contadorThread = 0;
				}
				break;
			case 3:
				showText = true;
				
				break;
			case 4:

				break;
			default:
				printf("\nNo se encontro la opcion\n");
				exit(0);
				break;
		}
	}
	return 0;



	
	


	return EXIT_SUCCESS;
}





	

