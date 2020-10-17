
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int showText = 1;
int threadCount = 0;

char* getTime()
{
  time_t rawtime;
  struct tm * timeinfo;

  time ( &rawtime );
  timeinfo = localtime ( &rawtime );
  return  asctime (timeinfo) ;
}

typedef struct MyData
{
    int value;
} MYDATA, *PMYDATA;

PMYDATA pDataArray[3];

DWORD WINAPI ThreadFunc( LPVOID lpParam )
{
    PMYDATA pDataArray;
    //Cast data
    pDataArray = (PMYDATA)lpParam;

    int i = 0;

	while ( i < 5)
	{
	// creating file pointer to work with files
        FILE *fptr;

        // opening file in writing mode
        fptr = fopen("C:\\Users\\user\\Desktop\\program.txt", "a");

        // exiting program
        if (fptr == NULL)
        {
            printf("Error reading file!");
            exit(1);
        }
        else
        {
            //Write to file
            fprintf(fptr, "Thread %d - %s\n", pDataArray->value, getTime());
            i++;
        }
        fclose(fptr);
	}
    return 0;
}


int  CrearNuevoThread()
{
    threadCount++;
    //Assign data
    pDataArray[0] = (PMYDATA) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(MYDATA));
    pDataArray[0]->value = threadCount;
    //Create thread
    int res = CreateThread(NULL, 0, ThreadFunc, pDataArray[0], 0, NULL);
    printf("Thread created succesfully!\n");
    return res;
}


int main()
 {
    HANDLE  hThread1 = CrearNuevoThread();
    sleep(1);
    HANDLE  hThread2 = CrearNuevoThread();

    if ( hThread1 )
    {
        sleep(1);
    }
    if ( hThread2 )
    {
        sleep(1);
    }


    return 0;
}
