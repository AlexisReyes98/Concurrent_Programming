/*
**    Autor: Giovanny ALexis Reyes Vilchis
**
**    Productor-consumidor
**
**    Compilar con : gcc youtube_prod_cons.c -lpthread -o you_prod
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>

/*** Declaraciones para el buffer circular ***/

// Declaración del nuevo tipo de datos Buffer
typedef struct {
   char** elementos;  // almacenamiento interno del buffer
   int capacidad;     // capacidad máxima del buffer
   int frente;        // índice para apuntar al elemento que está al frente
   int final;         // índice para apuntar al elemento que está al final
} Buffer;

/** Funciones para modificar el buffer **/

// buf: dirección de variable tipo Buffer ya declarada
// tam: tamaño máximo fijo del buffer que se creará
void creaBuffer(Buffer* buf, int tam);

// buf: dirección de un buffer ya declarado e INICIALIZADO
// p: el producto que se va insertar
static inline void meterProducto(Buffer *b, char *p);

// buf: dirección de un buffer ya declarado e INICIALIZADO
// p: DIRECCIÓN de una variable de tipo producto donde se DEJARÁ el resultado
static inline void sacarProducto(Buffer *b, char *p);

// buf: dirección de un buffer ya declarado e INICIALIZADO
void destruyeBuffer(Buffer* buf);

// Estas funciones son internas al buffer
static inline bool bufLleno(Buffer* buf);  
static inline bool bufVacio(Buffer* buf);

/*** FIN Declaraciones para el buffer circular ***/

void consumidor(void* ptr);

int obtenerLineas(char *nombreArchivo);

#define TAREA_ESPECIAL "Detener Sistema"

#define NUM_CONSUMIDORES 6

#define LONGITUD_MAX 100   // Longitud maxima para cada cadena URL
int CANTIDAD_VIDEOS;    // Cantidad de videos

// Número máximo de peticiones y tamaño del buffer
#define TAM_BUFF 4
Buffer bufTareas;    // Buffer de tareas para los consumidores

/* Declaración de semáforos y mutex */
sem_t lugares;    // Núm. de lugares disponibles en el búfer
sem_t productos;   // Núm. de productos en el búfer
pthread_mutex_t mutexBuffer;  // Proteger acceso exclusivo al búfer

int main(int argc, char const *argv[]) 
{
   char* nombreArchivo = "peticiones.txt";

   CANTIDAD_VIDEOS = obtenerLineas(nombreArchivo);

   //printf("\n\nlineas: %d\n\n", CANTIDAD_VIDEOS);

   // Arreglo de cadenas: aquí se almacenan todas las cadenas de caracteres
   char arreglo[CANTIDAD_VIDEOS][LONGITUD_MAX];
   char buferArchivo[LONGITUD_MAX];    // Útil para leer el archivo
   FILE *archivoEntrada = fopen(nombreArchivo, "r");  // Abrir el archivo para lectura
   if (archivoEntrada == NULL)
   {
      printf("No se puede abrir el archivo");
      return 0;
   }
   
   int indice = 0;   // Iterador para saber en que línea vamos
   // Mientras se pueda leer una línea del archivo
   while (fgets(buferArchivo, LONGITUD_MAX, archivoEntrada))
   {
      // Remover salto de línea
      strtok(buferArchivo, "\n");
      // Copiar la línea al arreglo, usando el índice
      memcpy(arreglo[indice], buferArchivo, LONGITUD_MAX);
      // Aumentarlo en cada iteración
      indice++;
   }

   // Terminamos de leer
   fclose(archivoEntrada);

   // Ahora ya tenemos el arreglo y podemos imprimirlo
   /*
   for (int i = 0; i < CANTIDAD_VIDEOS; i++)
      printf("%s\n", arreglo[i]);
   */

   // Inicializar el buffer con una capacidad máxima fija
   creaBuffer(&bufTareas, TAM_BUFF);

   // Incialización de semáforos y mutex
   sem_init(&lugares, 0, TAM_BUFF);
   sem_init(&productos, 0, 0);
   pthread_mutex_init(&mutexBuffer, NULL);

   // Declaración de los hilos consumidores
   pthread_t hiloCons[NUM_CONSUMIDORES];

   // Creación de los hilos consumidores para que ejecuten 
   // la función "consumidor"
   for (size_t i = 0; i < NUM_CONSUMIDORES; i++)
   {
      pthread_create(&hiloCons[i], NULL, (void *) consumidor, NULL);
   }

   /** El hilo principal será el PRODUCTOR **/

   bool continuar = true;  // Variable adecuada para poder terminar el ciclo
   int linea_actual = 0;  // Iterador para recorrer el arreglo

   int numAleatorio;  // Número aleatorio para el sleep

   srand(time(NULL));   // Semilla
   
   while ( continuar )
   {
      if ( strcmp(arreglo[linea_actual], TAREA_ESPECIAL) == 0 )   // Si se lee la tarea especial para detener el sistema
      {
         continuar = false;
      }
      else  // De lo contrario se mete el siguiente link al buffer
      {
         // Uso semáforos
         sem_wait(&lugares);   // Espera lugares
         pthread_mutex_lock(&mutexBuffer);
            meterProducto(&bufTareas, arreglo[linea_actual]);  // Añade producto al buffer
         pthread_mutex_unlock(&mutexBuffer);
         sem_post(&productos);   // Señal
      }

      // Generar números aleatorios del 5 al 10
      numAleatorio = rand()%6+5;  // Le indicamos que el número será entre 0-5 y sumamos 5

      sleep(numAleatorio);
      linea_actual++;
   }

   // Meter la tarea especial para indicar que ya terminó todo
   sem_wait(&lugares);
   pthread_mutex_lock(&mutexBuffer);
      meterProducto(&bufTareas, TAREA_ESPECIAL);
   pthread_mutex_unlock(&mutexBuffer);
   sem_post(&productos);

   // Esperar a todos los hilos consumidores
   printf("\nProductor: Ya no hay más peticiones, esperaré a los consumidores...\n\n");
   
   for(int i = 0 ; i < NUM_CONSUMIDORES ; i ++)
   {
      pthread_join(hiloCons[i], NULL);
   }

   destruyeBuffer(&bufTareas);

   pthread_mutex_destroy(&mutexBuffer);

   printf("\n\nFIN.\n\n");

   return 0;
}

/* Definición de la tarea que hará un consumidor */
void consumidor(void* ptr)
{
   char extraer[LONGITUD_MAX];
   bool continuar = true;
   while(continuar)
   {
      printf("\nConsumidor: quiero tomar un producto...");
      sem_wait(&productos);   // Espera producto
      pthread_mutex_lock(&mutexBuffer);
         sacarProducto(&bufTareas, extraer); // Saca producto
      pthread_mutex_unlock(&mutexBuffer);
      sem_post(&lugares);  // Señal
      printf("Listo\n");

      if( strcmp(extraer, TAREA_ESPECIAL) != 0 )   // Si no se lee la tarea especial para detener el sistema
      {
         char comando[LONGITUD_MAX] = "youtube-dl ";
         strcat(comando, extraer);
         printf("Descargando el video de la siguiente URL: %s\n", extraer);
         system(comando);   // Comando bash para descargar los videos
      }
      else
      {
         continuar = false;
      }
   }
   // Se debe avisar a los n hilos consumidores que esten esperando
   // de lo contrario los hilos se bloquean y no se enteran de que la tarea termino
   printf("\nConsumidor: metí la tarea especial\n");
   sem_wait(&lugares);
   pthread_mutex_lock(&mutexBuffer);
      meterProducto(&bufTareas, TAREA_ESPECIAL);
   pthread_mutex_unlock(&mutexBuffer);
   sem_post(&productos);
}

int obtenerLineas(char *nombreArchivo)
{
   // Se obtiene el total de lineas del archivo de entrada
   int cont = 0;
   FILE* archivoLineas = fopen(nombreArchivo, "r");
   if (!archivoLineas)
      exit(0);

   char *contenido = NULL;
   size_t leng = 0;
   while (getline(&contenido, &leng, archivoLineas) != -1){
      cont++;
   }

   fclose(archivoLineas);
   free(contenido);

   return cont;
}

// FUNCIONES DE USO PARA EL BUFFER

void creaBuffer(Buffer *buf, int tam)
{
   buf->capacidad = tam;
   buf->elementos = (char **)malloc(tam * sizeof(char *));
   buf->frente = -1; // Para indicar que está vacía la cola
   buf->final = -1;  // Para indicar que está vacía la cola
   for (size_t i = 0; i < CANTIDAD_VIDEOS; i++)
   {
      buf->elementos[i] = (char *) malloc(LONGITUD_MAX * sizeof(char *));
   }
}

void destruyeBuffer(Buffer *buf)
{
   free(buf->elementos);
}

static inline bool bufLleno(Buffer *b)
{
   int next = (b->final + 1) % b->capacidad;

   if (b->frente == next)
      return true;
   else
      return false;
}

static inline bool bufVacio(Buffer *b)
{
   if (b->frente == -1)
      return true;
   else
      return false;
}

static inline void meterProducto(Buffer *b, char *p)
{
   if (!bufLleno(b))
   {
      b->final = (b->final + 1) % b->capacidad;
      strcpy(b->elementos[b->final], p);
      if (b->frente == -1) // Cuando estaba vacia, el nuevo elemento esta en pos 0.
         b->frente = 0;
   }
}

static inline void sacarProducto(Buffer *b, char *p)
{
   if (!bufVacio(b))
   {
      strcpy(p, b->elementos[b->frente]);

      if (b->frente == b->final)
      {  // Era el unico elemento.
         b->frente = -1;
         b->final = -1;
      }
      else
         b->frente = (b->frente + 1) % b->capacidad;
   }
}

