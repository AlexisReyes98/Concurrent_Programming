/*
**    Ejercicio 6: topología de anillo (variante)
**
**    Compilar con : mpicc comunicacion_logaritmica_2.c -o com2 -lm
**
**    Autor: Giovanny Alexis Reyes Vilchis
**    
**    El proceso 0 envía un número a los demás procesos y cada porceso multiplica el número po 2
**    Después se utiliza el algoritmo de Rondas de comunicación para sumar todos los valores
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

    int valor = 0;
    int cuentaParcial = 0;
    int cuentaParcialRecv = 0;
    int cuentaTotal = 0;
    int id;    // Número que le toca a cada proceso
    int numRondas, i, j, pot, recibo, envio;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    if (my_rank == RAIZ) {
        valor = 2;
        /* i comienza desde 1 poque el raiz no envia mensaje */
        for (i = 1; i < comm_size; i++)
        {
            MPI_Send(&valor, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            printf("\nProceso %d: Ya envié el mensaje\n", my_rank);
        }
    }
    else
    {
        printf("\nProceso %d: Estoy esperando el mensaje...\n", my_rank);
        MPI_Recv(&valor, 1, MPI_INT, RAIZ, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("\nProceso %d: Recibi el mensaje: %d\n", my_rank, valor);

        cuentaParcial = valor*2;
    }

    // Rondas de comunicación
    id = my_rank;
    numRondas = log2(comm_size);
    j = 0;
    pot = 1;
    while (id % 2 == 0 && j < numRondas)
    {
        recibo = my_rank+pot;
        // Recibe la cuenta parcial
        MPI_Recv(&cuentaParcialRecv, 1, MPI_INT, recibo, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        cuentaParcial += cuentaParcialRecv;    // Se van realizando las cuentas
        id = id/2;
        j++;
        pot = 2*pot;
    }

    if (my_rank != RAIZ)
    {
        envio = my_rank-pot;
        // Envía el resultado de su cuenta parcial
        MPI_Send(&cuentaParcial, 1, MPI_INT, envio, 0, MPI_COMM_WORLD);
    }
    else
    {
        cuentaTotal += cuentaParcial;   // Realiza la suma total
        printf("\nProceso %d: El resultado de la cuenta es: %d\n\n", my_rank, cuentaTotal);
    }

    MPI_Finalize();
    
    printf("\nSoy proceso %d, y ya terminé\n", my_rank);

    return 0;
}
