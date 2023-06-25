#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    printf("\n\nHola Mundo.\n\n");

    pid_t pid = fork();
    pid_t pMismo = getpid();	// EL mismo proceso
    pid_t pPadre = getppid();	// Proceso padre
    printf("\nYa cree un proceso nuevo.\n");

    printf("\nPID del proceso mismo %d\n", pMismo);
    printf("\nPID del proceso padre %d\n", pPadre);
    printf("\nPID fork %d\n", pid);

    sleep(10);
    return 0;
}
