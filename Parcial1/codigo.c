#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <fcntl.h>
#include <signal.h>


//Threads
int threadCount = -1;
bool showText = false;
pthread_mutex_t lock; 
pthread_t thread[100];

//Processes
int process;	
int processState = 0;

//Others
int messageSize = 0;

//Functions
void *thread_process();
void createThread(void);
void destroyThread(void);
void destroyAll(void);
int writeToFile(char* path, char* content);
char* getTime(void);
void createNewProcess(void);
void stopProcess(void);
void resumeProcess(void);
void killProcess(void);

#define true 1
#define false 0

#define RUNNING 1

int main(int argc, char **argv) 
{

	//Escribir en archivo
	char* path = "/home/jose/Desktop/file.txt";
	char *msg;
	int n;
	n = snprintf(NULL, 0, "\nCompilacion exitosa en %s \n", getTime() );
	messageSize = n + 1; 
	malloc(n + 1);
	n = snprintf(msg, messageSize, "\nCompilacion exitosa en %s \n", getTime() );
	writeToFile(path, msg);
	
	
	//Iniciar proceso para sincronizacion de threads
	pthread_mutex_init(&lock, NULL);
	int opcion;
	
	while(true)
	{
		printf("\nMenu:\n");
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
				if (threadCount >= 0) 
				{
					showText = true;
					sleep(5);
					showText = false;
				} 
				else 
				{
					printf("\nNo hay threads para mostrar...\n");
				}
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
	createNewProcess();
	stopProcess();
	resumeProcess();
	killProcess();
	return 0;
}

int writeToFile(char* path, char* content)
{
	int fd;
	fd = open(path, O_CREAT | O_WRONLY | O_APPEND, S_IRUSR | S_IWUSR);
	
	write(fd, content, messageSize - 1);
	//write(fd, content, sizeof(content));
	
	close(fd);
	
	return fd;
}	

char* getTime(void)
{
	time_t mytime = time(NULL);
	char * time_str = ctime(&mytime);
	time_str[strlen(time_str)-1] = '\0';
	return time_str;
}

void createThread(void) 
{
	//Crear nuevo thread y dirigirlo al thread_process
	threadCount++;
	pthread_create(&thread[threadCount], NULL, thread_process, NULL);
	printf("\nthread %ld creado con exito\n\n", thread[threadCount]);
}

void destroyThread(void) 
{
	if (threadCount >= 0) 
	{
		//Destruir ultimo thread creado
		printf("\nSe ha eliminado el thread %ld\n", thread[threadCount]);
		pthread_cancel( thread[threadCount] );
		threadCount--;
	} 
	else 
	{
		printf("\nYa no hay threads para eliminar...\n");
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

void createNewProcess(void)
{
	if (processState == !RUNNING) 
	{
		if ((process = fork()) < 0) 
		{
			printf("Error creating the process\n\n");
		} 
		else if (process  == 0) 
		{
			printf("New Process running on PID %d\n\n", getpid());
			//producer(shmc);
		} 
		else 
		{
			processState = RUNNING;
			sleep(1);
		}
	} 
	else 
	{
		printf("Process already running\n\n");
	}

}

void stopProcess(void)
{
	if (processState == RUNNING) 
	{
		kill(process, SIGSTOP);	
		printf("Process %d stopped\n\n", process);
	}
	else
	{
		printf("Process not running\n\n");
	} 
}

void resumeProcess(void)
{
	if (processState == RUNNING) 
	{
		kill(process, SIGCONT);	
		printf("Process %d resumed\n\n", process);
	}
	else
	{
		printf("Process not running\n\n");
	} 
}

void killProcess(void)
{
	if (processState == RUNNING) 
	{
		kill(process, SIGTERM);	
		printf("Process %d killed\n\n", process);
		processState = !RUNNING;
	}
	else
	{
		printf("Process not running\n\n");
	} 
}

Shm *shmnew(size_t size)
{
	Shm *shm = calloc(1, sizeof *shm);
	shm->size = size;

	if ((shm->id = shmget(IPC_PRIVATE, size, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR)) < 0) 
	{
		perror("shmget");
		free(shm);
		return NULL;
	}

	return shm;
}

void shmdel(Shm *shm) 
{
	shmctl(shm->id, IPC_RMID, 0);
	free(shm);
}

void shmread(void *data, Shm *shm)
{
	void *shm_data;

	if ((shm_data = shmat(shm->id, NULL, 0)) == (void *) -1) 
	{
		perror("read");
		return;

	}
	memcpy(data, shm_data, shm->size);
	shmdt(shm_data);
}

void shmwrite(Shm *shm, void *data)
{
	void *shm_data;

	if ((shm_data = shmat(shm->id, NULL, 0)) == (void *) -1) 
	{
		perror("write");
		return;
	}

	memcpy(shm_data, data, shm->size);
	shmdt(shm_data);
}

