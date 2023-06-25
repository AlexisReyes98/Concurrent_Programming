/*
**      Giovanny ALexis Reyes Vilchis
**      2163031812
**      Compilar con: gcc Ejercicio_4i.c -o ejercicio4i
**                    ./ejercicio4i 5
*/

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char const *argv[]) {
    int N;
    sscanf(argv[1], "%d", &N);

    int padre = 1;
    
    for (int i=0; i < N; i++)
    {
        if (padre == 1)
        {
            if (fork() == 0) /* Proceso hijo */
            {
                printf("\nYo soy el HIJO con PID: %d\n", getpid());
                padre = 0;
            }
            else /* Proceso padre */
            {
                printf("\nYo soy el PADRE con pid: %d\n", getpid());
                padre = 1;
            }
        }
        sleep(10);
    }
    
    return 0;
}