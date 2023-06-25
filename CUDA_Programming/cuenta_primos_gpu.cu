/*
**    Programa para ilustrar el uso de los hilos y la memoria compartida.
**
**    Compilar con : nvcc --gpu-architecture=sm_50 cuenta_primos_gpu.cu -o primosGPU
**
**    Autor: Giovanny Alexis Reyes Vilchis
**    Matricula: 2163031812
*/

#include "../CUDA-by-Example/common/book.h"

/* Prototipo de las funciones */
__global__ void contarPrimos( int *enteros, int *siNo );
__device__ bool esPrimo(long n);
__host__ void llenarArreglo(int *arr, int tam);
__host__ void printArreglo(int *arr, int tam);

#define N (64 * 1024)

int main( void ) {
    /* Arreglos que tendrán los enteros que queremos verificar si son primos o no */
    int *enteros, *siNo;
    int *dev_enteros, *dev_siNo;

    /* Inicializa la semilla para generar números aleatorios */
    srand(time(NULL));

    printf("\nTamaño del arreglo: %d\n", N);

    // Asigna la memoria en la CPU
    enteros = (int *) malloc(N * sizeof(int));
    siNo = (int *) malloc(N * sizeof(int));

    /* El CPU debe mostrar el conteo final */
    printf("\nCPU: esperando resultados...\n");

    // Asigna la memoria en la GPU
    HANDLE_ERROR( cudaMalloc( (void**)&dev_enteros, N * sizeof(int) ) );
    HANDLE_ERROR( cudaMalloc( (void**)&dev_siNo, N * sizeof(int) ) );

    // Llena el arreglo 'enteros' en la CPU
    llenarArreglo(enteros, N);

    // Copia el arreglo 'enteros' a la GPU
    HANDLE_ERROR( cudaMemcpy( dev_enteros, enteros, N * sizeof(int), cudaMemcpyHostToDevice ) );

    contarPrimos<<<32,1024>>>( dev_enteros, dev_siNo );

    // Copia el arreglo 'siNo' de la GPU a la CPU
    HANDLE_ERROR( cudaMemcpy( siNo, dev_siNo, N * sizeof(int), cudaMemcpyDeviceToHost ) );

    //printArreglo(enteros, N);
    printf("\nLa GPU ya termino de identificar números primos.\n");
    //printArreglo(siNo, N);
    
    int numPrimos = 0;
    for (int i=0; i < N; i++)
        numPrimos += siNo[i];

    printf("\nHay %d primos.\n\n", numPrimos);

    // Libera la memoria que asignamos en la GPU
    HANDLE_ERROR( cudaFree( dev_enteros ) );
    HANDLE_ERROR( cudaFree( dev_siNo ) );

    // Libera la memoria que asignamos en la CPU
    free( enteros );
    free( siNo );

    return 0;
}

__global__ void contarPrimos( int *enteros, int *siNo ) {
    int tid = threadIdx.x + blockIdx.x * blockDim.x;

    while (tid < N)
    {
        if (esPrimo( enteros[tid] ) == true)
            siNo[tid] = true;
        else
            siNo[tid] = false;
    
        tid += blockDim.x * gridDim.x;
    }
}

__device__ bool esPrimo(long n) {
    bool primo = true;
    
    /* Buscar un divisor p entre 2 y n-1 para nuestro numero n*/
    long p = 2;
    while (primo == true && p < n-1) {
        if (n % p == 0)
            primo = false;

        p++;
    }
    
    return primo;
}

__host__ void llenarArreglo(int* arr, int tam) {
    for (int i=0; i < tam; i++) {
        arr[i] = 1000000 + rand()%10000;
        arr[i] = 2*arr[i] + 1;  // Para que el entero sea IMPAR.
    }
}

__host__ void printArreglo(int* arr, int tam) {
    for (int i=0; i < tam; i++)
        printf("%d ", arr[i]);
}
