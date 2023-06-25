/*
**      Programa para ilustrar la Suma Logaritmica.
**      
**      Compilar con : mpicc -std=c99 -lm suma_logaritmica.c -o sumaLog
**      Servidor UAM:
**      time mpiexec -n "numProcesos" --hostfile "ruta lista_nodos" ./sumaLog
**
**      Autores:
**      Reyes Vilchis Giovanny Alexis
**      Ramírez Calderón Monserrat Valeria
**      López Chávez Anel Jesali
**      Ruiz Martínez Diego Armando
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <mpi.h>    /* Para la funciones de MPI, etc. */

#define RAIZ 0
#define MAX_BITS 64

long long suma_binaria(long long a, long long b);
long long bin2dec(int* bin);
void dec2bin(long long a, int* bin);
void printBin(int* bin);
void initBin(int* bin);

/* Tamaño del arreglo */
const long long N = 85000000;

int main(int argc, char *argv[])
{
    int numProcs;   // Número de procesos
    int myRank;     // Identificador del proceso

    long long ARRAY_TAM;        // Arreglo de tamaño N/p
    long long *arreglo;         // El proceso myRank crea el arreglo dinámicamente
    long long sumaParcial = 0;  // Para guardar la suma parcial
    long long sumaTotal = 0;    // Para guardar la suma total
    long long sumaRecv = 0;     // Para recibir las sumas parciales

    int id;    // Número que le toca a cada proceso
    int numRondas, j, pot, recibo, envio;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

    /* Inicializa la semilla para generar números aleatorios */
    srand(time(NULL));

    /* Crea el arreglo de tamaño N */
    arreglo = (long long *) malloc(N * sizeof(long long));

    ARRAY_TAM = N/numProcs;

    if (myRank == 0)
    {
        printf("\nTamaño de entrada: %lld\n", N);
        printf("\nTamaño del arreglo por proceso: %lld\n", ARRAY_TAM);
    }

    /* Se llena con numeros aleatorios enteros */
    if (myRank != 0)
    {
        for (size_t i = 0; i < ARRAY_TAM; i++)
            arreglo[i] = (10+rand()%50)*myRank;
    } else
    {
        for (size_t i = 0; i < ARRAY_TAM; i++)
            arreglo[i] = 10+rand()%50;
    }
    
    /*for (size_t i = 0; i < ARRAY_TAM; i++)
        printf("%lld ", arreglo[i]);
    
    printf("\n");*/
    
    /* Suma los elementos del arreglo que le tocó */
    for (size_t i = 0; i < ARRAY_TAM; i++)
        sumaParcial += arreglo[i];

    free(arreglo);

    /* Estrategia de la suma logaritmica */
    id = myRank;
    numRondas = log2(numProcs);
    j = 0;
    pot = 1;
    while (id % 2 == 0 && j < numRondas)
    {
        recibo = myRank+pot;
        // Recibe la suma parcial
        MPI_Recv(&sumaRecv, 1, MPI_LONG_LONG, recibo, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        sumaParcial = suma_binaria(sumaParcial, sumaRecv); // Se van realizando las sumas parciales
        id = id/2;
        j++;
        pot = 2*pot;
    }

    if (myRank != RAIZ)
    {
        envio = myRank-pot;
        // Envía el resultado de su suma parcial
        MPI_Send(&sumaParcial, 1, MPI_LONG_LONG, envio, 0, MPI_COMM_WORLD);
    }
    else
    {
        sumaTotal += sumaParcial;   // Realiza la suma total (RAIZ)
        printf("\nEl resultado de la suma es: %lld\n", sumaTotal);
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
