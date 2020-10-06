# Threads en python

import threading
import time
_
# Metodo que sera llamado para ejecutarse en un thread
def run():
    while True:
        print('thread running')
        global stop_threads
        # Se revisa la bandera para terminar el thread
        if stop_threads:
            # currentThread().getName() devueleve el ID del thread actual
            print(threading.currentThread().getName(), 'Killed')
            break

# Metodo que sera llamado para ejecutarse en un thread
def Hello():
        print('Hello Wolrd!')


# Crear y matar un thread

# Para matar al thread se puede crear una bandera que indiquee cuando detenerse
stop_threads = False
# Se crea el thread
t1 = threading.Thread(target=run)
# Se inicia el thread
t1.start()
time.sleep(1)
# Se cambia el valor de la bandera para matar el thread
stop_threads = True
# Esperar hasta que termine el thread llamado.
t1.join()
print('End')



# Daemon

# Al lanzar un thread como un daemon, se ejecuta sin bloquear
# el hilo principal de la aplicación permitiéndole salir en cualquier momento.
# Crear el thread
t2 = threading.Thread(target=run)
# Se define el thread como un daemon
t2.setDaemon(True)
# Se inicia el thread
t2.start()
print('Program ended')
