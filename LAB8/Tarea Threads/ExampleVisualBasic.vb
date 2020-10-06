Imports System
Imports System.Threading

Module Program
    Sub Main(ByVal args As String())

        ' Crear nuevo thread

        Dim t1 As Thread = New Thread(New ThreadStart(AddressOf PrintInfo))

        t1.Start()

        ' Esperar a que el thread termine de ejecutarse

        t1.Join()

        Console.WriteLine("Thread Execution Completed")

        Console.ReadLine()


        ' Crear nuevo thread

        Dim t2 As Thread = New Thread(New ThreadStart(AddressOf PrintInfo))

        ' Obtener estado del thread

        Console.WriteLine("Thread State: {0}", t2.ThreadState)

        ' Iniciar thread

        t2.Start()

        Console.WriteLine("Thread State: {0}", t2.ThreadState)

        ' Suspender thread (Descontinuado por cuestiones de seguridad)

        ' t2.Suspend()

        ' Continuar thread (Descontinuado por cuestiones de seguridad)

        ' t2.[Resume]()

        Console.WriteLine("Thread State: {0}", t2.ThreadState)

        ' Matar thread

        Try
            t2.Abort()
        Catch Ex As Exception
        End Try


    End Sub

    Private Sub PrintInfo()

        For i As Integer = 1 To 5

            Console.WriteLine("Thread paused for {0} seconds", 1)

            ' Pausar el thread por 1 segundo

            Thread.Sleep(1000)

            Console.WriteLine("i value: {0}", i)

        Next

    End Sub
End Module
