/* Programa que ilustra el uso de alarmas: USA_ALARMAS */

#include <stdio.h> /* funciones de I/O standard */
#include <unistd.h>/* func standard de unix como alarm()*/
#include <signal.h>/* nombres de macros de señales, y el prototipo de signal() */
#include <stdlib.h>

char usuario[40]; /* buffer para leer el nombre de usuario */

/* define el manejador de la alarma */
void cacha_alarma(int sig_num)
{   printf("Expiró el tiempo de espera  ... ciao!!! ...\n\n");
    exit(0);
}

int main(int argc, char* argv[])
{   signal(SIGALRM, cacha_alarma); /* Cacha la señal ALRM */
    printf("Usuario? : "); /* Pide datos al usuario */
    fflush(stdout);
    alarm(10); /* arranca la alarma de 30 segundos */
    scanf("%s", usuario); /*  Espera la entrada del usuario */
    alarm(0); /* apaga la alarma */
    printf("Usuario: '%s'\n", usuario);
    return 0;
}
