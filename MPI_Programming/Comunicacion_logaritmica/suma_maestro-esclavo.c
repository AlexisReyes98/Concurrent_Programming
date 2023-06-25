/*  
**   Suma maestro-trabajador: implementación en MPI de la versión de suma de un arreglo
**   usando el modelo maestro-trabajador
**
**   Autor: Giovanny Alexis Reyes Vilchis
*/  
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>    /* Para la funciones de MPI, etc. */

#define MAESTRO 0
#define MAX_BITS 64

long long suma_binaria(long long a, long long b);
long long bin2dec(int* bin);
void dec2bin(long long a, int* bin);
void printBin(int* bin);
void initBin(int* bin);

// Tamaño del arreglo
const long long ARRAY_TAM = 6500000;

int main(int argc, char *argv[])
{
    int numProcs;   // Número de procesos
    int myRank;     // Identificador del proceso

    long long* arreglo;     // El proceso 0 crea el arreglo dinámicamente
    int inicio;     // Indice donde inicia el subarreglo que enviará el MAESTRO
    long long tamSubarreglo;    // Número de elementos de cada subarreglo
    long long elementosRecibidos;   // Número de elementos que se envía a cada proceso
    //int residuo;      // Si el tamaño del arreglo NO es múltiplo del número de procesadores

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

    if (myRank == MAESTRO)
    {
        // SOLAMENTE el proceso MAESTRO
        // Crea el arreglo de tamaño ARRAY_TAM
        arreglo = (long long *) malloc(ARRAY_TAM * sizeof(long long));

        // Se llena con los numeros 0, 1, 2,...,ARRAY_TAM-1
        for (size_t i = 0; i < ARRAY_TAM; i++)
            arreglo[i] = i;
        
        // Se calcula la división entera de cada subarreglo
        tamSubarreglo = ARRAY_TAM / numProcs;
        // Se calcula el residuo
        //residuo = ARRAY_TAM%numProcs;

        // Se comprueba si se ejecuta más de 1 proceso
        if (numProcs > 1)
        {
            int i;
            inicio = 0;
            // Se distribuye la parte del arreglo a los demás procesos
            // para calcular sus sumas parciales
            // Hasta numProcs-1
            for (i = 1; i < numProcs-1; i++)
            {
                inicio = inicio + tamSubarreglo;
                MPI_Send(&tamSubarreglo, 1, MPI_LONG_LONG, i, 0, MPI_COMM_WORLD);
                MPI_Send(&arreglo[inicio], tamSubarreglo, MPI_LONG_LONG, i, 0, MPI_COMM_WORLD);
            }

            // Último proceso en recibir su parte
            inicio = inicio + tamSubarreglo;
            int ultimos = ARRAY_TAM - inicio;
            MPI_Send(&ultimos, 1, MPI_LONG_LONG, i, 0, MPI_COMM_WORLD);
            MPI_Send(&arreglo[inicio], ultimos, MPI_LONG_LONG, i, 0, MPI_COMM_WORLD);
        }

        // Para guardar la suma total de los elementos del arreglo
        long long sumaTotal = 0;

        // El MAESTRO realiza la suma de la parte que le toca
        for (size_t i = 0; i < tamSubarreglo; i++)
        {
            sumaTotal = suma_binaria(sumaTotal, arreglo[i]);
        }
        
        // Recibe las sumas parciales
        long long sumaParcialRes = 0;
        for (size_t i = 1; i < numProcs; i++)
        {
            MPI_Recv(&sumaParcialRes, 1, MPI_LONG_LONG, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            sumaTotal += sumaParcialRes;    // Realiza la suma total
        }
        
        printf("\nEl resultado de la suma es: %lld\n\n", sumaTotal);

        free(arreglo);
    }
    else
    {
        // Esto solo lo harán los trabajadores
        // Este esclavo debe reservar memoria para la parte que le toca
        // Podrá usar también 'arreglo' porque cada proceso es independiente
        // Solamente se reserva para los elementos que recibirá
        MPI_Recv(&elementosRecibidos, 1, MPI_LONG_LONG, MAESTRO, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        arreglo = (long long *) malloc(elementosRecibidos * sizeof(long long));

        // Recibe en arreglo los elementos que deberá sumar
        MPI_Recv(arreglo, elementosRecibidos, MPI_LONG_LONG, MAESTRO, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        // Para guardar la suma parcial de los elementos del arreglo de cada proceso
        long long sumaParcial = 0;
        
        // Suma los elementos del arreglo que le tocó
        for (size_t i = 0; i < elementosRecibidos; i++)
            sumaParcial = suma_binaria(sumaParcial, arreglo[i]);
        
        // Envía el resultado de su suma parcial
        MPI_Send(&sumaParcial, 1, MPI_LONG_LONG, MAESTRO, 0, MPI_COMM_WORLD);

        free(arreglo);
    }

    MPI_Finalize();

    return 0;
}

long long suma_binaria(long long a, long long b) {
    /* Convierte a y b a binario */
    int binA[MAX_BITS];
    int binB[MAX_BITS];
    int binC[MAX_BITS];

    initBin(binA);
    initBin(binB);
    initBin(binC);

    dec2bin(a, binA);
    dec2bin(b, binB);
    
    //printBin(binA);
    //printBin(binB);

    int res = 0;
    int i = 0;
    do
    {
        int sumaBits = binA[i] + binB[i] + res;
        binC[i] = sumaBits % 2;
        res = sumaBits / 2;
        i++;
    } while (i < MAX_BITS);

    //printBin(binC);

    /* Regresa el resultado en decimal */
    long long resultado = bin2dec(binC);

    return resultado;
}

void dec2bin(long long a, int* bin) {
    int i = 0;
    while (a != 0 && i < MAX_BITS)
    {
        long long residuo = a % 2;
        bin[i] = residuo;
        a = a/2;
        i++;
    }
}

long long bin2dec(int* bin) {
    long long potencia = 1;
    long long decimal = 0;
    for (size_t i = 0; i < MAX_BITS; i++)
    {
        decimal = decimal + bin[i]*potencia;
        potencia *= 2;
    }
    return decimal;
}

void printBin(int* bin) {
    printf("\n");
    for (size_t i = 0; i < MAX_BITS; i++)
        printf("%d", bin[i]);
}

void initBin(int* bin) {
    for (size_t i = 0; i < MAX_BITS; i++)
        bin[i] = 0;
}
