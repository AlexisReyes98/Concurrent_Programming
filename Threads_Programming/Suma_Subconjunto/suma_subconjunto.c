/*
**    Programa para ilustrar la aceleración de algoritmos multi-hilados.
**      
**    Compilar con : gcc suma_subconjunto.c -lpthread -lm -o sumaSub
**
**    Autor: Giovanny Alexis Reyes Vilchis
**
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

int ivG_tamArr;     /* Tamaño del arreglo */
int ivG_numHilos;   /* Número de hilos */

/* Este arreglo global es COMPARTIDO por todos los hilos */
int *iaG_listNum;

/* Variable COMPARTIDA donde los hilos donde acumularan el conteo de subconjuntos que suman 0 */
long long llvG_sumaSub;

int ivG_bitsBase; /* Patrón de h bits al final de la cadena (bits base) */

void calculaSuma(void *ptrEntero);
void mostrarArreglo(int *arr, int inicio, int fin);

/* Mutex para sincronizar el acceso a la variable llvG_sumaSub */
pthread_mutex_t mutex_sumaSub;

int main(int argc, char const *argv[]) {
    /**** Comienza el hilo principal (main) ****/
    int *iaL_listBinBase;   /* Cadena binaria, bitsBase, de h bits */
    /* sumBitsBase es para sumar los elementos del subconjunto representado por sus bitsBase */
    int sumBitsBase;
    int pos;   /* Posicion del arreglo binario a sumar */
    int acarreo;    /* Acarreo para la suma */
    /* Use estás variables para saber donde iniciar y detener la suma representada por los bits base */
    int posInicio;
    int posFin;
    int j;  /* Indice de la cadena binaria de bits base */
    int q;  /* Para guardar la suma de inicio de cada hilo */

    /* Se verifica que nos hayan pasado el tamaño del arreglo y el número de hilos */
    if (argc != 3)
    {
        printf("\nModo de uso:\n");
        printf("$ %s tamArr numHilos\n\n", argv[0]);
        printf("Ejemplo:\n\n");
        printf("$ %s 5 2\n", argv[0]);

        return 1;
    }

    /* Se inicializa el mutex */
    pthread_mutex_init(&mutex_sumaSub, NULL);

    /* Se convierten las cadenas a enteros */
    ivG_tamArr = (int)atof(argv[1]);
    ivG_numHilos = (int)atof(argv[2]);

    srand(time(NULL));

    /* El hilo principal inicializa el arreglo */
    iaG_listNum = (int *) malloc(ivG_tamArr * sizeof(int));

    /* Se llena con numeros aleatorios del -9 al 20 */
    for (int i = 0; i < ivG_tamArr; i++)
        iaG_listNum[i] = -9+rand()%20;

    printf("\nArreglo de enteros: ");
    mostrarArreglo(iaG_listNum, 0, ivG_tamArr);
    
    /* El hilo principal inicializa la variable compartida */
    llvG_sumaSub = 0;

    /* Se obtiene el valor de los bits base */
    ivG_bitsBase = log2(ivG_numHilos);
    //printf("h (bits base): %d\n",ivG_bitsBase);

    /* Se inicializa el arreglo binario */
    iaL_listBinBase = (int *) malloc(ivG_bitsBase * sizeof(int));

    /* Se llena con 0s en un inicio */
    for (int i = 0; i < ivG_bitsBase; i++)
        iaL_listBinBase[i] = 0;

    //printf("\nArreglo binario de bits base (inicio): ");
    //mostrarArreglo(iaL_listBinBase, 0, ivG_bitsBase);
    
    /* Arreglo de hilos que haran la suma */
    pthread_t hilos[ivG_numHilos];

    /* Arreglo para tener la suma de inicio de cada hilo */
    int iniHilos[ivG_numHilos];
    
    posInicio = ivG_tamArr-1;
    posFin = (ivG_tamArr-1)-ivG_bitsBase;
    q = 1;

    /* Se crean los hilos */
    for (int i = 1; i <= ivG_numHilos; i++)
    {
        sumBitsBase = 0;
        j = 1;
        for (int i = posInicio; i > posFin; i--)
        {
            if (iaL_listBinBase[j] == 1)
            {
                sumBitsBase = sumBitsBase + iaG_listNum[i];
            }
            j++;
        }
        iniHilos[q] = sumBitsBase;

        pthread_create(&hilos[i], NULL, (void *) calculaSuma, &iniHilos[q]);

        if (ivG_numHilos >= 2)
        {
            acarreo = 1;
            pos = ivG_bitsBase;
            while (acarreo == 1)
            {
                iaL_listBinBase[pos] = (iaL_listBinBase[pos]+acarreo)%2;
                if (iaL_listBinBase[pos] == 1)
                {
                    acarreo = 0;
                }
                pos--;
            }
        }
        q++;
    }
    //printf("\nArreglo binario de bits base (fin): ");
    //mostrarArreglo(iaL_listBinBase, 0, ivG_bitsBase);

    /* Se espera a todos los hilos */
    for (int i = 1; i <= ivG_numHilos; i++)
    {
        pthread_join(hilos[i], NULL);
    }

    printf("\nLos subconjuntos que suman 0 son: %lld\n", llvG_sumaSub-1);

    /* Se libera memoria */
    free(iaG_listNum);
    free(iaL_listBinBase);

    /* Se destruye el mutex */
    pthread_mutex_destroy(&mutex_sumaSub);

    return 0;
}

/* Esta función la ejecutan los hilos */
void calculaSuma(void *ptrEntero) {
    /* Calcula la suma del subconjunto */
    int* intPtr = (int *) ptrEntero;    /* Se convierte el apuntador a entero */
    int miInicio = *intPtr;   /* Se obtiene el valor de la variable a donde apunta */
    int *iaL_listBin;   /* Arreglo binario */
    int pos;    /* Posicion del arreglo binario a sumar */
    int acarreo;    /* Acarreo para la suma */
    /* Tamaño de la cadena binaria para cada hilo (|E| - h bits) */
    int tamSub = ivG_tamArr-ivG_bitsBase;
    /* elevado es para saber hasta donde se incrementaran las cadenas binarias */
    long long elevado = pow(2, tamSub);
    /* sumParcial es para sumar los elementos de cada subconjunto */
    int sumParcial;

    /* Se inicializa el arreglo binario */
    iaL_listBin = (int *) malloc(tamSub * sizeof(int));

    /* Se llena con 0s en un inicio */
    for (int i = 0; i < tamSub; i++)
        iaL_listBin[i] = 0;

    //printf("\nSuma de inicio hilo: %d...", miInicio);
    //printf("\nArreglo binario de |E| - h bits (inicio): ");
    //mostrarArreglo(iaL_listBin, 0, tamSub);

    /* Se calcula la suma de cada subconjunto */
    for (int i = 0; i < elevado-1; i++)
    {
        sumParcial = miInicio;
        for (int i = 0; i < tamSub; i++)
        {
            if (iaL_listBin[i] == 1)
            {
                sumParcial = sumParcial + iaG_listNum[i];
            }
        }
        if (sumParcial == 0)
        {
            pthread_mutex_lock(&mutex_sumaSub);
                llvG_sumaSub = llvG_sumaSub + 1;
            pthread_mutex_unlock(&mutex_sumaSub);
        }
        
        acarreo = 1;
        pos = tamSub-1;
        while (acarreo == 1)
        {
            iaL_listBin[pos] = (iaL_listBin[pos]+acarreo)%2;
            if (iaL_listBin[pos] == 1)
            {
                acarreo = 0;
            }
            pos--;
        }
    }

    //printf("\nArreglo binario de |E| - h bits (fin): ");
    //mostrarArreglo(iaL_listBin, 0, tamSub);
    free(iaL_listBin);
}

/* Función para depurar el código, solo lo usa el hilo principal (main) */
void mostrarArreglo(int *arr, int inicio, int fin) {
    for (int i = inicio; i < fin; i++)
        printf("%d ", arr[i]);
    
    printf("\n\n");
}
