/*
**      Giovanny ALexis Reyes Vilchis
**      2163031812
**      Compilar con: gcc Ejercicio_2b.c -o ejercicio2b
**                    ./ejercicio2b
*/

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>

int main() {

    int pesos = 5;
    printf("\nHola Mundo.\n");

    pid_t pid = fork(); // 0: hijo,  > 0: padre

    if (pid != 0) { // El padre recibe un PID, el cual es diferente de 0.
        printf("\nYo soy el PADRE con pid: %d y mi hijo es: %d\n", getpid(), pid);
        int status;
        pid_t pidHijo = wait(&status);

        printf("\nPadre: Factorial de %d es: %d\n", pesos, WEXITSTATUS(status));
        printf("\nPID de la primera llamada a wait: %d\n", pidHijo);
        
        pid_t pidHijo2 = wait(&status); // Segunda llamada a wait
        printf("\nPID de la segunda llamada a wait: %d\n", pidHijo2);
    }
    else { // Si me regresaron 0, soy el hijo.
        printf("\nYo soy el HIJO con PID: %d\n", getpid());
        int fact = 1;
        // Calcular factorial
        for (int i = 1; i <= pesos; i++){
            fact = fact * i;
        }

        // Resultado
        printf("\nHijo: Factorial de %d es: %d\n", pesos, fact);

        _exit(fact);
    }

    //sleep(30);
    printf("\nTengo %d pesos.\n", pesos);

    return 0;
}