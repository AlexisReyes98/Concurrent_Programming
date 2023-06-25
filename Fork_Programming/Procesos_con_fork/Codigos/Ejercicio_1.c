/*
**      Giovanny ALexis Reyes Vilchis
**      2163031812
**      Compilar con: gcc Ejercicio_1.c -o ejercicio1
**                    ./ejercicio1
*/

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main() {

    int pesos = 5;
    printf("\nHola Mundo.\n");

    pid_t pid = fork(); // 0: hijo,  > 0: padre

    if (pid != 0) { // El padre recibe un PID, el cual es diferente de 0.
        printf("\nYo soy el PADRE con pid: %d y mi hijo es: %d\n", getpid(), pid);
        sleep(30);
    }
    else { // Si me regresaron 0, soy el hijo.
        printf("\nYo soy el HIJO con PID: %d\n", getpid());
        //pesos += 50;
        int fact = 1;
        // Calcular factorial
        for (int i = 1; i <= pesos; i++){
            fact = fact * i;
        }

        // Resultado
        printf("\nHijo: Factorial de %d es: %d\n", pesos, fact);
        sleep(30);
    }

    printf("\nTengo %d pesos.\n", pesos);

    return 0;
}