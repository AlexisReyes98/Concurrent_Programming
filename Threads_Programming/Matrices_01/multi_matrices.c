/*
**    Autor: Giovanny ALexis Reyes Vilchis
**
**    Programa para que realice la multiplicación de 2 matrices A y B
**    para obtener como resultado una matriz C, todas de tamaño NxN
**
**    Compilar con : gcc multi_matrices.c -o multiMat
**
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void multiplica(int **matA, int **matB, int **matC, int N);
int** leerMatriz(char *input_file, int N);
void escribeMatrizC(int **m, int N);
void imprimeMatriz(int **m, int N);

int main() {
    int N = 0;
    int **matriz_A;
    int **matriz_B;
    int **matriz_C;
    
    /* 
        Si existen los archivos
        Se llenan las matrices con los valores de los archivos
        N se toma de un archivo diferente
    */
    FILE *input_file = fopen("tam.txt","r");
    if (input_file == NULL)
    {
        printf("\nError al abrir el achivo de entrada\n");
        exit(0);
    }
    fscanf(input_file,"%d", &N);
    fclose(input_file);

    /* Reserva de Memoria */
    /* Filas */
    matriz_A = (int **) malloc(N * sizeof(int *));
    matriz_B = (int **) malloc(N * sizeof(int *));
    matriz_C = (int **) malloc(N * sizeof(int *));

    /* Columnas */
    for (int i = 0; i < N; i++)
    {
        matriz_A[i] = (int *) malloc(N * sizeof(int));
        matriz_B[i] = (int *) malloc(N * sizeof(int));
        matriz_C[i] = (int *) malloc(N * sizeof(int));
    }

    matriz_A = leerMatriz("matA.txt", N);
    matriz_B = leerMatriz("matB.txt", N);
    
    multiplica(matriz_A, matriz_B, matriz_C, N);
    
    /* Se imprimen las matrices A, B y C */
    printf("\nMatriz A: \n");
    imprimeMatriz(matriz_A, N);
    printf("\n\nMatriz B: \n");
    imprimeMatriz(matriz_B, N);
    printf("\n\nMatriz C: \n");
    imprimeMatriz(matriz_C, N);
    printf("\n");

    /* Se escribe la  matriz C */
    escribeMatrizC(matriz_C, N);

    /* Se libera memoria */
    free(matriz_A);
    free(matriz_B);
    free(matriz_C);
    
    return 0;
}

void multiplica(int **matA, int **matB, int **matC, int N) {
    /* Se realiza la multiplicación de matrices A*B y se obtiene como resultado la matriz C */
    for (int i = 0; i < N; i++) /* Filas de A */
    {
        for (int j = 0; j < N; j++) /* Columnas de B */
        {
            matC[i][j] = 0;
            for (int k = 0; k < N; k++) /* Columnas de A */
            {
                matC[i][j] += matA[i][k] * matB[k][j];
            }
        }
    }
}

void imprimeMatriz(int **m, int N) {
    for (int i = 0; i < N; i++)
    {
        printf("\n");
        for (int j = 0; j < N; j++)
        {
            printf("%d ", m[i][j]);
        }
    }
}

/* EStá función sirve para leer de los archivos la matriz A y B */
int** leerMatriz(char *fileName, int N) {
    FILE *input_file = fopen(fileName, "r");    /* Se lee un archivo de texto */
	if (input_file == NULL)
    {
        printf("\nError al abrir los achivos de entrada\n");
	    exit(0);
	}
    char linea[100]; /* Está linea va ir leyendo del archivo */
    char *token;    /* Se divide la cadena en tokens */
    int **mAux = (int **) malloc(N * sizeof(int *));  /* Matriz uxiliar */
    for (int i = 0; i < N; i++) /* Se recorre la matriz */
    {
        mAux[i] = (int *) malloc(N * sizeof(int));  /* Columnas de la matriz auxiliar */
        fgets(linea, 99, input_file);   /* Se extrae del archivo */
        token = strtok(linea, " "); /* Se lee la linea y el delimitador en un espacio */
        mAux[i][0] = atoi(token); /* Se toma el primer token, por eso el siguiente ciclo inicia en 1 */
        for (int j = 1; j < N; j++)
        {
            token = strtok(NULL, " ");  /* Siguiente token en memoria */
            mAux[i][j] = atoi(token);   /* Se convierte cada valor a entero */
        }
    }
    
    fclose(input_file); /* Se cierra el archivo */

	return mAux;    /* Se regresa la matriz leida */
}

/* EStá función sirve para escribir en un archivo la matriz C */
void escribeMatrizC(int **m, int N) {
    FILE *input_file = fopen("mAtC.txt","w"); /* Se genera un archivo de texto */
    if (input_file == NULL)
    {
        printf("\nError al abrir el achivo de entrada\n");
        exit(0);
    }
    char linea[100]; /* Está linea va ir escribiendo en el archivo */
    for (int i = 0; i < N; i++) /* Se recorre la matriz */
    {
        linea[0] = '\0';    /* Se limpia la linea a imprimir en el archivo */
        for (int j = 0; j < N; j++)
        {
            char buffer[10];    /* Se crea un buffer para convertir el entero a cadena */
            sprintf(buffer, "%d ", m[i][j]);    /* Se imprime el valor de la matriz */
            strcat(linea, buffer);  /* Se va concatenando en la linea */
        }
        int len = strlen(linea); /* Cantidad de elmentos en la linea */
	    linea[len - 1] = '\n';  /* El espacio del final será reemplazado por un salto de linea */
	    fputs(linea, input_file);   /* Se imprime en el archivo */
    }
    fclose(input_file); /* Se cierra el archivo */
}
