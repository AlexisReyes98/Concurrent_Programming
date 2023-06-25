/*
**      Giovanny ALexis Reyes Vilchis
**
**      Compilar con: gcc servidor.c -o servidor
**                    ./serviidor
*/

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    int N;
    char usuario[30];
    pid_t pid;
    int padre = 1;
    
    while (1)
    {
        printf("\nIngresa tu nombre: ");
        scanf("%s", usuario);

        if (strcmp(usuario, "Sayonnara") == 0)  // Se verifica si se ingresa "Sayonnara"
        {
            printf("\nEsperando procesos...\n\n");
            while (1)   // Se espera a todos los procesos hijo
            {
                if (wait(NULL) == -1)   /// En caso de que no haya procesos hijos, termina la ejecución
                {
                    printf("Servidor cerrado! Vuelve pronto :)\n\n");
                    exit(EXIT_SUCCESS);    //EXIT_SUCCESS, una macro constante de valor 0 que denota convencionalmente un retorno exitoso
                }   
            }
        }
        else
        {
            if (padre == 1) // Para que solo sea el padre con el mismo ID que cree procesos
            {
                pid = fork();   // Se utiliza la llamada al sistema fork()
                if (pid == 0) // Proceso hijo
                {
                    padre = 0;
                    // Utlice execvp() quien recibe un arreglo de apuntadores a cadenas de caracteres
                    // La lista de argumentos contiene el nombre del programa xterm, la opción -e,
                    // el nombre del archivo ejecutable del programa jalisco.c, el contenido de la variable usuario
                    // y termina con un puntero NULL
                    char *v[]={"xterm", "-e", "./jalisco", usuario, NULL};
                    execvp("xterm",v);
                }
                else // Proceso padre
                {
                    padre = 1;
                }
            }
        }
    }
    
    return 0;
}
