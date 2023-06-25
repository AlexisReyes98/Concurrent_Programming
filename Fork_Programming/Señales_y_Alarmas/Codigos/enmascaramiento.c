/*
*     Programa que ilustra el enmascaramiento de señales.     
*     El programa instala rutinas de atención a las interrupciones INT y SUSPEND.
*     Cuenta el número de ctrl-c dados por el usuario y si se presiona 
*     ctrl-z imprime el contador.
*     Posteriormente entra en un loop infinito.
*/

#include <stdio.h>     /* funciones de I/O standard                      */
#include <unistd.h>    /* funciones unix standard como  getpid()         */
#include <signal.h>    /* macros de señales y el prototipo signal()      */
#include <stdlib.h>

/* primero, se define un contador de Ctrl-C y lo inicializamos a 0 */
int cuenta_ctrl_c = 0;
#define    LIMITE_CTRL_C    5

/* instalamos el manejador de Ctrl-C */
void cacha_int(int sig_num)
{
    sigset_t mascara;        /* para poner una mascara de señales   */
    sigset_t vieja_mascara; /* para guardar la vieja mascara de señales */

    /* reinstala el manejador de señales para cacharla la siguiente vez  */
    signal(SIGINT, cacha_int);

    /* enmascaramos las otras señales mientras estamos en el manejador  */
    sigfillset(&mascara);
    sigprocmask(SIG_SETMASK, &mascara, &vieja_mascara);
    
    /* incrementamos el contador y verficamos si se alcanzó el limite */
    cuenta_ctrl_c++;
    if (cuenta_ctrl_c >= LIMITE_CTRL_C) {
    char respuesta[30];

    /* pregunta al usuario si realmente quiere salir */
    printf("\nRealmente quieres terminar? [s/n]: ");
    fflush(stdout);
    scanf("%s", respuesta);
    if (respuesta[0] == 's' || respuesta[0] == 'S') {
        printf("\nAu revoir!!!...\n\n\n");
        fflush(stdout);
        exit(0);
    }
    else {
        printf("\nContinuamos\n");
        fflush(stdout);
        /* reseteamos el contador de  Ctrl-C */
        cuenta_ctrl_c = 0;
    }
    }
    /* restauramos la vieja mascara de señales */
    sigprocmask(SIG_SETMASK, &vieja_mascara, NULL);
}

/* El manejador de la señal Ctrl-Z  */
void cacha_suspend(int sig_num) {
    sigset_t mascara;        /*para poner una mascara de señales */
    sigset_t vieja_mascara; /*para almacenar la vieja mascara de señales */

    /* reinstala el manejador de señales para cacharla la siguiente vez  */
    signal(SIGTSTP, cacha_suspend);
   
    /* enmascaramos las otras señales mientras estamos en el manejador */
    sigfillset(&mascara);
    sigprocmask(SIG_SETMASK, &mascara, &vieja_mascara);

    /* imprime el contador de Ctrl-C  */
    printf("\n\nHasta ahora, se han dado '%d' Ctrl-C \n\n", cuenta_ctrl_c);
    fflush(stdout);

    /* restauramos la vieja mascara de señales */
    sigprocmask(SIG_SETMASK, &vieja_mascara, NULL);
}

int main(int argc, char* argv[]) {
    /* instala los manejadores de Ctrl-C y Ctrl-Z */
    signal(SIGINT, cacha_int);
    signal(SIGTSTP, cacha_suspend);

    /* Entramos en un loop infinito de espera por señales */
    for ( ;; )
    pause();

    return 0;
}