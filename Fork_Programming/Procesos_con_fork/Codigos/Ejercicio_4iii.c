/*
**      Giovanny ALexis Reyes Vilchis
**      2163031812
**      Compilar con: gcc Ejercicio_4iii.c -o ejercicio4iii
**                    ./ejercicio4iii
*/

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char const *argv[]) {
    int N = 3;

    int hijos = 0;
    pid_t procs;

    while (N > 0 && hijos < 2)
    {
        procs = fork();    // El hijo recibe 0, el padre el PID del hijo

        if (procs == 0)
        {
            printf("\nYo soy el HIJO con PID: %d\n", getpid());
            N--;
            hijos = 0;
        }
        else
        {
            printf("\nYo soy el PADRE con pid: %d\n", getpid());
            hijos++;
        }
        sleep(10);
    }

    return 0;
}