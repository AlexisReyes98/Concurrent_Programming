#include <stdio.h>  /* funciones estándar de I/O                      */
#include <unistd.h> /* funciones unix, como getpid()                  */
#include <signal.h> /* signal name macros, y el prototipo signal()    */
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <sys/wait.h>

typedef struct
{
    char nombre[11];
    int id;
} Nodoss;

const int maximo = 11;
const int cantidadMaxima = 7;
const int porcesosMax = 8;
char nombre[11];
Nodoss procesos[8];
pid_t pid, ppid;
int contador = 0;
int rv;
int nArchivos = 0;
int iden = 100;
int af = 0;
int idenr[8];

/*
    La función crea un nombre de proceso y lo inserta en el Array de struct.
    Al insertarlo busca un ID en 0, que significa que esta disponible y cuando lo 
    encuentre, lo inserta y se rompe para que no lo inserte más veces en caso
    que existan más lugares.
*/
int creaNombreProceso()
{
    srand(time(NULL));
    int numero = 0;
    char letras[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
                     'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e',
                     'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x',
                     'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

    for (int i = 0; i < 8; i++)
    {
        numero = rand() % 62;
        nombre[i] = letras[numero];
    }

    iden++;

    for (int i = 0; i < 8; i++)
    {
        if (procesos[i].id == 0)
        {
            procesos[i].id = iden;
            strncpy(procesos[i].nombre, nombre, maximo);
            break;
        }
    }

    idenr[contador] = iden;
    contador++;

    return iden;
}

/*
    Esta funciión lo que hace es que busca el ID del proceso dentro del Array de struct
    y lo que hace es que pone el ID en 0 para que otro proceso nuevo lo inserte en ese lugar.
*/
void eliminaProceso(int iden)
{   
    int valor = 0;
    nArchivos--;
    for(int j = 0; j < 8; j ++) {
        if(idenr[j] == iden) {
            valor = idenr[j];
        }
    }
    for (int i = 0; i < porcesosMax; i++)
    {
        if (procesos[i].id == valor)
        {
            procesos[i].id = 0;
        }
    }
    if (nArchivos==0)
        procesos[0].id = 0;
}

/*
    Esta es la función que cacha la señal de SIGUSR1
    la cual elimina al proceso y espera a que lo termine.
*/
void function()
{
    signal(SIGUSR1, function);
    eliminaProceso(af);
    wait(NULL);
}

int main()
{

    srand(time(NULL));
    int opcion = 0;
    bool salida = false;                                    //Se utiliza para salir del menu.

    signal(SIGUSR1, function);

    do
    {

        printf("::Bienvenido::\n::Este es el menu::\n");
        printf("::1.Procesar::\n::2.Mostrar::\n::3.Terminar\n");
        printf("Que numero de opcion deseas?:: ");
        scanf("%i", &opcion);
        printf("\n");

        switch (opcion)
        {
        case 1:
            if (nArchivos + 1 <= cantidadMaxima)
            {
                nArchivos++;                                //Aumenta el número de procesos
                int time = rand() % 30 + 1;                 //Genera un tiempo aleatorio que se utiliza en el sleep (1-30s).
                pid = fork();
                af = creaNombreProceso();                   //Retorna el ID del nuevo proceso que se utiliza para eliminarlo una vez que termine.
                if (pid == 0)
                {
                    sleep(time);
                    kill(getppid(), SIGUSR1);
                    exit(rv);
                }
            }
            else
            {
                printf("::El servidor esta lleno::\n");
                printf("::Intente más tarde perro::\n\n");
            }
            break;
        case 2:
            printf("::Estos son los procesos activos::\n");
            for (int i = 0; i < 8; i++)                       //Se recorre el array de procesos
            {
                if (procesos[i].id != 0)                      //Se hace la comparación que solo se tome en cuenta los ID distintos de 0 (donde hay procesos activos)    
                    printf("%s", procesos[i].nombre);         //Se imprimen en la consola esos procesos.

                printf("\n");
            }
            break;
        case 3:
            for(int i = 1; i <= 8; i++){                       //De acuerdo con la cantidad de procesos o numero de archivos sera el wait de los procesos activos.
                wait(NULL);
            }             
            printf("Adios my love!\n\n");
            salida = true;
            break;
        default:
            printf("Esa opcion no existe, ¡Ponte chingon!\n\n");
            break;
        }
    } while (!salida);

    return 0;
}