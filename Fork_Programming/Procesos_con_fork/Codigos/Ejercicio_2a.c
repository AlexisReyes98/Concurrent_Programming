/*
**      Giovanny ALexis Reyes Vilchis
**      2163031812
**      Compilar con: gcc Ejercicio_2a.c -o ejercicio2a
**                    ./ejercicio2a
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
        wait(&status);

        printf("\nPadre: Factorial de %d es: %d\n", pesos, WEXITSTATUS(status));
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

    printf("\nTengo %d pesos.\n", pesos);

    return 0;
}