#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

//CREAR THREAD LINUX
void * threadFunc(void * arg)
{
    std::cout << "Thread Function :: Start" << std::endl;
    // Sleep por 2s
    sleep(2);
    std::cout << "Thread Function :: End" << std::endl;
    return NULL;
}
int main()
{
    // se asigna un id al thread
    pthread_t threadId;
    // crea un thread que funciona en threadFunc()
    int check = pthread_create(&threadId, NULL, &threadFunc, NULL);
    // chequea si el thread se hizo correctamente 
    if (check)
    {
        std::cout << "Thread creation failed : " << strerror(check);
        return check;
    }
    else
        std::cout << "Thread Created with ID : " << threadId << std::endl;
    // Do some stuff in Main Thread
    std::cout << "Waiting for thread to exit" << std::endl;
    // Wait for thread to exit
    check = pthread_join(threadId, NULL);
    // check if joining is sucessful
    if (check)
    {
        std::cout << "Failed to join Thread : " << strerror(check) << std::endl;
        return check;
    }
    std::cout << "Exiting Main" << std::endl;
    return 0;
}