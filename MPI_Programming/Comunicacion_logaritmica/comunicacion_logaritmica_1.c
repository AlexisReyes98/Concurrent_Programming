/*
**    Ejercicio 6: topología de anillo (variante)
**
**    Compilar con : mpicc comunicacion_logaritmica_1.c -o com1 -lm
**
**    Autor: Giovanny Alexis Reyes Vilchis
**    
*/
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <mpi.h> /* For MPI functions, etc. */

#define RAIZ 0

int main(int argc, char *argv[])
{
    int comm_size; /* Número de procesos. */
    int my_rank;   /* Mi identificador de proceso. */

    int id;    // Número que le toca a cada proceso
    int numRondas, j, pot, recibo, envio;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    // Rondas de comunicación
    id = my_rank;
    numRondas = log2(comm_size);
    j = 0;
    pot = 1;
    while (id % 2 == 0 && j < numRondas)
    {
        int recvRank;
        recibo = my_rank+pot;
        // Recibe la cuenta parcial
        MPI_Recv(&recvRank, 1, MPI_INT, recibo, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("\nProceso %d: Recibo mensaje de %d\n", my_rank, recvRank);
        id = id/2;
        j++;
        pot = 2*pot;
    }

    if (my_rank != RAIZ)
    {
        envio = my_rank-pot;
        MPI_Send(&my_rank, 1, MPI_INT, envio, 0, MPI_COMM_WORLD);
        printf("\nProceso %d: Envio mensaje a %d\n", my_rank, envio);
    }

    MPI_Finalize();
    
    printf("\nSoy proceso %d, y ya terminé\n", my_rank);

    return 0;
}
