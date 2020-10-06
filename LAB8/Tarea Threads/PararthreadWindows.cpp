//Programa para que pare un thread


#include "pch.h"
#include <thread>
#include <iostream>
#include <assert.h>
#include <chrono>
#include <future>
//WINDOWS
void threadFunction(std::future<void> futureObj)
{
        std::cout << "Creacion de Thread" << std::endl;
        while (futureObj.wait_for(std::chrono::milliseconds(1)) == std::future_status::timeout)
        {
                std::cout << "Thread ejecutandose" << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
        std::cout << "Finalizacion de thread" << std::endl;
}




int main()
{
        // promise object
        std::promise<void> exitSignal;
        //Fetch, objeto future asociado con promise
        std::future<void> futureObj = exitSignal.get_future();
        // Comenzando Thread & mover future object en la funcion lambda por reference
        std::thread th(&threadFunction, std::move(futureObj));
        //Esperar 10s
        std::this_thread::sleep_for(std::chrono::seconds(10));
        std::cout << "Preguntar al thread para parar" << std::endl;
        //especificar el valor en promise
        exitSignal.set_value();
        //esperar al thread para el join
        th.join();
        std::cout << "Saliendose de la funcion principal" << std::endl;
        return 0;
}