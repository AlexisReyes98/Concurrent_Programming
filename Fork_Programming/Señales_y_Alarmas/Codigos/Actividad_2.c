/*
**      Giovanny ALexis Reyes Vilchis
**      2163031812
**      Compilar con: gcc Actividad_2.c -o actividad2
**                    ./actividad2
*/

#include <stdio.h>  /* funciones standard de I/O */
#include <unistd.h> /* funciones unix, como getpid() */
#include <signal.h> /* signal macros, y el prot. signal() */
#include <sys/wait.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

#define MAX 10
#define procsMAX 5

typedef struct
{
    char nomProcs[MAX];
    int id;
} archActivos;

int crearArchivo();
void procesarArchivo();
void cachaSing();

archActivos procsActivos[procsMAX];
int idActivos[procsMAX];
int nIDs = 0;
int nArchs = 0;
int idGenerado = 5000;
int idActual = 0;

int main() {
    int M = procsMAX;
    pid_t pid;
    int menu = 0;
    signal(SIGUSR1, cachaSing);

    do
    {
        printf("\nMenu Principal:\n");
        printf("Escribe 1 para iniciar el procesamiento de un archivo\n");
        printf("Escribe 2 para desplegar lista de archivos en proceso\n");
        printf("Escribe 3 para terminar la ejecucion\n");
        printf("---> "); scanf("%d", &menu);
        printf("\n");
        switch (menu)
        {
        case 1:
            if (nArchs < M)
            {
                printf("Procesando un nuevo archivo:\n\n");
                nArchs++;    // Se aumenta el número de procesos
                pid = fork();
                idActual = crearArchivo();    // Se obtiene el ID del proceso que se utiliza para darle fin una vez que "termine"
                if (pid == 0)   // Si me regresaron 0, soy el hijo.
                {
                    procesarArchivo();
                }
            }
            else
            {
                printf("Servidor saturado... Intente mas tarde. Gracias :)\n");
            }
            break;
        case 2:
            printf("Archivos que se estan procesando:\n\n");
            for (int i = 0; i < procsMAX; i++) //Se recorre buscando todos los procesos
            {
                if (procsActivos[i].id != 0)    // Solo se toman en cuenta los procesos activos
                {
                    printf("%s - %d", procsActivos[i].nomProcs, procsActivos[i].id);
                }
                printf("\n");
            }
            break;
        case 3: // Sale del programa
            printf("Esperando procesos...\n");
            for(int i = 0; i < procsMAX; i++)
            {
                wait(NULL);
            }
            break;
        default:
            printf("Esta opcion no corresponde al menu...\n\n");
            break;
        }
    } while (menu != 3);

    printf("\nHasta pronto!!!\n");

    return 0;
}

/*
    Se crea un archivo y se guarda en la estructura antes definida.
*/
int crearArchivo() {
    char nomArch[MAX];
    char abecedario[] = {'A','R','C','H','I','V','O','_'};
    int n, leng = (int)( sizeof(abecedario)/sizeof(abecedario[0]));

    for (n = 0; n < leng; n++)  // Se le da un nombre secuencial al archivo proesado
    {
        nomArch[n] = abecedario[n];
    }
    nomArch[n] = (nIDs+1)+'0';
    idGenerado++;

    for (int i = 0; i < procsMAX; i++)
    {
        if (procsActivos[i].id == 0)    // Se guarda en la lista de procesos activos
        {
            procsActivos[i].id = idGenerado;
            strncpy(procsActivos[i].nomProcs, nomArch, MAX);
            break;
        }
    }
    idActivos[nIDs] = idGenerado;
    nIDs++;

    return idGenerado;
}

/*
    Esta función simulará procesar un archivo.
*/
void procesarArchivo() {
    srand(time(NULL));
    int aleatorio = 5+rand()%20; // Se simula procesar un archivo con un tiempo aleatorio
    sleep(aleatorio);
    kill(getppid(), SIGUSR1);
    exit(0);
}

/*
    Se cacha la señal.
    Esta función sirve para que el padre reciba la señal SIGUSER1.
    El padre interrumpirá su trabajo para atender la señal con la
    función de finalizar un proceso.
    Se busca el ID del proceso y se pone el ID en 0
    para que el siguiente proceso que venga pueda almacenarlo.
*/
void cachaSing() {
    signal(SIGUSR1, cachaSing);
    int idEliminar = 0;
    nArchs--;   // Se decrementa el número de procesos
    for(int i = 0; i < procsMAX; i++) {     // Busca el ID
        if(idActivos[i] == idActual) {
            idEliminar = idActivos[i];
        }
    }
    for (int i = 0; i < procsMAX; i++)
    {
        if (procsActivos[i].id == idEliminar)   // Se descarta dicho ID
        {
            procsActivos[i].id = 0;
        }
    }
    if (nArchs == 0)
        procsActivos[0].id = 0;
    
    wait(NULL); // wait usado en la práctica 1
}
