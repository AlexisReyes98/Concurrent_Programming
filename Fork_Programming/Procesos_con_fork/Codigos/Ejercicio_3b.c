/*
**      Giovanny ALexis Reyes Vilchis
**      2163031812
**      Compilar con: gcc Ejercicio_3b.c -o ejercicio3b
**                    ./ejercicio3b
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
        pid_t pid2 = fork();

        if (pid2 != 0)
        {
            printf("\nYo soy el PADRE con pid: %d y mi otro hijo es: %d\n", getpid(), pid2);
        }
        else
        {
            printf("\nYo soy el otro HIJO con PID: %d\n", getpid());
            sleep(30);
        }
        wait(NULL);
    }
    else { // Si me regresaron 0, soy el hijo.
        printf("\nYo soy el HIJO con PID: %d\n", getpid());
        sleep(30);
    }

    printf("\nTengo %d pesos.\n", pesos);

    return 0;
}