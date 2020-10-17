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

//File paths
char* pathConfig = "/home/jose/Desktop/Parcial/config.txt";
char* pathOutput = "/home/jose/Desktop/Parcial/";
char* pathInput = "/home/jose/Desktop/Parcial/input.txt";

//Functions
void *thread_process();
void createThread(void);
void destroyThread(void);
void destroyAll(void);
int writeToFile(char* path, char* content);
char* getTime(void);

void stringFactorial(int input);
int writeToFileFactorial(char* path, char content[], int Size);

#define true 1
#define false 0

#define RUNNING 1


int arrayValues[5] = { 0 };
int actualFactorialValue = 0;
int indexArray = 0;

int main(int argc, char **argv) 
{
	
	//Leer input
	FILE *fp = fopen("/home/jose/Desktop/input.txt", "r");
	    int i;
	   
	for (i = 0; i < 5; i++) 
	{
		fscanf(fp, "%d", &arrayValues[i]);
	}

	fclose(fp);
	
	printf("%d\n", arrayValues[0]);
	printf("%d\n", arrayValues[1]);
	printf("%d\n", arrayValues[2]);
	printf("%d\n", arrayValues[3]);
	printf("%d\n", arrayValues[4]);
	
	actualFactorialValue = arrayValues[0];
	
	printf("Factorial: %d", actualFactorialValue);
	stringFactorial(actualFactorialValue);
	
	//Leer cantidad de threads
	
	int cantThreads = 2;
	
	//Inicializar procesos con threads
	while(indexArray < 5)
	{
		while(cantThreads > 0)
		{
			createThread();
			cantThreads--;
		}
		indexArray++;
	}
	
	
	//Output
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
	while(actualFactorialValue > 0)
	{
		printf("thread %ld value %d", pthread_self(), actualFactorialValue);
		stringFactorial(actualFactorialValue);
		actualFactorialValue--;
	}
}

void stringFactorial(int input)
{
	while(input > 0)
	{
		char* path = "/home/jose/Desktop/factorial.txt";
		int num = input;
		int factorial = num;
		int Size;
		int n;

		char answer[100] = {0,};
		n = snprintf(NULL, 0, "%d = ", num );
		snprintf(answer, n + 1, "%d = ", num );
		
		writeToFileFactorial(path, answer, n);
		printf("%s", answer);
		
		while(num > 1)
		{
			char buff[20] = {0,};
			// next factor
			factorial *= --num;
			
			// concatenate string
			n = snprintf(NULL, 0, "(%d)", num );
			snprintf(buff, n + 1, "(%d)", num );
			
			writeToFileFactorial(path, buff, n);
			printf("%s", buff);
		}
		
		char last[100] = {0,};
		n = snprintf(NULL, 0, " = %d\n", factorial );
		snprintf(last, n + 1, " = %d\n",  factorial );
		
		writeToFileFactorial(path, last, n);
		printf("%s\n", last);	
	input--;
	}
}

int writeToFileFactorial(char* path, char content[], int Size)
{
	int fd;
	fd = open(path, O_CREAT | O_WRONLY | O_APPEND, S_IRUSR | S_IWUSR);
	
	write(fd, content, Size);

	close(fd);
	
	return fd;
}	









