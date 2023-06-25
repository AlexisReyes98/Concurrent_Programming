/*
**    Programa para que realice la multiplicación de 2 matrices A y B de tamaños
**    nxm y mxq, respectivamente, para obtener como resultado una matriz C de tamaño nxq.
**
**    Compilar con : gcc multi_matrices.c -o multiMat
**
**    Autor: Giovanny ALexis Reyes Vilchis
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define n 3840
#define m 1500
#define q 2500

void multiplica();

void imprimeMatrizA(int mA[n][m]);
void imprimeMatrizB(int mB[m][q]);
void imprimeMatrizC(int mC[n][q]);

int matriz_A[n][m];
int matriz_B[m][q];
int matriz_C[n][q];

int main()
{
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

    multiplica();
    /*
    imprimeMatrizA(matriz_A);
    imprimeMatrizB(matriz_B);
    imprimeMatrizC(matriz_C);
    printf("\n");
    */

    return 0;
}

void multiplica()
{
    /* Se realiza la multiplicación de matrices A*B y se obtiene como resultado la matriz C */
    for (int i = 0; i < n; i++) /* Filas de A */
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
