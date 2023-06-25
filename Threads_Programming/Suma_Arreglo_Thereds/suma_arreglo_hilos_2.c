/*
**    Programa para ilustrar el uso de los hilos
**    y memoria compartida.
**      
**    Compilar con : gcc suma_arreglo_hilos_2.c -lpthread -o suma2
**
**    Autor: Giovanny ALexis Reyes Vilchis
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int ivG_tamArr;     /* Tamaño del arreglo compartido */
int ivG_numHilos;   /* Número de hilos */

/* Este arreglo global es COMPARTIDO por todos los hilos */
long long *llaG_listNUm;

/* Variable COMPARTIDA donde los hilos acumularan la suma parcial de los elementos que se le asignaron */
long long llvG_sumaTotal;

void calculaSuma(void *ptrEntero);
void mostrarArreglo(long long *arr, int inicio, int fin);

/* Mutex para sincronizar el acceso a la variable llvG_sumaTotal */
pthread_mutex_t mutex_sumaTotal;

int main(int argc, char const *argv[]) {
    /**** Comienza el hilo principal (main) ****/

    /* Se verifica que nos hayan pasado el tamaño del arreglo y el número de hilos */
    if (argc != 3)
    {
        printf("\nModo de uso:\n");
        printf("$ %s TAM_ARR numHilos\n\n", argv[0]);
        printf("Ejemplo:\n\n");
        printf("$ %s 10000 4\n", argv[0]);

        return 1;
    }

    pthread_mutex_init(&mutex_sumaTotal, NULL);

    /* Se convierten las cadenas a enteros */
    ivG_tamArr = (int)atof(argv[1]);
    ivG_numHilos = (int)atof(argv[2]);
    //sscanf(argv[1], "%d", &ivG_tamArr);
    //sscanf(argv[2], "%d", &ivG_numHilos);

    /* El hilo principal inicializa el arreglo */
    llaG_listNUm = (long long *) malloc(ivG_tamArr * sizeof(long long));

    /* Se llena con los numeros 1, 2,...,ivG_tamArr */
    for (int i = 0; i < ivG_tamArr; i++)
        llaG_listNUm[i] = i+1;
    
    //mostrarArreglo(llaG_listNUm, 0, ivG_tamArr);
    
    /* El hilo principal inicializa la variable compartida */
    llvG_sumaTotal = 0;

    /* Arreglo de hilos que haran la suma */
    pthread_t hilos[ivG_numHilos];

    /* Arreglo para tener identificadores de cada hilo */
    int idHilos[ivG_numHilos];

    /* Se crean los hilos */
    for (int i = 0; i < ivG_numHilos; i++)
    {
        idHilos[i] = i;
        pthread_create(&hilos[i], NULL, (void *) calculaSuma, &idHilos[i]);
    }

    /* Se espera a todos los hilos */
    for (int i = 0; i < ivG_numHilos; i++)
    {
        pthread_join(hilos[i], NULL);
    }

    /* La fórmula para la suma aritmética de 1 hasta N es n(n+1)/2 */
    long long sumFormula = (ivG_tamArr*(ivG_tamArr+1))/2;

    printf("Suma total de los elementos del arreglo usando la formula: %lld\n", sumFormula);

    printf("Suma total de los elementos del arreglo usando hilos: %lld\n", llvG_sumaTotal);

    free(llaG_listNUm);
    
    pthread_mutex_destroy(&mutex_sumaTotal);

    return 0;
}

/* Esta función la ejecutan los hilos */
void calculaSuma(void *ptrEntero) {
    /* Calcula la suma de la parte del arreglo que le toca */
    int* intPtr = (int *) ptrEntero;    /* Se convIerte el apuntador a entero */
    int miID = *intPtr;   /* Se obtiene el valor de la variable a donde apunta */
    long long ivL_tamSubarreglo;    /* Número de elementos de cada subarreglo */
    int ivL_residuo;    /* Si el tamaño del arreglo NO es múltiplo del número de procesadores */
    int ivL_inicio;
    int ivL_final;
    //printf("\nID: %d\n", miID);

    // TODOS los hilos calculan este valor
    // Se calcula la división entera de cada subarreglo
    ivL_tamSubarreglo = ivG_tamArr / ivG_numHilos;
    // Se calcula el residuo
    ivL_residuo = ivG_tamArr % ivG_numHilos;
    // Se calcula del número de elementos
    if (miID < ivL_residuo)
    {
        ivL_inicio = miID*ivL_tamSubarreglo+miID;
        ivL_final = ivL_inicio+ivL_tamSubarreglo;
    }
    else
    {
        ivL_inicio = miID*ivL_tamSubarreglo+ivL_residuo;
        ivL_final = ivL_inicio+ivL_tamSubarreglo;
    }

    for (int i = ivL_inicio; i < ivL_final; i++)
    {
        pthread_mutex_lock(&mutex_sumaTotal);
            /* Suma el número actual a la suma total */
            llvG_sumaTotal += llaG_listNUm[i];
        pthread_mutex_unlock(&mutex_sumaTotal);
    }
}

/* Función para depurar el código, solo lo usa el hilo principal (main) */
void mostrarArreglo(long long *arr, int inicio, int fin) {
    printf("\nEl arreglo: ");
    for (int i = inicio; i < fin; i++)
        printf("%lld ", arr[i]);
    
    printf("\n\n");
}
