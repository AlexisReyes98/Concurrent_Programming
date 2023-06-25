/*
**      Giovanny ALexis Reyes Vilchis
**      2163031812
**      Compilar con: gcc Ejercicio_4ii.c -o ejercicio4ii
**                    ./ejercicio4ii 5
*/

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char const *argv[]) {
    int N;
    sscanf(argv[1], "%d", &N);

    pid_t pid = 0;
    
    for (int i=0; i < N; i++)
    {
        if (pid == 0)
        {
            pid = fork();
        }
        printf("\nProceso con PID: %d\n", getpid());
        sleep(10);
    }
    
    return 0;
}