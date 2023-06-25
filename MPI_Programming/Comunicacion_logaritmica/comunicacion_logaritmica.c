/*
**    Autor: Giovanny Alexis Reyes Vilchis
*/

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <mpi.h>    /* For MPI functions, etc. */

int main(int argc, char *argv[])
{
    int numProcs;  // Número de procesos
    int myRank;    // Identificador de proceso
    int numeral;
    int numRondas, i, recibo, envio;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

    numeral = myRank;
    numRondas = log2(numProcs);
    i = 0;
    while (numeral % 2 == 0 && i < numRondas)
    {
        recibo = myRank+pow(2,i);
        printf("\nProceso %d: Recibo mensaje de %d\n", myRank, recibo);
        numeral = numeral/2;
        i++;
    }

    if (myRank != 0)
    {
        envio = myRank-pow(2,i);
        printf("\nProceso %d: Envio mensaje a %d\n", myRank, envio);
    }
    
    MPI_Finalize();
    
    return 0;
} /* fin del main */
