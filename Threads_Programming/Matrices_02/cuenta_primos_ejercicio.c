/*
**    Programa para ilustrar el uso de los hilos
**    y la memoria compartida.
**
**    Compilar con : gcc cuenta_primos_ejercicio.c -lpthread -o primos
**
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* Supondremos que el número de enteros siempre es divisible entre el número de hilos. */
#define MAX_INT 16000
#define NUM_HILOS 16

/* Prototipo de la función para que un hilo sume */
void contarPrimos(void *ptr);

bool esPrimo(long n);

/* Esta variable COMPARTIDA por todos lo hilos indica el inicio del subintervalo para cada hilo */
long inicioSub = 1;

/* Esta es una variable COMPARTIDA por todos los hilos */
long long numPrimos = 0;

pthread_mutex_t mutexPrimos;

int main()
{
   pthread_mutex_init(&mutexPrimos, NULL);

   /* Deben declarar un arreglo para manejar los hilos de tipo pthread_t */
   pthread_t threads[NUM_HILOS];

   /* Entre todos los hilos deben contar la cantidad de números primos que hay
    * en el intervalo [1, MAX_INT].
    * Es decir se dividirán el intevarlo en partes iguales, para que cada quien
    * cuente los primos en el subintervalo que le corresponda.
    */    
   /* Crear los N hilos */
   for (int i = 0; i < NUM_HILOS; i++) {
      pthread_create(&threads[i], NULL, (void *) contarPrimos, NULL);
   }

   /* El hilo principal debe mostrar el número de primos totales. */
   
   printf("\nHilo principal: esperando resultados...\n\n");
   
   for (int i = 0; i < NUM_HILOS; i++) {
      pthread_join(threads[i], NULL);
   }
   
   printf("\nHilo principal: El número de primos en el intervalo es: %lld\n\n", numPrimos);

   pthread_mutex_destroy(&mutexPrimos);
   
   return 0;
}

/* Función que va a ejecutar cada hilo sobre el intervalo que le corresponda. */
/* Deben de identificar las secciones críticas */
/* Recuerden esto:
1. ¿Hay variables compartidas? Si es así, ¿al menos uno de los hilos escribe en la variable compartida?
2. Los candados se definen para CADA variable compartida y NO un candado para todo.
3. Los candados deben abarcar exclusivamente la sección crítica, es decir, deben dejar fuera en la medida de los posible,
el acceso a variables locales.
*/
void contarPrimos(void *ptr)
{
   long long tamSub = MAX_INT/NUM_HILOS; /* aquí deben calcular el número de elementos que debe revisar este hilo. */

   /* Recuerden que al inicio de todo, inicioSub vale 1 */
   pthread_mutex_lock(&mutexPrimos);
   long inicio = inicioSub;
   inicioSub = inicioSub+tamSub;   /* Aquí deben incrementar inicioSub para indicar el inicio del intervalo para el siguiente hilo. */
   pthread_mutex_unlock(&mutexPrimos);
   
   printf("\nHilo nuevo: tamaño intervalo: %lld\n", tamSub);

   /* Verificar si cada uno de los enteros que me tocaron son primos o no.*/
   for (long i = inicio; i < inicio+tamSub; i++) {
      if (esPrimo(i) == true){
         pthread_mutex_lock(&mutexPrimos);
         numPrimos = numPrimos + 1;
         pthread_mutex_unlock(&mutexPrimos);
      }
   }
}

bool esPrimo(long n) 
{
   bool primo = true;
   
   /* Buscar un divisor p entre 2 y n-1 para nuestro numero n*/
   long p = 2;
   while (primo == true && p < n-1) {
      if (n % p == 0) {
         primo = false;
      }
      p++;
   }
   
   return primo;
}
