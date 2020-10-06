using System;
using System.Threading;

namespace ThreadsCSharp
{
    // Delegado que define la firma para el metodo callback
    public delegate void ExampleCallback(int data);

    public class Example
    {
        public static void Main()
        {
            // Transferir informacion necesaria para la tarea
            ThreadWithState tws = new ThreadWithState("Hello World!");

            // ------------Ejemplo: Crear thread-------------
            // Crear thread para la tarea
            Thread t = new Thread(tws.ThreadProc);
            // Iniciar thread
            t.Start();
            // El thread principal hace un trabajo y luego espera a que termine el otro thread
            Console.WriteLine("Main thread");
            t.Join();

            // ------------Ejemplo: Interrumpir thread-------------
            // Crear thread para la tarea
            Thread t1 = new Thread(tws.ThreadProc);
            // Iniciar thread
            t1.Start();
            // Despertar el thread en caso de que este pausado y continuar normalmente
            t1.Interrupt();

            // ------------Ejemplo: Interrumpir thread-------------
            // Crear thread para la tarea
            Thread t2 = new Thread(tws.ThreadProc);
            // Iniciar thread
            t2.Start();
            // Eliminar thread por completo
            t2.Abort();

            // ------------Ejemplo: Prioridad-------------
            // Crear thread para la tarea
            Thread t3 = new Thread(tws.ThreadProc);
            Thread t4 = new Thread(tws.ThreadProc);
            // Cambiar prioridad de 'normal' a una mas importante
            t3.Priority = ThreadPriority.AboveNormal;
            // Cambiar prioridad de 'normal' a una menos importante
            t4.Priority = ThreadPriority.BelowNormal;
            // Iniciar thread
            t3.Start();
            t4.Start();

            // ------------Ejemplo: Recibir data del thread-------------
            // Enviar informacion y el delegado
            ThreadWithState tws1 = new ThreadWithState("Hello!", new ExampleCallback(ResultCallback));
            // Crear thread
            Thread t5 = new Thread(new ThreadStart(tws.ThreadProc));
            // Iniciar thread
            t5.Start();
            Console.WriteLine("Main thread does some work...");
            //El thread principal espera a que el otro thread termine
            //El otro thread termina y ejecuta el metodo 'ResultCallback'
            t5.Join();
        }

        public static void ResultCallback(int data)
        {
            Console.WriteLine(
                "Independent task sent {0}", data);
        }
    }

    // Esta clase contiene la info que se necesita para el thread y un metodo que lo ejecuta
    public class ThreadWithState
    {
        // Valor manejado dentro del thread
        private string value;

        // Delegado usado para ejecutar el metodo callback method cuando la tarea se completa
        private ExampleCallback callback;

        // El constructor obtiene la informacion.
        public ThreadWithState(string text)
        {
            value = text;
        }

        // El constructor obtiene la informacion y el delegado.
        public ThreadWithState(string text, ExampleCallback callbackDelegate)
        {
            value = text;
            callback = callbackDelegate;
        }

        // The thread procedure performs the task
        public void ThreadProc()
        {
            try
            {
                // ------------Ejemplo: Pausar thread-------------
                // Se pausa el thread actual por 20 segundos
                Thread.Sleep(20000);
                // Se pausa el thread actual por tiempo indefinido
                // Thread.Sleep(Timeout.Infinite);
            }
            catch (ThreadInterruptedException)
            {
                    Console.WriteLine("Thread awoken.");
            }
            catch (ThreadAbortException)
            {
                Console.WriteLine("Thread aborted.");
            }
            finally
            {
                Console.WriteLine("Message");
            }

            Console.WriteLine("Thread finishing...");
            Console.WriteLine(value);

            // Enviar data al thread principal
            if (callback != null)
            {
                callback(1);
            }
        }
    }
}