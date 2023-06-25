/*
**      Giovanny ALexis Reyes Vilchis
**
**      Compilar con: gcc jalisco.c -o jalisco
**                    ./jalisco name
*/

#include <stdio.h>

void juego(char *name[]);

int main(int argc, char *argv[]) {
    
    juego(argv);

    return 0;
}

void juego(char *name[]) {
    printf("\nBienvenido: %s\n", name[1]);
    printf("Jueguemos! El juego termina cuando ingreses un numero negativo\n\n");

    int num=0, cont=1;

    while (1)
    {
        printf("\nPartida: %d", cont);  // En cada iteración se muestra un número de partida
        printf("\nIngresa un numero: ");
        scanf("%d", &num);
        
        if (num < 0)    // Si número es negativo sale del programa
        {
            break;
        }
        else    // En caso de que no, se incrementa el número escrito con un mensaje de salida
        {
            num++;
            printf("\nTE GANÉ!!! escribi el numero: %d\n", num);
            cont++;
        }
    }

    printf("\nBuen juego! Hasta pronto...\n");
}
