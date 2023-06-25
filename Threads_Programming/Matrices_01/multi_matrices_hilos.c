/*
**    Autor: Giovanny ALexis Reyes Vilchis
**
**    Programa multihilado para que realice la multiplicación de 2 matrices A y B
**    para obtener como resultado una matriz C, todas de tamaño NxN
**
**    Compilar con : gcc multi_matrices_hilos.c -lpthread -o multiMatHilos
**                  ./multiMatHilos
**
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void multiplica(void *ptrID);
void leerMatrizA(void *ptrID);
void leerMatrizB(void *ptrID);
void escribeMatrizC();
void imprimeMatriz(int **m);

int **matriz_A;
int **matriz_B;
int **matriz_C;
int N, NUM_HILOS, REG_PROCESAR;
int inicio, fin;

int main() {
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

    printf("\nIngresa el numero de hilos entre 2 y N\n");
    printf("N está dado en el archivo de entrada: tam.txt\n");
    printf("> "); scanf("%d", &NUM_HILOS);

    // Se verifica que se escriba el número correcto de hilos
    if (NUM_HILOS < 2 && NUM_HILOS > N)
    {
        printf("\nNUM_HILOS debe ser un número entre 2 y N\n");
        printf("N está dado en el archivo de entrada: tam.txt\n");
        
        exit(0);
    }

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

    /* Hilos para leer las matrices A y B */
    pthread_t hilo1;
    int idHilo1 = 1;
    pthread_create(&hilo1, NULL, (void *) leerMatrizA, &idHilo1);
    
    pthread_t hilo2;
    int idHilo2 = 2;
    pthread_create(&hilo2, NULL, (void *) leerMatrizB, &idHilo2);

    /* El hilo padre espera a que terminen los hijos que creo para leer las matrices */
    pthread_join(hilo1, NULL);
    pthread_join(hilo2, NULL);

    /* Arreglo de hilos */
    pthread_t hilos[NUM_HILOS];
    /* Arreglo para tener identificadores */
    int idHilos[NUM_HILOS]; /* Renglón inicial para cada hilo */

    inicio = 0, fin = 0;
    /* Se crean nHilos */
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

    /* Se imprimen las matrices A, B y C */
    printf("\nMatriz A: \n");
    imprimeMatriz(matriz_A);
    printf("\n\nMatriz B: \n");
    imprimeMatriz(matriz_B);
    printf("\n\nMatriz C: \n");
    imprimeMatriz(matriz_C);
    printf("\n");

    /* Se escribe la  matriz C */
    escribeMatrizC();

    /* Se libera memoa */
    free(matriz_A);
    free(matriz_B);
    free(matriz_C);

    return 0;
}

void multiplica(void *ptrID) {
    int* intPtr = (int *) ptrID;    /* Se convierte el apuntador a entero */
    int miID = *intPtr; /* Se obteniene el valor de la variable a donde apunta */
    
    int newN = N / NUM_HILOS;   /* Se calcula la división entera */
    int residuo = N % NUM_HILOS;    /* Se calcula el residuo */
    // Se calcula del número de elementos dependiendo del ID
    if (miID < residuo)
        REG_PROCESAR = newN+1;
    else
        REG_PROCESAR = newN;
    
    /*
        Para saber en que posición debe iniciar el correspondiene hilo
        inicio toma el valor de fin que es dode se quedo el hilo anterior.
        Para el primer hilo fin tiene valor de 0 por lo que inicio también.
        Ahora, fin toma el valor de la acumulación de su último valor y del valor de filas
        que le toco procesar a ese hilo.
        Para el primer hilo que inicia en 0, fin tendra la suma de 0+REG_PROCESAR y sera
        hasta ese valor que le toca hacer la operación de multiplicación.
    */
    inicio = fin;
    fin += REG_PROCESAR;

    /* Se realiza la multiplicación de matrices A*B y se obtiene como resultado la matriz C */
    for (int i = inicio; i < fin; i++)    /* Filas de A */
    {
        for (int j = 0; j < N; j++) /* Columnas de B */
        {
            matriz_C[i][j] = 0;
            for (int k = 0; k < N; k++) /* Columnas de A */
            {
                matriz_C[i][j] += matriz_A[i][k] * matriz_B[k][j];
            }
        }
    }
}

/*
    Estás funciones sirven para leer de los archivos las matrices A y B
    Cada una es usada por un hilo
*/
void leerMatrizA(void *ptrID) {
    FILE *input_file = fopen("matA.txt", "r");   /* Se lee un archivo de texto */
	if (input_file == NULL)
    {
        printf("\nError al abrir los achivos de entrada\n");
	    exit(0);
	}
    char linea[100]; /* Está linea va ir leyendo del archivo */
    char *token;    /* Se divide la cadena en tokens */
    for (int i = 0; i < N; i++) /* Se recorre la matriz */
    {
        fgets(linea, 99, input_file);   /* Se extrae del archivo */
        token = strtok(linea, " "); /* Se lee la linea y el delimitador en un espacio */
        matriz_A[i][0] = atoi(token); /* Se toma el primer token, por eso el siguiente ciclo inicia en 1 */
        for (int j = 1; j < N; j++)
        {
            token = strtok(NULL, " ");  /* Siguiente token en memoria */
            matriz_A[i][j] = atoi(token);   /* Se convierte cada valor a entero */
        }
    }
    
    fclose(input_file); /* Se cierra el archivo */
}

void leerMatrizB(void *ptrID) {
    FILE *input_file = fopen("matB.txt", "r");   /* Se lee un archivo de texto */
	if (input_file == NULL)
    {
        printf("\nError al abrir los achivos de entrada\n");
	    exit(0);
	}
    char linea[100]; /* Está linea va ir leyendo del archivo */
    char *token;    /* Se divide la cadena en tokens */
    for (int i = 0; i < N; i++) /* Se recorre la matriz */
    {
        fgets(linea, 99, input_file);   /* Se extrae del archivo */
        token = strtok(linea, " "); /* Se lee la linea y el delimitador en un espacio */
        matriz_B[i][0] = atoi(token); /* Se toma el primer token, por eso el siguiente ciclo inicia en 1 */
        for (int j = 1; j < N; j++)
        {
            token = strtok(NULL, " ");  /* Siguiente token en memoria */
            matriz_B[i][j] = atoi(token);   /* Se convierte cada valor a entero */
        }
    }
    
    fclose(input_file); /* Se cierra el archivo */
}

/* Estos procedimientos solamente los usa el hilo principal (main) */
void imprimeMatriz(int **m) {
    for (int i = 0; i < N; i++)
    {
        printf("\n");
        for (int j = 0; j < N; j++)
        {
            printf("%d ", m[i][j]);
        }
    }
}

/* EStá función sirve para escribir en un archivo la matriz C */
void escribeMatrizC() {
    FILE *input_file = fopen("mAtC.txt","w"); /* Se genera un archivo de texto */
    if (input_file == NULL)
    {
        printf("\nError al abrir el achivo de entrada\n");
        exit(0);
    }
    char linea[100]; /* Está linea va ir escribiendo en el archivo */
    for (int i = 0; i < N; i++) /* Se recorre cada fila de la matriz */
    {
        linea[0] = '\0';    /* Se limpia la linea a imprimir en el archivo */
        for (int j = 0; j < N; j++)
        {
            char buffer[10];    /* Se crea un buffer para convertir el entero a cadena */
            sprintf(buffer, "%d ", matriz_C[i][j]);    /* Se imprime el valor de la matriz */
            strcat(linea, buffer);  /* Se va concatenando en la linea */
        }
        int len = strlen(linea); /* Cantidad de elmentos en la linea */
	    linea[len - 1] = '\n';  /* El espacio del final será reemplazado por un salto de linea */
	    fputs(linea, input_file);   /* Se imprime en el archivo */
    }
    fclose(input_file); /* Se cierra el archivo */
}
