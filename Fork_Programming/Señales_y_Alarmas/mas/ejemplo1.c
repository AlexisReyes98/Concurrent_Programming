/*
*   Programa que ilustra la instalación de una rutina de atención a señal 
*         
*   El programa instala una rutina de atención a la interrupción INT y entra en un 
*   loop infinito.
*/

#include <stdio.h>     /* funciones estándar de I/O                      */
#include <unistd.h>    /* funciones unix, como getpid()                  */
#include <signal.h>    /* signal name macros, y el prototipo signal()    */

void verificarBorrado(int ret1, int ret2){
    if(ret1 == 0 && ret2 == 0) {
      printf("Archivo borrado correctamente");
   } else {
      printf("Error al eliminar archivos");
   }
}

void crearArchivo(){

   int ret1, ret2;
   FILE *fp;
   FILE *fp1;
   char file1[] = "tmp1.txt";
   char file2[] = "tmp2.txt";
   fp = fopen(file1, "w");
   fp1 = fopen(file2, "w");

   fprintf(fp, "%s", "HOla");
   fprintf(fp1, "%s", "Hola");
   fclose(fp);
   fclose(fp1);
   sleep(10);
   ret1 = remove(file1);
   ret2 = remove(file2);
   verificarBorrado(ret1, ret2);
}

/* Esta rutina es el manejador de la señal */
void cacha_int(int sig_num)
{
    int ret1, ret2;
    /* reinstala el manejador de la señal de nuevo para cacharla la siguiente vez */
    signal(SIGINT, cacha_int);
    printf("No hagas eso!!!\n");
    ret1 = remove("temp1.txt");
    ret2 = remove("temp2.txt");
    fflush(stdout);
}


int main(int argc, char* argv[])
{
    
    /* poner la rutina cacha_int como manejador de la señal INT (ctrl-c)*/
    signal(SIGINT, cacha_int);
    crearArchivo();
    /* Entramos en un loop infinito sin hacer nada */
    for ( ;; )
        pause();
}