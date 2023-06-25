/*
**    Programa multihilado para que realice la multiplicación de 2 matrices A y B de tamaños
**    nxm y mxq, respectivamente, para obtener como resultado una matriz C de tamaño nxq.
**
**    Compilar con : gcc multi_matrices_hilos.c -lpthread -o multiMatHilos
**
**    Autor: Giovanny ALexis Reyes Vilchis
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define n 3840
#define m 1500
#define q 2500

void multiplica(void *ptrID);

void imprimeMatrizA(int mA[n][m]);
void imprimeMatrizB(int mB[m][q]);
void imprimeMatrizC(int mC[n][q]);

int matriz_A[n][m];
int matriz_B[m][q];
int matriz_C[n][q];

int NUM_HILOS;
#define REG_PROCESAR n/NUM_HILOS;

int main(int argc, char const *argv[])
{

    /* Verificar que se escriba el entero */
    if (argc < 2) { /* Si pasaron menos de 2 argumentos */
        printf("\nDebes correr este programa así:\n");
        printf("\n $> %s NUM_HILOS\n", argv[0]);
        
        return 0;
    }

    sscanf(argv[1], "%d", &NUM_HILOS);

    srand(time(NULL));

    /* Se genera la Matriz A */
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            matriz_A[i][j] = 10+rand()%50;
        }
    }

    /* Se genera la Matriz B */
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < q; j++)
        {
            matriz_B[i][j] = 10+rand()%50;
        }
    }

    /* Arreglo de hilos */
    pthread_t hilos[NUM_HILOS];
    /* Arreglo para tener identificadores */
    int idHilos[NUM_HILOS]; /* renglón inicial para cada hilo */

    /* Se crean NHilos */
    for (int i = 0; i < NUM_HILOS; i++)
    {
        idHilos[i] = i;
        pthread_create(&hilos[i], NULL, (void *) multiplica, &idHilos[i]);
    }

    /* Se espera a todos los hilos */
    for (int i = 0; i < NUM_HILOS; i++)
    {
        pthread_join(hilos[i], NULL);
    }
    
    /*
    imprimeMatrizA(matriz_A);
    imprimeMatrizB(matriz_B);
    imprimeMatrizC(matriz_C);
    printf("\n");
    */

    return 0;
}

void multiplica(void *ptrID)
{
    int* intPtr = (int *) ptrID; /* Se convierte el apuntador a entero */
    int miID = *intPtr; /* Se obteniene el valor de la variable a donde apunta */
    int inicio = miID*REG_PROCESAR;
    int final = (miID+1)*REG_PROCESAR;

    /* Se realiza la multiplicación de matrices A*B y se obtiene como resultado la matriz C */
    for (int i = inicio; i < final; i++) /* Filas de A */
    {
        for (int j = 0; j < q; j++) /* Columnas de B */
        {
            matriz_C[i][j] = 0;
            for (int k = 0; k < m; k++) /* Columnas de A y Filas de B */
            {
                matriz_C[i][j] += matriz_A[i][k] * matriz_B[k][j];
            }
        }
    }
}

/* Estos procedimientos solamente los usa el hilo principal (main) */
void imprimeMatrizA(int mA[n][m])
{
    printf("Matriz A:");
    for (int i = 0; i < n; i++)
    {
        printf("\n");
        for (int j = 0; j < m; j++)
        {
            printf("%d ", mA[i][j]);
        }
    }
}

void imprimeMatrizB(int mB[m][q])
{
    printf("\n\nMatriz B:");
    for (int i = 0; i < m; i++)
    {
        printf("\n");
        for (int j = 0; j < q; j++)
        {
            printf("%d ", mB[i][j]);
        }
    }
}

void imprimeMatrizC(int mC[n][q])
{
    printf("\n\nResultado Matriz C:");
    for (int i = 0; i < n; i++)
    {
        printf("\n");
        for (int j = 0; j < q; j++)
        {
            printf("%d ", mC[i][j]);
        }
    }
}
