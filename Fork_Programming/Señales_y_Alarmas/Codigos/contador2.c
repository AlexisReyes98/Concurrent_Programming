/* Programa que ilustra la instalación de una rutina de atención a señal: cachaCRTL.c */
/* kill -9 PID */

#include <stdio.h> /* funciones standard de I/O */                     
#include <unistd.h> /* funciones unix, como getpid()*/         
#include <signal.h> /* signal macros, y el prot. signal()*/

int cont = 0;
/* Esta rutina es el manejador de la señal */
void cacha_int(int sig_num)
{/* reinstala el manejador de nuevo para la siguiente vez */
    signal(SIGINT, cacha_int);
    if (cont++%2 == 0) {
	printf("\nNo hagas eso, porfavor...\n");
    }
    else{
	printf("\nQue dejes de hacer eso!!!\n");
    }

    fflush(stdout);
}

int main(int argc, char* argv[])
{ /* poner cacha_int como manejador de la señal INT (ctrl-c)*/
    signal(SIGINT, cacha_int);
  /* Entramos en un loop infinito sin hacer nada */
    for (;;)
        pause();
}
